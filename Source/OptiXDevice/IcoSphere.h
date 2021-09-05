//
// Created by jackjwang on 2021/9/5.
// Generate IsoSphere vertices and indices using stl only
//

#ifndef WJ_PLAYGROUND_ICOSPHERE_H
#define WJ_PLAYGROUND_ICOSPHERE_H

#include <vector>
#include <map>
#include <assert.h>

struct IcoSphereVertex
{
    IcoSphereVertex(float inX, float inY, float inZ)
    : x(inX), y(inY), z(inZ)
    {

    }

    union
    {
        struct
        {
            float x;
            float y;
            float z;
        };

        float vertices[3];
    };

    IcoSphereVertex operator+(const IcoSphereVertex& rhs) const
    {
        return
        {
            x + rhs.x,
            y + rhs.y,
            z + rhs.z,
        };
    };

    IcoSphereVertex operator/(float val) const
    {
        return
        {
            x / val,
            y / val,
            z / val,
        };
    };

    IcoSphereVertex& Normalize()
    {
        float length = sqrtf(x * x + y * y + z * z);
        x /= length;
        y /= length;
        z /= length;

        return *this;
    };
};



struct IcoSphereTriangleIndex
{
    IcoSphereTriangleIndex(int inIndex0, int inIndex1, int inIndex2)
        : index0(inIndex0), index1(inIndex1), index2(inIndex2)
    {

    }

    union
    {
        struct
        {
            int index0;
            int index1;
            int index2;
        };

        int indices[3];
    };
};

typedef int IcoSphereIndex;

class IcoSphere {
public:
    IcoSphere(int inSubDivideCount)
        : subDivideCount(inSubDivideCount)
    {

    }

    void MakeIcoSphere()
    {
        MakeBaseSphere();

        assert(subDivideCount >= 0);
        for (int i = 0; i < subDivideCount; ++i)
        {
            DoSubDivide();
        }
    }

    const std::vector<IcoSphereVertex>& GetSphereVertices() const
    {
        return vertices;
    }

    const std::vector<IcoSphereTriangleIndex>& GetSphereIndices() const
    {
        return triangleIndices;
    }

private:
    void MakeBaseSphere()
    {
        Clear();

        const float X=.525731112119133606f;
        const float Z=.850650808352039932f;
        const float N= 0.0f;

        vertices.push_back(IcoSphereVertex(-X, N, Z));
        vertices.push_back(IcoSphereVertex( X, N, Z));
        vertices.push_back(IcoSphereVertex(-X, N,-Z));
        vertices.push_back(IcoSphereVertex( X, N,-Z));
        vertices.push_back(IcoSphereVertex( N, Z, X));
        vertices.push_back(IcoSphereVertex( N, Z,-X));
        vertices.push_back(IcoSphereVertex( N,-Z, X));
        vertices.push_back(IcoSphereVertex( N,-Z,-X));
        vertices.push_back(IcoSphereVertex( Z, X, N));
        vertices.push_back(IcoSphereVertex(-Z, X, N));
        vertices.push_back(IcoSphereVertex( Z,-X, N));
        vertices.push_back(IcoSphereVertex(-Z,-X, N));

        constexpr int baseTriangleCount = 20;
        int baseIndices[] =
                {
                        0,4,1,0,9,4,9,5,4,4,5,8,4,8,1,
                        8,10,1,8,3,10,5,3,8,5,2,3,2,7,3,
                        7,10,3,7,6,10,7,11,6,11,0,6,0,1,6,
                        6,1,10,9,0,11,9,11,2,9,2,5,7,2,11
                };

        for (int i = 0; i < baseTriangleCount; ++i)
        {
            triangleIndices.push_back({baseIndices[3 * i],
                                       baseIndices[3 * i + 1],
                                       baseIndices[3 * i + 2] });
        }
    }

    void DoSubDivide()
    {
        std::map<std::pair<int, int>, int> edgeLookUpMap;
        std::vector<IcoSphereTriangleIndex> newTriangleIndices;

        for (const auto& Triangle : triangleIndices)
        {
            int newVertexIndices[3] = {-1, -1, -1};

            for (int i = 0; i < 3; ++i)
            {
                std::pair<int, int> edge = {Triangle.indices[i % 3], Triangle.indices[(i + 1) % 3]};
                if (edgeLookUpMap.find(edge) == edgeLookUpMap.end())
                {
                    // Create new vertex in edge middle point
                    const auto& edgeVertex0 = vertices[edge.first];
                    const auto& edgeVertex1 = vertices[edge.second];

                    IcoSphereVertex newVertex = (edgeVertex0 + edgeVertex1);
                    newVertex.Normalize();

                    edgeLookUpMap.emplace(edge, vertices.size());
                    vertices.push_back(newVertex);
                }

                assert(edgeLookUpMap.find(edge) != edgeLookUpMap.end());
                newVertexIndices[i] = edgeLookUpMap[edge];
            }

            newTriangleIndices.push_back({newVertexIndices[0], newVertexIndices[1], newVertexIndices[2]});
            newTriangleIndices.push_back({Triangle.indices[0], newVertexIndices[0], newVertexIndices[2]});
            newTriangleIndices.push_back({Triangle.indices[1], newVertexIndices[1], newVertexIndices[0]});
            newTriangleIndices.push_back({Triangle.indices[2], newVertexIndices[2], newVertexIndices[1]});
        }

        triangleIndices = newTriangleIndices;
    }

    void Clear() {
        vertices.clear();
        triangleIndices.clear();
    }

    std::vector<IcoSphereVertex> vertices;
    std::vector<IcoSphereTriangleIndex> triangleIndices;

    int subDivideCount;
};

#endif //WJ_PLAYGROUND_ICOSPHERE_H
