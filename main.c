/**
 * @file test.c
 * @author Timothy Wilkie (timothy.wilkie@gmail.com)
 * @brief 
 * @version 0.1
 * @date 2022-09-08
 * 
 * @copyright Released under the GNU GPLv3 Licence
 * @note Uses the jpeglib.h library distributed by libjpeg
 */

#include <stdlib.h>
#include <stdio.h>
#include <turbojpeg.h>
#include <getopt.h>
#include <errno.h>

// Completely arbitrary number of allowed colors for now.
#define MAX_NUM_COLORS 20

void print_help(char *file_name){
  char *out_string =  "Usage: %s file <options>\n"
                      "operations:\n"
                      "  %s file -h, --help\t\t\tPrints this message\n"
                      "  %s file -n colors, --number=colors\tSets the max number of dominant colors to return\n";
  printf(out_string, file_name, file_name);
}

int main(int argc, char **argv) {
  int option;                             //Value for getopt
  char *file_in;                          //The file path we are given
  int colors_to_return = 10;              //Number of dominant colors to find
  FILE *p_file;                           //File

      //Define the options we accept
  static char *VALID_OPTIONS = "hn:";

  int this_option_optind = optind ? optind : 1;
  int option_index = 0;
  static struct option long_options[] =
  {
    {"help",   no_argument,         0,  'h'},
    {"number", required_argument,   0,  'n'},
    {"verbose", no_argument,        0,   0},
    {0,         0,                  0,   0}
  };

  while(1){//parse options
    option = getopt_long(argc, argv, VALID_OPTIONS,
                         long_options, &option_index);
    if(option == -1){
      break;
    }
    switch(option){
      case (int)'n': //Set a custom number of colors to return
        char *p_end_char;
        long long temp = strtol(optarg, &p_end_char, 10);
        if(temp == 0 || errno == ERANGE || temp > MAX_NUM_COLORS){
          printf("Invalid option for -n = %s\n", optarg);
          print_help(argv[0]);
          exit(1);
        }
        colors_to_return = (int)temp;
        break;
      case (int)'h': //print help message
      case '?':
      default:
        print_help(argv[0]);
        exit(0);    
    }
  }

  if(optind >= argc || argc < 2){
    printf("ERROR: Must specify a file\n");
    print_help(argv[0]);
    exit(1);
  }else{
    file_in = argv[optind];
    p_file = fopen(file_in, "rb");
    if(!p_file){
      printf("ERROR: File %s does not exist\n", file_in);
      exit(1);
    }
  }

  printf("%s, %d\n", file_in, colors_to_return);

  return 0;
}