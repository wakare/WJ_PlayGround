//
// Created by jackjwang on 2021/8/8.
//

#ifndef WJ_PLAYGROUND_TRIANGLEMESH_H
#define WJ_PLAYGROUND_TRIANGLEMESH_H

#include "gdt/math/AffineSpace.h"
#include "TriangleMeshMaterial.h"
#include "IcoSphere.h"
#include <vector>

using namespace gdt;

struct TriangleMesh
{
    /*! add a unit cube (subject to given xfm matrix) to the current
        triangleMesh */
    void addUnitCube(const affine3f& xfm)
    {
        /*
         *
         *   6 ------ 7
         *  /        /|
         * 2 ------ 3 |
         * | |      | |
         * | 4 -----| 5
         * |/       |/
         * 0 ------ 1
         * */
        int firstVertexID = (int)vertex.size();
        vertex.push_back(xfmPoint(xfm,vec3f(0.f,0.f,0.f)));
        vertex.push_back(xfmPoint(xfm,vec3f(1.f,0.f,0.f)));
        vertex.push_back(xfmPoint(xfm,vec3f(0.f,1.f,0.f)));
        vertex.push_back(xfmPoint(xfm,vec3f(1.f,1.f,0.f)));
        vertex.push_back(xfmPoint(xfm,vec3f(0.f,0.f,1.f)));
        vertex.push_back(xfmPoint(xfm,vec3f(1.f,0.f,1.f)));
        vertex.push_back(xfmPoint(xfm,vec3f(0.f,1.f,1.f)));
        vertex.push_back(xfmPoint(xfm,vec3f(1.f,1.f,1.f)));


        int indices[] = {0,1,3, 2,0,3,
                         5,6,7, 5,4,6,
                         0,4,5, 0,5,1,
                         2,3,7, 2,7,6,
                         1,5,7, 1,7,3,
                         4,0,2, 4,2,6
        };
        for (int i=0;i<12;i++)
            index.push_back(firstVertexID+vec3i(indices[3*i+0],
                                                indices[3*i+1],
                                                indices[3*i+2]));
    }

    void addBackfaceUnitCube(const affine3f& xfm)
    {
        /*
         *
         *   6 ------ 7
         *  /        /|
         * 2 ------ 3 |
         * | |      | |
         * | 4 -----| 5
         * |/       |/
         * 0 ------ 1
         * */
        int firstVertexID = (int)vertex.size();
        vertex.push_back(xfmPoint(xfm,vec3f(0.f,0.f,0.f)));
        vertex.push_back(xfmPoint(xfm,vec3f(1.f,0.f,0.f)));
        vertex.push_back(xfmPoint(xfm,vec3f(0.f,1.f,0.f)));
        vertex.push_back(xfmPoint(xfm,vec3f(1.f,1.f,0.f)));

        int indices[] = {
            0,3,1, 2,3,0,
        };

        for (int i=0;i<2;i++)
            index.push_back(firstVertexID+vec3i(indices[3*i+0],
                                                indices[3*i+2],
                                                indices[3*i+1]));
    }

    void addSphere(const vec3f &center, const vec3f &size, int subDivideCount)
    {
        affine3f xfm;
        xfm.p = center - 0.5f*size;
        xfm.l.vx = vec3f(size.x,0.f,0.f);
        xfm.l.vy = vec3f(0.f,size.y,0.f);
        xfm.l.vz = vec3f(0.f,0.f,size.z);

        // addUnitSphere(xfm, subDivideCount);

        IcoSphere unitSphere(subDivideCount);
        unitSphere.MakeIcoSphere();

        int indexOffset = vertex.size();
        for (const auto& sphereVertex : unitSphere.GetSphereVertices())
        {
            vertex.push_back(xfmPoint(xfm, {sphereVertex.x, sphereVertex.y, sphereVertex.z}));
        }

        for (const auto& Triangle : unitSphere.GetSphereIndices())
        {
            index.push_back(
            {
                            Triangle.indices[0] + indexOffset,
                            Triangle.indices[1] + indexOffset,
                            Triangle.indices[2] + indexOffset,
            });
        }
    }

    //! add aligned cube aith front-lower-left corner and size
    void addCube(const vec3f &center, const vec3f &size)
    {
        affine3f xfm;
        xfm.p = center - 0.5f*size;
        xfm.l.vx = vec3f(size.x,0.f,0.f);
        xfm.l.vy = vec3f(0.f,size.y,0.f);
        xfm.l.vz = vec3f(0.f,0.f,size.z);
        addUnitCube(xfm);
    }

    void addBackfaceCube(const vec3f &center, const vec3f &size)
    {
        affine3f xfm;
        xfm.p = center - 0.5f*size;
        xfm.l.vx = vec3f(size.x,0.f,0.f);
        xfm.l.vy = vec3f(0.f,size.y,0.f);
        xfm.l.vz = vec3f(0.f,0.f,size.z);
        addBackfaceUnitCube(xfm);
    }

    void setMaterial(const TriangleMeshMaterial& Material)
    {
        meshMaterial = Material;
    }

    std::vector<vec3f> vertex;
    std::vector<vec3i> index;

    //vec3f meshColor;
    TriangleMeshMaterial meshMaterial;
};

#endif //WJ_PLAYGROUND_TRIANGLEMESH_H
