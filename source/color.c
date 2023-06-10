#include <stdlib.h>
#include <stdio.h>

#include "common.h"
#include "color.h"


pixel_t *calculate_complement(pixel_t *pixel);
pixel_t **calculate_monochromes(pixel_t *pixel, int number);
pixel_t **calculate_analogous(pixel_t *pixel);
pixel_t **calculate_triadic(pixel_t *pixel);
pixel_t **calculate_tetradic(pixel_t *pixel);

color_t *create_color(pixel_t *value, int monochrom_num){
  color_t *r_val = malloc(sizeof(color_t));
  r_val->channels = value->channels;
  r_val->monochrome_depth = 2;
  r_val->color = copy_pixel(value);
  r_val->complement = calculate_complement(value);
  r_val->monochrome = calculate_monochromes(value, monochrom_num);
  r_val->analogous = calculate_analogous(value);
  r_val->triadic = calculate_triadic(value);
  r_val->tetradic = calculate_tetradic(value);
  if(!r_val->color || !r_val->monochrome || !r_val->analogous 
  || !r_val->triadic || !r_val->tetradic || !r_val->complement){
    destroy_color(r_val);
    return NULL;
  }
  
  return r_val;
}

void print_colors(color_t *color){
  printf("%d\n", color->monochrome_depth);
}

void destroy_color(color_t *to_destroy){
  if(!to_destroy) return;
  int i;
  destroy_pixel(to_destroy->color);
  destroy_pixel(to_destroy->complement);
  for(i = 0; i < to_destroy->monochrome_depth; i++){
    destroy_pixel(to_destroy->monochrome[i]);
  }
  for(i = 0; i < ANALOGOUS_SIZE; i++){
    destroy_pixel(to_destroy->analogous[i]);
  }
  for(i = 0; i < TRIADIC_SIZE; i++){
    destroy_pixel(to_destroy->triadic[i]);
  }
  for(i = 0; i < TETRADIC_SIZE; i++){
    destroy_pixel(to_destroy->tetradic[i]);
  }
  if(to_destroy->monochrome) free(to_destroy->monochrome);
  if(to_destroy->analogous) free(to_destroy->analogous);
  if(to_destroy->triadic) free(to_destroy->triadic);
  if(to_destroy->tetradic) free(to_destroy->tetradic);
  free(to_destroy);
}

/**
 * @brief Calculate the oposite color from the base color in pixel
 * 
 * @fn In terms of HSV (or other cylindrical-doordinate color models), 
 * this is on the oposite side of the color wheel (hue offset by 180°).
 * In terms of additive color models like rgb, this is the maximum value of
 * each primary color (R,G,B) subtracted by the color value of the base color.
 * 
 * @param[in] pixel the representation of the base color
 * @return The complementary color
 */
pixel_t *calculate_complement(pixel_t *pixel){
  pixel_t *r_value;
  if(pixel->representation == kHSV){
    float *comp = malloc(sizeof(float)*pixel->channels);
    if(!comp) return NULL;
    comp[0] = fmodf(pixel->values[0] + 180, 360);
    comp[1] = pixel->values[1];
    comp[2] = pixel->values[2];
    r_value = create_pixel(pixel->channels, comp, kHSV);
    free(comp);
  }else if(pixel->representation == kRGB){
    float *comp = malloc(sizeof(float)*pixel->channels);
    if(!comp) return NULL;
    for(int i = 0; i < pixel->channels; i++){
      comp[i] = pixel->maximums[i] - pixel->values[i];
    }
    r_value = create_pixel(pixel->channels, comp, kRGB);
    free(comp);
  }
  return r_value;
}

pixel_t **calculate_monochromes(pixel_t *pixel, int number){
  pixel_t **r_value = malloc(sizeof(pixel_t *) * number);
  if(!r_value) return NULL;
  
  if(pixel->representation == kHSV){
    float initial = pixel->values[2];
    float interval = pixel->maximums[2];
    float threshold = 0.2f;
    int starting_index = 
    float offset = fmodf(initial, interval);
  }

  return NULL;
}

pixel_t **calculate_analogous(pixel_t *pixel){

  return NULL;
}

pixel_t **calculate_triadic(pixel_t *pixel){

  return NULL;
}

pixel_t **calculate_tetradic(pixel_t *pixel){

  return NULL;
}