
#include <opencv2/core/cuda/vec_traits.hpp>

#include "overlay.h"

void _overlay_image(uchar4* const h_base_image_rgba,
				   int h_base_rows, int h_base_columns,
				   struct OverlayImage h_overlay_image_rgba);