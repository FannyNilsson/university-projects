//
// Created by nilsnils on 2017-09-06.
//

#include "Skeleton.h"

//--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

Skeleton::Skeleton()
{
//    shader = make_shared<ShaderObject>();											//vi skapar en smart pekare till ett tomt shaderobject
//    shader->startProgram("vertex_shader.txt", "fragment_shader.txt");
//
//    light.setShader(shader);														//h�r b�rjar vi applicera alla v�ra v�rden f�r v�r lightNode. Vi b�rjar med att s�tta shadern
//    light.generateLight();
}

Skeleton::~Skeleton()
{

}

Vector3D Skeleton::ConvertString(std::string convertStr)
{
    std::stringstream input_stringstream(convertStr);
    std::string segment;
    std::vector<std::string> seglist;
    Vector3D tempVec;
    int i = 0;
    while (std::getline(input_stringstream, segment, ','))
    {
        seglist.push_back(segment);
        tempVec[i] = std::stof(seglist[i]);
        i++;
    }
    return tempVec;
}

void Skeleton::iterateNodesXML(const TiXmlNode* elem)
{
    //loop through all attributes in the elements with name "Joint" and save all joint info in the member variables
    for (const TiXmlNode* node=elem->FirstChild(); node; node=node->NextSibling())
    {
        if((std::string)node->Value()=="Joint")
        {
            Joints joint;
            joint.name_ = node->ToElement()->Attribute("name");
            joint.indx = std::stoi(node->ToElement()->Attribute("index"));
            joint.parent = std::stoi(node->ToElement()->Attribute("parent"));
            joint.position = ConvertString(node->ToElement()->Attribute("position"));
            Vector3D tempVec = ConvertString(node->ToElement()->Attribute("rotation"));
            joint.rotation = Matrix3D::QuaternionToMatrix(tempVec);
            joint.rotation = joint.rotation.transpose(joint.rotation);
            joint.scale = ConvertString(node->ToElement()->Attribute("scale"));
            joint.position = joint.position * 0.3;

            joint.translation = Matrix3D::translate(joint.rotation, joint.position);
            joint.translation = joint.translation.transpose(joint.translation);
            JointList.push_back(joint);
        }
        iterateNodesXML(node);
    }
}


void Skeleton::ParseXMLfile(std::string filename)
{
    TiXmlDocument file(filename);
    if(file.LoadFile())
    {
        TiXmlElement* root = file.FirstChildElement("Nebula3");
        iterateNodesXML(root);
    }

    //loop through the list of joints to add the children in every parents children list
    for(int i=JointList.size()-1;i>=0;i--)
    {
        if(JointList[i].parent == -1)
        {
            break;
        }
        else
        {
            JointList[JointList[i].parent].children.push_back(&JointList[i]);
        }
    }
}

//Call on the recursive function which calculates the translation for all joints
void Skeleton::Draw()
{
    Matrix3D mat;
    CalcTrans(&JointList[0], mat);
}

//recursive function
void Skeleton::CalcTrans(Joints* joint, Matrix3D mat)
{
    //save the parents matrix in parentMat so we can use it to draw lines later
    float* parentMat = Matrix3D::MatrixConvertion(mat);
    //if we haven't reached the root (root = -1) we multiply our translation matrix with the parents translation matrix
    if(joint->parent != -1)
    {
        //our matrix*parents matrix (mat) is saved in mat
        mat = joint->translation*mat;
    }
    else
    {
        //first iteration mat will first be an identity matrix and here we change it to our roots matrix (alas for next iteration this will be the joints parent matrix)
        mat = joint->translation;
    }

    for(int i=0;i<joint->children.size();i++)
    {
        CalcTrans(joint->children[i], mat);
    }

    glPushMatrix();

    if(joint->indx == 0)
    {
        glColor3f(1.0f,0.0f,0.0f);
    }
    else if(joint->indx == 4)
    {
        glColor3f(0.0f,1.0f,0.0f);
    }
    else if(joint->indx == 11 || joint->indx == 7)
    {
        glColor3f(0.0f,0.0f,1.0f);
    }
    else if(joint->indx == 19 || joint->indx == 15)
    {
        glColor3f(0.0f,0.5f,0.5f);
    }
    else
    {
        glColor3f(0.5f,0.5f,0.5f);
    }

    float* currentMat = Matrix3D::MatrixConvertion(mat);
    glLoadMatrixf(currentMat);
    glutWireSphere(0.02,8,8);
    glPopMatrix();

    //child - the start joint which the line will be drawn from later
    if(joint->parent > 0)
    {
        float sx = currentMat[12];
        float sy = currentMat[13];
        float sz = currentMat[14];

        //parent - the goal joint which the line will be drawn to later

        float gx = parentMat[12];
        float gy = parentMat[13];
        float gz = parentMat[14];

        glLineWidth(1);
        glColor3f(0,0,255);
        glBegin(GL_LINES);
        glVertex3f(sx,sy,sz);
        glVertex3f(gx,gy,gz);
        glEnd();
    }
    matrix = mat;

}