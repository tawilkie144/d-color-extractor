#ifndef D_COLOR_EXTRACTOR_H
#define D_COLOR_EXTRACTOR_H

//===========================================================================//
//---------------------------------Constants---------------------------------//
//===========================================================================//


// Completely arbitrary number of allowed colors for now.
#define MAX_NUM_COLORS 20
// Completely arbitrary bucket size
#define MAX_BUCKET_SIZE 85
// Three chanels, R,G, and B.
#define CHANELS 3

#define JPEG_EXTS 4

//===========================================================================//
//----------------------------------Records----------------------------------//
//===========================================================================//


typedef enum SUPPORTED_EXT{
  JPEG,
  undefined
} supported_ext_t;

/**
 * @brief Representation of a pixel with R,G,B, and Alpha values
 * 
 */
typedef struct pixel{
  unsigned char red;
  unsigned char green;
  unsigned char blue;
  unsigned char alpha;
}pixel_t;

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
  pixel_t representative;
}bucket_t;

const char * tpyes[] = {"jpg", "jfif", "pjpeg", "pjp"};

//===========================================================================//
//---------------------------------Functions---------------------------------//
//===========================================================================//

void print_help(char *);
int main(int, char **);

#endif