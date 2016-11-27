#include <stdio.h>
#include <stdlib.h>
#include <err.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <limits.h>
#include <sys/uio.h>
#include <sys/time.h>


/***************************************************
  Insertion sort implementation
***************************************************/


static void insertion_sort (int * arr, int len)
{
  int ii;
  for (ii = 1; ii < len; ++ii) {
    int jj, ival;
    ival = arr[ii];
    for (jj = ii; jj > 0; --jj) {
      if (ival >= arr[jj-1])
	break;
      arr[jj] = arr[jj-1];
    }
    arr[jj] = ival;
  }
}



/***************************************************
  Declarations of utility functions used in the benchmark. They are
  defined after the main function. You do not need to understand how
  they work, just use them in your benchmarks by copy-pasting.
***************************************************/

/* Returns a random integer in the specified range. */
static int random_int (int minval, int maxval);

/* Returns a freshly allocated array with len random integers from the
   specified range. The returned array has to be freed at the end of
   the program. */
static int * random_array (int minval, int maxval, size_t len);

/* Returns the current time as milliseconds in double format. The first
   time this function is called, it resets an internal reference and
   returns zero. From then on it returns the number of milliseconds
   since that reset. */
static double clockms (void);


/***************************************************
  The main benchmark code begins here.
***************************************************/

int main (int argc, char ** argv)
{
  int dmin    =       0;
  int dmax    =    1000;
  int nstart  =      20;
  int nmax    =   50000;
  double nfac =       1.2;
  double nd;
  int * data;
  int * dup;
  
  /*
    Allocate one big random input data array. For varying the and
    making sure we always start with random input, we will use a
    duplicate (with just the first N entries) at each iteration.
  */
  
  printf ("# generating input data (this can take a while...)\n");
  fflush (stdout);
  data = random_array (dmin, dmax, nmax);
  dup = malloc (nmax * sizeof(int));
  if (NULL == dup) {
    err (EXIT_FAILURE, __FILE__": %s: malloc", __func__);
  }
  
  printf ("################################################\n"
	  "#\n"
	  "# merge sort runtime measurements\n"
	  "#\n"
	  "# column 1: N\n"
	  "# column 2: T [ms]\n"
	  "#\n");
  
  /*
    Main benchmarking loop.
    
    Note that we use a floating point "length" so that we can easily
    create a geometric series for N. This spaces the sampled array
    sizes more densely for small N, and for big N (where things
    usually take much longer) we don't take so many samples (otherwise
    running the benchmark takes a really long time).
  */
  
  for (nd = nstart; nd <= nmax; nd *= nfac) {
    int nn;
    double tstart, tstop;
    
    /*
      convert the floating point "length" to an integer
    */
    nn = nd;
    
    printf ("%8d", nn);
    fflush (stdout);
    
    /*
      use a fresh duplicate from the random input data
    */
    memcpy (dup, data, nn * sizeof(int));
    
    tstart = clockms ();
    insertion_sort (dup, nn);
    tstop = clockms ();
    
    
    printf ("  %8g\n", tstop - tstart);
  }
  
  /*
    End of the program.
  */
  
  free (data);
  free (dup);
  
  return 0;
}


/***************************************************
  Implementations of the utility functions to create random data and
  measure execution time.
  
  YOU DO NOT NEED TO UNDERSTAND THESE FUNCTIONS! Just copy-paste them
  as needed when you create new benchmark programs.
***************************************************/


int random_int (int minval, int maxval)
{
  static int fd = -1;
  unsigned int delta, tmp, mask;
  
  if (0 > fd && 0 > (fd = open ("/dev/urandom", O_RDONLY))) {
    err (EXIT_FAILURE, __FILE__": %s: open /dev/urandom", __func__);
  }
  
  if (maxval < minval) {
    tmp = minval;
    minval = maxval;
    maxval = tmp;
  }
  if (0 == (delta = maxval - minval)) {
    return minval;
  }
  
  for (tmp = 1 << (8 * sizeof(int) - 1); 0 == (tmp & delta); tmp >>= 1) {
    /* nop */;
  }
  for (mask = tmp; 0 != tmp; tmp >>= 1) {
    mask |= tmp;
  }
  
  for (;;) {
    if (sizeof(int) != read (fd, &tmp, sizeof(int))) {
      err (EXIT_FAILURE, __FILE__": %s: read /dev/urandom", __func__);
    }
    tmp &= mask;
    if (tmp <= delta) {
      break;
    }
  }
  
  return minval + tmp;
}


int * random_array (int minval, int maxval, size_t len)
{
  int * arr;
  int * ii;
  
  arr = malloc (len * sizeof(int));
  if (NULL == arr) {
    err (EXIT_FAILURE, __FILE__": %s: malloc", __func__);
  }
  
  for (ii = arr; len > 0; ++ii, --len) {
    *ii = random_int (minval, maxval);
  }
  
  return arr;
}


double clockms (void)
{
  static struct timeval t0 = { 0, 0 };
  struct timeval t1;
  
  if (0 == t0.tv_sec) {
    if (0 != gettimeofday (&t0, NULL)) {
      err (EXIT_FAILURE, __FILE__": %s: gettimeofday", __func__);
    }
  }
  if (0 != gettimeofday (&t1, NULL)) {
    err (EXIT_FAILURE, __FILE__": %s: gettimeofday", __func__);
  }
  
  return 1e3 * (t1.tv_sec - t0.tv_sec) + 1e-3 * (t1.tv_usec - t0.tv_usec);
}
