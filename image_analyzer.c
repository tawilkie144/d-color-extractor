#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

#include "common.h"
#include "models/histogram.h"
#include "models/pixel.h"
#include "image_analyzer.h"

void extract_dominant_colors(unsigned char *image_data, int channels,
                              int bucket_size){
  // FILE *temp_file = fopen("/home/timothy/Documents/Index.txt", "w");
  // int number_channel_buckets = ceil(255.0/(float)bucket_size);
  // unsigned char *temp = malloc(sizeof(unsigned char) * 3);
  // for(int r = 0; r < number_channel_buckets; r++){
  //   temp[0] = r;
  //   for(int g = 0; g < number_channel_buckets; g++){
  //     temp[1] = g;
  //     for(int b = 0; b < number_channel_buckets; b++){
  //       temp[2] = b;
  //       fprintf(temp_file, "(%d,%d,%d): %d\n", (int)temp[0], (int)temp[1], (int)temp[2], calculate_index(temp, 3, bucket_size));
  //     }
  //   }
  // }
  // free(temp);
  // fclose(temp_file);

  int size_of_histogram;

  int number_channel_buckets = ceil(255.0/(float)bucket_size);
  long temp = ciell(powl(number_channel_buckets, channels));
  if(temp >= INT_MAX){
    bucket_size = ceil(255/ciell(logl(INT_MAX - 1)/log(channels)));
    size_of_histogram = INT_MAX - 1;
  }else{
    size_of_histogram = temp;
  }
  
  histogram_t *histogram = create_histogram();


  //Running into an issue here. so for each bucket in each channel, I need to
  //initialize all the other buckets of the other channes. This is, of course.
  //exponential (see use of pow above), but I'm too tired to figure out what
  //I need to do right now to do this. Below is multiplicative,
  //(i.e. initialize the number of buckets for each channel)


  //Decided to laizly instantiate the buckets as needed

}