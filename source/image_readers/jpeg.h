#ifndef JPEG_H
#define JPEG_H

#include "../common.h"//for ColorRep_t....seems bad;

struct image;
typedef struct image image_t;

int decompress_image(char *, image_t *, ColorRep_t);

#endif