#include <stdio.h>
#include <stdlib.h>
#include <png.h>
#include "icon.h"

#define ICON_WIDTH 128
#define ICON_HEIGHT 128

// Function to load an icon from a PNG file
int load_png_icon(const char* filepath, Icon* icon)
{
  // Open the file
  FILE* fp = fopen(filepath, "rb");
  if (fp == NULL) {
    fprintf(stderr, "Failed to open file: %s\n", filepath);
    return -1;
  }

  // Read the PNG header
  png_byte header[8];
  fread(header, 1, 8, fp);

  // Check that the file is a PNG image
  if (png_sig_cmp(header, 0, 8)) {
    fprintf(stderr, "File is not a PNG image: %s\n", filepath);
    fclose(fp);
    return -1;
  }

  // Create a PNG read structure
  png_structp png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING,
					       NULL, NULL, NULL);
  if (png_ptr == NULL) {
    fprintf(stderr, "Failed to create PNG read structure\n");
    fclose(fp);
    return -1;
  }

  // Create a PNG info structure
  png_infop info_ptr = png_create_info_struct(png_ptr);
  if (info_ptr == NULL) {
    fprintf(stderr, "Failed to create PNG info structure\n");
    png_destroy_read_struct(&png_ptr, NULL, NULL);
    fclose(fp);
    return -1;
  }

  // Set error handling for the PNG read structure
  if (setjmp(png_jmpbuf(png_ptr))) {
    fprintf(stderr, "Error reading PNG image: %s\n", filepath);
    png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
    fclose(fp);
    return -1;
  }

  // Set the input function for the PNG read structure
  png_init_io(png_ptr, fp);

  // Set the PNG read structure
  // Set the PNG read structure to read from the beginning of the file
  png_set_sig_bytes(png_ptr, 8);

  // Read the image info
  png_read_info(png_ptr, info_ptr);

  // Get the width and height of the image
  int width = png_get_image_width(png_ptr, info_ptr);
  int height = png_get_image_height(png_ptr, info_ptr);

  // Check that the image has the correct dimensions
  if (width != ICON_WIDTH || height != ICON_HEIGHT) {
    fprintf(stderr, "Invalid image dimensions: %dx%d (expected %dx%d)\n",
	    width, height, ICON_WIDTH, ICON_HEIGHT);
    png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
    fclose(fp);
    return -1;
  }

  // Set the image transformation
  png_set_interlace_handling(png_ptr);
  png_set_scale_16(png_ptr);
  png_read_update_info(png_ptr, info_ptr);

  // Allocate memory for the pixel data
  png_bytep* row_pointers =
    (png_bytep*) malloc(ICON_HEIGHT * sizeof(png_bytep));
  if (row_pointers == NULL) {
    fprintf(stderr, "Failed to allocate memory for row pointers\n");
    png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
    fclose(fp);
    return -1;
  }
  icon->pixels = (uint32_t*) malloc(ICON_WIDTH * ICON_HEIGHT * sizeof(uint32_t));
  if (icon->pixels == NULL) {
    fprintf(stderr, "Failed to allocate memory for pixels\n");
    free(row_pointers);
    png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
    fclose(fp);
    return -1;
  }

  // Set the row pointers
  for (int y = 0; y < ICON_HEIGHT; y++) {
    row_pointers[y] = (png_byte*) (icon->pixels + y * ICON_WIDTH);
  }

  // Read the pixel data
  png_read_image(png_ptr, row_pointers);

  // Clean up
  free(row_pointers);
  png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
  fclose(fp);

  return 0;
}
