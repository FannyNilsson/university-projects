//
// Created by nilsnils on 2017-09-06.
//
#pragma once
#ifndef GSCEPT_LAB_ENV_SKELETON_H
#define GSCEPT_LAB_ENV_SKELETON_H
#include <iostream>
#include <string>
#include <vector>
#include <typeinfo>
#include <GL/glut.h>
#include <GL/gl.h>
#include "Matrix3D.h"
#include "tinyxml.h"
//#include "ShaderObject.h"

class Skeleton {
private:
public:
    Matrix3D matrix;
    //std::shared_ptr<ShaderObject> shader;
    //LightNode light;
    struct Joints
    {
        std::string name_;
        int indx;
        int parent;
        Vector3D position;
        Vector3D scale;
        Matrix3D rotation;
        Matrix3D translation;
        std::vector<Joints*> children;
        bool isRotating = false;
    };

    std::vector<Joints> JointList;

    Skeleton();
    ~Skeleton();
    void ParseXMLfile(std::string filename);
    Vector3D ConvertString(std::string convertStr);
    void iterateNodesXML(const TiXmlNode* elem);
    void Draw();
    void CalcTrans(Joints* joint, Matrix3D mat);
};


#endif //GSCEPT_LAB_ENV_SKELETON_H
