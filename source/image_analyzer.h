#ifndef IMAGE_ANALYZER_H
#define IMAGE_ANALYZER_H

typedef struct pixel pixel_t;

void extract_dominant_colors(pixel_t **image_data, int data_count, int channels,
                             int bucket_size);


#endif