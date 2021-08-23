//
// Created by jackjwang on 2021/7/4.
//

#include "OptiXRenderer.h"
#include <optix_function_table_definition.h>

namespace gdt {
    extern "C" char embedded_ptx_code[];

/*! SBT record for a raygen program */
    struct __align__(OPTIX_SBT_RECORD_ALIGNMENT) RaygenRecord {
        __align__(OPTIX_SBT_RECORD_ALIGNMENT) char header[OPTIX_SBT_RECORD_HEADER_SIZE];
        // just a dummy value - later examples will use more interesting
        // data here
        void *data;
    };

/*! SBT record for a miss program */
    struct __align__(OPTIX_SBT_RECORD_ALIGNMENT) MissRecord {
        __align__(OPTIX_SBT_RECORD_ALIGNMENT) char header[OPTIX_SBT_RECORD_HEADER_SIZE];
        // just a dummy value - later examples will use more interesting
        // data here
        void *data;
    };

/*! SBT record for a hitgroup program */
    struct __align__(OPTIX_SBT_RECORD_ALIGNMENT) HitgroupRecord {
        __align__(OPTIX_SBT_RECORD_ALIGNMENT) char header[OPTIX_SBT_RECORD_HEADER_SIZE];
        // just a dummy value - later examples will use more interesting
        // data here
        HitGroupUserParams userParams;
    };

/*! constructor - performs all setup, including initializing
  optix, creates module, pipeline, programs, SBT, etc. */
    OptiXRenderer::OptiXRenderer(const std::vector<TriangleMesh>& model)
        : models(model)
    {
        initOptix();

        std::cout << "#osc: creating optix context ..." << std::endl;
        createContext();

        std::cout << "#osc: setting up module ..." << std::endl;
        createModule();

        std::cout << "#osc: creating raygen programs ..." << std::endl;
        createRaygenPrograms();
        std::cout << "#osc: creating miss programs ..." << std::endl;
        createMissPrograms();
        std::cout << "#osc: creating hitgroup programs ..." << std::endl;
        createHitgroupPrograms();

        launchParams.traversable = buildAccel(models);

        std::cout << "#osc: setting up optix pipeline ..." << std::endl;
        createPipeline();

        std::cout << "#osc: building SBT ..." << std::endl;
        buildSBT();

        launchParamsBuffer.alloc(sizeof(launchParams));
        std::cout << "#osc: context, module, pipeline, etc, all set up ..." << std::endl;

        std::cout << GDT_TERMINAL_GREEN;
        std::cout << "#osc: Optix 7 Sample fully set up" << std::endl;
        std::cout << GDT_TERMINAL_DEFAULT;
    }

/*! helper function that initializes optix and checks for errors */
    void OptiXRenderer::initOptix() {
        std::cout << "#osc: initializing optix..." << std::endl;

        // -------------------------------------------------------
        // check for available optix7 capable devices
        // -------------------------------------------------------
        cudaFree(0);
        int numDevices;
        cudaGetDeviceCount(&numDevices);
        if (numDevices == 0)
            throw std::runtime_error("#osc: no CUDA capable devices found!");
        std::cout << "#osc: found " << numDevices << " CUDA devices" << std::endl;

        // -------------------------------------------------------
        // initialize optix
        // -------------------------------------------------------
        OPTIX_CHECK(optixInit());
        std::cout << GDT_TERMINAL_GREEN
                  << "#osc: successfully initialized optix... yay!"
                  << GDT_TERMINAL_DEFAULT << std::endl;
    }

    static void context_log_cb(unsigned int level,
                               const char *tag,
                               const char *message,
                               void *) {
        fprintf(stderr, "[%2d][%12s]: %s\n", (int) level, tag, message);
    }

/*! creates and configures a optix device context (in this simple
    example, only for the primary GPU device) */
    void OptiXRenderer::createContext() {
        // for this sample, do everything on one device
        const int deviceID = 0;
        CUDA_CHECK(SetDevice(deviceID));
        CUDA_CHECK(StreamCreate(&stream));

        cudaGetDeviceProperties(&deviceProps, deviceID);
        std::cout << "#osc: running on device: " << deviceProps.name << std::endl;

        CUresult cuRes = cuCtxGetCurrent(&cudaContext);
        if (cuRes != CUDA_SUCCESS)
            fprintf(stderr, "Error querying current context: error code %d\n", cuRes);

        OPTIX_CHECK(optixDeviceContextCreate(cudaContext, 0, &optixContext));
        OPTIX_CHECK(optixDeviceContextSetLogCallback
                            (optixContext, context_log_cb, nullptr, 4));
    }


/*! creates the module that contains all the programs we are going
    to use. in this simple example, we use a single module from a
    single .cu file, using a single embedded ptx string */
    void OptiXRenderer::createModule() {
        moduleCompileOptions.maxRegisterCount = 50;
        moduleCompileOptions.optLevel = OPTIX_COMPILE_OPTIMIZATION_DEFAULT;
        moduleCompileOptions.debugLevel = OPTIX_COMPILE_DEBUG_LEVEL_NONE;

        pipelineCompileOptions = {};
        pipelineCompileOptions.traversableGraphFlags = OPTIX_TRAVERSABLE_GRAPH_FLAG_ALLOW_SINGLE_GAS;
        pipelineCompileOptions.usesMotionBlur = false;
        pipelineCompileOptions.numPayloadValues = 2;
        pipelineCompileOptions.numAttributeValues = 2;
        pipelineCompileOptions.exceptionFlags = OPTIX_EXCEPTION_FLAG_NONE;
        pipelineCompileOptions.pipelineLaunchParamsVariableName = "optixLaunchParams";

        pipelineLinkOptions.maxTraceDepth = 2;

        const std::string ptxCode = embedded_ptx_code;

        char log[2048];
        size_t sizeof_log = sizeof(log);
        OPTIX_CHECK(optixModuleCreateFromPTX(optixContext,
                                             &moduleCompileOptions,
                                             &pipelineCompileOptions,
                                             ptxCode.c_str(),
                                             ptxCode.size(),
                                             log, &sizeof_log,
                                             &module
        ));
        if (sizeof_log > 1) PRINT(log);
    }


/*! does all setup for the raygen program(s) we are going to use */
    void OptiXRenderer::createRaygenPrograms() {
        // we do a single ray gen program in this example:
        raygenPGs.resize(1);

        OptixProgramGroupOptions pgOptions = {};
        OptixProgramGroupDesc pgDesc = {};
        pgDesc.kind = OPTIX_PROGRAM_GROUP_KIND_RAYGEN;
        pgDesc.raygen.module = module;
        pgDesc.raygen.entryFunctionName = "__raygen__renderFrame";

        // OptixProgramGroup raypg;
        char log[2048];
        size_t sizeof_log = sizeof(log);
        OPTIX_CHECK(optixProgramGroupCreate(optixContext,
                                            &pgDesc,
                                            1,
                                            &pgOptions,
                                            log, &sizeof_log,
                                            &raygenPGs[0]
        ));
        if (sizeof_log > 1) PRINT(log);
    }

/*! does all setup for the miss program(s) we are going to use */
    void OptiXRenderer::createMissPrograms() {
        // we do a single ray gen program in this example:
        missPGs.resize(1);

        OptixProgramGroupOptions pgOptions = {};
        OptixProgramGroupDesc pgDesc = {};
        pgDesc.kind = OPTIX_PROGRAM_GROUP_KIND_MISS;
        pgDesc.miss.module = module;
        pgDesc.miss.entryFunctionName = "__miss__radiance";

        // OptixProgramGroup raypg;
        char log[2048];
        size_t sizeof_log = sizeof(log);
        OPTIX_CHECK(optixProgramGroupCreate(optixContext,
                                            &pgDesc,
                                            1,
                                            &pgOptions,
                                            log, &sizeof_log,
                                            &missPGs[0]
        ));
        if (sizeof_log > 1) PRINT(log);
    }

/*! does all setup for the hitgroup program(s) we are going to use */
    void OptiXRenderer::createHitgroupPrograms() {
        // for this simple example, we set up a single hit group
        hitgroupPGs.resize(1);

        OptixProgramGroupOptions pgOptions = {};
        OptixProgramGroupDesc pgDesc = {};
        pgDesc.kind = OPTIX_PROGRAM_GROUP_KIND_HITGROUP;
        pgDesc.hitgroup.moduleCH = module;
        pgDesc.hitgroup.entryFunctionNameCH = "__closesthit__radiance";
        pgDesc.hitgroup.moduleAH = module;
        pgDesc.hitgroup.entryFunctionNameAH = "__anyhit__radiance";

        char log[2048];
        size_t sizeof_log = sizeof(log);
        OPTIX_CHECK(optixProgramGroupCreate(optixContext,
                                            &pgDesc,
                                            1,
                                            &pgOptions,
                                            log, &sizeof_log,
                                            &hitgroupPGs[0]
        ));
        if (sizeof_log > 1) PRINT(log);
    }


/*! assembles the full pipeline of all programs */
    void OptiXRenderer::createPipeline() {
        std::vector<OptixProgramGroup> programGroups;
        for (auto pg : raygenPGs)
            programGroups.push_back(pg);
        for (auto pg : missPGs)
            programGroups.push_back(pg);
        for (auto pg : hitgroupPGs)
            programGroups.push_back(pg);

        char log[2048];
        size_t sizeof_log = sizeof(log);
        OPTIX_CHECK(optixPipelineCreate(optixContext,
                                        &pipelineCompileOptions,
                                        &pipelineLinkOptions,
                                        programGroups.data(),
                                        (int) programGroups.size(),
                                        log, &sizeof_log,
                                        &pipeline
        ));
        if (sizeof_log > 1) PRINT(log);

        OPTIX_CHECK(optixPipelineSetStackSize
                            (/* [in] The pipeline to configure the stack size for */
                                    pipeline,
                                    /* [in] The direct stack size requirement for direct
                                       callables invoked from IS or AH. */
                                    2 * 1024,
                                    /* [in] The direct stack size requirement for direct
                                       callables invoked from RG, MS, or CH.  */
                                    2 * 1024,
                                    /* [in] The continuation stack requirement. */
                                    2 * 1024,
                                    /* [in] The maximum depth of a traversable graph
                                       passed to trace. */
                                    1));
        if (sizeof_log > 1) PRINT(log);
    }


/*! constructs the shader binding table */
    void OptiXRenderer::buildSBT() {
        // ------------------------------------------------------------------
        // build raygen records
        // ------------------------------------------------------------------
        std::vector<RaygenRecord> raygenRecords;
        for (int i = 0; i < raygenPGs.size(); i++) {
            RaygenRecord rec;
            OPTIX_CHECK(optixSbtRecordPackHeader(raygenPGs[i], &rec));
            rec.data = nullptr; /* for now ... */
            raygenRecords.push_back(rec);
        }
        raygenRecordsBuffer.alloc_and_upload(raygenRecords);
        sbt.raygenRecord = raygenRecordsBuffer.d_pointer();

        // ------------------------------------------------------------------
        // build miss records
        // ------------------------------------------------------------------
        std::vector<MissRecord> missRecords;
        for (int i = 0; i < missPGs.size(); i++) {
            MissRecord rec;
            OPTIX_CHECK(optixSbtRecordPackHeader(missPGs[i], &rec));
            rec.data = nullptr; /* for now ... */
            missRecords.push_back(rec);
        }
        missRecordsBuffer.alloc_and_upload(missRecords);
        sbt.missRecordBase = missRecordsBuffer.d_pointer();
        sbt.missRecordStrideInBytes = sizeof(MissRecord);
        sbt.missRecordCount = (int) missRecords.size();

        // ------------------------------------------------------------------
        // build hitgroup records
        // ------------------------------------------------------------------

        // we don't actually have any objects in this example, but let's
        // create a dummy one so the SBT doesn't have any null pointers
        // (which the sanity checks in compilation would complain about)
        int numObjects = models.size();
        std::vector<HitgroupRecord> hitgroupRecords;
        for (int i = 0; i < numObjects; i++) {
            int objectType = 0;
            HitgroupRecord rec;
            OPTIX_CHECK(optixSbtRecordPackHeader(hitgroupPGs[objectType], &rec));
            rec.userParams.color = models[i].meshColor;
            hitgroupRecords.push_back(rec);
        }
        hitgroupRecordsBuffer.alloc_and_upload(hitgroupRecords);
        sbt.hitgroupRecordBase = hitgroupRecordsBuffer.d_pointer();
        sbt.hitgroupRecordStrideInBytes = sizeof(HitgroupRecord);
        sbt.hitgroupRecordCount = (int) hitgroupRecords.size();
    }


/*! render one frame */
    void OptiXRenderer::render() {
        // sanity check: make sure we launch only after first resize is
        // already done:
        if (launchParams.frame.size.x == 0) return;

        launchParamsBuffer.upload(&launchParams,1);

        OPTIX_CHECK(optixLaunch(/*! pipeline we're launching launch: */
                pipeline,stream,
                /*! parameters and SBT */
                launchParamsBuffer.d_pointer(),
                launchParamsBuffer.sizeInBytes,
                &sbt,
                /*! dimensions of the launch: */
                launchParams.frame.size.x,
                launchParams.frame.size.y,
                1
        ));
        // sync - make sure the frame is rendered before we download and
        // display (obviously, for a high-performance application you
        // want to use streams and double-buffering, but for this simple
        // example, this will have to do)
        CUDA_SYNC_CHECK();
    }

/*! resize frame buffer to given resolution */
    void OptiXRenderer::resize(const vec2i &newSize) {
        // if window minimized
        if (newSize.x == 0 | newSize.y == 0) return;

        // resize our cuda frame buffer
        colorBuffer.resize(newSize.x*newSize.y*sizeof(uint32_t));

        // update the launch parameters that we'll pass to the optix
        // launch:
        launchParams.frame.size  = newSize;
        launchParams.frame.colorBuffer = (uint32_t*)colorBuffer.d_pointer();

        // and re-set the camera, since aspect may have changed
        setCamera(lastSetCamera);
    }

/*! download the rendered color buffer */
    void OptiXRenderer::downloadPixels(uint32_t h_pixels[]) {
        colorBuffer.download(h_pixels,
                             launchParams.frame.size.x*launchParams.frame.size.y);
    }

    OptixTraversableHandle OptiXRenderer::buildAccel(const std::vector<TriangleMesh>& models) {
        // upload the models to the device: the builder

        vertexBuffer.resize(models.size());
        indexBuffer.resize(models.size());
        std::vector<OptixBuildInput> triangleInputs(models.size());

        OptixTraversableHandle asHandle { 0 };

        size_t modelIndex = 0;
        for (const auto& model : models)
        {
            vertexBuffer[modelIndex].alloc_and_upload(model.vertex);
            indexBuffer[modelIndex].alloc_and_upload(model.index);

            // ==================================================================
            // triangle inputs
            // ==================================================================
            OptixBuildInput triangleInput = {};
            triangleInput.type
                    = OPTIX_BUILD_INPUT_TYPE_TRIANGLES;

            // create local variables, because we need a *pointer* to the
            // device pointers
            CUdeviceptr d_vertices = vertexBuffer[modelIndex].d_pointer();
            CUdeviceptr d_indices  = indexBuffer[modelIndex].d_pointer();

            triangleInput.triangleArray.vertexFormat        = OPTIX_VERTEX_FORMAT_FLOAT3;
            triangleInput.triangleArray.vertexStrideInBytes = sizeof(vec3f);
            triangleInput.triangleArray.numVertices         = (int)model.vertex.size();
            triangleInput.triangleArray.vertexBuffers       = &d_vertices;

            triangleInput.triangleArray.indexFormat         = OPTIX_INDICES_FORMAT_UNSIGNED_INT3;
            triangleInput.triangleArray.indexStrideInBytes  = sizeof(vec3i);
            triangleInput.triangleArray.numIndexTriplets    = (int)model.index.size();
            triangleInput.triangleArray.indexBuffer         = d_indices;

            uint32_t triangleInputFlags[1] = { 0 };

            // in this example we have one SBT entry, and no per-primitive
            // materials:
            triangleInput.triangleArray.flags               = triangleInputFlags;
            triangleInput.triangleArray.numSbtRecords               = 1;
            triangleInput.triangleArray.sbtIndexOffsetBuffer        = 0;
            triangleInput.triangleArray.sbtIndexOffsetSizeInBytes   = 0;
            triangleInput.triangleArray.sbtIndexOffsetStrideInBytes = 0;

            triangleInputs[modelIndex] = triangleInput;

            modelIndex++;
        }

        // ==================================================================
        // BLAS setup
        // ==================================================================

        OptixAccelBuildOptions accelOptions = {};
        accelOptions.buildFlags             = OPTIX_BUILD_FLAG_NONE
                                              | OPTIX_BUILD_FLAG_ALLOW_COMPACTION
                ;
        accelOptions.motionOptions.numKeys  = 1;
        accelOptions.operation              = OPTIX_BUILD_OPERATION_BUILD;

        OptixAccelBufferSizes blasBufferSizes;
        OPTIX_CHECK(optixAccelComputeMemoryUsage
                            (optixContext,
                             &accelOptions,
                             triangleInputs.data(),
                             triangleInputs.size(),  // num_build_inputs
                             &blasBufferSizes
                            ));

        // ==================================================================
        // prepare compaction
        // ==================================================================

        CUDABuffer compactedSizeBuffer;
        compactedSizeBuffer.alloc(sizeof(uint64_t));

        OptixAccelEmitDesc emitDesc;
        emitDesc.type   = OPTIX_PROPERTY_TYPE_COMPACTED_SIZE;
        emitDesc.result = compactedSizeBuffer.d_pointer();

        // ==================================================================
        // execute build (main stage)
        // ==================================================================

        CUDABuffer tempBuffer;
        tempBuffer.alloc(blasBufferSizes.tempSizeInBytes);

        CUDABuffer outputBuffer;
        outputBuffer.alloc(blasBufferSizes.outputSizeInBytes);

        OPTIX_CHECK(optixAccelBuild(optixContext,
                /* stream */0,
                                    &accelOptions,
                                    triangleInputs.data(),
                                    triangleInputs.size(),
                                    tempBuffer.d_pointer(),
                                    tempBuffer.sizeInBytes,

                                    outputBuffer.d_pointer(),
                                    outputBuffer.sizeInBytes,

                                    &asHandle,

                                    &emitDesc,1
        ));
        CUDA_SYNC_CHECK();

        // ==================================================================
        // perform compaction
        // ==================================================================
        uint64_t compactedSize;
        compactedSizeBuffer.download(&compactedSize,1);

        asBuffer.alloc(compactedSize);
        OPTIX_CHECK(optixAccelCompact(optixContext,
                /*stream:*/0,
                                      asHandle,
                                      asBuffer.d_pointer(),
                                      asBuffer.sizeInBytes,
                                      &asHandle));
        CUDA_SYNC_CHECK();

        // ==================================================================
        // aaaaaand .... clean up
        // ==================================================================
        outputBuffer.free(); // << the UNcompacted, temporary output buffer
        tempBuffer.free();
        compactedSizeBuffer.free();

        return asHandle;
    }

    void OptiXRenderer::setCamera(const Camera &camera) {
        lastSetCamera = camera;
        launchParams.camera.position  = camera.from;
        launchParams.camera.direction = normalize(camera.at-camera.from);
        const float cosFovy = 0.66f;
        const float aspect = launchParams.frame.size.x / float(launchParams.frame.size.y);
        launchParams.camera.horizontal
                = cosFovy * aspect * normalize(cross(launchParams.camera.direction,
                                                     camera.up));
        launchParams.camera.vertical
                = cosFovy * normalize(cross(launchParams.camera.horizontal,
                                            launchParams.camera.direction));
    }
}