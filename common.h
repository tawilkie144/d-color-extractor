#ifndef COMMON_H
#define COMMON_H

#define DATA_DEPTH 255

typedef struct image{
    unsigned char *image_data;
    int width;
    int height;
    int bytes_per_pixel;
  } image_t;

#endif