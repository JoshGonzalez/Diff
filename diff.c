#include "diff.h"
#include "para.h"
#include "util.h"




void print_version(void){
  printf("Version number: %.2f\n", VERSION_NUM);
  printf("Copyright (C) 2020 CSUF\n");
  printf("Created by Joshua Gonzalez...\n");
}

void loadfiles(const char* filename1, const char* filename2){
  memset(buf, 0, sizeof(buf));
  memset(strings1, 0, sizeof(strings1));
  memset(strings2, 0, sizeof(strings2));

  FILE *fin1 = openfile(filename1, "r");
  FILE *fin2 = openfile(filename2, "r");

  while (!feof(fin1) && fgets(buf, BUFSIZ, fin1) != NULL) { strings1[count1++] = strdup(buf); }  fclose(fin1);
  while (!feof(fin2) && fgets(buf, BUFSIZ, fin2) != NULL) { strings2[count2++] = strdup(buf); }  fclose(fin2);

  if (count1 != count2) {
    different = ON;
  }
//potential segmentation fault
  for (int i = 0, j = 0; i < count1 && j < count2;  ++i, ++j) {
    if (strcmp(strings1[i], strings2[j]) == 0) {
        same[same_index][0] = strings1[i];
        same[same_index][1] = strings2[j];
        ++same_index;
      }
    if (strcmp(strings1[i], strings2[j]) != 0) {
        differ[differ_index][0] = strings1[i];
        differ[differ_index][1] = strings2[j];
        ++differ_index;
      }
    }
}

void perform_flags(int argc, const char* argv[]){
  const char* files[] = {NULL, NULL};

  check_flags(argc, argv);

  if(version){
    print_version();
    exit(0);
  }

  if(files_check(argc, argv) == 2){
    files[0] = argv[argc - 2];
    files[1] = argv[argc - 1];
  }
  else{
    fprintf(stderr, "diff: Not the right amount of files\n");
    exit(1);
  }

  loadfiles(files[0], files[1]);

  FILE* fin1 = openfile(files[0], "r");
  FILE* fin2 = openfile(files[1], "r");
  //Go through arguments and flip states on flags



  if (identical_files && brief) {
    brief = OFF;
    identical_files = OFF;
    identical_or_different_files = ON;
  }

  para* p = para_first(strings1, count1);
  para* q = para_first(strings2, count2);
  while (!feof(fin1) && fgets(buf, BUFSIZ, fin1) != NULL) { strings1[count1++] = strdup(buf); }
  while (!feof(fin2) && fgets(buf, BUFSIZ, fin2) != NULL) { strings2[count2++] = strdup(buf); }

  if(argc == 2){
    normal(p, q);
  }

  if(brief && different){
    printf("Files %s and %s are different\n", files[0], files[1]);
    //potential short
    for(int i = 0; i < differ_index; ++i){
      printf("Line %d: %s %50s %s", i, differ[i][0], ">", differ[i][1]);
    }
  }

  if(identical_files && !different){
    printf("Files %s and %s are identical\n", files[0], files[1]);
    for(int i = 0; i < same_index; ++i){
      printf("Line %d: %s %50s %s", i, same[i][0], ">", same[i][1]);
    }
  }

  if(identical_or_different_files){
    if(different){
      printf("Files %s and %s are different\n", files[0], files[1]);
      for(int i = 0; i < differ_index; ++i){
        printf("Line %d: %s %50s %s", i, differ[i][0], ">", differ[i][1]);
      }
    }
    else{
      printf("Files %s and %s are identical\n", files[0], files[1]);
      for(int i = 0; i < same_index; ++i){
        printf("Line %d: %s %50s %s", i, same[i][0], ">", same[i][1]);
      }
    }
  }

  if(_normal){
    normal(p, q);
  }

  if(side_by_side){
    sidebyside(p , q);
  }

  if(left){
    leftcolumn(p, q);
  }

  if(suppress_common){
    suppresscommon(p, q);
  }

  if(_context){
    context(p, q);
  }

  if(_unified){
    unified(p, q);
  }

  fclose(fin1);
  fclose(fin2);
}

void check_flags(int argc, const char* argv[]){
  while(argc-- > 0){
    const char* arg = *argv;
    set_flags(arg, "-v", "--version", &version);
    set_flags(arg, "-q", "--brief", &brief);
    //set_flags(arg, "-i",       "--ignore-case",              &ignorecase);
    set_flags(arg, "-s", "--report-identical-files", &identical_files);
    set_flags(arg, "--normal", NULL, &_normal);
    set_flags(arg, "-y", "--side-by-side", &side_by_side);
    set_flags(arg, "--left-column", NULL, &left);
    set_flags(arg, "--suppress-common-lines", NULL, &suppress_common);
    set_flags(arg, "-c", "--context", &_context);
    set_flags(arg, "-u", "showunified", &_unified);
  }

  if (!_context && !_unified && !side_by_side && !left) {_normal = ON;}
  if (version) { print_version();  exit(0); }
  if (((side_by_side || left) && (_normal || _context || _unified)) ||(_context && _unified) || (_normal && (_context || _unified))) {
    fprintf(stderr, "diff: Conflicting output style options\n");
    exit(1);
  }
}

void set_flags(const char* arg, const char* s, const char* t, int* value){
  if ((strcmp(arg, s) == 0) || ((t != NULL && strcmp(arg, t) == 0))) {
    *value = ON;
  }
}

int files_check(int argc, const char* argv[]){
  int count = 0;
  if (argc == 2){ return 2;}
  if(argc < 2){
    return 0;
  }
  for(int i = argc; i > 0; --i){
    if(strstr(argv[i], ".txt") != NULL){count++;}
    else if(count > 2 || count != 2){
      fprintf(stderr, "diff: This version of diff only handles two .txt files");
      exit(1);
    }
  }
  if(count == 0){
    fprintf(stderr, "diff: No file names have been inserted\n");
    exit(1);
  }
  return count;
}

int main(int argc, const char* argv[]){
  perform_flags(--argc, ++argv);
  return 0;
}
