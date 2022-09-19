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
#include "jpeg.h"
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

ext_assigned:
  image_t *image_spec = malloc(sizeof(image_t));
  switch(file_type){
    case JPEG:
      decompress_image(file_in, image_spec);
      break;
    default:
      printf("Invalid File Tpye %s", ext);
      break;
  }

  extract_dominant_colors(image_spec->image_data, 3, bucket_size);

  printf("colors returned: %d\n\nwidth: %d\nheight: %d\n",colors_to_return, image_spec->width, image_spec->height);

  for(int y = 0; y < image_spec->height; y++){
    for(int x = 0; x < image_spec->width; x++){
      int pixel = (y * x) + x;
      printf("(");
      for(int b = 0; b < image_spec->bytes_per_pixel; b++){
        int byte = (pixel * image_spec->bytes_per_pixel) + b;
        printf("%u", image_spec->image_data[byte]);
        if(b != image_spec->bytes_per_pixel - 1) printf(", ");
      }
      printf(")\t");
    }
    printf("\n");
  }

  jpg_free(image_spec->image_data);
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