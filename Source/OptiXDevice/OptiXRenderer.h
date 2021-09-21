//
// Created by jackjwang on 2021/7/4.
//

#ifndef WJ_PLAYGROUND_OPTIXRENDERER_H
#define WJ_PLAYGROUND_OPTIXRENDERER_H

#include "CUDABuffer.h"
#include "LaunchParams.h"
#include "TriangleMesh.h"
#include "gdt/math/vec.h"

using namespace gdt;

namespace gdt
{
    struct Camera {
        /*! camera position - *from* where we are looking */
        vec3f from;
        /*! which point we are looking *at* */
        vec3f at;
        /*! general up-vector */
        vec3f up;
    };

    class OptiXRenderer {
    public:
        /*! constructor - performs all setup, including initializing
          optix, creates module, pipeline, programs, SBT, etc. */
        OptiXRenderer(const std::vector<TriangleMesh>& model, uint32_t spp);

        /*! render one frame */
        void render();

        /*! resize frame buffer to given resolution */
        void resize(const vec2i &newSize);

        /*! download the rendered color buffer */
        void downloadPixels(uint32_t h_pixels[]);

        /*! set camera to render with */
        void setCamera(const Camera &camera);
    protected:
        // ------------------------------------------------------------------
        // internal helper functions
        // ------------------------------------------------------------------

        /*! helper function that initializes optix and checks for errors */
        void initOptix();

        /*! creates and configures a optix device context (in this simple
          example, only for the primary GPU device) */
        void createContext();

        /*! creates the module that contains all the programs we are going
          to use. in this simple example, we use a single module from a
          single .cu file, using a single embedded ptx string */
        void createModule();

        /*! does all setup for the raygen program(s) we are going to use */
        void createRaygenPrograms();

        /*! does all setup for the miss program(s) we are going to use */
        void createMissPrograms();

        /*! does all setup for the hitgroup program(s) we are going to use */
        void createHitgroupPrograms();

        /*! assembles the full pipeline of all programs */
        void createPipeline();

        /*! optix ai denoiser */
        void denoise();

        /*! constructs the shader binding table */
        void buildSBT();

        /*! build an acceleration structure for the given triangle mesh */
        OptixTraversableHandle buildAccel(const std::vector<TriangleMesh>& model);

        bool setupLaunchParams(uint32_t spp);

        void computeFinalPixelColors();

    private:
        CUcontext           cudaContext;
        CUstream            stream;
        cudaDeviceProp      deviceProps;

        //! the optix context that our pipeline will run in.
        OptixDeviceContext optixContext;

        /*! @{ the pipeline we're building */
        OptixPipeline               pipeline;
        OptixPipelineCompileOptions pipelineCompileOptions = {};
        OptixPipelineLinkOptions    pipelineLinkOptions    = {};
        /*! @} */

        /*! @{ the module that contains out device programs */
        OptixModule                 module;
        OptixModuleCompileOptions   moduleCompileOptions = {};
        /* @} */

        /*! vector of all our program(group)s, and the SBT built around
            them */
        std::vector<OptixProgramGroup> raygenPGs;
        CUDABuffer raygenRecordsBuffer;
        std::vector<OptixProgramGroup> missPGs;
        CUDABuffer missRecordsBuffer;
        std::vector<OptixProgramGroup> hitgroupPGs;
        CUDABuffer hitgroupRecordsBuffer;
        OptixShaderBindingTable sbt = {};

        OptixDenoiser denoiser;

        /*! @{ our launch parameters, on the host, and the buffer to store
            them on the device */
        LaunchParams launchParams;
        CUDABuffer   launchParamsBuffer;
        /*! @} */

        CUDABuffer sourceFrameBuffer;
        CUDABuffer denoiseFrameBuffer;
        CUDABuffer finalColorBuffer;

        CUDABuffer denoiserStateBuffer;
        CUDABuffer denoiserScratchBuffer;

        /*! the camera we are to render with. */
        Camera lastSetCamera;

        /*! the models we are going to trace rays against */
        std::vector<TriangleMesh> models;
        std::vector<CUDABuffer> vertexBuffer;
        std::vector<CUDABuffer> indexBuffer;
        //! buffer that keeps the (final, compacted) accel structure
        CUDABuffer asBuffer;

        // Enable denoiser?
        bool enableDenoiser;
    };
}

#endif //WJ_PLAYGROUND_OPTIXRENDERER_H
