#ifndef HISTOGRAM_H
#define HISTOGRAM_H

typedef struct pixel pixel_t;

//===========================================================================//
//----------------------------------Records----------------------------------//
//===========================================================================//

/**
 * @brief Represents a histogram bucket. Holds a representative of the values
 *        in this bucket. The composition of the representative should be
 *        decided by the needs of the applicaiton. 
 * 
 * @property count
 * @property representative
 */
typedef struct histogram_bucket{
  int count;
  int data_size;
  pixel_t *representative;
}bucket_t;

/**
 * @brief A simple histogram histogram. Relies on the @ref bucket_t container
 *        to store the actual data.
 * 
 * @property values
 * @property bucket_size
 * @property buckets_per_channel
 * @property size
 * @property capacity
 */
typedef struct histogram{
  //represents the buckets
  bucket_t **values;
  //represents how large the buckets are for each channel
  float *bucket_size;
  //how many buckets are allocated to each channel of the color
  int buckets_per_channel;
  //represents how many buckets are currently in use in the histogram
  int size;
  //represents how many buckets are in the histogram
  int capacity;
} histogram_t;


//===========================================================================//
//---------------------------------Functions---------------------------------//
//===========================================================================//

histogram_t *create_histogram(int size, float *bucket_size, int channel_size);
int hist_load_data(histogram_t *hist, pixel_t **data, int data_count, 
                   int data_size);
int trim_histogram(histogram_t *to_trim);
void destroy_histogram(histogram_t *to_destroy);

#endif