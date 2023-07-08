#include <stdlib.h>
#include <stdio.h>

#include "common.h"
#include "color.h"


pixel_t *calculate_complement(pixel_t *pixel);
pixel_t **calculate_monochromes(pixel_t *pixel, int number, int *indx);
pixel_t **calculate_analogous(pixel_t *pixel);
pixel_t **calculate_triadic(pixel_t *pixel);
pixel_t **calculate_tetradic(pixel_t *pixel);

color_t *create_color(pixel_t *value, int monochrom_num){
  color_t *r_val = malloc(sizeof(color_t));
  r_val->channels = value->channels;
  r_val->monochrome_depth = 2;
  r_val->color = copy_pixel(value);
  r_val->complement = calculate_complement(value);
  r_val->monochrome = calculate_monochromes(value, monochrom_num,
                                            &r_val->pixel_monochrome_num);
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

pixel_t **calculate_monochromes(pixel_t *pixel, int number, /*out*/int *indx){
  pixel_t **r_value = malloc(sizeof(pixel_t *) * number);
  if(!r_value) return NULL;
  float min_sat = 0.3f;
  float max_sat = 0.9f;
  
  if(pixel->representation == kHSV){
    int i = 0;
    float init = min(max(pixel->values[2], min_sat), max_sat);
    float intvl = (pixel->maximums[2] - (min_sat + 1 - max_sat)) / number;

    if(init == min_sat){
      *indx = 0;
    }else{
      float init_inv = 1.0f / init;
      float intvl_inv = 1.0f / intvl;

      //calculate which is the next interval that is greater than our input
      float intvl_up = ceilf(init * intvl_inv) * intvl > max_sat
                     ? max_sat : ceilf(init * intvl_inv) * intvl;
      //calculate which is the next interval that is less than our input
      float intvl_down = floorf(init * intvl_inv) * intvl < min_sat 
                       ? min_sat : floorf(init * intvl_inv) * intvl;

      //Calculate which index our input color would be on the scale that 
      //equally partitions the range of saturation values
      *indx = ceil(((fabsf(intvl_up - init) < fabsf(intvl_down - init) 
            ? intvl_up : intvl_down) - min_sat) * intvl_inv);
    }

    float *values = malloc(sizeof(float) * pixel->channels);
    if(!values){
      free(r_value);
      return NULL;
    }

    for(i = 0; i < pixel->channels; i++){
      values[i] = pixel->values[i];
    }

    int offset = 0;
    for(i = 0; i < number; i++){
      if(i == *indx) offset++;
      values[2] = (i + offset) * intvl;
      r_value[i] = create_pixel(pixel->channels, values, kHSV);
    }

    
    free(values);
    return r_value;
  }else if(pixel->representation == kRGB){
    int max_idx = pixel->values[0] > pixel->values[1] ? 0 : 1;
    max_idx = pixel->values[max_idx] > pixel->values[2] ? max_idx : 2;

    int min_idx = pixel->values[0] < pixel->values[1] ? 0 : 1;
    min_idx = pixel->values[min_idx] < pixel->values[2] ? min_idx : 2;

    if(pixel->values[min_idx] == pixel->values[max_idx]){

    }else{
      float chroma = pixel->values[max_idx] - pixel->values[min_idx];
      float sat = chroma/pixel->values[max_idx];
    }
  }

  return NULL;
}

pixel_t **calculate_analogous(pixel_t *pixel){
  pixel_t **r_value = malloc(sizeof(pixel_t *) * ANALOGOUS_SIZE);
  if(!r_value) return NULL;
  float rotation = 30.f;
  
  if(pixel->representation == kHSV){
    float *values = malloc(sizeof(float) * pixel->channels);
    if(!values){
      free(r_value);
      return NULL;
    }
    unsigned char index;
    values[0] = fmodf(pixel->values[0] + rotation, pixel->maximums[0]);
    values[1] = pixel->values[1];
    values[2] = pixel->values[2];
    index = values[0] > pixel->values[0] ? 1 : 0;
    r_value[index] = create_pixel(pixel->channels, values,
                                  pixel->representation);                          
    values[0] = fmodf(pixel->values[0] + (pixel->maximums[0]-rotation),
                      pixel->maximums[0]);
    index = index == 1 ? 0 : 1;  
    r_value[index] = create_pixel(pixel->channels, values,
                                  pixel->representation);
    free(values);
    return r_value;
  }else if(pixel->representation == kRGB){
    //For RGB we convert to HSV, rotate, then convert back to RGB.
    //Not very preformant
    r_value[0] = rotate_rgb(pixel, rotation);
    r_value[1] = rotate_rgb(pixel, 360.f-rotation);
  }

  return NULL;
}

pixel_t **calculate_triadic(pixel_t *pixel){
  pixel_t **r_value = malloc(sizeof(pixel_t *) * TRIADIC_SIZE);
  if(!r_value) return NULL;
  float rotation = 120.f;
  
  if(pixel->representation == kHSV){
    float *values = malloc(sizeof(float) * pixel->channels);
    unsigned char index;
    values[0] = fmodf(pixel->values[0] + rotation, pixel->maximums[0]);
    values[1] = pixel->values[1];
    values[2] = pixel->values[2];
    index = values[0] > pixel->values[0] ? 1 : 0;
    r_value[index] = create_pixel(pixel->channels, values,
                                  pixel->representation);                          
    values[0] = fmodf(pixel->values[0] + (pixel->maximums[0]-rotation),
                      pixel->maximums[0]);
    index = index == 1 ? 0 : 1;  
    r_value[index] = create_pixel(pixel->channels, values,
                                  pixel->representation);
    free(values);
    return r_value;
  }else if(pixel->representation == kRGB){
    r_value[0] = rotate_rgb(pixel, rotation);
    r_value[1] = rotate_rgb(pixel, 360.f-rotation);
    return r_value;
  }

  return NULL;
}

pixel_t **calculate_tetradic(pixel_t *pixel){
  pixel_t **r_value = malloc(sizeof(pixel_t *) * TETRADIC_SIZE);
  if(!r_value) return NULL;
  float rotation = 90.f;
  if(pixel->representation == kHSV){
    float *values = malloc(sizeof(float) * pixel->channels);
    values[1] = pixel->values[1];
    values[2] = pixel->values[2];
    for(int i = 0; i < TETRADIC_SIZE; i++){
      values[0] = fmodf(pixel->values[0] + (rotation * i), pixel->maximums[0]);
      r_value[i] = create_pixel(pixel->channels, values,
                                pixel->representation);    
    }
    free(values);
    return r_value;
  }else if(pixel->representation == kRGB){
    for(int i = 0; i < TETRADIC_SIZE; i++){
      r_value[i] = rotate_rgb(pixel, i * rotation);
    }
    return r_value;
  }

  return NULL;
}