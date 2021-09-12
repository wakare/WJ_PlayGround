//
// Created by jackjwang on 2021/7/18.
//

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "PTUtil.h"

bool PTUtil::WritePNGChannel(const char *output_filename, const void *data, int channel, int width, int height)
{
    if (channel != 3  && channel != 4)
    {
        return false;
    }

    stbi_write_png(output_filename, width, height, channel, data, width * channel);
    return true;
}
