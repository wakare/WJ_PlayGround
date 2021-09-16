#include "PTBasicDefine.h"
#include "PTScene.h"
#include "OptiXTest.h"
#include "PTUtil.h"

using namespace PTMain;

void DoCPURayTracing(int width, int height, int spp)
{
    PT::PTRandom<float>::SetSeed(2);

    PTMain::PTScene Scene;
    Scene.DoPathTracing(width, height, spp);
}

void DoOptixRayTracing(int width, int height, int spp)
{
    Camera camera = { /*from*/vec3f(0.f,0.f,-10.f),
            /* at */vec3f(0.f,0.f,0.f),
            /* up */vec3f(0.f,1.f,0.f) };

    OptiXTest Test(camera, width, height, spp);
    // Test.Run();
    Test.Render();
    Test.OutputImageToFile("OptixOutputImage.png");
}

int main()
{
	int width = 800;
	int height = 600;
    int spp = 64;

    DoOptixRayTracing(width, height, spp);

	return EXIT_SUCCESS;
}