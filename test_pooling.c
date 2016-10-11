/* Check whether two images are the same */
#include "lodepng.h"
#include <stdio.h>
#include <stdlib.h>

int do_pooling(char* input_filename, char* output_filename,int num_threads)
{
  unsigned error;
  unsigned char *image, *new_image;
  unsigned width, height;
  unsigned new_width, new_height;

  error = lodepng_decode32_file(&image, &width, &height, input_filename);
  if(error){
    printf("error %u: %s\n", error, lodepng_error_text(error));
    return -1;
  }
  long unsigned size = width * height * 4 * sizeof(unsigned char)
  long unsigned new_size = new_width * new_height * 4 * sizeof(unsigned char)
  new_width = width/2;
  new_height = height/2;
  new_image = malloc(new_size);

  printf(" old_height %d, old_width %d,old size %lu \n",height,width,size);
  printf(" new_height %d, new_width %d,new size %lu \n",new_height,new_width,new_size);
  // process image
  unsigned char max;
  for (int i = 0; i < height; i+=2) {
    for (int j = 0; j < width; j+=2) {
      for(int k = 0;k< 4;k++){
        if(i == j) printf("coord %d,%d,location %d / %lu \n",i,j,4*width*i + 4*j + k,size );
        max = image[4*width*i + 4*j + k];
        if(i == j) printf("max1: %d\n",max);
        if(image[4*width*(i+1) + 4*j + k]>max) max = image[4*width*(i+1) + 4*j + k];
        if(i == j) printf("max2: %d\n",max );
        if(image[4*width*(i+1) + 4*(j+1) + k]>max) max = image[4*width*(i+1) + 4*(j+1) + k];
        if(i == j) printf("max3: %d\n",max );
        if(image[4*width*i + 4*(j+1) + k]>max) max = image[4*width*i + 4*(j+1) + k];
        if(i == j) printf("max4: %d\n",max );
        if(i==j) printf("new image written at coord %d, %d, locatin %d / %lu\n",i/2,i/2,2*width*i + 2*j + k,new_size);
        new_image[2*width*i + 2*j + k] = max;
      }
    }
  }
  lodepng_encode32_file(output_filename, new_image, new_width, new_height);

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

    int error = do_pooling(input_filename,output_filename,num_threads);
    if(error == -1){
      printf("An error occured. ( %d )\n",error);
    }else{
      printf("The pooling ran with success.\n");
    }

  }else{
    printf("There is inputs missing.\n");
  }
  return 0;
}
