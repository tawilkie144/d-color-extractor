#ifndef COMMON_H
#define COMMON_H

#define APP_NAME "DominantColors"

#define max(x,y) (x > y ? x : y)
#define min(x,y) (x < y ? x : y)


//===========================================================================//
//----------------------------------Records----------------------------------//
//===========================================================================//

typedef enum ColorRepresentation{
  kRGB,
  kHSV
} ColorRep_t;

/**
 * @brief 
 * 
 */
typedef struct pixel{
  ColorRep_t representation;
  float *values;
  float *maximums;
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
pixel_t *create_pixel(int depth, float *data, ColorRep_t type);
pixel_t *copy_pixel(pixel_t *in_pixel);
void print_pixel(pixel_t *in_pixel, int verbose);
void destroy_pixel(pixel_t *);
void convert_rgb_hsv(float r, float g, float b,
                     /*out*/float *h,/*out*/float *s,/*out*/float *v);
void print_image(image_t *image);
pixel_t *rotate_rgb(pixel_t *pixel, float rotation);
#endif