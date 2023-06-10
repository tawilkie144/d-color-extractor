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

#define hue_conv(n) (modulusf(n + (h/60.0f), 6))

void convert_hsv_rgb(float h, float s, float v,
      /*out*/float *r,/*out*/float *g,/*out*/float *b){
  *r = (v - (v * s * max(0, min(min(1,hue_conv(5)), 4-hue_conv(5)))))*255;
  *g = (v - (v * s * max(0, min(min(1,hue_conv(3)), 4-hue_conv(3)))))*255;
  *b = (v - (v * s * max(0, min(min(1,hue_conv(1)), 4-hue_conv(1)))))*255;
}