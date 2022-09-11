#ifndef JPEG_H
#define JPEG_H

#define COLOR_COMPONENTS 3

struct image;
typedef struct image image_t;

int decompress_image(char *, image_t *);

void jpg_free(unsigned char *);

#endif