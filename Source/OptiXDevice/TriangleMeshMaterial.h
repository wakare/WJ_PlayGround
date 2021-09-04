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
    ETriangleMeshMaterialType MaterialType = ETMMT_Diffuse;

    vec3f Emissive;
    vec3f Diffuse;

    __both__ void SampleDirection(LCG<>& RandGenerator, const vec3f& normal, const vec3f& inDirection, vec3f& outDirection, float& outPdf) const
    {
        // Only implement diffuse sample
        assert(MaterialType == ETMMT_Diffuse);

        //outDirection = normal;
        //outPdf = 1.0f;
        //return;

        const float rand0 = 2.0f * M_PI * RandGenerator();
        const float rand1 = 0.5f * M_PI * RandGenerator();

        vec3f u = normalize(normal);
        vec3f v = normalize(cross((abs(u.x) > FLT_EPSILON) ? vec3f (0.0f, 1.0f, 0.0f) : vec3f(1.0f, 0.0f, 0.0f), u));
        vec3f w = normalize(cross(u, v));

        outDirection = sinf(rand0) * cosf(rand1) * u + cosf(rand0) * cosf(rand1) * v + sinf(rand1) * w;

        /*printf("[INFO] rand0 %f rand1 %f u %f %f %f v %f %f %f w %f %f %f Sample direction %f %f %f\n", rand0, rand1,
               u.x, u.y, u.z,
               v.x, v.y, v.z,
               w.x, w.y, w.z,
               outDirection.x, outDirection.y, outDirection.z);*/

        normalize(outDirection);

        outPdf = 0.5f * M_PI_INV;
    }
};




#endif //WJ_PLAYGROUND_TRIANGLEMESHMATERIAL_H
