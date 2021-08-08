//
// Created by jackjwang on 2021/7/18.
//

#ifndef WJ_PLAYGROUND_OPTIXTEST_H
#define WJ_PLAYGROUND_OPTIXTEST_H

#include "OptixRenderer.h"

class OptiXTest {
public:
    OptiXTest(const TriangleMesh& model, const Camera& camera, int width = 800, int height = 600);
    void Render();
    bool Download(uint32_t Result[]);

private:
    OptiXRenderer Renderer;
    int width;
    int height;
};


#endif //WJ_PLAYGROUND_OPTIXTEST_H
