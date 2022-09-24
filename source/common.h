#ifndef COMMON_H
#define COMMON_H

#define DATA_DEPTH 255
#define DEPTH 4

//===========================================================================//
//----------------------------------Records----------------------------------//
//===========================================================================//

typedef struct pixel{
  unsigned char *values;
  int channels;
} pixel_t;

typedef struct image{
    pixel_t **pixel_data;
    int width;
    int height;
    int bytes_per_pixel;
  } image_t;

//===========================================================================//
//---------------------------------Functions---------------------------------//
//===========================================================================//

void destroy_image(image_t *);
pixel_t *create_pixel(int depth, unsigned char *data);
void destroy_pixel(pixel_t *);

#endif