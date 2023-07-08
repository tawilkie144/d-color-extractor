#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

#include "common.h"
#include "models/histogram.h"
#include "image_analyzer.h"
#include "color.h"

int bucket_comparator(const void *lhs, const void *rhs);

color_t **extract_dominant_colors(int *number_to_return, pixel_t **image_data,
                             int data_count, int channels, 
                             int buckets_per_channel)
{
  int size_of_histogram;
  float *bucket_size;

  float hsv_data_depth[3] = {360.f,1.f,1.f};

  if(data_count < 1) return NULL;

  bucket_size = calloc(sizeof(float), channels);
  for(int i = 0; i < channels;i++){
    bucket_size[i] = hsv_data_depth[i]/(float)buckets_per_channel;
  }

  size_of_histogram = pow(buckets_per_channel, channels);
  histogram_t *histogram = create_histogram(size_of_histogram, bucket_size,
                                            buckets_per_channel);
  int loaded = hist_load_data(histogram, image_data, data_count, channels);
  //if verbose
  printf("Loaded %d pixels", loaded);
  if(loaded < data_count){
    printf(": missing %d pixels", data_count - loaded);
  }
  printf("\n\n");

  int adjusted_size = trim_histogram(histogram);

  //TODO: Remove this printing stuff
  printf("histogram size: %d\nadjusted size: %d\n\n", histogram->size,
                                                      adjusted_size);

  qsort(histogram->values, histogram->capacity, sizeof(bucket_t *),
         bucket_comparator);

  //TODO: Remove this printing stuff
  for(int i = 0; i < histogram->capacity; i++){
    if(histogram->values[i]){
      print_pixel(histogram->values[i]->representative, 0);
      printf("):\t%d\n",histogram->values[i]->count);
    }
  }

  if(histogram->size < *number_to_return){
    *number_to_return = histogram->size;
    //if verbose
    printf("You have requested more colors that are present in the image.\n"
           "Try reducing the number of buckets, or the number of desired"
           "dominant colors.\nThere are %d colors in the image with"
           "%d buckets\n\n", histogram->size, buckets_per_channel);
  } 

  color_t **r_val = malloc(sizeof(color_t *) * *number_to_return);
  for(int i = 0; i < *number_to_return; i++){
    //TODO: need to set number of monochromes values
    r_val[i] = create_color(histogram->values[i]->representative, 1);
  }

  destroy_histogram(histogram);
  free(bucket_size);

  return r_val;
}

int bucket_comparator(const void *lhs, const void *rhs){
  bucket_t *l = *(bucket_t **)lhs;
  bucket_t *r = *(bucket_t **)rhs;

  if(!l) return 1;
  if(!r) return -1;


  int l_val = (*(bucket_t **)lhs)->count;
  int r_val = (*(bucket_t **)rhs)->count;
   
   return -(l_val-r_val);
}