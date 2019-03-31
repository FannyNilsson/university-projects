//
// Created by nilsnils on 2017-10-03.
//

#ifndef GSCEPT_LAB_ENV_SKINNING_H
#define GSCEPT_LAB_ENV_SKINNING_H

#include "VertexComponents.h"
#include "core/app.h"
#include <vector>
#include "Matrix3D.h"
#include <sys/mman.h>
#include <sys/types.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>
#include <assert.h>
#include "Skeleton.h"
#include <chrono>

struct SkinningVertex
{
    Vector3D pos;
    Vector3D uv;
    Vector3D normal;
    Vector3D tangent;
    Vector3D biNormal;
    Vector3D skinWeight;
    Vector3D indice;
};

struct TempVertex
{
    float x;
    float y;
    float z;
    float w;
};

struct Nvx2Header
{
    uint magic;
    uint numGroups;
    uint numVertices;
    uint vertexWidth;
    uint numIndices;
    uint numEdges;
    uint vertexComponentMask;
};

//the skinning is divided in parts and one group holds its own data such as its vertices, triangles and faces
struct Nvx2Group
{
    uint firstVertex;
    uint numVertices;
    uint firstTriangle;
    uint numTriangles;
    uint firstEdge;
    uint numEdges;
};

struct Nvx2Collection
{
    //Nax3Curve* curveList;
    Nvx2Group group;
    int curveListSize;
};

class Skinning {
    typedef unsigned long ulong;
    typedef unsigned int uint;
    typedef unsigned short ushort;
    typedef unsigned char uchar;

private:
    Vector3D* vertexPtrArr;
    //Nvx2Collection* collectionList;

public:
    bool NVX2Parser(const char* filename);
    size_t getFilesize(const char* filename);
    void SetUpVertices();
    void SetUpVBO();
    void SetUpIBO();
    void SetBuffers();
    void DrawSkinning(Skeleton* skeleton);
    void BindSkeleton();


    //these pointers are used to keep track of where the groupdata, vertexdata and indexdata ends in the file
    void* mapPtr;
    void* groupDataPtr;
    void* vertexDataPtr;
    void* indexDataPtr;

    //values to save how big size the data from the nvx2 file is
    //these save the data size of vertices, indices and group(parts of the skinning)
    size_t groupDataSize;
    size_t vertexDataSize;
    size_t indexDataSize;

    uint numGroups;
    uint numVertices;
    uint vertexWidth;
    uint numIndices;
    uint numEdges;
    uint vertexComponentMask;
    std::vector<VertexComponents> vertexComps;
    SkinningVertex* originalVBO;
    SkinningVertex* VBO;
    int* IBO;
    GLuint vertexbuffer, indexBuffer;

protected:
    enum N2VertexComponent
    {
        N2Coord        = (1<<0),      // 3 floats
        N2Normal       = (1<<1),      // 3 floats
        N2NormalB4N   = (1<<2),      // 4 unsigned bytes, normalized
        N2Uv0          = (1<<3),      // 2 floats
        N2Uv0S2        = (1<<4),      // 2 shorts, 4.12 fixed point
        N2Uv1          = (1<<5),      // 2 floats
        N2Uv1S2        = (1<<6),      // 2 shorts, 4.12 fixed point
        N2Uv2          = (1<<7),      // 2 floats
        N2Uv2S2        = (1<<8),      // 2 shorts, 4.12 fixed point
        N2Uv3          = (1<<9),      // 2 floats
        N2Uv3S2        = (1<<10),     // 2 shorts, 4.12 fixed point
        N2Color        = (1<<11),     // 4 floats
        N2ColorUB4N    = (1<<12),     // 4 unsigned bytes, normalized
        N2Tangent      = (1<<13),     // 3 floats
        N2TangentB4N  = (1<<14),     // 4 unsigned bytes, normalized
        N2Binormal     = (1<<15),     // 3 floats
        N2BinormalB4N = (1<<16),     // 4 unsigned bytes, normalized
        N2Weights      = (1<<17),     // 4 floats
        N2WeightsUB4N  = (1<<18),     // 4 unsigned bytes, normalized
        N2JIndices     = (1<<19),     // 4 floats
        N2JIndicesUB4  = (1<<20),     // 4 unsigned bytes

        N2NumVertexComponents = 21,
        N2AllComponents = ((1<<N2NumVertexComponents) - 1),
    };
};


#endif //GSCEPT_LAB_ENV_SKINNING_H
