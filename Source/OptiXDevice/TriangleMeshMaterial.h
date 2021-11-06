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

    __both__ void SampleDirection(LCG<>& RandGenerator, const vec3f& normal, const vec3f& inDirection, vec3f& outDirection, float& outPdf, float& outRayOffset) const
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

                outRayOffset = -0.0001f;
                outPdf = 1.0f;

            }
            break;

            case ETMMT_Specular:
            {
                outDirection = normalize(normalize(inDirection) + 2.0f * normalize(normal));

                outRayOffset = -0.0001f;
                outPdf = 1.0f;
            }
            break;

            case ETMMT_Refract:
            {
                // Assume glass material refract
                const bool bIntoInternal = dot(normal, inDirection) < 0.0f;
                vec3f refractNormal = bIntoInternal ? normal : -normal;
                vec3f reflectDirection = normalize(inDirection - refractNormal * 2 * dot(refractNormal, inDirection));

                float nc = 1.0f;
                float nt = 1.5f;
                float nnt = (bIntoInternal) ? (nc / nt) : (nt / nc);
                float ddn = dot(inDirection, normal);

                float cos2t = 1.0f - nnt * nnt * (1 - ddn * ddn);
                if (cos2t < 0.0f)
                {
                    // Total internal reflect
                    outDirection = reflectDirection;

                    outRayOffset = -0.0001f;
                    outPdf = 1.0f;
                }
                else
                {
                    vec3f refractDirection = normalize(inDirection * nnt - refractNormal * (ddn * nnt + sqrt(cos2t)));
                    double a = nt - nc, b = nt + nc, R0 = a * a / (b * b), c = 1 + (bIntoInternal ? ddn : dot(refractDirection, refractNormal));
                    double Re = R0 + (1 - R0) * c * c * c * c * c, Tr = 1 - Re, P = 0.25 + 0.5 * Re, RP = Re / P, TP = Tr / (1 - P);

                    if (RandGenerator() < P)
                    {
                        outDirection = reflectDirection;
                        outRayOffset = -0.0001f;
                        outPdf = P;
                    }
                    else
                    {
                        outDirection = refractDirection;
                        if (!bIntoInternal)
                        {
                            //outDirection = inDirection;
                            //outDirection =-outDirection;
                        }
                        //if (dot(outDirection, refractNormal) > 0.0f)
                        //{
                        //    printf("[DEBUG] Error refract direction!\n");
                        //}
                        outRayOffset = 0.0001f;
                        outPdf = 1 - P;
                    }
                }
            }
            break;
        }
    }
};




#endif //WJ_PLAYGROUND_TRIANGLEMESHMATERIAL_H
