// ======================================================================== //
// Copyright 2018-2019 Ingo Wald                                            //
//                                                                          //
// Licensed under the Apache License, Version 2.0 (the "License");          //
// you may not use this file except in compliance with the License.         //
// You may obtain a copy of the License at                                  //
//                                                                          //
//     http://www.apache.org/licenses/LICENSE-2.0                           //
//                                                                          //
// Unless required by applicable law or agreed to in writing, software      //
// distributed under the License is distributed on an "AS IS" BASIS,        //
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied. //
// See the License for the specific language governing permissions and      //
// limitations under the License.                                           //
// ======================================================================== //

#include <optix_device.h>

#include "LaunchParams.h"

using namespace gdt;

namespace gdt {
    /*! launch parameters in constant memory, filled in by optix upon
      optixLaunch (this gets filled in from the buffer we pass to
      optixLaunch) */
    extern "C" __constant__ LaunchParams optixLaunchParams;

    // for this simple example, we have a single ray type
    enum { SURFACE_RAY_TYPE=0, RAY_TYPE_COUNT };

    static __forceinline__ __device__
    void *unpackPointer( uint32_t i0, uint32_t i1 )
    {
        const uint64_t uptr = static_cast<uint64_t>( i0 ) << 32 | i1;
        void*           ptr = reinterpret_cast<void*>( uptr );
        return ptr;
    }

    static __forceinline__ __device__
    void  packPointer( void* ptr, uint32_t& i0, uint32_t& i1 )
    {
        const uint64_t uptr = reinterpret_cast<uint64_t>( ptr );
        i0 = uptr >> 32;
        i1 = uptr & 0x00000000ffffffff;
    }

    template<typename T>
    static __forceinline__ __device__ T *getPRD()
    {
        const uint32_t u0 = optixGetPayload_0();
        const uint32_t u1 = optixGetPayload_1();
        return reinterpret_cast<T*>( unpackPointer( u0, u1 ) );
    }

    static __forceinline__ __device__ vec3f colorMul(const vec3f& color0, const vec3f& color1)
    {
        return {color0.x * color1.x, color0.y * color1.y, color0.z * color1.z};
    }

    static __forceinline__ __device__ vec3f scaleVector(const vec3f& vector, float scale)
    {
        return {scale * vector.x,scale * vector.y,scale * vector.z };
    }

    static __forceinline__ __device__ bool hasNAN(const vec3f& vector)
    {
         return isnan(vector.x) || isnan(vector.y) || isnan(vector.z);
    }

    static __forceinline__ __device__ bool hasINF(const vec3f& vector)
    {
        return isinf(vector.x) || isinf(vector.y) || isinf(vector.z);
    }

    //------------------------------------------------------------------------------
    // closest hit and anyhit programs for radiance-type rays.
    //
    // Note eventually we will have to create one pair of those for each
    // ray type and each geometry type we want to render; but this
    // simple example doesn't use any actual geometries yet, so we only
    // create a single, dummy, set of them (we do have to have at least
    // one group of them to set up the SBT)
    //------------------------------------------------------------------------------

    extern "C" __global__ void __closesthit__radiance()
    {
        /*const int   primID = optixGetPrimitiveIndex();
        vec3f &prd = *(vec3f*)getPRD<vec3f>();
        prd = gdt::randomColor(primID);*/

        const HitGroupUserParams* hitParams = (const HitGroupUserParams*)optixGetSbtDataPointer();
        RayPayload& payload = (*getPRD<RayPayload>());
        ++payload.bounceCount;

        //printf("Current bounce count: %d\n", payload.bounceCount);

        const int   primID = optixGetPrimitiveIndex();
        const vec3i index = hitParams->index[primID];
        const vec3f& v0 = hitParams->vertex[index.x];
        const vec3f& v1 = hitParams->vertex[index.y];
        const vec3f& v2 = hitParams->vertex[index.z];
        const vec3f normal = normalize(cross(v1 - v0, v0 - v2));

        vec3f sampleDirection = vec3f(0.0f, 0.0f, 0.0f);
        float samplePdf = 0.0f;
        hitParams->MeshMaterial.SampleDirection(payload.RandGenerator, normal, optixGetWorldRayDirection(),
                                                sampleDirection, samplePdf);

        // Apply ray pdf
        float pdf = payload.pdf;
        //if (payload.bounceCount > 5)
        {
            if (payload.bounceCount > 20)
            {
                payload.color = hitParams->MeshMaterial.Emissive / pdf;
                return;
            }

            const vec3f& MatDiffuse = hitParams->MeshMaterial.Diffuse;
            float maxChannel = max(MatDiffuse.z, max(MatDiffuse.x, MatDiffuse.y));
            float rand = payload.RandGenerator();
            if (rand >= maxChannel || maxChannel < 0.00001f)
            {
                payload.color = hitParams->MeshMaterial.Emissive / pdf;
                return;
            }

            payload.pdf = maxChannel;
        }

        uint32_t u0 = optixGetPayload_0();
        uint32_t u1 = optixGetPayload_1();
        vec3f hitLocation = (vec3f)optixGetWorldRayOrigin() + scaleVector(optixGetWorldRayDirection(),
                                                                          optixGetRayTmax() - 0.00001f);

        //float2 baryCentricesCoord = optixGetTriangleBarycentrics();
        //vec3f location =  baryCentricesCoord.x * v1 + baryCentricesCoord.y * v2 + (1 - baryCentricesCoord.x - baryCentricesCoord.y) * v0;
        //printf ("location %f %f %f  bc %f %f %f  %f %f %f\n", hitLocation.x,hitLocation.y,hitLocation.z,
        //        baryCentricesCoord.x, baryCentricesCoord.y, 1 - baryCentricesCoord.x-baryCentricesCoord.y,
        //        location.x,location.y,location.z);

        optixTrace(optixLaunchParams.traversable,
                   hitLocation,
                   sampleDirection,
                   0.f,    // tmin
                   1e20f,  // tmax
                   0.0f,   // rayTime
                   OptixVisibilityMask( 255 ),
                   OPTIX_RAY_FLAG_DISABLE_ANYHIT,//OPTIX_RAY_FLAG_NONE,
                   SURFACE_RAY_TYPE,             // SBT offset
                   RAY_TYPE_COUNT,               // SBT stride
                   SURFACE_RAY_TYPE,             // missSBTIndex
                   u0, u1);

        payload.color = (colorMul(hitParams->MeshMaterial.Diffuse, payload.color) + hitParams->MeshMaterial.Emissive) / pdf;
    }

    extern "C" __global__ void __anyhit__radiance()
    { /*! for this simple example, this will remain empty */ }



    //------------------------------------------------------------------------------
    // miss program that gets called for any ray that did not have a
    // valid intersection
    //
    // as with the anyhit/closest hit programs, in this example we only
    // need to have _some_ dummy function to set up a valid SBT
    // ------------------------------------------------------------------------------

    extern "C" __global__ void __miss__radiance()
    {
        RayPayload &prd = *getPRD<RayPayload>();
        // set to constant white as background color
        if (prd.bounceCount == 0)
        {
            prd.color = {0.0f, 0.0f, 0.0f};
        }
    }

    //------------------------------------------------------------------------------
    // ray gen program - the actual rendering happens in here
    //------------------------------------------------------------------------------
    extern "C" __global__ void __raygen__renderFrame()
    {
        const int spp = optixLaunchParams.spp;

        // compute a test pattern based on pixel ID
        const int ix = optixGetLaunchIndex().x;
        const int iy = optixGetLaunchIndex().y;
        const int frameIndex = optixLaunchParams.frameIndex;

        const auto &camera = optixLaunchParams.camera;

        // normalized screen plane position, in [0,1]^2
        const vec2f screen(vec2f(ix+.5f, iy+.5f)
                           / vec2f(optixLaunchParams.frame.size));

        // generate ray direction
        vec3f rayDir = normalize(camera.direction
                                 + (screen.x - 0.5f) * camera.horizontal
                                 + (screen.y - 0.5f) * camera.vertical);

        // our per-ray data for this example. what we initialize it to
        // won't matter, since this value will be overwritten by either
        // the miss or hit program, anyway
        RayPayload Payload;
        Payload.RandGenerator = LCG<>((frameIndex * 21321424 * iy) ^ ix, (frameIndex * 92374894 * ix) ^ iy);

        //vec3f pixelColorPRD = vec3f(0.f);
        // the values we store the PRD pointer in:
        uint32_t u0, u1;
        //packPointer( &pixelColorPRD, u0, u1 );
        packPointer( &Payload, u0, u1 );

        vec3f totalColor = {0.0f, 0.0f, 0.0f};
        for (int i = 0; i < spp; ++i)
        {
            Payload.color = vec3f(0.0f, 0.0f, 0.0f);
            Payload.bounceCount = 0;
            Payload.pdf = 1.0f;

            optixTrace(optixLaunchParams.traversable,
                       camera.position,
                       rayDir,
                       0.f,    // tmin
                       1e20f,  // tmax
                       0.0f,   // rayTime
                       OptixVisibilityMask( 255 ),
                       OPTIX_RAY_FLAG_DISABLE_ANYHIT,//OPTIX_RAY_FLAG_NONE,
                       SURFACE_RAY_TYPE,             // SBT offset
                       RAY_TYPE_COUNT,               // SBT stride
                       SURFACE_RAY_TYPE,             // missSBTIndex
                       u0, u1 );
            totalColor += Payload.color;
        }

        totalColor /= spp;
        totalColor.x = clamp(totalColor.x, 0.0f, 1.0f);
        totalColor.y = clamp(totalColor.y, 0.0f, 1.0f);
        totalColor.z = clamp(totalColor.z, 0.0f, 1.0f);

        // and write to frame buffer ...
        const uint32_t fbIndex = ix+iy*optixLaunchParams.frame.size.x;

        // blend with previous frame result
        const float previousR = optixLaunchParams.sourceFrame.source[fbIndex].x;
        const float previousG = optixLaunchParams.sourceFrame.source[fbIndex].y;
        const float previousB = optixLaunchParams.sourceFrame.source[fbIndex].z;

        float alpha = (1.0f * frameIndex) / (frameIndex + 1);
        float oneMinusAlpha = 1.0f - alpha;
        float finalR = alpha * previousR + oneMinusAlpha * totalColor.x;
        float finalG = alpha * previousG + oneMinusAlpha * totalColor.y;
        float finalB = alpha * previousB + oneMinusAlpha * totalColor.z;

        optixLaunchParams.sourceFrame.source[fbIndex].x = finalR;
        optixLaunchParams.sourceFrame.source[fbIndex].y = finalG;
        optixLaunchParams.sourceFrame.source[fbIndex].z = finalB;

        const int finalRInt = int(255.99f * finalR);
        const int finalGInt = int(255.99f * finalG);
        const int finalBInt = int(255.99f * finalB);

        // convert to 32-bit rgba value (we explicitly set alpha to 0xff
        // to make stb_image_write happy ...
        const uint32_t rgba = 0xff000000
                              | (finalRInt<<0) | (finalGInt<<8) | (finalBInt<<16);

        optixLaunchParams.frame.colorBuffer[fbIndex] = rgba;
    }
  
} // ::osc
