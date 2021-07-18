//
// Created by jackjwang on 2021/7/18.
//

#ifndef WJ_PLAYGROUND_LAUNCHPARAMS_H
#define WJ_PLAYGROUND_LAUNCHPARAMS_H

#include "gdt/math/vec.h"

using namespace gdt;

namespace gdt
{
    struct LaunchParams
    {
        int       frameID { 0 };
        uint32_t *colorBuffer;
        vec2i     fbSize;
    };
}

#endif //WJ_PLAYGROUND_LAUNCHPARAMS_H
