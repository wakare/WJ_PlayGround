//
// Created by jackjwang on 2021/7/18.
//

#ifndef WJ_PLAYGROUND_OPTIXTEST_H
#define WJ_PLAYGROUND_OPTIXTEST_H

#include "OptixRenderer.h"
#include "../PTAppWindow/CameraFrame.h"
#include <GL/gl.h>

struct SampleWindow : public GLFCameraWindow
{
    SampleWindow(const std::string &title,
                 const std::vector<TriangleMesh> &model,
                 const Camera &camera,
                 const float worldScale,
                 uint32_t spp)
            : GLFCameraWindow(title,camera.from,camera.at,camera.up,worldScale),
              sample(model, spp)
    {
    }

    virtual void render() override
    {
        if (cameraFrame.modified) {
            sample.setCamera(Camera{ cameraFrame.get_from(),
                                     cameraFrame.get_at(),
                                     cameraFrame.get_up() });
            cameraFrame.modified = false;
        }
        sample.render();
    }

    virtual void draw() override
    {
        sample.downloadPixels(pixels.data());
        if (fbTexture == 0)
            glGenTextures(1, &fbTexture);

        glBindTexture(GL_TEXTURE_2D, fbTexture);
        GLenum texFormat = GL_RGBA;
        GLenum texelType = GL_UNSIGNED_BYTE;
        glTexImage2D(GL_TEXTURE_2D, 0, texFormat, fbSize.x, fbSize.y, 0, GL_RGBA,
                     texelType, pixels.data());

        glDisable(GL_LIGHTING);
        glColor3f(1, 1, 1);

        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();

        glEnable(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, fbTexture);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        glDisable(GL_DEPTH_TEST);

        glViewport(0, 0, fbSize.x, fbSize.y);

        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        glOrtho(0.f, (float)fbSize.x, 0.f, (float)fbSize.y, -1.f, 1.f);

        glBegin(GL_QUADS);
        {
            glTexCoord2f(0.f, 0.f);
            glVertex3f(0.f, 0.f, 0.f);

            glTexCoord2f(0.f, 1.f);
            glVertex3f(0.f, (float)fbSize.y, 0.f);

            glTexCoord2f(1.f, 1.f);
            glVertex3f((float)fbSize.x, (float)fbSize.y, 0.f);

            glTexCoord2f(1.f, 0.f);
            glVertex3f((float)fbSize.x, 0.f, 0.f);
        }
        glEnd();
    }

    virtual void resize(const vec2i &newSize)
    {
        fbSize = newSize;
        sample.resize(newSize);
        pixels.resize(newSize.x*newSize.y);
    }

    vec2i                 fbSize;
    GLuint                fbTexture {0};
    OptiXRenderer         sample;
    std::vector<uint32_t> pixels;
};

class OptiXTest {
public:
    OptiXTest(const Camera& camera, int width = 800, int height = 600, uint32_t spp = 16);
    void Run();
    void Render();
    bool Download(uint32_t Result[]);
    bool OutputImageToFile(const std::string& output_filename);

private:
    const std::vector<TriangleMesh> BuildTestScene() const;
    const std::vector<TriangleMesh> BuildRefractTestScene() const;
    SampleWindow Renderer;
    int width;
    int height;
};


#endif //WJ_PLAYGROUND_OPTIXTEST_H
