#include <stdlib.h>

#include "histogram.h"
#include "pixel.h"

histogram_t *create_histogram(){

  return NULL;
}

void destroy_histogram(histogram_t *to_destroy){

}

bucket_t *create_histogram_bucket(){

  return NULL;
}

void destroy_histogram_bucket(bucket_t *to_destroy){

}

int calculate_index(int bucket_size, unsigned char *value, int value_size){
  double index = 0;
  for(int i = value_size - 1; i >= 0; i--){
    index += ceil(((int *)value)[i]/bucket_size) * pow(value_size+1, (value_size-1) - i);
  }

  return (int)index;
}

double moving_average(double last_average, double added, int new_count){
  return ((new_count-1)*last_average + added)/new_count;
}

void add_to_average(bucket_t * bucket, pixel_t *pixel){
  bucket->count++;
  for(int i = 0; i < pixel->channels; i++){
    bucket->representative[i] = moving_average(bucket->representative[i],
                                               pixel->values[i],
                                               bucket->count);
  }
}

int add_to_histogram(histogram_t *histogram, unsigned char *pixel, int channels){
  int index = calculate_index(pixel, channels, histogram->bucket_size);
  if(!histogram->values[index]){
    histogram->values[index] = malloc(sizeof(bucket_t));
    histogram->values[index]->count = 1;
    histogram->values[index]->representative = malloc(sizeof(unsigned char) * channels);
    for(int i = 0; i < channels; i++){
      //histogram->values[index]->representative[i] = pixel[i];
    }
  }else{
    histogram->values[index]->count++;
    add_to_average(histogram->values[index], pixel);
  }
}
