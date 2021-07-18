//
// Created by jackjwang on 2021/7/18.
//

#include "OptiXTest.h"

OptiXTest::OptiXTest(int width, int height) :
        width(width),
        height(height)
{
    Renderer.resize({width, height});
}

bool OptiXTest::Download(uint32_t *Result) {
    Renderer.downloadPixels(Result);
    return true;
}

void OptiXTest::Render() {
    Renderer.render();
}


