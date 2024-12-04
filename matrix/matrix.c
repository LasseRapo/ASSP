/*Fixed Point Implementation of N*M in M*K matrix Multiplication (2 point)
This program is just a nested loop. The only difference is that it should be implemented
as a fixed point program, i.e. it’s inputs and outputs are all FLOAT32 but the middle
computations all will be fixed point and half floating point and the final results again
must be FLOAT32 either printed in screen or stored in a float32 array. Please use the
following function format so I can use it in my test code to check the results:
int Matrxi_Mul( float * A, float * B, float *Result, int rows_of_A, int columns_of_A ,int rows_of_B, int
columns_of_B, bool Fix_Or_Float16)
*/
// A is first matrix, B is the second, Result is the resultant matrix which obviously would be of size
//rows_of_A in
//Fix_Or_Float16 determines if the middle computations are fixed or float 16
// return 1 ; //if multiplication can be done
// return 0 ; //if multiplication cannot be done


#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>


float dot(float* A, float* B, int length)
{   

    float sum = 0;

    for(int i = 0; i < length; i++)
    {
        sum += A[i] * B[i];
    }

    return sum;
}

int Matrix_Mul( float * A, 
                float * B, 
                float * Result, 
                int rows_of_A, 
                int columns_of_A, 
                int rows_of_B,
                int columns_of_B,
                bool Fix_Or_Float16 )

{
    if( columns_of_A != rows_of_B )
    {
        return 0;
    }

    // reserve dynamically memory for result 

    printf("Allocating memory with sizeof %d\n", rows_of_A * columns_of_B );
    Result = malloc( rows_of_A * columns_of_B * sizeof(float) );
    
    if( Result == NULL ) 
    {
        printf("ERROR: Failed to allocate memory for result matrix\n");
        exit(1);
    }


    // TODO: FIXED point implementation and float16 implementation
    
    int total_iterations = 0;

    for( int i = 0; i < rows_of_A; i++ )
    {
        for( int j = 0; j < columns_of_B; j++ )
        {
            // i is row, j is column

            Result[total_iterations] = 0.0f;

            for (int k = 0; k < columns_of_A; k++)
            {

                // loop used to calculate the dot product

                Result[total_iterations] += A[i* rows_of_A + k] * B[k * columns_of_B + j];

                printf("iter | j-val | index B | A val   | B val\n");
                printf("%d     %d        %d        %f  %f\n\n", total_iterations, j, k*columns_of_B + j, A[i * rows_of_A + k], B[k * (columns_of_B) + j] );
            }
            
            //printf("%d %f\n", total_iterations, Result[total_iterations]);
            total_iterations++;
        }


            // [[1, 2], [3, 4]] * [[1, 0], [0, 1]] = [[1, 2], [3, 4]] = [[1*1 + 0*3, 2*1 + 4*0], [1*0 + 3*1, 2*0 + 4*1]]
            
            // A[0] * B[0] + A[2] * B[1] 
            // A[1] * B[0] + A[3] * B[1]
            // A[0] * B[2] + A[2] * B[3]
            // A[1] * B[2] + A[3] * B[3]

            // NxM matriisi 

            /* 
            A = [[1, 2, 3],
                 [4, 5, 6],
                 [7, 8, 9]]

            B = [[1, 0, 0],
                 [0, 1, 0],
                 [0, 0, 1]] 

            // A[0] * B[0] + A[3] * B[1] + A[6] * B[2]
            // A[1] * B[0] + A[4] * B[1] + A[7] * B[2]
            // A[2] * B[0] + A[5] * B[1] + A[8] * B[2]
            // A[0] * B[3] + A[3] * B[4] + A[6] * B[5]
            // A[1] * B[3] + A[4] * B[4] + A[7] * B[5]
            // A[2] * B[3] + A[5] * B[4] + A[8] * B[5]
            // A[0] * B[6] + A[3] * B[7] + A[6] * B[8]
            // A[1] * B[6] + A[4] * B[7] + A[7] * B[8]
            // A[2] * B[6] + A[5] * B[7] + A[8] * B[8]

            // 3x3 * 2x3 => 2x3 
            
            
            // FIRST ROW
            // A[0] * B[0] + A[3] * B[1] + A[6] * B[2]
            // A[1] * B[0] + A[4] * B[1] + A[7] * B[2]
            // A[2] * B[0] + A[5] * B[1] + A[8] * B[2]

            // SECOND ROW
            // A[0] * B[3] + A[3] * B[4] + A[6] * B[5]
            // A[1] * B[3] + A[4] * B[4] + A[7] * B[5]
            // A[2] * B[3] + A[5] * B[4] + A[8] * B[5]

            A[j], B[...], A[...], B[+1]

            A[0] = 1 A[1] = 2 A[2] = 3 ...
            B[0] = 1 B[1] = 0 B[2] = 0, B[3] = 0

            */

            


            // [[1, 2, 3], 
            //  [4, 5, 6]]
    }

    for( int i = 0; i < rows_of_A * columns_of_B; i++ )
    {
        printf("%f\n", Result[i]);
    }

    return 1;
}

int main( void )
{
    float A1[16], B1[16]; // two 4x4 matrices
    float A2[4], B2[4]; // two square (2x2) matrices
    float A3[8], B3[8]; // A is 4x2 and B is 2x4 matrices
    float A4[8], B4[8]; // A is 4x2 and B is 4x2 matrices (this should return 0)

    float A5[9] = {1, 2, 3, 4, 5, 6, 7, 8, 9}; 
    float B5[6] = {1, 0, 0, 1, 0, 0}; 

    float* Result1 = NULL;
    float* Result2 = NULL;
    float* Result3 = NULL;
    float* Result4 = NULL;
    float* Result5 = NULL;

    int status[5];

    //status[0] = Matrix_Mul(A1, B1, Result1, 4, 4, 4, 4, false);
    //status[1] = Matrix_Mul(A2, B1, Result2, 2, 2, 2, 2, false);
    //status[2] = Matrix_Mul(A3, B1, Result3, 4, 2, 2, 4, false);
    //status[3] = Matrix_Mul(A4, B4, Result4, 4, 2, 4, 2, false);
    status[4] = Matrix_Mul(A5, B5, Result5, 3, 3, 3, 2, false);


    // Check Result5

    printf("Printing statuses\n");

    for( int i = 0; i < 5; i++ ) 
    {
        if( status[i] == 0 )
        {
            printf("Multiplication: NOT POSSIBLE\n");
        } else
        {
            printf("Multiplication: POSSIBLE\n");
        }
    } 


    free(Result1);
    free(Result2);
    free(Result3);
    free(Result4);
    return 0;

    // Matrix multiplication fails when columns of A do not equal to Rows of B i.e assuming A is NxM and B is KxP matrices
    // matrix multiplication fails when M != K 
}
