#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <math.h>
#include <string.h>

#define FP 128 // 2^7


// NOTE: Copied from half_to_float.c from Lab1
// Static function to convert float bits to float16 representation
static uint16_t float_to_halfI(uint32_t i) {
    int s =  (i >> 16) & 0x00008000;                   // sign
    int e = ((i >> 23) & 0x000000ff) - (127 - 15);     // exponent
    int f =   i        & 0x007fffff;                   // fraction

    // Handle NaNs and Infs
    if (e <= 0) {
        if (e < -10) {
            if (s)  // Handle -0.0
                return 0x8000;
            else
                return 0;
        }
        f = (f | 0x00800000) >> (1 - e);
        return s | (f >> 13);
    } else if (e == 0xff - (127 - 15)) {
        if (f == 0)  // Inf
            return s | 0x7c00;
        else {  // NaN
            f >>= 13;
            return s | 0x7c00 | f | (f == 0);
        }
    } else {
        if (e > 30)  // Overflow
            return s | 0x7c00;
        return s | (e << 10) | (f >> 13);
    }
}

// Static function to convert float16 bits to float representation
static uint32_t half_to_floatI(uint16_t y) {
    int s = (y >> 15) & 0x00000001;                   // sign
    int e = (y >> 10) & 0x0000001f;                   // exponent
    int f =  y        & 0x000003ff;                   // fraction

    // Handle special cases
    if (e == 0) {
        if (f == 0)  // Plus or minus zero
            return s << 31;
        else {  // Denormalized number -- renormalize it
            while (!(f & 0x00000400)) {
                f <<= 1;
                e -=  1;
            }
            e += 1;
            f &= ~0x00000400;
        }
    } else if (e == 31) {
        if (f == 0)  // Inf
            return (s << 31) | 0x7f800000;
        else  // NaN
            return (s << 31) | 0x7f800000 | (f << 13);
    }

    e = e + (127 - 15);
    f = f << 13;

    return ((s << 31) | (e << 23) | f);
}

// Function to convert float to float16 representation
uint16_t float_to_float16(float value) {
    union { float f; uint32_t i; } v;
    v.f = value;
    return float_to_halfI(v.i);
}

// Function to convert float16 to float representation
float float16_to_float(uint16_t value) {
    union { float f; uint32_t i; } v;
    v.i = half_to_floatI(value);
    return v.f;
}

//  Fix_Or_Float16 = false -> fixed point
//  Fix_Or_Float16 = true -> float16
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
    
    int total_iterations = 0;

    for( int i = 0; i < rows_of_A; i++ )
    {
        for( int j = 0; j < columns_of_B; j++ )
        {
            // i is row, j is column

            for (int k = 0; k < columns_of_A; k++)
            {

                if (Fix_Or_Float16 == false)
                {
                    // loop used to calculate the dot product
                    int A_fix = A[i * rows_of_A + k] * FP;
                    int B_fix = B[k * columns_of_B + j] * FP;

                    int product = A_fix * B_fix;

                    Result[total_iterations] += (float)product / (FP * FP); // scale back to floating point presentation, could also be done using bit shifting but fractional parts would require additional configuration

                } else
                {
                    // Float16 implementation
                    uint16_t A_f16 = float_to_float16(A[i * columns_of_A + k]);
                    uint16_t B_f16 = float_to_float16(B[k * columns_of_B + j]);
                    float A_f32 = float16_to_float(A_f16);
                    float B_f32 = float16_to_float(B_f16);
                    float product = A_f32 * B_f32;
                    Result[total_iterations] += product;
                }
            }
            //printf("%d %f\n", total_iterations, Result[total_iterations]);
            total_iterations++;
        }

    }

    return 1;
}

int main( void )
{
    float A1[16], B1[16]; // two 4x4 matrices
    float A2[4], B2[4]; // two square (2x2) matrices
    float A3[8], B3[8]; // A is 4x2 and B is 2x4 matrices
    float A4[8], B4[8]; // A is 4x2 and B is 4x2 matrices (this should return 0)

    float A5[9] = {1.122222222, 2.23333333333333, 3.4555555555555555555, 4.56, 5.78, 6.19, 7.23, 8.54, 9.12}; 
    float B5[9] = { 1, 0, 0,
                    0, 1, 0, 
                    0, 0, 1 }; 

    float Result1[16] = {0};
    float Result2[4] = {0};
    float Result3[8] = {0};
    float Result4[8] = {0};
    float Result5[9] = {0};


    int status[5];

    //status[0] = Matrix_Mul(A1, B1, Result1, 4, 4, 4, 4, false);
    //status[1] = Matrix_Mul(A2, B1, Result2, 2, 2, 2, 2, false);
    //status[2] = Matrix_Mul(A3, B1, Result3, 4, 2, 2, 4, false);
    //status[3] = Matrix_Mul(A4, B4, Result4, 4, 2, 4, 2, false);
    status[4] = Matrix_Mul(A5, B5, Result5, 3, 3, 3, 3, true);


    // Check Result5

    printf("Print result of Result5 matrix\n");
    int total_iter = 0;
    for(int i = 0; i < 3; i++)
    {
        for( int j = 0; j < 3; j++)
        {   
            printf("%f ", Result5[total_iter]);
            total_iter++;       
        }
        printf("\n");
    }

    printf("\nPrinting statuses\n");

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

    return 0;

    // Matrix multiplication fails when columns of A do not equal to Rows of B i.e assuming A is NxM and B is KxP matrices
    // matrix multiplication fails when M != K 
}
