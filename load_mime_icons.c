#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <png.h>
#include "icon.h"

#define ICON_WIDTH 128
#define ICON_HEIGHT 128

// Function to load icons from a tab-separated text file
int load_mime_icons(const char* filename, Icon** icons, int* num_icons)
{
  // Open the file
  FILE* fp = fopen(filename, "r");
  if (fp == NULL) {
    fprintf(stderr, "Failed to open file: %s\n", filename);
    return -1;
  }

  // Read the file line by line
  char* buffer = NULL;
  size_t buffer_size = 0;
  while (getline(&buffer, &buffer_size, fp) != -1) {
    // Split the line into fields
    char* mime_type = strtok(buffer, "\t");
    char* filepath = strtok(NULL, "\t");

    // Allocate memory for a new Icon structure
    *icons = realloc(*icons, (*num_icons + 1) * sizeof(Icon));
    if (*icons == NULL) {
      fprintf(stderr, "Failed to allocate memory for icons\n");
      fclose(fp);
      return -1;
    }

    // Load the icon from the file
    if (load_png_icon(filepath, &(*icons)[*num_icons]) < 0) {
      fprintf(stderr, "Failed to load icon from file: %s\n", filepath);
      fclose(fp);
      return -1;
    }

    // Set the MIME type for the icon
    (*icons)[*num_icons].mime_type = strdup(mime_type);

    // Increment the number of icons
    (*num_icons)++;
  }

  // Free the buffer
  free(buffer);

  // Close the file
  fclose(fp);

  return 0;
}

