/* Check whether two images are the same */
#include "lodepng.h"
#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
#include "wm.h"


int do_convolution(char* input_filename, char* output_filename,int num_t){
  unsigned error;
  unsigned char *image, *new_image;
  unsigned width, height;
  unsigned new_width, new_height;

  error = lodepng_decode32_file(&image, &width, &height, input_filename);
  if(error){
    printf("error %u: %s\n", error, lodepng_error_text(error));
    return -1;
  }
  new_width = width-2;
  new_height = height-2;
  long unsigned size = width * height * 4 * sizeof(unsigned char);
  long unsigned new_size = new_width * new_height * 4 * sizeof(unsigned char);
  new_image = malloc(new_size);
  printf("number of threads %d\n",num_t);
  printf(" old_height %d, old_width %d,old size %lu \n",height,width,size);
  printf(" new_height %d, new_width %d,new size %lu \n",new_height,new_width,new_size);
  // process image
  int i;
  //#pragma omp parallel for num_threads(num_t)
  for (i = 1; i < height-1; i++) {
    int j,k;
    float currentWF;
    unsigned char value;
    for (j = 1; j < width-1; j++) {
      for(k = 0;k< 4;k++){
        value = 0;
        int ii,jj;
        for(ii = 0;ii<3;ii++){
            for(jj = 0; jj<3;jj++){
              currentWF = w[ii][jj];
                if(j==1 && i == 1) printf("this is the weight for ii %d and jj %d %f\n",ii,jj,currentWF);
                value += ((float)image[4*width*(i+ii-1) + 4*(j+jj-1) + k]) * currentWF;
                if(i==1 && j==1) printf("this is the current value %d \n",value);
            }
        }
        if(value<0) value = 0;
        if(value>255) value = 255;
        new_image[4*new_width*(i-1) + 4*(j-1) + k] = value;
      }
    }
    printf("thread %d processed element %d\n", omp_get_thread_num(), i);
  }
  lodepng_encode32_file(output_filename, new_image, new_width, new_height);

  free(image);
  free(new_image);

  return 0;
}

int main(int argc, char *argv[]){

  if ( argc >= 4 ){
    char* input_filename = argv[1];
    char* output_filename = argv[2];
    int num_threads = atoi(argv[3]);

    int error = do_convolution(input_filename,output_filename,num_threads);
    
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
