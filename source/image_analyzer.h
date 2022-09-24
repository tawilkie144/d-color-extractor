#ifndef IMAGE_ANALYZER_H
#define IMAGE_ANALYZER_H

typedef struct pixel pixel_t;
typedef struct color color_t;

color_t **extract_dominant_colors(int *number_to_return, pixel_t **image_data,
                             int data_count, int channels, int bucket_size);


#endif