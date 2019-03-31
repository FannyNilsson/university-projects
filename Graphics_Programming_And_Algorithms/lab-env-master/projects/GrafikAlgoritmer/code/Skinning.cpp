//
// Created by nilsnils on 2017-10-03.
//

#include "Skinning.h"
#include <config.h>

bool Skinning::NVX2Parser(const char* filename)
{
    void* map;
    void* beginning;
    int fileSizeBytes = getFilesize(filename);                      //get size of file
    int fileDescriptor = open(filename, O_RDONLY);                  //open the file as read-only

    map = mmap(NULL,fileSizeBytes, PROT_READ, MAP_PRIVATE, fileDescriptor, 0);
    beginning = map;                                                //save the beginning of the file to a pointer
    uchar* ptr = (uchar*) map;                                      //cast the void* to a uchar*

    if(map == (void*)-1)
    {
        std::cerr<< "VARNING! HELVETET BRYTER LÖST!" << std::endl;
        return false;
    }

    // read header
    Nvx2Header* nvxHead = (Nvx2Header*) ptr;

    //the magic variable checks if the file actually is a nvx2 file
    //if it isn't the program stops and prints that it won't work
    if(nvxHead->magic == 'NVX2')
    {
        //fill our own header-file data with the nvxheaders data from the file that is parsed
        this->numGroups = nvxHead->numGroups;
        this->numVertices = nvxHead->numVertices;
        this->vertexWidth = nvxHead->vertexWidth;
        this->numIndices = nvxHead->numIndices;
        this->numEdges = nvxHead->numEdges;
        this->vertexComponentMask = nvxHead->vertexComponentMask;

        //the data of each type is "calculated and saved to each data-variable
        this->groupDataSize = 6 * sizeof(uint) * this->numGroups;
        this->vertexDataSize = this->numVertices * this->vertexWidth * sizeof(float);
        this->indexDataSize = this->numIndices * sizeof(int);

        //each pointer is moved past all parts that are not relevant for that particular type
        //this is done in the order that the file is built
        //first comes the header, then the group data, then the vertex data and finally the index data
        this->groupDataPtr += sizeof(nvxHead) + 1;
        this->vertexDataPtr = ((uchar*)this->groupDataPtr) + this->groupDataSize;
        this->indexDataPtr = ((uchar*)this->vertexDataPtr) + this->vertexDataSize;
    }

    else
    {
        printf("Shit ain't gonna happen!");
    }

    close(fileDescriptor);

}

void Skinning::SetUpVertices()
{
    //very simply explained this is where the vertex data from the parsed file is "sorted" to right vertex
    //and then the vertex (in the format from the file) is saved in a list of skinning vertices
    for (int i = 0; i < N2NumVertexComponents; i++)
    {
        //semantic name is just the variable name (basically)
        //format is the type of the variable
        VertexComponents::SemanticName sem;
        VertexComponents::Format fmt;
        int index = 0;
        //using bitwise operations
        //here the AND and LEFT SHIFT operators
        //check where in the file "we are"
        if (vertexComponentMask & (1<<i))
        {
            //then check which variable we are looking at right now
            switch (1<<i)
            {
                case N2Coord:
                {
                    sem = VertexComponents::Position;
                    fmt = VertexComponents::Float3;
                    break;
                }
                case N2Normal:
                {
                    sem = VertexComponents::Normal;
                    fmt = VertexComponents::Float3;
                    break;
                }
                case N2NormalB4N:
                {
                    sem = VertexComponents::Normal;
                    fmt = VertexComponents::Byte4N;
                    break;
                }
                case N2Uv0:
                {
                    sem = VertexComponents::TexCoord1;
                    fmt = VertexComponents::Float2;
                    index = 0;
                    break;
                }
                case N2Uv0S2:
                {
                    sem = VertexComponents::TexCoord1;
                    fmt = VertexComponents::Short2;
                    index = 0;
                    break;
                }
                case N2Uv1:
                {
                    sem = VertexComponents::TexCoord2;
                    fmt = VertexComponents::Float2;
                    index = 1;
                    break;
                }
                case N2Uv1S2:
                {
                    sem = VertexComponents::TexCoord2;
                    fmt = VertexComponents::Short2;
                    index = 1;
                    break;
                }
                case N2Color:
                {
                    sem = VertexComponents::Color;
                    fmt = VertexComponents::Float4;
                    break;
                }
                case N2ColorUB4N:
                {
                    sem = VertexComponents::Color;
                    fmt = VertexComponents::UByte4N;
                    break;
                }
                case N2Tangent:
                {
                    sem = VertexComponents::Tangent;
                    fmt = VertexComponents::Float3;
                    break;
                }
                case N2TangentB4N:
                {
                    sem = VertexComponents::Tangent;
                    fmt = VertexComponents::Byte4N;
                    break;
                }
                case N2Binormal:
                {
                    sem = VertexComponents::Binormal;
                    fmt = VertexComponents::Float3;
                    break;
                }
                case N2BinormalB4N:
                {
                    sem = VertexComponents::Binormal;
                    fmt = VertexComponents::Byte4N;
                    break;
                }
                case N2Weights:
                {
                    sem = VertexComponents::SkinWeights;
                    fmt = VertexComponents::Float4;
                    break;
                }
                case N2WeightsUB4N:
                {
                    sem = VertexComponents::SkinWeights;
                    fmt = VertexComponents::UByte4N;
                    break;
                }
                case N2JIndices:
                {
                    sem = VertexComponents::SkinJIndices;
                    fmt = VertexComponents::Float4;
                    break;
                }
                case N2JIndicesUB4:
                {
                    sem = VertexComponents::SkinJIndices;
                    fmt = VertexComponents::UByte4;
                    break;
                }
                default:
                {
                    sem = VertexComponents::Position;
                    fmt = VertexComponents::Float3;
                    break;
                }
            }
            //save the vertex in the vertex-list
            this->vertexComps.push_back(VertexComponents(sem, index, fmt));
        }
    }
}

//now we have to save the vertices in some way and to do that we create our own SkinningVertex type
//loop through the list of vertices again, creating a skinningvertex for each, and then looping though all our saved vertices from the SetUpVertices-function since...
//...there aren't any actual vertices but only connected and "sorted" data which we now have to handle to create our own vertex buffer
void Skinning::SetUpVBO()
{
    //set the size and type of our vertex buffer to be filled with the amount of numVertices and the type of SkinningVertex
    //also make a copy to save the original VBO since it till be needed when the VBO is changed depending on the animations
    this->VBO = new SkinningVertex[this->numVertices];
    this->originalVBO = new SkinningVertex[this->numVertices];

    //loop through all vertices in the parsed file
    for(int i=0; i < this->numVertices; i++)
    {
        //for each vertex in the file we create one SkinningVertex of our own to hold all info it needs to
        SkinningVertex skVrtx;

        for(int j=0; j < vertexComps.size(); j++)
        {
            switch (this->vertexComps[j].semName)
            {
                case VertexComponents::Position:
                {
                    //1. Create tempPointer and set it to the position of the vertexDataPointer (since it is in the beginning of all vertex data (bytewise)
                    //2. Set the real vertex data pos from the tempPointer
                    //3. Move the vertexdatapointer x amount of bytes (sizeof 3 floats) so it is correct for next data to be read
                    TempVertex* pos;
                    pos = (TempVertex*) this->vertexDataPtr;
                    skVrtx.pos = Vector3D(pos->x, pos->y, pos->z);
                    this->vertexDataPtr += sizeof(float)*3;
                    break;
                }
                case VertexComponents::Normal:
                {
                    //1.)Create tempPointer(CHAR* OR IT WILL BE FUCKED IN THE FILE ORDER) and set it to the normal of the vertexDataPointer (since it is in the beginning of all vertex data (bytewise)
                    //2.)Set the normals values from the tempPointer (REMEMBER TO CAST IT TO FLOATS)
                    //3.)Move the vertexdatapointer 4 bytes (sizeof 4 CHARS) so it is correct for next data to be read
                    char* norm;
                    norm = (char*) this->vertexDataPtr;
                    skVrtx.normal = Vector3D((float)norm[0], (float)norm[1], (float)norm[2], (float)norm[3]);
                    this->vertexDataPtr += sizeof(char)*4;
                    break;
                }

                case VertexComponents::TexCoord1:
                {
                    //1.)Create tempPointer and set it to the uv of the vertexDataPointer (since it is in the beginning of all vertice data (bytewise)
                    //2.)Set the uv values from the tempPointer
                    //3.)Move the vertexdatapointer x amount of bytes (sizeof 2 floats) so it is correct for next data to be read
                    TempVertex* uv;
                    uv = (TempVertex*) this->vertexDataPtr;
                    skVrtx.uv = Vector3D(uv->x, uv->y, 0);
                    this->vertexDataPtr += sizeof(float)*2;
                    break;
                }

                case VertexComponents::Tangent:
                {
                    //exactly as the normal
                    char* tangent;
                    tangent = (char*) this->vertexDataPtr;
                    skVrtx.tangent = Vector3D((float)tangent[0], (float)tangent[1], (float)tangent[2], (float)tangent[3]);
                    this->vertexDataPtr += sizeof(char)*4;
                    break;
                }

                case VertexComponents::Binormal:
                {
                    //exactly as the normal
                    char* binorm;
                    binorm = (char*) this->vertexDataPtr;
                    skVrtx.biNormal = Vector3D((float)binorm[0], (float)binorm[1], (float)binorm[2], (float)binorm[3]);
                    this->vertexDataPtr += sizeof(char)*4;
                    break;
                }

                case VertexComponents::SkinWeights:
                {
                    Vector3D vec = Vector3D(1.0f,1.0f,1.0f,1.0f);
                    unsigned char* weight;
                    weight = (unsigned char*) this->vertexDataPtr;
                    skVrtx.skinWeight = Vector3D((float)weight[0], (float)weight[1], (float)weight[2], (float)weight[3]);
                    skVrtx.skinWeight = skVrtx.skinWeight * (1.0f/(skVrtx.skinWeight.QuatDotproduct(vec)));
                    this->vertexDataPtr += sizeof(unsigned char)*4;
                    break;
                }

                case VertexComponents::SkinJIndices:
                {
                    unsigned char* indx;
                    indx = (unsigned char*) this->vertexDataPtr;
                    skVrtx.indice = Vector3D((float)indx[0], (float)indx[1], (float)indx[2], (float)indx[3]);
                    this->vertexDataPtr += sizeof(unsigned char)*4;
                    break;
                }
            }
        }
        //add the vertex to both buffers
        this->VBO[i] = skVrtx;
        this->originalVBO[i] = skVrtx;
    }
}

void Skinning::SetUpIBO()
{
    //collect all index-data and set up an indexbuffer with it
    //basically in the same way as with the vertexbuffer but much simpler
    IBO = new int[numIndices];
    for(int i=0; i < numIndices; i++)
    {
        int* indData = (int*) this->indexDataPtr;
        IBO[i] = *indData;
        this->indexDataPtr += sizeof(int);
    }
}

void Skinning::DrawSkinning(Skeleton* skeleton)
{
    SetBuffers();

    //skeleton->shader->ModifyMatrix("rotation", (float*)skeleton->matrix.transpose());

    //GLint position = glGetUniformLocation(skeleton->shader->program, "skeleton");
    //glUniformMatrix4fv(position, 21, GL_TRUE, (float*)skeleton->matrix);

    glBindBuffer(GL_ARRAY_BUFFER, this->vertexbuffer);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->indexBuffer);

    //the identifiers for the vertexcomponents
    //they are seven since in the vertex components we have 7 different variables (position, uv etc.)
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glEnableVertexAttribArray(2);
    glEnableVertexAttribArray(3);
    glEnableVertexAttribArray(4);
    glEnableVertexAttribArray(5);
    glEnableVertexAttribArray(6);

    //instruct how the program will read the data in the file
    //first we tell which identifier is read, the next argument decides how much we will read, etc, etc
    //the sizeof()-argument tells how much the total of the reading file size is and it is * 28 because all 7 variables will be represented with a vector3d which always contains 4 floats no matter what we use
    //finally we tell the program where to start reading
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float32) * 28, NULL);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(float32) * 28, (GLvoid*)(sizeof(float32) * 4));
    glVertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE, sizeof(float32) * 28, (GLvoid*)(sizeof(float32) * 8));
    glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, sizeof(float32) * 28, (GLvoid*)(sizeof(float32) * 12));
    glVertexAttribPointer(4, 4, GL_FLOAT, GL_FALSE, sizeof(float32) * 28, (GLvoid*)(sizeof(float32) * 16));
    glVertexAttribPointer(5, 4, GL_FLOAT, GL_FALSE, sizeof(float32) * 28, (GLvoid*)(sizeof(float32) * 20));
    glVertexAttribPointer(6, 4, GL_FLOAT, GL_FALSE, sizeof(float32) * 28, (GLvoid*)(sizeof(float32) * 24));

    //bind the buffers etc.
    glDrawElements(GL_TRIANGLES, this->numIndices, GL_UNSIGNED_INT, NULL);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void Skinning::BindSkeleton()
{

}

void Skinning::SetBuffers()
{
    // vertex-buffer
    glGenBuffers(1, &vertexbuffer);																//buffern skapas
    glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);													//buffern aktiveras
    glBufferData(GL_ARRAY_BUFFER, numVertices*sizeof(SkinningVertex), VBO, GL_STATIC_DRAW);			//skapar och initialiserar buffern. H�r ger man buffern en viss storlek i bytes(storleken av arrayn buffer). I vertex_buffer "matas" arrayn buffer in
    glBindBuffer(GL_ARRAY_BUFFER, 0);																//deaktiverar buffern

    //index-buffer
    glGenBuffers(1, &indexBuffer);														//skapar buffern
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBuffer);								//aktiverar buffern
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(int32_t)*numIndices*sizeof(int), IBO, GL_STATIC_DRAW);		//data "laddas in" om den
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

//get the size of a file in bytes using the struct stat
size_t Skinning::getFilesize(const char* filename)
{
    struct stat st;
    stat(filename, &st);
    return st.st_size;
}