#include <stdlib.h>
#include <limits.h>
#include <math.h>

#include "histogram.h"
#include "../common.h"

void destroy_histogram_bucket(bucket_t *to_destroy);
bucket_t *create_histogram_bucket(pixel_t *pixel, int data_size);

histogram_t *create_histogram(int size, int bucket_size){
  histogram_t *r_value = malloc(sizeof(histogram_t));
  r_value->size = size;
  r_value->bucket_size = bucket_size;
  r_value->values = malloc(sizeof(bucket_t *) * size);

  return r_value;
}

int hist_load_data(histogram_t *hist, pixel_t **data, int data_count, int data_size){
  if(!hist || !data){
    return -1;
  }
  int count_added = 0;
  for(int i = 0; i < data_count; i++){
    int index_to_insert = calculate_index(hist->bucket_size, data[i], data_size);
    if(index_to_insert > hist->size) return i -1;
    if(!hist->values[index_to_insert]){
      bucket_t *insert = create_histogram_bucket(data[i], data_size);
      if(!insert){
        return i-1;
      }
      hist->values[index_to_insert] = insert;
    }else{
      if(data_size != hist->values[index_to_insert]->data_size) return i-1;
      add_to_bucket(hist->values[index_to_insert], data[i]);
    }
    count_added++;
  }

  return count_added;
}

void destroy_histogram(histogram_t *to_destroy){
  if(!to_destroy){
    return;
  }
  if(to_destroy->values){
    for(int i = 0; i < to_destroy->size; i++){
      if(to_destroy->values[i]){
        destroy_histogram_bucket(to_destroy->values[i]);
      }
    }

    free(to_destroy->values);
  }
  free(to_destroy);
}

bucket_t *create_histogram_bucket(pixel_t *pixel, int pixel_depth){
  bucket_t *r_value = malloc(sizeof(bucket_t));
  r_value->count = 1;
  r_value->data_size = pixel_depth;
  r_value->representative = malloc(sizeof(unsigned char) * pixel_depth);
  for(int i = 0; i < pixel_depth; i++){
    r_value->representative->values[i] = pixel->values[i];
  }

  return r_value;
}

int add_to_bucket(bucket_t *bucket, pixel_t *pixel){
  if(bucket->count == INT_MAX) return -1;
  if(!bucket->representative) return -1;
  if(pixel->channels != bucket->data_size) return -1;
  
  bucket->count++;
  unsigned char *old_avg = bucket->representative->values;
  for(int i = 0; i < pixel->channels; i++){
    old_avg[i] = moving_average(old_avg[i], pixel->values[i], bucket->count);
  }

  return 0;
}

void destroy_histogram_bucket(bucket_t *to_destroy){
  if(!to_destroy){
    return;
  }
  if(to_destroy->representative){
    free(to_destroy->representative);
  }
  free(to_destroy);
}

int calculate_index(int bucket_size, pixel_t *value, int value_size){
  double index = 0;
  for(int i = value_size - 1; i >= 0; i--){
    index += ceil(((int *)value)[i]/bucket_size) * pow(value_size+1, (value_size-1) - i);
  }

  return (int)index;
}

double moving_average(double last_average, double added, int new_count){
  return ((new_count-1)*last_average + added)/new_count;
}
