#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

#include "common.h"
#include "models/histogram.h"
#include "image_analyzer.h"

int bucket_comparator(const void *lhs, const void *rhs);

void extract_dominant_colors(pixel_t **image_data, int data_count,
                             int channels, int bucket_size){
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

  int adjusted_size = 0;
  int next_empty = 0;
  int r_ptr = histogram->capacity - 1;

  while(next_empty < r_ptr){
    while(next_empty < histogram->capacity 
          && histogram->values[next_empty] && next_empty <= r_ptr){
      next_empty++;
      adjusted_size++;
      if(next_empty == histogram->capacity) goto sorted;
    }
    if(!histogram->values[next_empty] && histogram->values[r_ptr]){
      histogram->values[next_empty] = histogram->values[r_ptr];
      histogram->values[r_ptr] = NULL;
      adjusted_size++;
      next_empty++;
    }
    while(r_ptr > 0 && !histogram->values[r_ptr] && next_empty < r_ptr){
      r_ptr--;
    }
  }

sorted:
  printf("histogram size: %d\nadjusted size: %d\n\n", histogram->size, adjusted_size);

  for(int i = 0; i < adjusted_size; i++){
    printf("(");
    for(int j = 0; j < histogram->values[i]->data_size; j++){
      printf("%d", (int)histogram->values[i]->representative->values[j]);
      if(j!=histogram->values[i]->data_size - 1) printf("\t");
    }
    printf("):\t%d\n",histogram->values[i]->count);
  }
  printf("\n\n");

  qsort(histogram->values,histogram->size,sizeof(bucket_t *),bucket_comparator);

  for(int i = 0; i < adjusted_size; i++){
    printf("(");
    for(int j = 0; j < histogram->values[i]->data_size; j++){
      printf("%d", (int)histogram->values[i]->representative->values[j]);
      if(j!=histogram->values[i]->data_size - 1) printf("\t");
    }
    printf("):\t%d\n",histogram->values[i]->count);
  }

  destroy_histogram(histogram);
}

int bucket_comparator(const void *lhs, const void *rhs){
  int l = ((bucket_t *)lhs)->count;
  int r = ((bucket_t *)rhs)->count;
   
   return (l-r);
}