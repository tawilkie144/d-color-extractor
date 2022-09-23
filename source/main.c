/**
 * @file main.c
 * @author Timothy Wilkie (timothy.wilkie@gmail.com)
 * @brief 
 * @version 0.1
 * @date 2022-09-08
 * 
 * @copyright Released under the GNU GPLv3 Licence
 * @note Uses the jpeglib-turbo library
 * @note Uses the jpeglib.h library distributed by libjpeg
 */

#include <errno.h>
#include <stdlib.h>
#include <stdio.h>
#include <getopt.h>
#include <errno.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>


#include "d_color_extractor.h"
#include "image_analyzer.h"
#include "image_readers/jpeg.h"
#include "common.h"

int main(int argc, char **argv) {
  int option;                     //Value for getopt
  char *file_in;                  //The file path we are given
  int colors_to_return = 10;      //Number of dominant colors to find
  int bucket_size = 15;           //Bucket size. i.e. for each of R,G,B
                                  //if a color is within 0-14 15-29...240-255
  supported_ext_t file_type = undefined;//Type of file that is input

  //Define the options we accept
  static char *VALID_OPTIONS = "hn:b:";
  int option_index = 0;
  static struct option long_options[] =
  {
    {"help",   no_argument,         0,  'h'},
    {"number", required_argument,   0,  'n'},
    {"bucket", required_argument,   0,  'b'},
    {0,         0,                  0,   0}
  };

  {//Just here to be able to collapse section while editing
  //TODO: Remove when this file is "done"
    while(1){//parse options
      option = getopt_long(argc, argv, VALID_OPTIONS,
                          long_options, &option_index);
      if(option == -1){
        break;
      }

      char *p_end_char; //used for strtol
      long long temp;
      switch(option){
        case (int)'n': //Set a custom number of colors to return
          temp = strtol(optarg, &p_end_char, 10);
          if(temp == 0 || errno == ERANGE || temp > MAX_NUM_COLORS){
            printf("Invalid option for -n = %s\n", optarg);
            print_help(argv[0]);
            exit(1);
          }
          colors_to_return = (int)temp;
          break;
        case (int)'b': //Set bucket size. default of 15
          temp = strtol(optarg, &p_end_char, 10);
          if(temp <= 0 || errno == ERANGE || temp > MAX_BUCKET_SIZE){
            printf("Invalid option for -b = %s\n", optarg);
            print_help(argv[0]);
            exit(1);
          }
          bucket_size = (int)temp;
          break;
        case (int)'h': //print help message
        case '?':
        default:
          print_help(argv[0]);
          exit(0);    
      }
    }

    if(optind >= argc || argc < 2){
      printf("ERROR: Must specify a file\n");
      print_help(argv[0]);
      exit(1);
    }else{
      file_in = argv[optind];
    }
  }

  //Get type of image (currently only JPEG)
  char *ext = strrchr(file_in, '.');
  for(int i = 0; i < JPEG_EXTS; i++){
    if(strcmp(ext, tpyes[i])){
      file_type = JPEG;
      goto ext_assigned;
    }
  }

ext_assigned: ;
  image_t *image_spec = malloc(sizeof(image_t));
  switch(file_type){
    case JPEG:
      decompress_image(file_in, image_spec);
      break;
    default:
      printf("Invalid File Tpye %s", ext);
      break;
  }

  extract_dominant_colors(image_spec->pixel_data, image_spec->height * image_spec->width, 3, bucket_size);

  printf("colors returned: %d\n\nwidth: %d\nheight: %d\n\n\nbucket size: %d\n",colors_to_return, image_spec->width, image_spec->height, bucket_size);

  for(int p = 0; p < image_spec->height * image_spec->width; p++){
    printf("(");
    for(int c = 0; c < image_spec->bytes_per_pixel; c++){
      printf("%03u", image_spec->pixel_data[p]->values[c]);
      if(c!=image_spec->bytes_per_pixel) printf("\t");
    }
    printf("\n");
  }

  destroy_image(image_spec);
  return 0;
}


void print_help(char *file_name){
  char *out_string =  "Usage: %s file <options>\n"
                      "operations:\n"
                      "  -h, --help      \tPrints this message\n"
                      "  -n #, --number=#\tSets the max number of dominant\n"
                      "                  \tcolors to return\n"
                      "  -b #, --bucket=#\tSets the size of buckets to\n"
                      "                  \tdifferentiate colors by. Defaults\n"
                      "                  \tto 15 (0-14,15-30...240-255)";
  printf(out_string, file_name, file_name);
}

pixel_t *create_pixel(int depth, unsigned char *data){
  pixel_t *r_value = malloc(sizeof(pixel_t));
  if(!r_value) return NULL;
  r_value->channels = depth;
  r_value->values = malloc(sizeof(unsigned char) * depth);
  if(!r_value->values){
    free(r_value);
    return NULL;
  }
  for(int i = 0; i < depth; i++){
    r_value->values[i] = data[i];
  }

  return r_value;
}

void destroy_pixel(pixel_t *to_destroy){
  if(!to_destroy) 
    return;
  if(to_destroy->values)
    free(to_destroy->values);
  free(to_destroy);
}

void destroy_image(image_t *to_destroy){
  if(!to_destroy)
    return;
  if(to_destroy->pixel_data){
    for(int i = 0; i < to_destroy->height * to_destroy->width; i++){
      if(to_destroy->pixel_data[i])
        destroy_pixel(to_destroy->pixel_data[i]);
    }
    free(to_destroy->pixel_data);
  }
  free(to_destroy);
}