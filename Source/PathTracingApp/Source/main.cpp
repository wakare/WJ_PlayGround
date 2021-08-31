#include <cstdlib>

#include "PTBasicDefine.h"
#include "PTScene.h"
#include "OptiXTest.h"
#include "PTUtil.h"

using namespace PTMain;

int main()
{
	PT::PTRandom<float>::SetSeed(2);
	
	//PTMain::PTScene Scene;
	//Scene.DoPathTracing(800, 600, 64);

	int width = 800;
	int height = 600;

	uint32_t* DownloadBuffer = (uint32_t*) malloc (width * height * sizeof(uint32_t));

	std::vector<TriangleMesh> models;
    models.resize(2);

    std::vector<TriangleMeshMaterial> materials;
    materials.resize(2);

    materials[1].Color = {0.0f, 1.0f, 0.0f};

    // 100x100 thin ground plane
    models[1].addCube(vec3f(0.f,-1.5f,0.f),vec3f(10.f,.1f,10.f), {1.0f, 0.0f, 0.0f});
    models[1].setMaterial(materials[1]);

    materials[0].Color = {1.0f, 0.0f, 0.0f};

    // a unit cube centered on top of that
    models[0].addCube(vec3f(0.f,0.f,0.f),vec3f(2.f,2.f,2.f), {0.0f, 1.0f, 0.0f});
    models[0].setMaterial(materials[0]);

    Camera camera = { /*from*/vec3f(-10.f,2.f,-12.f),
            /* at */vec3f(0.f,0.f,0.f),
            /* up */vec3f(0.f,1.f,0.f) };

    OptiXTest Test(models, camera, width, height);
    Test.Render();
    Test.Download(DownloadBuffer);

    PTUtil::WritePNGChannel("OutputOptiXResult.png", DownloadBuffer, 4, width, height);

    free (DownloadBuffer);

	return EXIT_SUCCESS;
}
