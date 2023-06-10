#ifndef D_COLOR_EXTRACTOR_H
#define D_COLOR_EXTRACTOR_H

//===========================================================================//
//---------------------------------Constants---------------------------------//
//===========================================================================//


// Completely arbitrary number of allowed colors for now.
#define MAX_NUM_COLORS 20
// Ensure that if using RGB, each bucket is at least three pixels
#define MAX_NUM_BUCKETS_PER_CHANNEL 85
//Completely arbitrary number of allowed monochrome colors to find
#define MAX_NUM_MONOCHROME_COLORS 5

#define JPEG_EXTS 4

//===========================================================================//
//----------------------------------Records----------------------------------//
//===========================================================================//


typedef enum SUPPORTED_EXT{
  JPEG,
  undefined
} supported_ext_t;

const char * types[] = {"jpg", "jpeg", "jfif", "pjpeg", "pjp"};

//===========================================================================//
//---------------------------------Functions---------------------------------//
//===========================================================================//

void print_help(char *);
int main(int, char **);

#endif