// include standard headers
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/time.h>
#include <limits.h>

/** Slicing the array between the start and end index */
int* slice(int *array, int start_idx, int end_idx)
{
    int *result = (int *) malloc((end_idx - start_idx) * sizeof(int));
    int i;
    for (i = start_idx; i < end_idx; i++)
    {
        result[i - start_idx] = array[i];
    }
    return result;
}

/** Merge left and right subarrays iteratively into result */
void merge(int *result, int *left, int *right, int leftLen, int rightLen)
{
    int i = 0, j = 0;
    while(i < leftLen && j < rightLen)
    {
        if (left[i] < right[j])
        {
            result[i + j] = left[i];
            i++;
        }
        else
        {
            result[i + j] = right[j];
            j++;
        }
    }

    for(; i < leftLen; i++)
    {
        result[i + j] = left[i];
    }
    for(; j < rightLen; j++)
    {
        result[i + j] = right[j];
    }

    free(left);
    free(right);
}

// in-place merge sort
void mergeSort(int *arr, int len)
{
    if (len <= 1)
    {
        return;
    }
    int *left = slice(arr, 0, len / 2 + 1);
    int *right = slice(arr, len / 2, len);

    mergeSort(left, len / 2);
    mergeSort(right, len - (len / 2));

    merge(arr, left, right, len / 2, len - (len / 2));
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

	// fprintf(
	// 	stderr ,
	// 	"load data: %9lld usec ; data_array[ 0 ] = %10d , data_array[ %d ] = %10d , data_array[ %d ] = %10d\n" ,
	// 	usec , data_array[ 0 ] , N / 2 , data_array[ N / 2 ] , N - 1 , data_array[ N - 1 ]
	// );

	// close input data file
	fclose( input_data_file );

	// sort data array in-place;
	// measure the time it takes
	gettimeofday( &before , NULL );
	mergeSort(data_array, N);
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

	// calculate and report sorting time
	usec = (long long int)(after.tv_sec - before.tv_sec) * 1000000
			   + (after.tv_usec - before.tv_usec);
	printf(
		// stdout,
		"%9lld,", usec
	);

	// free allocated memory
	free( data_array );
	free( input_data_file_location );

	// exit without error status
	return( 0 );
}
