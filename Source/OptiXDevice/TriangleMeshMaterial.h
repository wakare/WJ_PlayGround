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
    ETMMT_Specular,
    ETMMT_Refract,
};

struct TriangleMeshMaterial
{
    ETriangleMeshMaterialType MaterialType = ETMMT_Diffuse;

    vec3f Emissive;
    vec3f Diffuse;
    vec3f Specular;

    __both__ void SampleDirection(LCG<>& RandGenerator, const vec3f& normal, const vec3f& inDirection, vec3f& outDirection, float& outPdf) const
    {
        switch (MaterialType)
        {
            case ETMMT_Diffuse:
            {
                const float rand0 = 2.0f * M_PI * RandGenerator();
                const float rand1 = 0.5f * M_PI * RandGenerator();

                vec3f u = normalize(normal);
                vec3f v = normalize(cross((abs(u.x) > FLT_EPSILON) ? vec3f (0.0f, 1.0f, 0.0f) : vec3f(1.0f, 0.0f, 0.0f), u));
                vec3f w = normalize(cross(u, v));

                outDirection = sinf(rand0) * cosf(rand1) * w + cosf(rand0) * cosf(rand1) * v + sinf(rand1) * u;
                normalize(outDirection);

                outPdf = 0.5f * M_PI_INV;

            }
            break;

            case ETMMT_Specular:
            {
                outDirection = normalize(normalize(inDirection) + 2.0f * normalize(normal));
                outPdf = 1.0f;
            }
            break;

            case ETMMT_Refract:
            {


            }
            break;
        }
    }
};




#endif //WJ_PLAYGROUND_TRIANGLEMESHMATERIAL_H
