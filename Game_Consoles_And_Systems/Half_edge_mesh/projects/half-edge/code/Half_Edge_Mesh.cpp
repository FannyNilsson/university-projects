//
// Created by nilsnils on 2017-09-12.
//

#include "Half_Edge_Mesh.h"
#include "config.h"

//almost the same function as loadOBJfile in MeshResources but fitted for half edge meshes
Half_Edge_Mesh* Half_Edge_Mesh::loadOBJfile(const char* filepath)
{
    Half_Edge_Mesh* he_mesh = new Half_Edge_Mesh;
    std::vector<unsigned int> vIndices, uvIndices, normalIndices;
    std::vector<Vector3D> temp_vertices;
    std::vector<Vector3D> temp_uvs;
    std::vector<Vector3D> temp_normals;

    std::vector<GLuint> indBuffer;


    FILE* file = fopen(filepath, "r");
    if (file == NULL) {
        printf("Cannot open file! \n");
        return NULL;
    }

    //abbreviatons in a OBJ file
	//v is a vertex
	//vt is the texture coordinate of one vertex
	//vn is the normal of one vertex
	//f is a face
    while (1)
    {

        char lineHeader[128];							
        int res = fscanf(file, "%s", lineHeader);		
        if (res == EOF)									
        {
            break;
        }

        if (strcmp(lineHeader, "v") == 0)											
        {
            Vector3D vertex;
            fscanf(file, "%f %f %f\n", &vertex[0], &vertex[1], &vertex[2]);			
            temp_vertices.push_back(vertex);
        }

        else if (strcmp(lineHeader, "vt") == 0)										
        {
            Vector3D uv;
            fscanf(file, "%f %f\n", &uv[0], &uv[1]);
            temp_uvs.push_back(uv);
        }

        else if (strcmp(lineHeader, "vn") == 0)										
        {
            Vector3D Normal;
            fscanf(file, "%f %f %f\n", &Normal[0], &Normal[1], &Normal[2]);
            temp_normals.push_back(Normal);
        }

        else if (strcmp(lineHeader, "f") == 0)										
        {
            std::string vertex1, vertex2, vertex3;
            unsigned int vertexIndex[4], uvIndex[4], normalIndex[4];

            int readFace = fscanf(file, "%d/%d/%d %d/%d/%d %d/%d/%d %d/%d/%d\n", &vertexIndex[0], &uvIndex[0], &normalIndex[0], &vertexIndex[1], &uvIndex[1], &normalIndex[1], &vertexIndex[2], &uvIndex[2], &normalIndex[2], &vertexIndex[3], &uvIndex[3], &normalIndex[3]);
            if (readFace == 9)
            {
                Quad* quad = new Quad;

                Vertex v;												
                v.Pos = temp_vertices[vertexIndex[0] - 1];				
                v.Norm = temp_normals[normalIndex[0] - 1];
                v.UV = temp_uvs[uvIndex[0] - 1];
                quad->verts[0] = v;                                      //connect the vertices in a quad (face) to later recognize which vertices create a face

                he_mesh->vertices.push_back(v);
                indBuffer.push_back(he_mesh->vertices.size() - 1);

                v.Pos = temp_vertices[vertexIndex[1] - 1];				//vertex 2 
                v.Norm = temp_normals[normalIndex[1] - 1];
                v.UV = temp_uvs[uvIndex[1] - 1];
                quad->verts[1] = v;

                he_mesh->vertices.push_back(v);
                indBuffer.push_back(he_mesh->vertices.size() - 1);

                v.Pos = temp_vertices[vertexIndex[2] - 1];				//vertex 3 
                v.Norm = temp_normals[normalIndex[2] - 1];
                v.UV = temp_uvs[uvIndex[2] - 1];
                quad->verts[2] = v;

                he_mesh->vertices.push_back(v);
                indBuffer.push_back(he_mesh->vertices.size() - 1);

                quad->numVertices = 3;

                he_mesh->quads.push_back(quad);                            //add the quad(face) to the list of quads(faces) in the object mesh
            }

            else if(readFace == 12)										
            {
                Vertex v;
                Quad* quad = new Quad;

                v.Pos = temp_vertices[vertexIndex[0] - 1];				//vertex 0
                v.Norm = temp_normals[normalIndex[0] - 1];
                v.UV = temp_uvs[uvIndex[0] - 1];
                quad->verts[0] = v;

                he_mesh->vertices.push_back(v);
                indBuffer.push_back(he_mesh->vertices.size() - 1);

                v.Pos = temp_vertices[vertexIndex[1] - 1];				//vertex 1
                v.Norm = temp_normals[normalIndex[1] - 1];
                v.UV = temp_uvs[uvIndex[1] - 1];
                quad->verts[1] = v;

                he_mesh->vertices.push_back(v);
                indBuffer.push_back(he_mesh->vertices.size() - 1);

                v.Pos = temp_vertices[vertexIndex[2] - 1];				//vertex 2
                v.Norm = temp_normals[normalIndex[2] - 1];
                v.UV = temp_uvs[uvIndex[2] - 1];
                quad->verts[2] = v;

                he_mesh->vertices.push_back(v);
                indBuffer.push_back(he_mesh->vertices.size() - 1);

                //nu har hela f�rsta triangeln lagts till i verticeser (0,1,2)

                v.Pos = temp_vertices[vertexIndex[0] - 1];				
                v.Norm = temp_normals[normalIndex[0] - 1];
                v.UV = temp_uvs[uvIndex[0] - 1];

                he_mesh->vertices.push_back(v);
                indBuffer.push_back(he_mesh->vertices.size() - 1);

                v.Pos = temp_vertices[vertexIndex[2] - 1];				//vertex 3
                v.Norm = temp_normals[normalIndex[2] - 1];
                v.UV = temp_uvs[uvIndex[2] - 1];

                he_mesh->vertices.push_back(v);
                indBuffer.push_back(he_mesh->vertices.size() - 1);

                v.Pos = temp_vertices[vertexIndex[3] - 1];				//vertex 4
                v.Norm = temp_normals[normalIndex[3] - 1];
                v.UV = temp_uvs[uvIndex[3] - 1];
                quad->verts[3] = v;

                he_mesh->vertices.push_back(v);								
                indBuffer.push_back(he_mesh->vertices.size() - 1);
                quad->numVertices = 4;

                he_mesh->quads.push_back(quad);                            //add the quad(face) to the list of quads(faces) in the object mesh
            }
            else
            {
                printf("File can't be read by our simple parser : ( Try exporting with other options\n");
                return NULL;
            }
        }
    }

    he_mesh->SetUpHE_Egdes();

    for (int i = 0; i < indBuffer.size(); i++)											//save all indices in a list with right type (int) 
    {
        he_mesh->IndBuff.push_back(int(indBuffer[i]));
    }
    //vertBuff = vertBuffer;
    he_mesh->setupVBO(&he_mesh->vertices[0], he_mesh->vertices.size());					//setup buffers with created mesh
    he_mesh->setupIBO(&indBuffer[0], indBuffer.size());
    return he_mesh;
}

void Half_Edge_Mesh::SetUpHE_Egdes()
{
    //loop through all the quads(faces) and find out how many vertices the quad has (quad = face, can be a triangle)
    for(int i=0; i<quads.size();i++)
    {
        HE_face* face = new HE_face;                                       				//create a face for each quad (a real HE_face instead of a quad)
        for(int j=0; j<quads[i]->numVertices;j++)
        {
            HE_edge* edge = new HE_edge;                                   				//create an edge
            edge->vert = &quads[i]->verts[j];                 							//the start-vertex of the edge is one of the quads vertices
            edge->vert->edge = edge;                        							//the vertex needs to fill it's edge with any edge - so that's what we're doing
            face->edge = edge;                              							//save the current edge in the face (this will be overwritten until the last loop - which is unnecessary but ok!)
            edge->face = face;                              							//save the face to the edge
            edges.push_back(edge);
        }

        //traverse through the list with all edges and set all their next edges
        for(int j=0;j<quads[i]->numVertices-1; j++)
        {
            //we have to multiply with numVertices to get the right edges in the list. Since the list contain all edges
            edges[i*quads[i]->numVertices+j]->next = edges[i*quads[i]->numVertices+j+1];
        }
        edges[i*quads[i]->numVertices + quads[i]->numVertices-1]->next = edges[i*quads[i]->numVertices];
        faces.push_back(face);
    }

    //check to find if the edges have any pairs
    for(int i=0; i < edges.size();i++)
    {
        for(int j=0; j < edges.size();j++)
        {
            //check if the edge in loop 1 has the same vertix as the edge's next vertix in loop 2 and vice versa! Also check so i and j are the same
            if(edges[i]->vert->Pos == edges[j]->next->vert->Pos && edges[j]->vert->Pos == edges[i]->next->vert->Pos)
            {
                edges[i]->pair = edges[j];
                break;
            }
        }
    }
}

void Half_Edge_Mesh::setupVBO(Vertex* buffer, int numOfVertices)
{
    glGenBuffers(1, &Vertex_buffer);																		//create buffer
    glBindBuffer(GL_ARRAY_BUFFER, Vertex_buffer);															//activate buffer
    glBufferData(GL_ARRAY_BUFFER, numOfVertices*sizeof(Vertex), buffer, GL_STATIC_DRAW);					//create and initialize buffer.
    glBindBuffer(GL_ARRAY_BUFFER, 0);																		//deactivate buffer
    return;
}
void Half_Edge_Mesh::setupIBO(GLuint * indices, int count)													//uses index buffer as argument (works as a "pointer to the buffer")
{
    glGenBuffers(1, &Index_buffer);																			//create buffer
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, Index_buffer);													//activate buffer
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, count*sizeof(int), indices, GL_STATIC_DRAW);						//"load data"
    this->indexCount = count;
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void Half_Edge_Mesh::DrawHE()
{
    //draw
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    glBindBuffer(GL_ARRAY_BUFFER, Vertex_buffer);															//activate buffer
    glEnableVertexAttribArray(0);																			//enable  vertexattributes in shadern
    //glEnableVertexAttribArray(1);																			
    //glEnableVertexAttribArray(2);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), NULL);									//define an array with attribute data. 3 means how many elements that are going to be used. Size of vertex struct gives the offset NULL (means that we start in the beginning)
    //glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)(sizeof(Vector3D)));			//same as above but start at the third element to continue were it left off
    //glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)(sizeof(Vector3D) * 2));
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, Index_buffer);													//activate index buffer
    glDrawElements(GL_TRIANGLES, this->indexCount, GL_UNSIGNED_INT, NULL);	                                //render
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void Half_Edge_Mesh::Subdivision()
{

}
