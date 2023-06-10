#include <stdlib.h>
#include <limits.h>
#include <math.h>

#include "histogram.h"
#include "../common.h"

void destroy_histogram_bucket(bucket_t *to_destroy);
bucket_t *create_histogram_bucket(pixel_t *pixel, int data_size);
int calculate_index(float *bucket_size,pixel_t *value,int value_size,int base);
float moving_average(float last_average, float added, int new_count);
int add_to_bucket(bucket_t* bucket, pixel_t *pixel);

histogram_t *create_histogram(int size, float *bucket_size, int channel_size)
{
  histogram_t *r_value = malloc(sizeof(histogram_t));
  r_value->size = 0;
  r_value->capacity = size;
  r_value->buckets_per_channel = channel_size;
  r_value->bucket_size = calloc(size, sizeof(float));
  for(int i = 0; i<channel_size;i++){
    r_value->bucket_size[i] = bucket_size[i];
  }
  r_value->values = calloc(size, sizeof(bucket_t *));

  return r_value;
}

void destroy_histogram(histogram_t *to_destroy){
  if(!to_destroy){
    return;
  }
  if(to_destroy->values){
    for(int i = 0; i < to_destroy->capacity; i++){
      if(to_destroy->values[i]){
        destroy_histogram_bucket(to_destroy->values[i]);
      }
    }

    free(to_destroy->values);
  }
  if(to_destroy->bucket_size){
    free(to_destroy->bucket_size);
  }
  free(to_destroy);
}

int hist_load_data(histogram_t *hist, pixel_t **data, int data_count,
                   int data_size)
{
  if(!hist || !data || data_count < 1){
    return -1;
  }
  const ColorRep_t type = data[0]->representation;
  int added = 0;
  for(int i = 0; i < data_count; i++){
    if(data[i]->representation != type) return i - 1;
    int index_to_insert = calculate_index(hist->bucket_size, data[i], data_size,
                                          hist->buckets_per_channel);
    if(index_to_insert > hist->capacity || index_to_insert < 0) return i - 1;
    if(!hist->values[index_to_insert]){
      hist->size++;
      bucket_t *insert = create_histogram_bucket(data[i], data_size);
      if(!insert){
        return i-1;
      }
      hist->values[index_to_insert] = insert;
    }else{
      if(data_size != hist->values[index_to_insert]->data_size) return i-1;
      add_to_bucket(hist->values[index_to_insert], data[i]);
    }
    added++;
  }

  return added;
}

int trim_histogram(histogram_t *to_trim){
  int adjusted_size = 0;
  int next_empty = 0;
  int r_ptr = to_trim->capacity - 1;

  while(next_empty < r_ptr){
    while(next_empty < to_trim->capacity 
          && to_trim->values[next_empty] && next_empty <= r_ptr){
      next_empty++;
      adjusted_size++;
      if(next_empty == to_trim->capacity || next_empty > r_ptr) goto sorted;
    }
    if(next_empty < r_ptr && !to_trim->values[next_empty] 
       && to_trim->values[r_ptr]){
      to_trim->values[next_empty] = to_trim->values[r_ptr];
      to_trim->values[r_ptr] = NULL;
      adjusted_size++;
      next_empty++;
    }
    while(r_ptr > 0 && !to_trim->values[r_ptr] && next_empty < r_ptr){
      r_ptr--;
    }
  }

sorted:
  to_trim->values = realloc(to_trim->values, adjusted_size*sizeof(bucket_t *));
  to_trim->capacity = adjusted_size;

  return adjusted_size;
}

int calculate_index(float *bucket_size,pixel_t *value,int value_size,int base){
  float index = 0;
  for(int i = value_size-1; i >= 0; i--){
    //We make our last bucket bigger to hold the final value of 255
    int offset = fabs(value->maximums[i] - value->values[i]) < 0.05f ? 1 : 0;
    index += (floorf(value->values[i]/bucket_size[i]) - offset)
           * pow(base, (value_size-1)-i);
  }

  return (int)index;
}

float moving_average(float last_average, float added, int new_count){
  return ((new_count-1)*last_average + added)/new_count;
}

bucket_t *create_histogram_bucket(pixel_t *pixel, int pixel_depth){
  bucket_t *r_value = malloc(sizeof(bucket_t));
  r_value->count = 1;
  r_value->data_size = pixel_depth;
  r_value->representative = copy_pixel(pixel);

  return r_value;
}

int add_to_bucket(bucket_t *bucket, pixel_t *pixel){
  if(bucket->count == INT_MAX) return -1;
  if(!bucket->representative) return -1;
  if(pixel->channels != bucket->data_size) return -1;
  
  bucket->count++;
  float *old_avg = bucket->representative->values;
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
    destroy_pixel(to_destroy->representative);
  }
  free(to_destroy);
}