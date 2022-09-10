#include <turbojpeg.h>
#include <stdio.h>

#include "jpeg.h"
#define ERROR_HANDLE(message){\
  printf("Error encountered: %s", message);\
  r_val = -1;\
  goto bailout; \
}

int decompress_image(char *file_path){
  tjscalingfactor scaling_factor = { 1, 1 };
  int pixel_format = TJPF_UNKNOWN;
  FILE *jpeg_file;
  long file_size;
  int in_subsample, in_colorspace;
  int out_subsample, out_colorspace;
  int width, height;
  unsigned long jpeg_size;
  unsigned char *jpeg_buff;
  unsigned char *img_buff;
  tjhandle jpeg_decompressor;
  int r_val = 0;

  if ((jpeg_file = fopen(file_path, "rb")) == NULL)
    ERROR_HANDLE("cannot open file!");
  if (fseek(jpeg_file, 0, SEEK_END) < 0 || ((file_size = ftell(jpeg_file)) < 0) ||
      fseek(jpeg_file, 0, SEEK_SET) < 0)
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

  if (tjDecompressHeader3(jpeg_decompressor, jpeg_buff, jpeg_size, &width, &height,
                          &in_subsample, &in_colorspace) < 0)
    ERROR_HANDLE("reading JPEG header");

  printf("Input Image:  %d x %d pixels",
          width, height);

  width = TJSCALED(width, scaling_factor);
  height = TJSCALED(height, scaling_factor);
  if (out_subsample < 0)
    out_subsample = in_subsample;

  pixel_format = TJPF_BGRX;
  if ((img_buff = (unsigned char *)tjAlloc(width * height *
                                          tjPixelSize[pixel_format])) == NULL)
    ERROR_HANDLE("allocating uncompressed image buffer");

  if (tjDecompress2(jpeg_decompressor, jpeg_buff, jpeg_size, img_buff, width, 0, height,
                    pixel_format, 0) < 0)
    ERROR_HANDLE("decompressing JPEG image");
  tjFree(jpeg_buff);  jpeg_buff = NULL;
  tjDestroy(jpeg_decompressor);  jpeg_decompressor = NULL;




bailout:
  if(jpeg_file) fclose(jpeg_file);
  if(jpeg_buff) tjFree(jpeg_buff);
  if(img_buff) tjFree(jpeg_buff);
  if(jpeg_decompressor) tjDestroy(jpeg_decompressor);
  return r_val;
}