#include <tceops.h>
 
#define HEADER_SIZE 44
#define NUM_COEFFS 4
#define COEFF_0 37
#define COEFF_1 109
#define COEFF_2 109
#define COEFF_3 37
#define BUFFER_SIZE 3
 
 
typedef unsigned char uint8_t;
 
int fir_filter(int input_sample, int *restrict delay_line) 
{
    int result = 0;
 
    // Update delay line with new input sample
 
    delay_line[3] = delay_line[2];
    delay_line[2] = delay_line[1];
    delay_line[1] = delay_line[0];
    delay_line[0] = input_sample;
 
    result = delay_line[0] * COEFF_0 + 
             delay_line[1] * COEFF_1 + 
             delay_line[2] * COEFF_2 + 
             delay_line[3] * COEFF_3;
 
    // Normalize result (scaling)
    result >>= 8;
 
    return result;
}
 
 
int main(void) {
    //uint8_t input, input1, output, output1;
    uint8_t input[BUFFER_SIZE];
    uint8_t output[BUFFER_SIZE];
    int signed_samples[BUFFER_SIZE];
    int filtered_samples[BUFFER_SIZE];
 
 
    int status = 1;
    int delay_line[NUM_COEFFS] = {0};
	int signed_sample, signed_sample1, filtered_sample, filtered_sample1;
 
    // Copy header from input to output
    uint8_t header[HEADER_SIZE];
 
	for (int i = 0; i < HEADER_SIZE; i++) {
		_TCEFU_FIFO_U8_STREAM_IN("stream_in", 0, header[i], status);
    	_TCEFU_FIFO_U8_STREAM_OUT("stream_out", header[i]);
	}
	_TCEFU_FIFO_U8_STREAM_OUT("stream_out", header);
 
 
 
 
    // Process audio samples
    while (status > 0) {
 
        _TCEFU_FIFO_U8_STREAM_IN("stream_in", 0, input[0], status);
        _TCEFU_FIFO_U8_STREAM_IN("stream_in", 0, input[1], status);
        _TCEFU_FIFO_U8_STREAM_IN("stream_in", 0, input[2], status);
 
 
        // Convert unsigned 8-bit to signed 8-bit
        signed_samples[0] = (int)input[0] - 128;
        signed_samples[1] = (int)input[1] - 128;
        signed_samples[2] = (int)input[2] - 128;
 
        // Apply FIR filter
        filtered_samples[0] = fir_filter(signed_samples[0], delay_line);
        filtered_samples[1] = fir_filter(signed_samples[1], delay_line);
        filtered_samples[2] = fir_filter(signed_samples[2], delay_line);
 
        // Convert back to unsigned 8-bit
        output[0] = (uint8_t)(filtered_samples[0] + 128);
        output[1] = (uint8_t)(filtered_samples[1] + 128);
        output[2] = (uint8_t)(filtered_samples[2] + 128);
 
        _TCEFU_FIFO_U8_STREAM_OUT("stream_out", output[0]);
        _TCEFU_FIFO_U8_STREAM_OUT("stream_out", output[1]);
        _TCEFU_FIFO_U8_STREAM_OUT("stream_out", output[2]);
 
    }
 
    return 0;
}
