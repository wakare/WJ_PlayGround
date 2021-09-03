//
// Created by jackjwang on 2021/8/29.
//

#ifndef WJ_PLAYGROUND_TRIANGLEMESHMATERIAL_H
#define WJ_PLAYGROUND_TRIANGLEMESHMATERIAL_H

#include "gdt/random/random.h"

using namespace gdt;

enum ETriangleMeshMaterialType
{
    ETMMT_Diffuse,
    ETMMT_Glossy,
    ETMMT_Specular
};

struct TriangleMeshMaterial
{
    ETriangleMeshMaterialType MaterialType;

    vec3f Emissive;
    vec3f Diffuse;


    __both__ void SampleDirection(LCG<>& RandGenerator, const vec3f& inDirection, vec3f& outDirection, float& outPdf) const
    {
        // Only implement diffuse sample
        assert(MaterialType == ETMMT_Diffuse);
        float rand0 = 2.0f * M_PI * RandGenerator() / LCG<>::LCG_RAND_MAX;
        float rand1 = 0.5f * M_PI * RandGenerator() / LCG<>::LCG_RAND_MAX;

        outDirection.x = sinf(rand0) * cosf(rand1);
        outDirection.y = cosf(rand0) * cosf(rand1);
        outDirection.z = sinf(rand1);

        outPdf = 0.5f * M_PI_INV;
    }
};




#endif //WJ_PLAYGROUND_TRIANGLEMESHMATERIAL_H
