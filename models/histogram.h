#ifndef HISTOGRAM_H
#define HISTOGRAM_H

struct pixel_t;

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
  unsigned char *representative;
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

histogram_t *create_histogram();
void destroy_histogram(histogram_t *to_destroy);
bucket_t *create_histogram_bucket();
void destroy_histogram_bucket(bucket_t *to_destroy);
int calculate_index(int bucket_size, unsigned char *value, int value_size);
double moving_average(double last_average, double added, int new_count);
void add_to_average(bucket_t * bucket, pixel_t *pixel);
int add_to_histogram(histogram_t *histogram, unsigned char *pixel,
                     int channels);

#endif