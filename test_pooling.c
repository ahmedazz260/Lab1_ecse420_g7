/* Check whether two images are the same */
#include "lodepng.h"
#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

int do_pooling(char* input_filename, char* output_filename,int num_t){
  unsigned error;
  unsigned char *image, *new_image;
  unsigned width, height;
  unsigned new_width, new_height;

  struct timeval start_time, end_time;

  error = lodepng_decode32_file(&image, &width, &height, input_filename);
  if(error){
    printf("error %u: %s\n", error, lodepng_error_text(error));
    return -1;
  }
  new_width = width/2;
  new_height = height/2;

  long unsigned size = width * height * 4 * sizeof(unsigned char);
  long unsigned new_size = new_width * new_height * 4 * sizeof(unsigned char);

  new_image = malloc(new_size);

  printf("number of threads %d\n",num_t);
  printf(" old_height %d, old_width %d,old size %lu \n",height,width,size);
  printf(" new_height %d, new_width %d,new size %lu \n",new_height,new_width,new_size);
  
  // process image

  gettimeofday(&start_time, NULL);

  int i;
  #pragma omp parallel for num_threads(num_t)
  for (i = 0; i < height; i+=2) {
   
    int j,k;
    unsigned char max;
    // if(i%10 == 0)printf("thread %d processed element %d\n", omp_get_thread_num(), i);
   
    for (j = 0; j < width; j+=2) {
   
      for(k = 0;k< 4;k++){
   
        max = image[4*width*i + 4*j + k];
   
        if(image[4*width*(i+1) + 4*j + k]>max) max = image[4*width*(i+1) + 4*j + k];
   
        if(image[4*width*(i+1) + 4*(j+1) + k]>max) max = image[4*width*(i+1) + 4*(j+1) + k];
   
        if(image[4*width*i + 4*(j+1) + k]>max) max = image[4*width*i + 4*(j+1) + k];
   
        new_image[2*new_width*i + 2*j + k] = max;
      }
    }
  }

  gettimeofday(&end_time, NULL);

  lodepng_encode32_file(output_filename, new_image, new_width, new_height);

  unsigned long long time_elapsed = 1000 * (end_time.tv_sec - start_time.tv_sec) + (end_time.tv_usec - start_time.tv_usec) / 1000;
 
  printf("Time Elapsed [%llu ms]\n", time_elapsed);

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
    printf("There are inputs missing.\n");
  }
  return 0;
}
