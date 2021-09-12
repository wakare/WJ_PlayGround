//
// Created by jackjwang on 2021/7/18.
//

#ifndef WJ_PLAYGROUND_PTUTIL_H
#define WJ_PLAYGROUND_PTUTIL_H

#include "stb_image_write.h"

class PTUtil {
public:
    static bool WritePNGChannel(const char* output_filename, const void *data, int channel, int width, int height);
};


#endif //WJ_PLAYGROUND_PTUTIL_H
