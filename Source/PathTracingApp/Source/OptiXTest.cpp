//
// Created by jackjwang on 2021/7/18.
//

#include "OptiXTest.h"
#include "PTUtil.h"

OptiXTest::OptiXTest(const Camera& camera, int width, int height, uint32_t spp) :
        width(width),
        height(height),
        Renderer("OptiXTestRenderer", BuildTestScene(), camera, width * 1.0f / height, spp)
{
    Renderer.resize({width, height});
}

bool OptiXTest::Download(uint32_t *Result)
{
    return true;
}

void OptiXTest::Render()
{
    Renderer.render();
}

bool OptiXTest::OutputImageToFile(const std::string &output_filename)
{
    uint32_t* downloadBuffer = (uint32_t*) malloc (width * height * sizeof(uint32_t));
    Renderer.sample.downloadPixels(downloadBuffer);

    bool result = PTUtil::WritePNGChannel(output_filename.c_str(), downloadBuffer, 4, width, height);
    free (downloadBuffer);
    return result;
}

void OptiXTest::Run()
{
    Renderer.run();
}

const std::vector<TriangleMesh> OptiXTest::BuildTestScene() const
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

    TriangleMesh LightBox;
    LightBox.addCube({2.0f, 0.0f, 0.0f}, {1.0f, 1.0f, 1.0f});
    LightBox.meshMaterial = WhiteMaterial;
    LightBox.meshMaterial.Emissive = {0.99f, 0.99f, 0.99f};

    TriangleMesh Box2;
    Box2.addCube(vec3f(-0.5f,-0.5f,2.0f),vec3f(2.5f,2.5f,1.0f));
    Box2.setMaterial(BlueMaterial);

    TriangleMesh ReflectSphere;
    ReflectSphere.addSphere(vec3f(2.0f, 2.0f, 1.0f), vec3f(0.7f, 0.7f, 0.7f), 8);
    ReflectSphere.setMaterial(ReflectMaterial);

    TriangleMesh RefractSphere;
    RefractSphere.addSphere(vec3f(1.0f, 1.0f, 0.0f), vec3f(0.5f, 0.5f, 0.5f), 8);
    RefractSphere.setMaterial(RefractMaterial);

    std::vector<TriangleMesh> models;

    models.push_back(planeBottom);
    models.push_back(planeTop);
    models.push_back(planeLeft);
    models.push_back(planeRight);
    models.push_back(planeBack);
    models.push_back(LightBox);
    models.push_back(Box2);
    models.push_back(ReflectSphere);
    models.push_back(RefractSphere);

    return models;
}

const std::vector<TriangleMesh> OptiXTest::BuildRefractTestScene() const {
    // planes
    float planeSize = 5.0f;
    float planeAxisOffset = 0.5f * planeSize;
    float planeHeight = 0.1f;

    TriangleMesh RefractPlane;
    //RefractPlane.addBackfaceCube({0.0f, 0.0f, -planeAxisOffset}, {planeSize, planeSize, planeHeight});
    RefractPlane.addCube({0.0f, 0.0f, -planeAxisOffset}, {planeSize, planeSize, planeHeight});

    TriangleMeshMaterial RefractMaterial;
    RefractMaterial.Emissive = {0.0f, 0.0f, 0.0f};
    RefractMaterial.Diffuse = {0.99f, 0.99f, 0.99f};
    RefractMaterial.MaterialType = ETriangleMeshMaterialType::ETMMT_Refract;
    RefractPlane.setMaterial(RefractMaterial);

    TriangleMesh RefractSphere;
    RefractSphere.addSphere({0.0f, 0.0f, 0.0f}, {4.0f, 4.0f, 4.0f}, 8);
    RefractSphere.setMaterial(RefractMaterial);


    // Emissive Plane
    TriangleMesh BackFaceTestPlane;
    BackFaceTestPlane.addBackfaceCube({0.0f, 0.0f, planeAxisOffset}, {4 * planeSize, 4 * planeSize, 4 * planeHeight});

    TriangleMeshMaterial DiffuseMaterial;
    DiffuseMaterial.Emissive = {1.0f, 1.0f, 1.0f};
    DiffuseMaterial.Diffuse = {0.0f, 0.0f, 0.0f};
    BackFaceTestPlane.setMaterial(DiffuseMaterial);

    std::vector<TriangleMesh> models;
    //models.push_back(RefractPlane);
    models.push_back(RefractSphere);
    models.push_back(BackFaceTestPlane);
    return models;

}


