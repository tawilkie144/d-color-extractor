#ifndef IMAGE_ANALYZER_H
#define IMAGE_ANALYZER_H

/**
 * @brief Represents a histogram bucket for colors Keeps a count of the number
 *        of pixels in the range as well as hold a representative color. In the
 *        implementation, the representative is the average of all colors in
 *        the bucket 
 * 
 * @property count
 * @property representative
 */
typedef struct histogram_bucket{
  int count;
  unsigned char *representative;
}bucket_t;

void extract_dominant_colors(unsigned char *, int, int);


#endif