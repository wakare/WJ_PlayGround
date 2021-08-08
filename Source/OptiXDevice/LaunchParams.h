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
        struct {
            uint32_t *colorBuffer;
            vec2i     size;
        } frame;

        struct {
            vec3f position;
            vec3f direction;
            vec3f horizontal;
            vec3f vertical;
        } camera;

        OptixTraversableHandle traversable;
    };
}

#endif //WJ_PLAYGROUND_LAUNCHPARAMS_H
