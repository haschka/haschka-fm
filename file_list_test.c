#include <stdio.h>
#include <stdlib.h>

#include "file_list.h"

int main(int argc, char** argv) {

  int i;
  FileInfo* files;
  int count;
  
  if (argc != 2) {
    fprintf(stderr, "Usage: %s <directory>\n", argv[0]);
    return 1;
  }

  if (get_file_list(argv[1], &files, &count) < 0) {
    fprintf(stderr, "Error getting file list\n");
    return 1;
  }

  for (i = 0; i < count; i++) {
    printf("%s: %s\n", files[i].name, files[i].mime_type);
    free(files[i].name);
    free(files[i].mime_type);
  }

  free(files);
  return 0;
}
