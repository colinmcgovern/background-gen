#ifndef ANIMATION_H
#define ANIMATION_H

#include <vector>
#include <string>
#include <algorithm>
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <float.h>
#include <limits.h>
#include <string.h>
#include <time.h>
#include <QString>
#include <memory>
#include <utility>
#include <map>

#include "gif.h"
#include "frame.h"
#include "utility.h"

class animation
{
public:
    animation(
        const char* input_file,

        int width,
        int height,
        int channels,
        uint8_t* rgb_image,

        uint num_frames,
        uint delay,

        uint pallete_num,
        double pallete_movement_per_frame,

        double x_movement_per_frame,
        double y_movement_per_frame,

        double x_amp,
        double x_period,
        double x_phase_shift_per_frame,

        double y_amp,
        double y_period,
        double y_phase_shift_per_frame
        );
};

#endif // ANIMATION_H
