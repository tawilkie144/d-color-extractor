#include <turbojpeg.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/stat.h>
#include <string.h>

#include "jpeg.h"
#include "../common.h"


#define ERROR_HANDLE(message){\
  printf("Error encountered: %s \\n", message);\
  r_val = -1;\
  goto bailout; \
}

pixel_t **format_as_rgb_pixels(float *image_data, int number_of_pixels,
                               int number_of_channels);
pixel_t **format_as_hsv_pixels(float *image_data, int number_of_pixels,
                               int number_of_channels);

int decompress_image(char *file_path, image_t *image_spec,
                     ColorRep_t prefered_representation){
  int pixel_format = TJPF_UNKNOWN;
  FILE *jpeg_file;
  long file_size;
  int in_subsample, in_colorspace;
  unsigned long jpeg_size;
  unsigned char *jpeg_buff;
  unsigned char *image_data;
  tjhandle jpeg_decompressor;
  int r_val = 0;
  struct stat st = {0};

  if ((jpeg_file = fopen(file_path, "rb")) == NULL)
    ERROR_HANDLE("cannot open file!");
  if (fseek(jpeg_file, 0, SEEK_END) < 0 ||
      ((file_size = ftell(jpeg_file)) < 0) || fseek(jpeg_file, 0, SEEK_SET) < 0)
        ERROR_HANDLE("Cannot read file");
  if (file_size == 0)
    ERROR_HANDLE("File is empty");
  jpeg_size = (unsigned long)file_size;

  if ((jpeg_buff = (unsigned char *)tjAlloc(jpeg_size)) == NULL)
    ERROR_HANDLE("allocating JPEG buffer");
  if (fread(jpeg_buff, jpeg_size, 1, jpeg_file) < 1)
    ERROR_HANDLE("reading input file");
  fclose(jpeg_file);  
  jpeg_file = NULL;

  if ((jpeg_decompressor = tjInitDecompress()) == NULL){
    ERROR_HANDLE("initializing decompressor");
  }

  if (tjDecompressHeader3(jpeg_decompressor, jpeg_buff, jpeg_size,
                          &image_spec->width, &image_spec->height,
                          &in_subsample, &in_colorspace) < 0)
    ERROR_HANDLE("reading JPEG header");

  printf("Input Image:  %d x %d pixels \n",
          image_spec->width, image_spec->height);

  pixel_format = TJPF_RGB;
  image_spec->bytes_per_pixel = tjPixelSize[pixel_format];
  if ((image_data = (unsigned char *)tjAlloc(image_spec->width * 
                                             image_spec->height *
                                             tjPixelSize[pixel_format]))==NULL){
    ERROR_HANDLE("allocating uncompressed image buffer");
  }

  if (tjDecompress2(jpeg_decompressor, jpeg_buff, jpeg_size, image_data,
                    image_spec->width, 0, image_spec->height, pixel_format,
                    0) < 0){
    ERROR_HANDLE("decompressing JPEG image");
  }
  
  size_t size_of_array = image_spec->width * image_spec->height * 
                         image_spec->bytes_per_pixel;
  float *image_float_data = malloc(sizeof(float) * size_of_array);
  if(!image_float_data) goto bailout;
  for(size_t i = 0; i < size_of_array; i++){
    image_float_data[i] = (float)image_data[i];
  }

  pixel_t **(*format_func)(float *, int, int);
  switch(prefered_representation){
    case kRGB:
      format_func = &format_as_rgb_pixels;
      break;
    case kHSV:
    default:
      format_func = &format_as_hsv_pixels;
      break;
  }
  image_spec->pixel_data = format_func(image_float_data, 
                                       image_spec->width * image_spec->height,
                                       image_spec->bytes_per_pixel);

  char *env = getenv("HOME");
  strcat(env, "/Pictures/");
  strcat(env, APP_NAME);
  if(stat(env, &st) == -1){
    mkdir(env, 0700);
  }
  strcat(env, "/test.png");
  tjSaveImage(env,  image_data, image_spec->width,
              0, image_spec->height, pixel_format, 0);
  tjFree(jpeg_buff);  jpeg_buff = NULL;
  tjDestroy(jpeg_decompressor);  jpeg_decompressor = NULL;


bailout:
  if(jpeg_file) fclose(jpeg_file);
  if(jpeg_buff) tjFree(jpeg_buff);
  if(jpeg_decompressor) tjDestroy(jpeg_decompressor);
  if(image_data) tjFree(image_data);
  if(image_float_data) free(image_float_data);
  return r_val;
}

pixel_t **format_as_rgb_pixels(float *image_data, int number_of_pixels,
                               int number_of_channels){
  int i = 0;

  pixel_t **r_val = malloc(sizeof(pixel_t) * number_of_pixels);
  if(!r_val) return NULL;

  for(i = 0; i < number_of_pixels; i++){
    r_val[i] = create_pixel(number_of_channels, 
                            (image_data + (i * number_of_channels)), kHSV);
    if(!r_val[i]) goto bailout;
  }

  return r_val;

bailout:
  if(r_val){
    for(i = 0; i < number_of_pixels; i++){
      if(!r_val[i])
        break;
      destroy_pixel(r_val[i]);
    }
    free(r_val);
  }
  return NULL;
}

pixel_t **format_as_hsv_pixels(float *image_data, int number_of_pixels,
                               int number_of_channels){
  int i = 0;

  pixel_t **r_val = malloc(sizeof(pixel_t) * number_of_pixels);
  if(!r_val) return NULL;

  for(i = 0; i < number_of_pixels; i++){
    r_val[i] = create_pixel(number_of_channels,
                            (image_data + (i * number_of_channels)), kHSV);
    if(!r_val[i]) goto bailout;
  }

  return r_val;
bailout:
  if(r_val){
    for(int i = 0; i < number_of_pixels; i++){
      if(!r_val[i]) break;
      destroy_pixel(r_val[i]);
    }
    free(r_val);
  }
  return NULL;
  
}
