#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

#include "common.h"
#include "models/histogram.h"
#include "image_analyzer.h"

int bucket_comparator(const void *lhs, const void *rhs);

color_t **extract_dominant_colors(int *number_to_return, pixel_t **image_data,
                             int data_count, int channels, int bucket_size)
{
  int size_of_histogram;

  int number_channel_buckets = ceil((double)DATA_DEPTH/(double)bucket_size);
  long temp = ceil(powl(number_channel_buckets, channels));
  if(temp >= INT_MAX){
    bucket_size = ceil(DATA_DEPTH/ceil(logl(INT_MAX - 1)/log(channels)));
    size_of_histogram = INT_MAX - 1;
  }else{
    size_of_histogram = temp;
  }
  
  histogram_t *histogram = create_histogram(size_of_histogram, bucket_size,
                                            number_channel_buckets);
  int loaded = hist_load_data(histogram, image_data, data_count, channels);
  printf("Loaded %d pixels\n\n", loaded);
  if(loaded < data_count){
    //not all data was processed
  }

  int adjusted_size = trim_histogram(histogram);

  printf("histogram size: %d\nadjusted size: %d\n\n", histogram->size, adjusted_size);

  qsort(histogram->values, histogram->capacity, sizeof(bucket_t *), bucket_comparator);

  for(int i = 0; i < histogram->capacity; i++){
    if(histogram->values[i]){
      printf("(");
      for(int j = 0; j < histogram->values[i]->data_size; j++){
        printf("%d", (int)histogram->values[i]->representative->values[j]);
        if(j!=histogram->values[i]->data_size - 1) printf("\t");
      }
      printf("):\t%d\n",histogram->values[i]->count);
    }
  }

  if(histogram->size < *number_to_return){
    *number_to_return = histogram->size;
    //if verbose
    printf("You have requested more colors that are present in the image.\n\
            Try reducing the bucket size, or the number of desired dominant \
            colors.\n There are %d colors in the image with the bucket size of\
            %d", histogram->size, bucket_size);
  } 

  color_t **r_val = malloc(sizeof(color_t *) * *number_to_return);
  for(int i = 0; i < *number_to_return; i++){
    r_val[i] = create_color(histogram->values[i]->representative);
  }

  destroy_histogram(histogram);

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