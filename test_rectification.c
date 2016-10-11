/* Check whether two images are the same */
#include "lodepng.h"
#include <stdio.h>
#include <stdlib.h>

int do_rectification(char* input_filename, char* output_filename,int num_threads)
{
  unsigned error;
  unsigned char *image, *new_image;
  unsigned width, height;

  error = lodepng_decode32_file(&image, &width, &height, input_filename);
  if(error) printf("error %u: %s\n", error, lodepng_error_text(error));
  new_image = malloc(width * height * 4 * sizeof(unsigned char));

  // process image
  unsigned char value;
  for (int i = 0; i < height; i++) {
    for (int j = 0; j < width; j++) { 
      for(int k = 0;k< 4;k++){
        value = image[4*width*i + 4*j + k];
        if(value<127) value = 127;
        new_image[4*width*i + 4*j + k] = value;
      }
    }
  }

  lodepng_encode32_file(output_filename, new_image, width, height);

  free(image);
  free(new_image);

  return 0;
}

int main(int argc, char *argv[])
{

  if ( argc >= 4 ){
    char* input_filename = argv[1];
    char* output_filename = argv[2];
    int num_threads = atoi(argv[3]);

    int error = do_rectification(input_filename,output_filename,num_threads);
    if(error == -1){
      printf("An error occured. ( %d )\n",error);
    }else{
      printf("The rectification ran with success.\n");
    }

  }else{
    printf("There is inputs missing.\n");
  }
  return 0;
}
