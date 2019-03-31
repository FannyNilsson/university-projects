//
// Created by nilsnils on 2018-04-05.
//

#ifndef GSCEPT_LAB_ENV_VERTEXCOMPONENTS_H
#define GSCEPT_LAB_ENV_VERTEXCOMPONENTS_H

class VertexComponents
{
public:
    enum SemanticName
    {
        Position = 0,
        Normal = 1,
        TexCoord1 = 2,
        Tangent = 3,
        Binormal = 4,
        Color = 5,
        TexCoord2 = 6,
        SkinWeights = 7,
        SkinJIndices = 8,

        Invalid,
    };

    /// component format
    enum Format
    {
        Float,      //> one-component float, expanded to (float, 0, 0, 1)
        Float2,     //> two-component float, expanded to (float, float, 0, 1)
        Float3,     //> three-component float, expanded to (float, float, float, 1)
        Float4,     //> four-component float
        UByte4,     //> four-component unsigned byte
        Byte4,		//> four-component signed byte
        Short2,     //> two-component signed short, expanded to (value, value, 0, 1)
        Short4,     //> four-component signed short
        UByte4N,    //> four-component normalized unsigned byte (value / 255.0f)
        Byte4N,		//> four-component normalized signed byte (value / 127.0f)
        Short2N,    //> two-component normalized signed short (value / 32767.0f)
        Short4N,    //> four-component normalized signed short (value / 32767.0f)
    };

    /// stride type tells if the compoent should be per-instance or per-vertex
    enum StrideType
    {
        PerVertex,
        PerInstance
    };

    VertexComponents()
    {

    }
    VertexComponents(SemanticName sn, int index, Format form)
    {
        semName = sn;
        semIndex = index;
        format = form;
    }

    SemanticName semName;
    int semIndex;
    Format format;
};

#endif //GSCEPT_LAB_ENV_VERTEXCOMPONENTS_H