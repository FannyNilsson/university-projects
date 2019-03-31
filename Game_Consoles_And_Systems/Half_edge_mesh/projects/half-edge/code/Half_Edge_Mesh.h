//
// Created by nilsnils on 2017-09-12.
//

#ifndef HALF_EDGE_MESH_HALF_EDGE_MESH_H
#define HALF_EDGE_MESH_HALF_EDGE_MESH_H
#include "Matrix3D.h"
#include <vector>
#include <iostream>
#include <stdio.h>
#include <string.h>
#include "core/app.h"

class Half_Edge_Mesh {

public:
    struct Vertex;
    struct HE_edge;
    struct HE_face;

    struct Vertex
    {
        Vector3D Pos;
        Vector3D UV;
        Vector3D Norm;
        HE_edge* edge;  // one of the half-edges emantating from the vertex

        Vertex(const Vertex &obj)			//copy constructor
        {
            Pos = obj.Pos;
            Norm = obj.Norm;
            UV = obj.UV;
        }
        Vertex(Vector3D Pos, Vector3D UV, Vector3D Norm)
        {
            this->Pos = Pos;
            this->Norm = Norm;
            this->UV = UV;
        }
        Vertex()							//default constructor
        {

        }
    };

    struct Quad
    {
        Vertex verts[4];
        int numVertices;
    };

    struct HE_edge
    {

        Vertex* vert;    // vertex at the end of the half-edge
        HE_edge* pair;   // oppositely oriented adjacent half-edge
        HE_face* face;   // face the half-edge borders
        HE_edge* next;   // next half-edge around the face
        bool isPaired = false;
    };


    struct HE_face
    {

        HE_edge* edge;  // one of the half-edges bordering the face

    };

    static Half_Edge_Mesh* loadOBJfile(const char* filepath);
    void SetUpHE_Egdes();
    void DrawHE();
    void setupVBO(Vertex* vertexbuffer, int numOfVertices);
    void setupIBO(GLuint* indexbuffer, int numOfIndices);
    void Subdivision();
private:
    std::vector<Vertex> vertices;
    std::vector<Quad*> quads;
    std::vector<HE_edge*> edges;
    std::vector<HE_face*> faces;

    //för att rita ut shit
    std::vector<int> IndBuff;
    //std::vector<Vertex> vertBuff;
    GLuint Vertex_buffer;
    GLuint Index_buffer;
    int indexCount;
    int* IntBuffer;
};


#endif //HALF_EDGE_MESH_HALF_EDGE_MESH_H
