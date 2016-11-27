#include <stdio.h>
#include <stdlib.h>
#include <err.h>


int main (int argc, char ** argv)
{
  /* 
     In this example, the matrix is a pointer to the first row, which
     is pointer to the first elements of an array.  So, the matrix
     variable is a pointer to a pointer. Again, here we create a
     matrix of integers, but you can of course create a matrix of any
     type, such as structs.
  */

  int ** matrix;
  int nrows, ncols;
  int ir, ic;
  
  /*
    Initialize and allocate a 5x7 matrix. Notice that we first
    allocate an array of nrows POINTERS to int. Each of these will
    point to the first element of a row, and each row is an array of
    ncols elements. Each of the latter has to be allocated (and freed)
    individually.
  */
  nrows = 5;
  ncols = 7;
  matrix = malloc (nrows * sizeof(int*)); /* notice sizeof POINTER to int */
  if (NULL == matrix) {
    err (EXIT_FAILURE, "malloc rows");
  }
  for (ir = 0; ir < nrows; ++ir) {
    matrix[ir] = malloc (ncols * sizeof(int));
    if (NULL == matrix[ir]) {
      err (EXIT_FAILURE, "malloc row %d", ir);
    }
  }
  
  /*
    Fill the matrix with some example values. Notice that now the
    array index is two-dimensional, this may look more natural: in
    order to access the element at (ir,ic) you just write [ir][ic].
  */
  for (ir = 0; ir < nrows; ++ir) {
    for (ic = 0; ic < ncols; ++ic) {
      if ((ir + ic) % 2) {
	matrix[ir][ic] = -10*ic - ir;
      }
      else {
	matrix[ir][ic] = 10*ir + ic;
      }
    }
  }
  
  /*
    Print the matrix as a nice table with a header row that lists the
    column indices, a header column which shows the row indices, and
    the values right-aligned into a field of width 3.
  */
  printf ("  |");
  for (ic = 0; ic < ncols; ++ic) {
    printf (" %3d", ic);
  }
  printf ("\n--+");
  for (ic = 0; ic < ncols; ++ic) {
    printf ("----");
  }
  printf ("\n");
  for (ir = 0; ir < nrows; ++ir) {
    printf ("%d |", ir);
    for (ic = 0; ic < ncols; ++ic) {
      printf (" %3d", matrix[ir][ic]);
    }
    printf ("\n");
  }
  
  /*
    Free the memory used by the matrix. Notice that now we first have
    to free each individual row, then the array of row pointers.
  */
  for (ir = 0; ir < nrows; ++ir)
    free (matrix[ir]);
  free (matrix);
  
  return 0;
}
