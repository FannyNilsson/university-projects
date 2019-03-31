//
// Created by nilsnils on 2017-09-18.
//

#ifndef GSCEPT_LAB_ENV_ANIMATION_H
#define GSCEPT_LAB_ENV_ANIMATION_H

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


typedef unsigned long ulong;
typedef unsigned int uint;
typedef unsigned short ushort;
typedef unsigned char uchar;

class Animation {

public:
    bool Nax3Parser(const char* filename);
    size_t getFilesize(const char* filename);
    void SetUpAnimation(Skeleton* skel, int ClipIndex);


    struct Nax3Header
    {
        uint magic;
        uint numClips;
        uint numKeys;
    };

    struct Nax3Clips
    {
        ushort numCurves;
        ushort startKeyIndex;
        ushort numKeys;
        ushort keyStride;
        ushort keyDuration;
        uchar preInfinityType;
        uchar postInfinityType;
        ushort numEvents;
        char name[50];
    };

    struct Nax3AnimEvent
    {
        char name[47];
        char category[15];
        ushort keyIndex;
    };

    struct Nax3Curve
    {
        uint firstKeyIndex;
        uchar isActive;                 // 0 or 1
        uchar isStatic;                 // 0 or 1
        uchar curveType;
        uchar _padding;
        float staticKeyX;
        float staticKeyY;
        float staticKeyZ;
        float staticKeyW;
    };


    struct Nax3Group
    {
        Nax3Curve* curveList;
        Nax3Clips clip;
        int curveListSize;
    };


    std::chrono::time_point<std::chrono::system_clock> start, end;
private:

    Nax3Clips* clipArrPtr;
    std::vector<Nax3Curve*> curves;
    Nax3Group* groupList;
    Vector3D* keyArrPtr;
    bool calculatedStarttime = false;
    bool addedKeyFrames = false;
    int keyframeIndex = 0;
    float timeElapsed = 0;
    int previousKeyframeIndex;
    int previousClip;
};


#endif //GSCEPT_LAB_ENV_ANIMATION_H
