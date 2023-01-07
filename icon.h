#define ICON_WIDTH 128
#define ICON_HEIGHT 128

#include <stdint.h>

typedef struct Icon {
    char* mime_type;
    uint32_t* pixels;
} Icon;

int load_png_icon(const char* filepath, Icon* icon);
