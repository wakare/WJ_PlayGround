//
// Created by jackjwang on 2021/7/3.
// Define ray tracing basic struct
//

#ifndef WJ_PLAYGROUND_PTBASICDEFINE_H
#define WJ_PLAYGROUND_PTBASICDEFINE_H
#include <cstdlib>
#include <cassert>
#include <Eigen/Dense>
#include "PTAPI.h"

namespace PT
{

// Class util definition
#define NO_COPY_CTOR(ClassName) ClassName(const ClassName&) = delete;
#define NO_MOVE_CTOR(ClassName) ClassName(ClassName&&) = delete;

#define NO_COPY_MOVE_CTOR(ClassName) \
    NO_COPY_CTOR(ClassName)          \
    NO_MOVE_CTOR(ClassName)

//  Math util definition
#define NO_HIT_DIST FLT_MAX
#define PT_PI EIGEN_PI

    using PTVector3f = Eigen::Vector3f;
    using PTVector3d = Eigen::Vector3d;

    template<typename scalar>
    class PTRandom
    {
    public:
        static void SetSeed(int seed)
        {
            srand(seed);
        }

        static scalar Generate()
        {
            assert(false);
            return scalar();
        }

        static scalar Generate01()
        {
            assert(false);
            return scalar();
        }
    };

    template<>
    inline float PTRandom<float>::Generate()
    {
        return 1.0f * rand();
    }

    template<>
    inline float PTRandom<float>::Generate01()
    {
        return Generate() / (RAND_MAX + 1);
    }

    // RGB spectrum
    struct PTRGBSpectrum
    {
        float r = 0.0f;
        float g = 0.0f;
        float b = 0.0f;
    };

    class PT_API PTCamera
    {
    public:
        NO_COPY_MOVE_CTOR(PTCamera)
        PTCamera() {}
        PTCamera(const PTVector3f& pos, const PTVector3f& dir) : Pos(pos), Dir(dir.normalized()) {}

        PTVector3f Pos;
        PTVector3f Dir;
    };

    enum MaterialReflectType
    {
        EMRT_DIFFUSE,
        EMRT_GLOSSY,
        EMRT_SPECULAR,
        EMRT_REFRACT,
        EMRT_UNKNOWN = 1000
    };

    class PT_API PTMaterial
    {
    public:
        NO_COPY_MOVE_CTOR(PTMaterial)
        PTMaterial(MaterialReflectType reflectType, const Eigen::Vector3f& diffuse, const Eigen::Vector3f& emission)
        : ReflectType(reflectType)
        , Diffuse(diffuse)
        , Emission(emission)
        {

        }

        MaterialReflectType ReflectType = EMRT_UNKNOWN;
        Eigen::Vector3f Diffuse;
        Eigen::Vector3f Emission;
    };

    // Hit result
    struct PTPayload
    {
        // Nearest hit distance
        float Dist = NO_HIT_DIST;

        // Hit RGBSpectrum
        PTRGBSpectrum Color;

        // Hit Material
        PTMaterial Mat;

        bool IsHit() const
        {
            return Dist != NO_HIT_DIST;
        }


    };
}

#endif //WJ_PLAYGROUND_PTBASICDEFINE_H
