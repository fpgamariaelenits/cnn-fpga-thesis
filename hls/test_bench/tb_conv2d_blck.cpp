#include "hls_stream.h"
#include "definitions.h"
#include <iostream>
#include <vector>
#include <cstdint>
#include "ConvPerChannel_data_1.h" 

void conv2d_blck(hls::stream<int>& out_stream, ConvType conv_type);

int main() {
    hls::stream<int> out_stream;

    // Select Convolution Type
    const ConvType current_conv_type = POINTWISE;

    std::cout << "Starting Simulation..." << std::endl;
    std::cout << "Input Dims: " << input_height << "x" << input_width << "x" << input_depth << std::endl;
    std::cout << "Filter Dims: " << output_depth << "x" << input_depth << "x" << filter_height << "x" << filter_width << std::endl;

    // Kernel Call
    conv2d_blck(out_stream, current_conv_type);

    // Read Output and Verify
    const int OUT_H = output_height;
    const int OUT_W = output_width;
    const int OC    = output_depth;

    std::cout << "Expected Output dims: " << OUT_H << " x " << OUT_W << " x " << OC << "\n";

    int mismatches = 0;
    int total = 0;
    int rIdx = 0;

    // NHWC loop verification
    for (int i = 0; i < OUT_H; ++i) {
        for (int j = 0; j < OUT_W; ++j) {
            for (int f = 0; f < OC; ++f) {
                if (out_stream.empty()) {
                    std::cerr << "Error: Stream empty!" << std::endl;
                    return 1;
                }

                int val = out_stream.read();
                int ref = (int)result[rIdx++];

                total++;
                if (val != ref) {
                    mismatches++;
                    if (mismatches < 10) {
                        std::cout << "Mismatch at (" << i << "," << j << "," << f << "): "
                                  << "Expected " << ref << " but got " << val << "\n";
                    }
                }
            }
        }
    }

    std::cout << "Compared " << total << " outputs." << std::endl;
    std::cout << "Mismatches: " << mismatches << std::endl;

    if (mismatches == 0) {
        std::cout << "Test PASSED!" << std::endl;
        return 0;
    } else {
        std::cout << "Test FAILED (but rounding errors might be expected)." << std::endl;
        return 1;
    }
}
