//
// Created by jackjwang on 2021/7/18.
//

#ifndef WJ_PLAYGROUND_LAUNCHPARAMS_H
#define WJ_PLAYGROUND_LAUNCHPARAMS_H

#include "gdt/math/vec.h"
#include "TriangleMesh.h"

using namespace gdt;

namespace gdt
{
    struct HitGroupUserParams
    {
        // vec3f color;
        TriangleMeshMaterial MeshMaterial;
        vec3f *vertex;
        vec3i *index;
    };

    struct LaunchParams
    {
        struct
        {
            float3* source;
            vec2i size;
        } sourceFrame;

        struct
        {
            uint32_t *colorBuffer;
            vec2i     size;
        } frame;

        struct
        {
            vec3f position;
            vec3f direction;
            vec3f horizontal;
            vec3f vertical;
        } camera;

        OptixTraversableHandle traversable;
        uint32_t spp;
        uint32_t frameIndex;
    };

    struct RayPayload
    {
        vec3f color;
        int bounceCount;
        float pdf;
        LCG<> RandGenerator;
    };
}

#endif //WJ_PLAYGROUND_LAUNCHPARAMS_H
