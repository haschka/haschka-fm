#include <dirent.h>
#include <sys/stat.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <magic.h>

typedef struct {
  char* name;
  char* mime_type;
} FileInfo;

int get_file_list(const char* dir_name, FileInfo** files, int* count) {

  DIR* dir;
  struct dirent* entry;
  struct stat st;
  char path[PATH_MAX];
  FileInfo* file;
  magic_t magic;
  const char* mime_type;
  int name_len, mime_type_len;

  /*  file = malloc(sizeof(FileInfo));
  if (file == NULL) {
    perror("malloc");
    return -1;
  }
  */
  dir = opendir(dir_name);
  if (dir == NULL) {
    perror("opendir");
    return -1;
  }

  *count = 0;
  *files = NULL;

  while ((entry = readdir(dir)) != NULL) {
    if ((entry->d_name[0] == '.' &&
	entry->d_name[1] == 0) ||
       (entry->d_name[0] == '.' &&
	entry->d_name[1] == '.' &&
	entry->d_name[1] == 0)) {
      continue;
    }

    snprintf(path, sizeof(path), "%s/%s", dir_name, entry->d_name);

    if (stat(path, &st) < 0) {
      perror("stat");
      continue;
    }

    *files = realloc(*files, (*count + 1) * sizeof(FileInfo));
    if (*files == NULL) {
      perror("realloc");
      file = &(*files)[*count];
      name_len = strnlen(entry->d_name, PATH_MAX) + 1;
      file->name = (char*)malloc(name_len);
      if (file->name == NULL) {
	perror("malloc");
	break;
      }
    }
    file = &(*files)[*count];
    name_len = strnlen(entry->d_name, PATH_MAX) + 1;
    file->name = (char*)malloc(name_len);
    memcpy(file->name, entry->d_name, name_len);
    
    if (S_ISDIR(st.st_mode)) {
      mime_type_len = strnlen("inode/directory", PATH_MAX) + 1;
      file->mime_type = (char*)malloc(mime_type_len);
      if (file->mime_type == NULL) {
	perror("malloc");
	break;
      }
      memcpy(file->mime_type, "inode/directory", mime_type_len);
    } else {
      magic = magic_open(MAGIC_MIME_TYPE);
      if (magic == NULL) {
	perror("magic_open");
	file->mime_type = NULL;
      } else {
	if (magic_load(magic, NULL) != 0) {
	  fprintf(stderr, "magic_load: %s\n", magic_error(magic));
	  file->mime_type = NULL;
	} else {
	  mime_type = magic_file(magic, path);
	  mime_type_len = strnlen(mime_type, PATH_MAX) + 1;
	  file->mime_type = (char*)malloc(mime_type_len);
	  if (file->mime_type == NULL) {
	    perror("malloc");
	    break;
	  }
	  memcpy(file->mime_type, mime_type, mime_type_len);
	  magic_close(magic);
	}
      }
    }
    
    (*count)++;
  }
  
  closedir(dir);
  return 0;
}
