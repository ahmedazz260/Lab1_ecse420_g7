/* Check whether two images are the same */
#include "lodepng.h"
#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
#include <sys/time.h>


int do_rectification(char* input_filename, char* output_filename, int num_t){
  unsigned error;
  unsigned char *image, *new_image;
  unsigned width, height;

  struct timeval start_time, end_time;


  error = lodepng_decode32_file(&image, &width, &height, input_filename);
  if(error) printf("error %u: %s\n", error, lodepng_error_text(error));

  new_image = malloc(width * height * 4 * sizeof(unsigned char));
  printf("Number of threads %d\n", num_t);
  
  gettimeofday(&start_time, NULL);
  // process image
  int i;

  #pragma omp parallel for num_threads(num_t)
  for (i = 0; i < height; i++) {
    
    int j,k;
    unsigned char value;
    //printf("thread %d processed element %d\n", omp_get_thread_num(), i);
    
    for (j = 0; j < width; j++) { 
      
      for(k = 0; k < 4;k++){
        
        value = image[4*width*i + 4*j + k];
        
        if(value<127) value = 127;
        
        new_image[4*width*i + 4*j + k] = value;
      
      }
    }
  }

  gettimeofday(&end_time, NULL);

  lodepng_encode32_file(output_filename, new_image, width, height);

  unsigned long long time_elapsed = 1000 * (end_time.tv_sec - start_time.tv_sec) + (end_time.tv_usec - start_time.tv_usec) / 1000;
 
  printf("Time Elapsed [%llu ms]\n", time_elapsed);

  free(image);
  free(new_image);

  return 0;
}

int main(int argc, char *argv[]){

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
