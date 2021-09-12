//
// Created by jackjwang on 2021/7/18.
//

#include "OptiXTest.h"
#include "PTUtil.h"

OptiXTest::OptiXTest(const std::vector<TriangleMesh>& model, const Camera& camera, int width, int height) :
        width(width),
        height(height),
        Renderer("OptiXTestRenderer", model, camera, width * 1.0f / height)
{
    Renderer.resize({width, height});
}

bool OptiXTest::Download(uint32_t *Result) {
    return true;
}

void OptiXTest::Render() {
    Renderer.render();
}

bool OptiXTest::OutputImageToFile(const std::string &output_filename)
{
    uint32_t* downloadBuffer = (uint32_t*) malloc (width * height * sizeof(uint32_t));
    Renderer.sample.downloadPixels(downloadBuffer);

    bool result = PTUtil::WritePNGChannel("OutputOptiXResult.png", downloadBuffer, 4, width, height);
    free (downloadBuffer);
    return result;
}

void OptiXTest::Run() {
    Renderer.run();
}


