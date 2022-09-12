#include <math.h>
#include <limits.h>

#include "common.h"
#include "image_analyzer.h"

void extract_dominant_colors(unsigned char *image_data, int bucket_size,
                              int channels){
  int size_of_histogram;

  int number_channel_buckets = ceil(255.0/(float)bucket_size);
  long temp = ciell(powl(number_channel_buckets, channels));
  if(temp >= INT_MAX){
    long temp = ciell(logl(INT_MAX - 1)/log(channels));
    bucket_size = ceil(255/temp);
    size_of_histogram = INT_MAX - 1;
  }else{
    size_of_histogram = temp;
  }
  
  bucket_t *histogram = malloc(sizeof(bucket_t) * size_of_histogram);

  //Running into an issue here. so for each bucket in each channel, I need to
  //initialize all the other buckets of the other channes. This is, of course.
  //exponential (see use of pow above), but I'm too tired to figure out what
  //I need to do right now to do this. Below is multiplicative,
  //(i.e. initialize the number of buckets for each channel)

  for(int i = 0; i < channels; i++){
    for(int j = 0; j < number_channel_buckets; i++){
      bucket_t temp;
      temp.count=0;
      temp.representative = malloc(sizeof(unsigned char) * channels);
      for(int c = 0; c < channels; c++){
        temp.representative[c] = malloc(sizeof(unsigned char));
      }
      //histogram[]
    }
  }



  // for(int i = 0; i < channels; i++){
  //   for(int j = 0; j < number_channel_buckets; j++){
  //     for(int c = 0; c < channels; c++){
  //       free(histogram[i * j].representative[c]);
  //     }
  //     free(histogram[i * j].representative);
  //   }
  // }
  // free(histogram);

}