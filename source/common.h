#ifndef COMMON_H
#define COMMON_H

#define DATA_DEPTH 255

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

typedef struct color{
  int channels;
  int monochrome_depth;
  int analogous_count;
  pixel_t *color;
  pixel_t *complement;
  pixel_t **monochrome;
  pixel_t **analogous;
  pixel_t **triadic;
  pixel_t **tetradic;
} color_t;

//===========================================================================//
//---------------------------------Functions---------------------------------//
//===========================================================================//

void destroy_image(image_t *);
pixel_t *create_pixel(int depth, unsigned char *data);
void destroy_pixel(pixel_t *);
color_t *create_color(pixel_t *value);
void print_colors(color_t *color);
void destroy_color(color_t *to_destroy);

#endif