
typedef struct {
  char* name;
  char* mime_type;
} FileInfo;

int get_file_list(const char* dir_name, FileInfo** files, int* count);

