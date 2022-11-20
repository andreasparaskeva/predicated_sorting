// include standard headers
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/time.h>
#include <limits.h>
// #define CHAR_BIT 8

void swap(int* xp, int* yp)
{
		int temp = *xp;
		// minimum
		*xp = (*yp) + (((*xp) - (*yp)) & (((*xp) - (*yp)) >>
            (sizeof(int) * CHAR_BIT - 1)));
		// maximum
		*yp = temp - ((temp - (*yp)) & ((temp - (*yp)) >>
            (sizeof(int) * CHAR_BIT - 1)));

}

// "predicated" in-place sort implementation
void sort_predicated_inplace( int N , int *data_array ) {

	int i, j;
	for (i = 0; i < N - 1; i++)
		// Last i elements are already sorted
		for (j = 0; j < N - i - 1; j++)
			swap(&data_array[j], &data_array[j + 1]);

	return;
}

// main program
int main( int argc , char ** argv ) {

	// variables for timing
	struct timeval before , after;
	long long int usec = 0;

	// check number command line arguments
	if ( argc != 3 ) {
		// incorrect number of command line arguments
		fprintf(
		    stderr ,
		    "need two arguments\n"
		);
		// exit with error status
		return( 1 );
	}

	// read first command line argument:
	// number of data values
	int N = atoi( argv[ 1 ] );

	// read second command line argument:
	// (absolute/relative) path to input data file
	char *input_data_file_location = strdup( argv[ 2 ] );
	// allocate memory for data array
	int *data_array = malloc( N * sizeof(int) );
	if ( data_array == NULL ) {
		// report error
		fprintf(
			stderr ,
			"malloc failed for data_array (%zu bytes)\n" ,
			N * sizeof(int)
		);
		// free allocated memory
		free( input_data_file_location );
		// exit with error status
		return( 1 );
	}

	// open input data file for reading
	FILE *input_data_file = fopen( input_data_file_location , "r" );
	if ( input_data_file == NULL ) {
		// report error
		perror( "fopen failed" );
		// free allocated memory
		free( data_array );
		free( input_data_file_location );
		// exit with error status
		return( 1 );
	}

	// load content of input data file in to data array;
	// measure and report time it takes
	gettimeofday( &before , NULL );
	for ( int i = 0 ; i < N ; i++ ) {
		int e = fscanf(
			input_data_file ,
			"%d\n" ,
			data_array + i
		);
		if ( e == EOF ) {
			// report error
			perror( "fscanf: premature EOF" );
			// close input data file
			fclose( input_data_file );
			// free allocated memory
			free( data_array );
			free( input_data_file_location );
			// exit with error status
			return( 1 );
		}
		if ( e < 1 ) {
			// report error
			perror( "fscanf failed" );
			// close input data file
			fclose( input_data_file );
			// free allocated memory
			free( data_array );
			free( input_data_file_location );
			// exit with error status
			return( 1 );
		}
	}
	gettimeofday( &after , NULL );
	usec = (long long int)(after.tv_sec - before.tv_sec) * 1000000
			   + (after.tv_usec - before.tv_usec);

	// close input data file
	fclose( input_data_file );

	// sort data array in-place;
	// measure the time it takes
	gettimeofday( &before , NULL );
	sort_predicated_inplace( N , data_array );
	gettimeofday( &after , NULL );

	// check result correctness
	for ( int i = 1 ; i < N ; i++ ) {
		if ( data_array[ i - 1 ] > data_array[ i ] ) {
			// report error
			fprintf(
				stderr ,
				"result not sorted:\n data_array[ %d ] == %10d\n data_array[ %d ] == %10d\n" ,
				i - 1 , data_array[ i - 1 ] , i , data_array[ i ]
			);
			// free allocated memory
			free( data_array );
			free( input_data_file_location );
			// exit with error status
			return( 1 );
		}
	}

	// calculate and report sorting time in microseconds
	usec = (long long int)(after.tv_sec - before.tv_sec) * 1000000
			   + (after.tv_usec - before.tv_usec);


	printf("%9lld,", usec);
	// free allocated memory
	free( data_array );
	free( input_data_file_location );

	// exit without error status
	return( 0 );
}
