#ifndef OVERLAY_IMAGE_H_
#define OVERLAY_IMAGE_H_

#include <opencv2/core/cuda/vec_traits.hpp>

struct OverlayImage {
    uchar4* image_rgba;
    int rows;
    int cols;
    int h_origin_x;
    int h_origin_y;
};

#endif