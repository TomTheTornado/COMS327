#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <math.h>

/* Do not modify write_pgm() or read_pgm() */
int write_pgm(char *file, void *image, uint32_t x, uint32_t y)
{
  FILE *o;

  if (!(o = fopen(file, "w"))) {
    perror(file);

    return -1;
  }

  fprintf(o, "P5\n%u %u\n255\n", x, y);

  /* Assume input data is correctly formatted. *
   * There's no way to handle it, otherwise.   */

  if (fwrite(image, 1, x * y, o) != (x * y)) {
    perror("fwrite");
    fclose(o);

    return -1;
  }

  fclose(o);

  return 0;
}

/* A better implementation of this function would read the image dimensions *
 * from the input and allocate the storage, setting x and y so that the     *
 * user can determine the size of the file at runtime.  In order to         *
 * minimize complication, I've written this version to require the user to  *
 * know the size of the image in advance.                                   */
int read_pgm(char *file, void *image, uint32_t x, uint32_t y)
{
  FILE *f;
  char s[80];
  unsigned i, j;

  if (!(f = fopen(file, "r"))) {
    perror(file);

    return -1;
  }

  if (!fgets(s, 80, f) || strncmp(s, "P5", 2)) {
    fprintf(stderr, "Expected P6\n");

    return -1;
  }

  /* Eat comments */
  do {
    fgets(s, 80, f);
  } while (s[0] == '#');

  if (sscanf(s, "%u %u", &i, &j) != 2 || i != x || j != y) {
    fprintf(stderr, "Expected x and y dimensions %u %u\n", x, y);
    fclose(f);

    return -1;
  }

  /* Eat comments */
  do {
    fgets(s, 80, f);
  } while (s[0] == '#');

  if (strncmp(s, "255", 3)) {
    fprintf(stderr, "Expected 255\n");
    fclose(f);

    return -1;
  }

  if (fread(image, 1, x * y, f) != x * y) {
    perror("fread");
    fclose(f);

    return -1;
  }

  fclose(f);

  return 0;
}

int main(int argc, char *argv[])
{
  uint8_t image[1024][1024];
  uint8_t out[1024][1024];
  
  /* Example usage of PGM functions */
  /* This assumes that file imported is a pgm image of size 1024x1024 */
  read_pgm(argv[1], image, 1024, 1024);

  /*Implements O-x Matrix*/
  int X[3][3] = {{-1, 0, 1}, {-2, 0, 2}, {-1, 0 ,1}};
  /*Implements O-y Matrix*/
  int Y[3][3] = {{-1, -2, -1}, {0, 0, 0}, {1, 2, 1}};

  int r;
  for(r = 0; r < 1024; r++){
    
    int c;
    for(c = 0; c < 1024; c++){
      
      int j;
      int accumulatorX = 0;
      int accumulatorY = 0;
      for(j = 0; j < 3; j++){
        
        int i;
        for(i = 0; i < 3; i++){
          
          accumulatorX += X[j][i] * image[r + (j - 1)][c + (i - 1)];

          accumulatorY += Y[j][i] * image[r + (j - 1)][c + (i - 1)];
        }
      }
      /*The number of the function below needs to be between 0 and 255 to be
        registered correctly to this image. The if-else statements below ensure 
        that it stays within that range.*/
      if(sqrt(pow(accumulatorX, 2) + pow(accumulatorY, 2)) > 255){
        out[r][c] = 255;
      }
      else if(sqrt(pow(accumulatorX, 2) + pow(accumulatorY, 2)) < 0){
        out[r][c] = 0;
      }
      else{
        out[r][c] = sqrt(pow(accumulatorX, 2) + pow(accumulatorY, 2));
      }
    }
  }
  

  /* After processing the image and storing your output in "out", write *
   * to sobel.pgm.                                            */
  write_pgm("sobel.pgm", out, 1024, 1024);
  
  return 0;
}
