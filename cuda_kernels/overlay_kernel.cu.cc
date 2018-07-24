#include <cstdio>

#include "overlay.h"

// Kernel function to add the elements of two arrays
__global__
void overlay_image_kernel(uchar4* const d_base_image_rgba,
                          int d_base_rows, int d_base_columns,
						              uchar4* const d_overlay_image_rgba,
                          int d_overlay_rows, int d_overlay_columns)
{
  int d_current_row = blockIdx.y * blockDim.y + threadIdx.y;
  int d_current_column = blockIdx.x * blockDim.x + threadIdx.x;

  if (d_current_row < d_overlay_rows && d_current_column < d_overlay_columns) {
    int d_overlay_i = d_current_row * d_overlay_columns + d_current_column;
    int d_base_i = d_current_row * d_base_columns + d_current_column;

    uchar4 input_byte = d_overlay_image_rgba[d_overlay_i];

    uchar4 output_byte = (uchar4) d_base_image_rgba[d_base_i];
    output_byte.x = (unsigned char) input_byte.x;
    output_byte.y = (unsigned char) input_byte.y;
    output_byte.z = (unsigned char) input_byte.z;
    output_byte.w = (unsigned char) input_byte.w;
    d_base_image_rgba[d_base_i] = output_byte;
	}
}

void _overlay_image(uchar4* const h_base_image_rgba,
				            int h_base_rows, int h_base_columns,
				            struct OverlayImage h_overlay_image_rgba)
{
  // Total number of pixels in each image
  int base_num_pixels = h_base_rows * h_base_columns;
  int overlay_rows = h_overlay_image_rgba.rows;
  int overlay_cols = h_overlay_image_rgba.cols;
  int overlay_num_pixels = overlay_rows * overlay_cols;

  // GPU data
  uchar4 *d_base_image_rgba;
  uchar4 *d_overlay_image_rgba;

  // Allocate GPU memory for rgba and grey images
  cudaMalloc((void **)&d_base_image_rgba, base_num_pixels * sizeof(uchar4));
  cudaMalloc((void **)&d_overlay_image_rgba, overlay_num_pixels * sizeof(uchar4));
//  cudaMalloc((void **)&d_output_image_rgba, num_pixels * sizeof(uchar4));

  // Copy rgba image from CPU memory to GPU memory
  cudaMemcpy(d_base_image_rgba, h_base_image_rgba, base_num_pixels * sizeof(uchar4),
             cudaMemcpyHostToDevice);
  cudaMemcpy(d_overlay_image_rgba, h_overlay_image_rgba.image_rgba, overlay_num_pixels * sizeof(uchar4),
             cudaMemcpyHostToDevice);

  // Launch rgba_to_grayscale CUDA kernel on GPU
  const dim3 blockSize(32, 32, 1);
  const dim3 gridSize((overlay_cols / 32) + 1, (overlay_rows / 32) + 1, 1);
  overlay_image_kernel<<<gridSize, blockSize>>>(d_base_image_rgba,
                                                h_base_rows, h_base_columns,
                                                d_overlay_image_rgba,
                                                overlay_rows, overlay_cols);
  // Copy grey image from GPU memory to CPU memory
  cudaMemcpy(h_base_image_rgba, d_base_image_rgba, base_num_pixels * sizeof(uchar4),
             cudaMemcpyDeviceToHost);

  // Free GPU memory
  cudaFree(d_base_image_rgba);
  cudaFree(d_overlay_image_rgba);
//  cudaFree(d_output_image_rgba);
}