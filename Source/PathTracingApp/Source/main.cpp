#include <cstdlib>

#include "PTBasicDefine.h"
#include "PTScene.h"
#include "OptiXTest.h"
#include "PTUtil.h"

using namespace PTMain;

struct SceneDesc
{
    std::vector<TriangleMesh> Meshes;
};

bool BuildTestScene(SceneDesc& Scene)
{
    const vec3f defaultEmissive = {0.2f, 0.2f, 0.2f};

    // planes
    float planeSize = 5.0f;
    float planeAxisOffset = 0.5f * planeSize;
    float planeHeight = 0.1f;

    TriangleMeshMaterial RedMaterial;
    RedMaterial.Emissive = {0.0f, 0.0f, 0.0f};
    RedMaterial.Diffuse = {0.99f, 0.0f, 0.0f};

    TriangleMeshMaterial GreenMaterial;
    GreenMaterial.Emissive = {0.0f, 0.0f, 0.0f};
    GreenMaterial.Diffuse = {0.0f, 0.99f, 0.0f};

    TriangleMeshMaterial WhiteMaterial;
    WhiteMaterial.Emissive = {0.0f, 0.0f, 0.0f};
    WhiteMaterial.Diffuse = {0.99f, 0.99f, 0.99f};

    TriangleMeshMaterial BlueMaterial;
    BlueMaterial.Emissive = {0.0f, 0.0f, 0.0f};
    BlueMaterial.Diffuse =  {0.0f, 0.0f, 0.99f};

    TriangleMesh planeTop;
    planeTop.addCube({0.0f, planeAxisOffset, 0.0f}, {planeSize, planeHeight, planeSize});
    planeTop.meshMaterial = WhiteMaterial;
    planeTop.meshMaterial.Emissive = {0.99f, 0.99f, 0.99f};

    TriangleMesh planeBottom;
    planeBottom.addCube({0.0f, -planeAxisOffset, 0.0f}, {planeSize, planeHeight, planeSize});
    planeBottom.meshMaterial = WhiteMaterial;

    TriangleMesh planeLeft;
    planeLeft.addCube({-planeAxisOffset, 0.0f, 0.0f}, {planeHeight, planeSize, planeSize});
    planeLeft.meshMaterial= GreenMaterial;

    TriangleMesh planeRight;
    planeRight.addCube({planeAxisOffset, 0.0f, 0.0f}, {planeHeight, planeSize, planeSize});
    planeRight.meshMaterial = RedMaterial;

    TriangleMesh planeBack;
    planeBack.addCube({0.0f, 0.0f, planeAxisOffset}, {planeSize, planeSize, planeHeight});
    planeBack.meshMaterial = WhiteMaterial;

    TriangleMesh Box;
    Box.addCube(vec3f(1.0f,2.0f,0.0f),vec3f(1.0f,1.0f,1.0f));
    Box.setMaterial(BlueMaterial);

    TriangleMesh Box2;
    Box2.addCube(vec3f(-0.5f,-0.5f,2.0f),vec3f(2.5f,2.5f,1.0f));
    Box2.setMaterial(BlueMaterial);

    TriangleMesh Sphere;
    Sphere.addSphere(vec3f(0.0f, 0.0f, 1.0f), vec3f(0.7f, 0.7f, 0.7f), 4);
    Sphere.setMaterial(WhiteMaterial);
    Sphere.meshMaterial.MaterialType = ETMMT_Specular;

    std::vector<TriangleMesh> models;
    models.reserve(7);

    models.push_back(planeBottom);
    models.push_back(planeTop);
    models.push_back(planeLeft);
    models.push_back(planeRight);
    models.push_back(planeBack);
    models.push_back(Box);
    models.push_back(Box2);
    models.push_back(Sphere);

    Scene.Meshes = models;

    return true;
}


int main()
{
    //PTMain::PTScene Scene;
    //Scene.DoPathTracing(800, 600, 64);

	PT::PTRandom<float>::SetSeed(2);

	int width = 800;
	int height = 600;

    SceneDesc TestScene;
    const bool bSuccess = BuildTestScene(TestScene);
    assert(bSuccess);

    Camera camera = { /*from*/vec3f(0.f,0.f,-10.f),
            /* at */vec3f(0.f,0.f,0.f),
            /* up */vec3f(0.f,1.f,0.f) };

    OptiXTest Test(TestScene.Meshes, camera, width, height);
    Test.Render();

    uint32_t* DownloadBuffer = (uint32_t*) malloc (width * height * sizeof(uint32_t));
    Test.Download(DownloadBuffer);

    PTUtil::WritePNGChannel("OutputOptiXResult.png", DownloadBuffer, 4, width, height);

    free (DownloadBuffer);

	return EXIT_SUCCESS;
}
