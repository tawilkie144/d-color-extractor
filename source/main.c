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
#include <limits.h>
#include <math.h>

#include "d_color_extractor.h"
#include "image_analyzer.h"
#include "image_readers/jpeg.h"
#include "color.h"
#include "common.h"

int main(int argc, char **argv) {
  int i;
  int option;                     //Value for getopt
  char *file_in;                  //The file path we are given
  int colors_to_return = 10;      //Number of dominant colors to find
  int number_of_buckets = 17;     //number of buckets. for each of e.g. H,S,V
  int number_of_monochromes = 2;  //number of monochrome colors to find

  supported_ext_t file_type = undefined;//Type of file that is input
  ColorRep_t representation = kHSV;

  color_t **dominant_colors = NULL;

  //Define the options we accept
  static char *VALID_OPTIONS = "hn:b:r:m:";
  int option_index = 0;
  static struct option long_options[] =
  {
    {"help",            no_argument,        0,  'h'},
    {"number",          optional_argument,  0,  'n'},
    {"bucket",          optional_argument,  0,  'b'},
    {"representation",  optional_argument,  0,  'r'},
    {"monochrome",      optional_argument,  0,  'm'},
    {0,                 0,                  0,   0}
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
          if(temp <= 0 || errno == ERANGE || temp>MAX_NUM_BUCKETS_PER_CHANNEL){
            printf("Invalid option for -b = %s\n", optarg);
            print_help(argv[0]);
            exit(1);
          }
          number_of_buckets = (int)temp;
          break;
        case (int)'m': //Set number of monochrome colors to find
          temp = strtol(optarg, &p_end_char, 10);
          if(temp <= 0 || errno == ERANGE || temp>MAX_NUM_MONOCHROME_COLORS){
            printf("Invalid option for -m = %s\n", optarg);
            print_help(argv[0]);
            exit(1);
          }
          number_of_monochromes = (int)temp;
          break;
        case (int)'r':
          strcmp(optarg, "rgb") == 0 ? representation = kRGB : kHSV;
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
  char *ext = strrchr(file_in, '.');
  if(ext == NULL || strlen(ext) < 2) goto early_exit;
  ext++;
  for(i = 0; i < JPEG_EXTS; i++){
    if(strcmp(ext, types[i]) == 0){
      file_type = JPEG;
      goto ext_assigned;
    }
  }

ext_assigned: ;
  image_t *image_spec = malloc(sizeof(image_t));
  switch(file_type){
    case JPEG:
      decompress_image(file_in, image_spec, representation);
      break;
    default:
      printf("Invalid File Tpye %s\n", ext);
      goto early_exit;
      break;
  }
  print_image(image_spec);

  dominant_colors = extract_dominant_colors(&colors_to_return,
                                image_spec->pixel_data,
                                image_spec->height * image_spec->width,
                                image_spec->bytes_per_pixel, number_of_buckets);

early_exit:
  if(image_spec) destroy_image(image_spec);
  if(dominant_colors){
    for(int i = 0; i < colors_to_return; i++){
      destroy_color(dominant_colors[i]);
    }
    free(dominant_colors);
  }
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
                      "                  \tto 15 (0-14,15-29...239-255)\n";
  printf(out_string, file_name, file_name);
}

pixel_t *create_hsv_pixel(int depth, float *data){
  if(depth != 3) return NULL;
  pixel_t *r_value = malloc(sizeof(pixel_t));
  if(!r_value) return NULL;
  r_value->representation = kHSV;
  r_value->channels = depth;
  r_value->values = calloc(depth, sizeof(float));
  r_value->maximums = calloc(depth, sizeof(float));
  if(!r_value->values || !r_value->maximums){
    destroy_pixel(r_value);
    return NULL;
  }

  convert_rgb_hsv(data[0], data[1], data[2],
              &r_value->values[0], &r_value->values[1], &r_value->values[2]);
  r_value->maximums[0] = 360.0f;
  r_value->maximums[1] = r_value->maximums[2] = 1.0f;

  return r_value;
}

pixel_t *create_rgb_pixel(int depth, float *data){
  pixel_t *r_value = malloc(sizeof(pixel_t));
  if(!r_value) return NULL;
  r_value->representation = kRGB;
  r_value->channels = depth;
  r_value->values = calloc(depth, sizeof(float));
  r_value->maximums = calloc(depth, sizeof(float));
  if(!r_value->values || !r_value->maximums){
    destroy_pixel(r_value);
    return NULL;
  }

  for(int i = 0; i < depth; i++){
    r_value->maximums[i] = 255;
    r_value->values[i] = data[i];
  }

  return r_value;
}

pixel_t *create_pixel(int depth, float *data, ColorRep_t type){
  switch(type){
    case kHSV:
      return create_hsv_pixel(depth, data);
      break;
    case kRGB:
      return create_rgb_pixel(depth, data);
      break;
    default:
      return create_hsv_pixel(depth, data);
      break;
  }
}

pixel_t *copy_pixel(pixel_t *in_pixel){
  pixel_t *r_value = malloc(sizeof(pixel_t));
  if(!r_value) return NULL;
  r_value->representation = in_pixel->representation;
  r_value->channels = in_pixel->channels;
  r_value->values = calloc(r_value->channels, sizeof(float));
  r_value->maximums = calloc(r_value->channels, sizeof(float));

  if(!r_value->values || !r_value->maximums){
    destroy_pixel(r_value);
    return NULL;
  }

  for(int i = 0; i<r_value->channels; i++){
    r_value->maximums[i] = in_pixel->maximums[i];
    r_value->values[i] = in_pixel->values[i];
  }

  return r_value;
}

void print_pixel(pixel_t *in_pixel, int verbose){
  if(verbose){
    printf("Representation: %s\nSize %d\nvalues: ",
                             in_pixel->representation == kHSV ? "HSV" : "RGB",
                             in_pixel->channels);
  }
  printf("(");
  for(int i = 0; i < in_pixel->channels; i++){

    if(i != in_pixel->channels-1){
      printf("%6.2f, ", in_pixel->values[i]);
    }
    else{
      printf("%6.2f)", in_pixel->values[i]);
    }
  }
}

void destroy_pixel(pixel_t *to_destroy){
  if(!to_destroy) 
    return;
  if(to_destroy->values)
    free(to_destroy->values);
  if(to_destroy->maximums)
    free(to_destroy->maximums);
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

void print_image(image_t *image){
  for(int y = 0; y < image->height; y++){
    for(int x = 0; x < image->width; x++){
      print_pixel(image->pixel_data[(y * image->width) + x],0);
      printf(" ");
    }
    printf("\n");
  }
}