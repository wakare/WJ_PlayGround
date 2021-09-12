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

    TriangleMeshMaterial ReflectMaterial;
    ReflectMaterial.MaterialType = ETMMT_Specular;
    ReflectMaterial.Diffuse = {0.99f, 0.99f, 0.99f};
    ReflectMaterial.Emissive = {0.0f, 0.0f, 0.0f};

    TriangleMeshMaterial RefractMaterial;
    RefractMaterial.MaterialType = ETMMT_Refract;
    RefractMaterial.Diffuse = {0.99f, 0.99f, 0.99f};
    RefractMaterial.Emissive = {0.0f, 0.0f, 0.0f};

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

    TriangleMesh Box2;
    Box2.addCube(vec3f(-0.5f,-0.5f,2.0f),vec3f(2.5f,2.5f,1.0f));
    Box2.setMaterial(BlueMaterial);

    TriangleMesh ReflectSphere;
    ReflectSphere.addSphere(vec3f(2.0f, 2.0f, 1.0f), vec3f(0.7f, 0.7f, 0.7f), 8);
    ReflectSphere.setMaterial(ReflectMaterial);

    TriangleMesh RefractSphere;
    RefractSphere.addSphere(vec3f(-1.0f, 2.0f, -2.0f), vec3f(1.2f, 1.2f, 1.2f), 4);
    RefractSphere.setMaterial(RefractMaterial);

    std::vector<TriangleMesh> models;

    models.push_back(planeBottom);
    models.push_back(planeTop);
    models.push_back(planeLeft);
    models.push_back(planeRight);
    models.push_back(planeBack);
    models.push_back(Box2);
    models.push_back(ReflectSphere);

    Scene.Meshes = models;

    return true;
}

void DoCPURayTracing(int width, int height, int spp)
{
    PT::PTRandom<float>::SetSeed(2);

    PTMain::PTScene Scene;
    Scene.DoPathTracing(width, height, spp);
}

void DoOptixRayTracing(int width, int height, int spp)
{
    SceneDesc TestScene;
    const bool bSuccess = BuildTestScene(TestScene);
    assert(bSuccess);

    Camera camera = { /*from*/vec3f(0.f,0.f,-10.f),
            /* at */vec3f(0.f,0.f,0.f),
            /* up */vec3f(0.f,1.f,0.f) };

    OptiXTest Test(TestScene.Meshes, camera, width, height);
    Test.Run();
}

int main()
{
	int width = 800;
	int height = 600;
    int spp = 512;

    DoOptixRayTracing(width, height, spp);

	return EXIT_SUCCESS;
}