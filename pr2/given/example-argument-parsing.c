#include <stdio.h>
#include <err.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>


/*
  One possibility of determining whether a character is a vowel. Other
  solutions exist, for example you could use a single switch
  statement.
*/
int isvowel (char cc)
{
  static char vowel[] = { 'a', 'e', 'i', 'o', 'u' };
  static int nvowels = sizeof vowel / sizeof *vowel;
  int ii;
  
  cc = tolower (cc);
  for (ii = 0; ii < nvowels; ++ii) {
    if (cc == vowel[ii]) {
      return 1;
    }
  }
  
  return 0;
}


int main (int argc, char ** argv)
{
  char *chk;
  int ii;
  
  /*
    Check that we were given two arguments.
  */
  if (argc < 3) {
    errx (EXIT_FAILURE, "please provide two strings on the command line");
  }
  
  /*
    Check that each argument contains letters only.
  */
  for (ii = 1; ii < 3; ++ii) {
    for (chk = argv[ii]; *chk != '\0'; ++chk) {
      if (0 == isalpha (*chk)) {
	errx (EXIT_FAILURE, "invalid character code %d in argument %d", (int) *chk, ii);
      }
    }
  }
  
  /*
    Print each argument, and annotate each letter with either 'v' if
    it is a vowel or 'c' if it is a consonant.
  */
  for (ii = 1; ii < 3; ++ii) {
    printf ("argument %d is %zu characters long.\n"
	    "  here is the string:   %s\n"
	    "  vowels or consonants: ",
	    ii, strlen (argv[ii]), argv[ii]);
    for (chk = argv[ii]; *chk != '\0'; ++chk) {
      printf (isvowel (*chk) ? "v" : "c");
    }
    printf ("\n\n");
  }
  
  return 0;
}
