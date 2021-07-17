//
// Created by jackjwang on 2021/7/4.
//

#ifndef WJ_PLAYGROUND_OPTIXRENDERER_H
#define WJ_PLAYGROUND_OPTIXRENDERER_H
#include "optix7.h"

class OptiXRenderer {
public:
    OptiXRenderer();

private:
    CUcontext cudaContext;
};


#endif //WJ_PLAYGROUND_OPTIXRENDERER_H
