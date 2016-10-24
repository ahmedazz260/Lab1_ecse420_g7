/* Check whether two images are the same */
#include "lodepng.h"
#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
#include <sys/time.h>
#include <mpi.h>


int main(int argc, char *argv[]){

  if ( argc >= 3 ){
    char* input_filename = argv[1];
    char* output_filename = argv[2];

    unsigned error;
    unsigned char *image, *new_image;
    unsigned width, height;

    struct timeval start_time, end_time;
    printf("We will rectify %s and get %s.\n",input_filename,output_filename);

    error = lodepng_decode32_file(&image, &width, &height, input_filename);
    if(error) printf("error %u: %s\n", error, lodepng_error_text(error));

    new_image = malloc(width * height * 4 * sizeof(unsigned char));
    
    gettimeofday(&start_time, NULL);
    // process image
    int i;
    // Initialize the MPI environment
    MPI_Init(&argc, &argv);

    // Get the number of processes
    int size;
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    // Get the rank of the process
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    int split = height/size + 1;
    printf("This is the split value from %d / %d = %d \n",height,size,split );
    for (i = rank*split; (i < (rank+1)*split) && i<height ; i++) {
      int j,k;
      unsigned char value;
      //printf("thread %d processed element %d\n", omp_get_thread_num(), i);
      
      for (j = 0; j < width; j++) { 
        if(i==j)printf("From rank %d out of %d)\ni is %d and the limit is %d\n",rank, size,i,(rank+1)*split);
        for(k = 0; k < 4;k++){
          
          value = image[4*width*i + 4*j + k];
          
          if(value<127) value = 127;
          
          new_image[4*width*i + 4*j + k] = value;
        
        }
      }
    }
    MPI_Finalize();

    gettimeofday(&end_time, NULL);

    lodepng_encode32_file(output_filename, new_image, width, height);

    unsigned long long time_elapsed = 1000 * (end_time.tv_sec - start_time.tv_sec) + (end_time.tv_usec - start_time.tv_usec) / 1000;
   
    printf("Time Elapsed [%llu ms]\n", time_elapsed);

    free(image);
    free(new_image);
    
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
