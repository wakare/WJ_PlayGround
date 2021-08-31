//
// Created by jackjwang on 2021/8/8.
//

#ifndef WJ_PLAYGROUND_TRIANGLEMESH_H
#define WJ_PLAYGROUND_TRIANGLEMESH_H

#include "gdt/math/AffineSpace.h"
#include "TriangleMeshMaterial.h"
#include <vector>

using namespace gdt;

struct TriangleMesh
{
    /*! add a unit cube (subject to given xfm matrix) to the current
        triangleMesh */
    void addUnitCube(const affine3f &xfm, const vec3f& color)
    {
        int firstVertexID = (int)vertex.size();
        vertex.push_back(xfmPoint(xfm,vec3f(0.f,0.f,0.f)));
        vertex.push_back(xfmPoint(xfm,vec3f(1.f,0.f,0.f)));
        vertex.push_back(xfmPoint(xfm,vec3f(0.f,1.f,0.f)));
        vertex.push_back(xfmPoint(xfm,vec3f(1.f,1.f,0.f)));
        vertex.push_back(xfmPoint(xfm,vec3f(0.f,0.f,1.f)));
        vertex.push_back(xfmPoint(xfm,vec3f(1.f,0.f,1.f)));
        vertex.push_back(xfmPoint(xfm,vec3f(0.f,1.f,1.f)));
        vertex.push_back(xfmPoint(xfm,vec3f(1.f,1.f,1.f)));


        int indices[] = {0,1,3, 2,3,0,
                         5,7,6, 5,6,4,
                         0,4,5, 0,5,1,
                         2,3,7, 2,7,6,
                         1,5,7, 1,7,3,
                         4,0,2, 4,2,6
        };
        for (int i=0;i<12;i++)
            index.push_back(firstVertexID+vec3i(indices[3*i+0],
                                                indices[3*i+1],
                                                indices[3*i+2]));

        meshMaterial.Color = color;
    }

    //! add aligned cube aith front-lower-left corner and size
    void addCube(const vec3f &center, const vec3f &size, const vec3f& color)
    {
        affine3f xfm;
        xfm.p = center - 0.5f*size;
        xfm.l.vx = vec3f(size.x,0.f,0.f);
        xfm.l.vy = vec3f(0.f,size.y,0.f);
        xfm.l.vz = vec3f(0.f,0.f,size.z);
        addUnitCube(xfm, color);
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
