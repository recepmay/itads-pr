#include <stdio.h>
#include <err.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <math.h>


// Struct to get the stuff together for the element inside the matrix to print.
typedef struct element
{
	int value;
	char bitmask[4];
} Element;


// Struct to get the stuff together for the arguments.
typedef struct argument
{
	char *arg;
	int length;
	char *vowel;
} Argument;


// Function declaration
void zero_matrix(Element *matrix[], int nrows, int ncols);
void checking_case_vowels(Argument input[], int length, int i);
void needleman(Argument input[], Element *matrix[], int nrows, int ncols);
int calc_match(Argument input[], int x, int y);
void calc_mdi(Element *matrix[], int match, int insertion, int deletion, int x, int y);
void get_backtrace(Argument input[], Element *matrix[], int nrows, int nclos);
void matrix_printing(Element *matrix[], Argument input[], int ncols, int nrows);
void free_matrix(Element *matrix[], int nrows);


// Main does the input / matrix init and calls all the functions.
int main(int argc, char ** argv)
{
    Argument input[2];
	int i;
	
	if(argc < 3)
	{
		errx(EXIT_FAILURE, "Not enought strings on the command line - should be two!!!");
	}
	
    // goes through the arguments, writing them to input and checks for vowels and case
	for(i = 1; i < 3; i ++)
	{
		input[i].arg = argv[i];
		
		int length = (int)strlen(input[i].arg);
		input[i].length = length;
        input[i].vowel = malloc(sizeof(char) * length);
        
        checking_case_vowels(input, length, i);
        
        length = 0;
	}
    
    // Matrix init stuff and allocation of memory
    struct element ** matrix;
	int nrows = input[1].length + 1;
	int ncols = input[2].length + 1;
    int ir;
    
    matrix = malloc (nrows * sizeof(Element*));
    if (NULL == matrix)
    {
        err (EXIT_FAILURE, "malloc rows");
    }
    for (ir = 0; ir < nrows; ++ir)
    {
        matrix[ir] = malloc (ncols * sizeof(Element));
        if (NULL == matrix[ir])
        {
            err (EXIT_FAILURE, "malloc row %d", ir);
        }
    }
    
    // zeros all the matrix for the first time. just to make sure!!
    zero_matrix(matrix, nrows, ncols);

	// Calculate everything for the elements inside the matrix
    needleman(input, matrix, nrows, ncols);    
    
    // print
    matrix_printing(matrix,input,ncols,nrows);
    
    // free memory
	free_matrix(matrix, nrows);
    
    return 0;
}

// zeros all the matrix for the first time. just to make sure!!
void zero_matrix(Element *matrix[], int nrows, int ncols)
{
    int x,y;
    // zeros matrix
    for (x = 0; x < nrows; x++)
    {
        for (y = 0; y < ncols; y++)
        {
            matrix[x][y].value = 0;
        }
    }
    
    // but \0 in the bitmask
    for (x = 0; x < nrows; x++)
    {
        for (y = 0; y < ncols; y++)
        {
            matrix[x][y].bitmask[3] = '\0';
        }
    }
    
    //first row filling with values
    for (y = 1; y < ncols; y++)
    {
        matrix[0][y].value = matrix[0][y-1].value - 5 ;
        matrix[0][y].bitmask[0] = ' ';
        matrix[0][y].bitmask[1] = ' ';
        matrix[0][y].bitmask[2] = 'i';
    }
    
    // first column filling with values
    for (x = 1; x < nrows; x++)
    {
        matrix[x][0].value = matrix[x-1][0].value - 5 ;
        matrix[x][0].bitmask[0] = ' ';
        matrix[x][0].bitmask[1] = 'd';
        matrix[x][0].bitmask[2] = ' ';
    }
}


// Checks if the letter are letter, makes them all to be
// "lower-case" and checks wether its a vowel or a constant
void checking_case_vowels(Argument input[], int length, int i)
{
    int j;

    for (j = 0; j < length; j ++)
    {
        // checks if all the input is letters
        if (0 == isalpha (input[i].arg[j]))
        {
            errx (EXIT_FAILURE, "invalid character code %d in argument %d", (int) *input[i].arg, i);
        }
    }

    // makes all letter "lower-case"
    for (j = 0; j < length; j++)
    {
        input[i].arg[j] = tolower(input[i].arg[j]);
    }

    // checks if it is a vowel
    for (j = 0; j < length; j++)
    {
        char check = input[i].arg[j];
        switch(check)
        {
            case 'a' :
            case 'e' :
            case 'i' :
            case 'o' :
            case 'u' :
                // vowel
                input[i].vowel[j] = 'v';
                break;
            default :
                // constant
                input[i].vowel[j] = 'c';
        }
        
    }
}

// The algorithm itself checks which of the three values (match, deletion, insertion)
// is the highest one and writes it to the matrix.
void needleman(Argument input[], Element *matrix[], int nrows, int ncols)
{
    int match, insertion, deletion, calc;
    int x, y;

    for (x = 1; x < nrows; x++)
    {
        for (y = 1; y < ncols; y++)
        {
            // calculates the match value based on the vowel/constant inside the input.
            calc = calc_match(input, x, y);
            
            match = matrix[x-1][y-1].value + calc;
            insertion = matrix[x][y-1].value + (-5);
            deletion = matrix[x-1][y].value + (-5);
            
            matrix[x][y].value = fmax(fmax(match, insertion),deletion);

            // Do all the match/deletion/insertation calculating
            calc_mdi(matrix, match, insertion, deletion, x, y);
        }
        // sets insertion and deletion to zero for next iteration
        insertion = 0;
        deletion = 0;
    }
}


// calculate all the values for match / deletion / insertion
int calc_match(Argument input[], int x, int y)
{
    if(input[1].arg[x-1] == input[2].arg[y-1])
    {
        // perfet match
        return 10;
    }
    else if(input[1].vowel[x-1] == 'v' && input[2].vowel[y-1] == 'v')
    {
        // vowel - vowel match
        return -2;
    }
    else if(input[1].vowel[x-1] == 'c' && input[2].vowel[y-1] == 'c')
    {
        // constant - constant match
        return -4;
    }
    else if((input[1].vowel[x-1] == 'v' && input[2].vowel[y-1] == 'c') || (input[1].vowel[x-1] == 'c' && input[2].vowel[y-1] == 'v'))
    {
        // vowel - constant match
        return -10;
    }
    return 0;
}


// look up the backtrace information used for backtracing and printing the matrix.
void calc_mdi(Element *matrix[], int match, int insertion, int deletion, int x, int y)
{
    if (match > deletion)
    {
        if (match > insertion)
        {
            // just a match
            matrix[x][y].bitmask[0] = 'm';
            matrix[x][y].bitmask[1] = ' ';
            matrix[x][y].bitmask[2] = ' ';
        }
        if (match == insertion)
        {
            // match & insertion
            matrix[x][y].bitmask[0] = 'm';
            matrix[x][y].bitmask[1] = ' ';
            matrix[x][y].bitmask[2] = 'i';
        }
        if (match < insertion)
        {
            if(insertion > deletion)
            {
                // just an insertion
                matrix[x][y].bitmask[0] = ' ';
                matrix[x][y].bitmask[1] = ' ';
                matrix[x][y].bitmask[2] = 'i';
            }
            if (insertion == deletion)
            {
                // deletion & insertion
                matrix[x][y].bitmask[0] = ' ';
                matrix[x][y].bitmask[1] = 'd';
                matrix[x][y].bitmask[2] = 'i';
            }
        }
    }
    if (deletion > match)
    {
        if (deletion > insertion)
        {
            // just a deletion
            matrix[x][y].bitmask[0] = ' ';
            matrix[x][y].bitmask[1] = 'd';
            matrix[x][y].bitmask[2] = ' ';
        }
        if (deletion == insertion)
        {
            // deletion & insertion
            matrix[x][y].bitmask[0] = ' ';
            matrix[x][y].bitmask[1] = 'd';
            matrix[x][y].bitmask[2] = 'i';
        }
        if (deletion < insertion)
        {
            if(insertion > match)
            {
                // just an insertion
                matrix[x][y].bitmask[0] = ' ';
                matrix[x][y].bitmask[1] = ' ';
                matrix[x][y].bitmask[2] = 'i';
            }
            if (insertion == match)
            {
                // match & insertion
                matrix[x][y].bitmask[0] = 'm';
                matrix[x][y].bitmask[1] = ' ';
                matrix[x][y].bitmask[2] = 'i';
            }
        }
    }
    if (insertion > deletion && insertion > match)
    {
        // just an insertion
        matrix[x][y].bitmask[0] = ' ';
        matrix[x][y].bitmask[1] = ' ';
        matrix[x][y].bitmask[2] = 'i';
    }
    if (match == deletion && match > insertion)
    {
        // match & deletion
        matrix[x][y].bitmask[0] = 'm';
        matrix[x][y].bitmask[1] = 'd';
        matrix[x][y].bitmask[2] = ' ';
    }
    
    if (match == deletion && match == insertion)
    {
        // all the same!
        matrix[x][y].bitmask[0] = 'm';
        matrix[x][y].bitmask[1] = 'd';
        matrix[x][y].bitmask[2] = 'i';
    }
}


// tried to get the backtrace printed out for the optional assignment and prints it.
void get_backtrace(Argument input[], Element *matrix[], int nrows, int ncols)
{
    int x = nrows - 1;
    int y = ncols - 1;
    int length = fmax(nrows,ncols);
    int i = length;
    
    char output_1[length];
    char output_2[length];
    
    while (y != 0)
    {
        // gets through the matrix finding the "best" way!
        if(matrix[x][y].bitmask[0] == 'm')
        {
            // if its a match u put letter to both outputs.
            output_1[i] = input[1].arg[x];
            output_2[i] = input[2].arg[y];
            
            x = x - 1;
            y = y - 1;
        }
        else if(matrix[x][y].bitmask[1] == 'd')
        {
            // only push a letter to output_2 and a line to output_1.
            output_1[i] = input[1].arg[x];
            output_2[i] = '_';
            
            x = x - 1;
            y = y;
        }
        else if(matrix[x][y].bitmask[2] == 'i')
        {
            // only push a letter to output_1 and a line to output_2.
            output_1[i] = '_';
            output_2[i] = input[2].arg[y];
            
            x = x;
            y = y - 1;
        }
        
        i = i - 1;
        
        if(x == 0 && y == 0)
        {
            output_1[i] = input[1].arg[0];
            output_2[i] = input[2].arg[0];
        }
    }
    
    // prints the given input backtraces with the backtrace information
    printf("input source:        %s\n", input[1].arg);
    printf("input destination:   %s\n", input[2].arg);
    printf("output source:       %s\n", output_1);
    printf("output destination:  %s\n", output_2);
    
}


// Print the matrix with the right stuff inside
void matrix_printing(Element *matrix[], Argument input[], int ncols, int nrows)
{
    int ir, ic;
    
    printf ("  |   _");
    for (ic = 0; ic < ncols-1; ++ic) {
        printf (" %3c", input[2].arg[ic]);
    }
    printf ("\n--+");
    for (ic = 1; ic < ncols; ++ic) {
        printf ("------");
    }
    printf ("\n");
    
    
    for (ir = 0; ir < nrows; ++ir)
    {
        // first row
        if(ir == 0)
        {
            printf ("- |");
            // printing the values
            for (ic = 0; ic < ncols; ++ic)
            {
                printf (" %3d", matrix[ir][ic].value);
            }
            printf ("\n");
            printf ("  |");
            // printing the backtrace values
            for (ic = 0; ic < ncols; ++ic)
            {
                printf (" %3s",matrix[ir][ic].bitmask);
            }
        }
        else
        {
            printf ("%c |", input[1].arg[ir-1]);
            // printing the values
            for (ic = 0; ic < ncols; ++ic)
            {
                printf (" %3d", matrix[ir][ic].value);                
            }
            printf ("\n");
            printf ("  |");
            // printing the backtrace values
            for (ic = 0; ic < ncols; ++ic)
            {
                printf (" %3s",matrix[ir][ic].bitmask);
            }
        }
        printf ("\n");
    }
    printf ("\n");
    
    // Get the backtrace right
    get_backtrace(input, matrix, nrows, ncols);
    
    printf ("\n");
    printf("DONE!\n");
}


// frees the memory
void free_matrix(Element *matrix[], int nrows)
{
    int ir;

    for (ir = 0; ir < nrows; ++ir)
        free (matrix[ir]);
    free (matrix);
}
