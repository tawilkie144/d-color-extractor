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
 * @property size
 */
typedef struct histogram{
  bucket_t **values;
  int bucket_size;
  int size;
} histogram_t;


//===========================================================================//
//---------------------------------Functions---------------------------------//
//===========================================================================//

histogram_t *create_histogram(int size, int bucket_size);
int hist_load_data(histogram_t *hist, pixel_t **data, int data_count, int data_size);
void destroy_histogram(histogram_t *to_destroy);
int calculate_index(int bucket_size, pixel_t *pixel, int value_size);
double moving_average(double last_average, double added, int new_count);
int add_to_bucket(bucket_t* bucket, pixel_t *pixel);
int add_to_histogram(histogram_t *histogram, pixel_t *pixel,
                     int channels);

#endif