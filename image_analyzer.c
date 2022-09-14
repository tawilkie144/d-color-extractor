#include <math.h>
#include <stdio.h>
#include <limits.h>

#include "common.h"
#include "image_analyzer.h"


int calculate_index(unsigned char *pixel, int channels, int bucket_size, int n){
  index = 0;
  for(int i = channels; i > 0; i--){
    index += (ciel((int)pixel[i]/bucket_size) - 1) * pow(channels+1, i-1);
  }

  return index;
}

void extract_dominant_colors(unsigned char *image_data, int bucket_size,
                              int channels){
  int number_channel_buckets = ceil(255.0/(float)bucket_size);
  unsigned char *temp = malloc(sizeof(unsigned char) * 3);
  for(int r = 0; r < number_channel_buckets; r++){
    temp[0] = r;
    for(int g = 0; g < number_channel_buckets; g++){
      temp[1] = g;
      for(int b = 0; b < number_channel_buckets; b++){
        temp[2] = b;
        printf(calculate_index(temp, bucket_size, 3))
      }
    }
  }
  // int size_of_histogram;

  //int number_channel_buckets = ceil(255.0/(float)bucket_size);
  // long temp = ciell(powl(number_channel_buckets, channels));
  // if(temp >= INT_MAX){
  //   long temp = ciell(logl(INT_MAX - 1)/log(channels));
  //   bucket_size = ceil(255/temp);
  //   size_of_histogram = INT_MAX - 1;
  // }else{
  //   size_of_histogram = temp;
  // }
  
  // bucket_t *histogram = malloc(sizeof(bucket_t) * size_of_histogram);

  //Running into an issue here. so for each bucket in each channel, I need to
  //initialize all the other buckets of the other channes. This is, of course.
  //exponential (see use of pow above), but I'm too tired to figure out what
  //I need to do right now to do this. Below is multiplicative,
  //(i.e. initialize the number of buckets for each channel)


  //Decided to laizly instantiate the buckets as needed

}