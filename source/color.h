#ifndef COLOR_H
#define COLOR_H

#define ANALOGOUS_SIZE 2
#define TRIADIC_SIZE 2
#define TETRADIC_SIZE 3

struct pixel_t;

typedef struct color{
  int channels;
  int monochrome_depth;
  int pixel_monochrome_num;
  pixel_t *color;
  pixel_t *complement;
  pixel_t **monochrome;
  pixel_t **analogous;
  pixel_t **triadic;
  pixel_t **tetradic;
} color_t;


color_t *create_color( pixel_t *value, int monochrome_num);
void print_colors(color_t *color);
void destroy_color(color_t *to_destroy);


#endif