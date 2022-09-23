#ifndef D_COLOR_EXTRACTOR_H
#define D_COLOR_EXTRACTOR_H

//===========================================================================//
//---------------------------------Constants---------------------------------//
//===========================================================================//


// Completely arbitrary number of allowed colors for now.
#define MAX_NUM_COLORS 20
// Completely arbitrary bucket size
#define MAX_BUCKET_SIZE 85
// Three chanels, R,G,B, and A.
#define CHANELS 4

#define JPEG_EXTS 4

//===========================================================================//
//----------------------------------Records----------------------------------//
//===========================================================================//


typedef enum SUPPORTED_EXT{
  JPEG,
  undefined
} supported_ext_t;

const char * tpyes[] = {"jpg", "jfif", "pjpeg", "pjp"};

//===========================================================================//
//---------------------------------Functions---------------------------------//
//===========================================================================//

void print_help(char *);
int main(int, char **);

#endif