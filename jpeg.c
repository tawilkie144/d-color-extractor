#include <turbojpeg.h>
#include <stdio.h>

#include "jpeg.h"
#include "common.h"


#define ERROR_HANDLE(message){\
  printf("Error encountered: %s \\n", message);\
  r_val = -1;\
  goto bailout; \
}

int decompress_image(char *file_path, image_t *image_spec){
  int pixel_format = TJPF_UNKNOWN;
  FILE *jpeg_file;
  long file_size;
  int in_subsample, in_colorspace;
  unsigned long jpeg_size;
  unsigned char *jpeg_buff;
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

  if (tjDecompressHeader3(jpeg_decompressor, jpeg_buff, jpeg_size, &image_spec->width, &image_spec->height,
                          &in_subsample, &in_colorspace) < 0)
    ERROR_HANDLE("reading JPEG header");

  printf("Input Image:  %d x %d pixels \n",
          image_spec->width, image_spec->height);

  pixel_format = TJPF_RGBA;
  image_spec->bytes_per_pixel = tjPixelSize[pixel_format];
  if ((image_spec->image_data = (unsigned char *)tjAlloc(image_spec->width * image_spec->height *
                                          tjPixelSize[pixel_format])) == NULL)
    ERROR_HANDLE("allocating uncompressed image buffer");

  if (tjDecompress2(jpeg_decompressor, jpeg_buff, jpeg_size, image_spec->image_data, image_spec->width, 0, image_spec->height,
                    pixel_format, 0) < 0)
    ERROR_HANDLE("decompressing JPEG image");

  tjSaveImage("/home/timothy/pictures/test.png",  image_spec->image_data, image_spec->width, 0, image_spec->height, pixel_format, 0);
  tjFree(jpeg_buff);  jpeg_buff = NULL;
  tjDestroy(jpeg_decompressor);  jpeg_decompressor = NULL;

  printf("Bytes per Line: %d\n\n", image_spec->width * tjPixelSize[pixel_format]);


bailout:
  if(jpeg_file) fclose(jpeg_file);
  if(jpeg_buff) tjFree(jpeg_buff);
  if(jpeg_decompressor) tjDestroy(jpeg_decompressor);
  return r_val;
}


void jpg_free(unsigned char *img_buff){
  if(img_buff) tjFree(img_buff);
}