#include <math.h>
#include <stdlib.h>

#include "common.h"

float modulusf(float a, float n){
  float k = fmodf(a,n);
  return (k < 0) ? k + n : k;
}

void convert_rgb_hsv(float r, float g, float b,
              /*out*/float *h,/*out*/float *s,/*out*/float *v){
  float max_val = r > g ? r : g;
  max_val = max_val > b ? max_val : b;

  float min_val = r < g ? r : g;
  min_val = min_val < b ? min_val : b;

  if(max_val < 0.5F) max_val = 0.0F;
  if(min_val < 0.5F) min_val = 0.0F;
  if(max_val == 0){
    *s = 0;
    *v = 0;
    *h = 0;
  }else{
    float chroma = max_val - min_val;
    *v = max_val/255.0F;
    *s = *v == 0.0F ? 0.0F : chroma/((*v)*255);

    float h_prime;
    if(chroma < 0.5F){
      h_prime = 0.0F;
    }
    else if(max_val == r){
      h_prime = modulusf((g - b)/chroma, 6);
    }
    else if(max_val == g){
      h_prime = ((b - r)/chroma) + 2.0F;
    }
    else if(max_val == b){
      h_prime = ((r -g)/chroma) + 4.0F;
    }

    *h = 60 * h_prime;
  }
}

pixel_t *rotate_rgb(pixel_t *pixel, float rotation){
  if(pixel->channels != 3 && pixel->representation != kRGB) return NULL;
  float *values = malloc(sizeof(float)*pixel->channels);
  if(!values) return NULL;
  float h_max = 360.f;
  if(rotation < 0) rotation = h_max + rotation;
  convert_rgb_hsv(pixel->values[0],pixel->values[1],pixel->values[2],
                  &values[0],&values[1],&values[2]);
  values[0] = fmodf(values[0] + rotation, h_max);
  convert_rgb(values[0], values[1], values[2],
              &values[0],&values[1],&values[2]);
  pixel_t *r_value = malloc(sizeof(pixel_t));
  if(!r_value){
    free(values);
    return NULL;
  }
  r_value = create_pixel(pixel->channels, values, kRGB);
  free(values);
  return r_value;
}