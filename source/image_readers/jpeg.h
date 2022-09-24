#ifndef JPEG_H
#define JPEG_H

struct image;
typedef struct image image_t;

int decompress_image(char *, image_t *);

#endif