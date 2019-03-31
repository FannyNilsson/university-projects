//
// Created by nilsnils on 2017-09-18.
//

#include "Animation.h"

bool Animation::Nax3Parser(const char* filename)
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
    Nax3Header* naxHead = (Nax3Header*) ptr;
    ptr += sizeof(Nax3Header);


    if(naxHead->numClips > 0)
    {
        int numClips_size = (int)naxHead->numClips;

        keyArrPtr = new Vector3D[naxHead->numKeys];                      //create an array which holds all the key pointers
        groupList = new Nax3Group[numClips_size];

        //loop through all the clips
        for(int clipIndx = 0; clipIndx<numClips_size; clipIndx++)
        {
            //move to next part of the file in the memory (the next startpoint will be after the Nax3Clips size
            Nax3Clips* naxClip = (Nax3Clips*)ptr;
            ptr += sizeof(Nax3Clips);

            groupList[clipIndx].clip = *naxClip;                                    //add the clip to the group in the groupList

            //loop through all the events
            for(int eventIndx = 0; eventIndx<naxClip->numEvents; eventIndx++)
            {
                //move to next part of the file in the memory (the next start point will be after the Nax3AnimEvent size
                Nax3AnimEvent* naxEvent = (Nax3AnimEvent*)ptr;
                ptr += sizeof(Nax3AnimEvent);
            }

            groupList[clipIndx].curveList = new Nax3Curve[naxClip->numCurves];          //allocate memory fo the list of curves in each clip
            //loop through all the curves
            for(int curveIndx = 0; curveIndx<naxClip->numCurves; curveIndx++)
            {
                //move to next part of the file in the memory (the next start point will be after the Nax3AnimEvent size
                Nax3Curve* naxCurve = (Nax3Curve*)ptr;
                ptr += sizeof(Nax3Curve);

                groupList[clipIndx].curveList[curveIndx] = *naxCurve;
                groupList[clipIndx].curveListSize = groupList[clipIndx].curveListSize + 1;
            }
        }

    }

    //loop through all the keys
    for(int keyIndx=0; keyIndx < naxHead->numKeys; keyIndx++)
    {
        Vector3D* key = (Vector3D*)ptr;
        ptr += sizeof(Vector3D);
        this->keyArrPtr[keyIndx] = *key;
        //std::cout << "Keys" << key->x << std::endl;
    }

    //std::cout << "Curves: " << curves.size() << std::endl;

    int fail = munmap(beginning, fileSizeBytes);
    if(fail == -1)
    {
        std::cerr << "OH NOOOOO!" << std::endl;
    }

    close(fileDescriptor);
}

//get the size of a file in bytes using the struct stat
size_t Animation::getFilesize(const char* filename)
{
    struct stat st;
    stat(filename, &st);
    return st.st_size;
}

void Animation::SetUpAnimation(Skeleton* skel, int ClipIndex)
{
    Nax3Clips clip = groupList[ClipIndex].clip;
    //if the previous animation(clip) isn't the same as the new one we save the last key in the clip as previous keyframeindex as the last since keyframeindex start at the beginning of the clip
    if(previousClip != ClipIndex)
    {
        previousClip = ClipIndex;
        previousKeyframeIndex = clip.numKeys-1;
        keyframeIndex = 0;
        start = std::chrono::system_clock::now();
    }
    end = std::chrono::system_clock::now();
    //calculate the frame
    std::chrono::duration<double, std::milli> elapsed_milliseconds = (end-start);
    float frameDuration = fmod(elapsed_milliseconds.count(),(double)clip.keyDuration);
    timeElapsed = timeElapsed + frameDuration;


    //check if the frameDuration is the same as the keyDuration (the time for the key is up) we want to go to next keyframe
    if(timeElapsed > clip.keyDuration)
    {
        timeElapsed = 0.0f;
        if(keyframeIndex+1 >= clip.numKeys)
        {
            keyframeIndex = 0;                                          //if the keyframe we are on is the last we have to reset the keyframeindex to reset the animation
            previousKeyframeIndex = clip.numKeys-1;
        }
        else
        {
            previousKeyframeIndex = keyframeIndex;
            keyframeIndex = keyframeIndex + 1;                          //add 1 to move to next keyframe
        }
    }
    start = std::chrono::system_clock::now();

    //time to interpolate between the current keyframe of a joint with the next keyframe of the joint
    for(int i=0, j=0; i < skel->JointList.size(); i++, j+=4)
    {
        //get the right curves for the right joint
        Nax3Curve* posCurve = &groupList[ClipIndex].curveList[j];
        Nax3Curve* rotCurve = &groupList[ClipIndex].curveList[j+1];
        Nax3Curve* scaleCurve = &groupList[ClipIndex].curveList[j+2];
        Nax3Curve* velCurve = &groupList[ClipIndex].curveList[j+3];

        //get the right keys
        Vector3D pos = keyArrPtr[posCurve->firstKeyIndex + clip.keyStride*previousKeyframeIndex];
        Vector3D rot = keyArrPtr[rotCurve->firstKeyIndex + clip.keyStride*previousKeyframeIndex];
        Vector3D scale = keyArrPtr[scaleCurve->firstKeyIndex + clip.keyStride*previousKeyframeIndex];
        Vector3D vel = keyArrPtr[velCurve->firstKeyIndex + clip.keyStride*previousKeyframeIndex];


        //lerp and slerp with the previousKeyframeIndex and the current
        Vector3D lerpPos = Vector3D::lerp(pos, keyArrPtr[posCurve->firstKeyIndex + clip.keyStride*keyframeIndex],timeElapsed/clip.keyDuration);
        Vector3D lerpScale = Vector3D::lerp(scale, keyArrPtr[scaleCurve->firstKeyIndex + clip.keyStride*keyframeIndex],timeElapsed/clip.keyDuration);
        Vector3D lerpVel = Vector3D::lerp(vel, keyArrPtr[velCurve->firstKeyIndex + clip.keyStride*keyframeIndex],timeElapsed/clip.keyDuration);
        Vector3D rotSlerp = rot.Slerp(rot, keyArrPtr[rotCurve->firstKeyIndex + clip.keyStride*keyframeIndex], timeElapsed/clip.keyDuration);

        //set the lerped and slerped pos, rot etc. on each joint
        skel->JointList[i].rotation = Matrix3D::QuaternionToMatrix(rot);
        skel->JointList[i].rotation = skel->JointList[i].rotation.transpose(skel->JointList[i].rotation);
        skel->JointList[i].position = lerpPos * 0.3;
        skel->JointList[i].translation = Matrix3D::translate(skel->JointList[i].rotation, skel->JointList[i].position);
        skel->JointList[i].translation = skel->JointList[i].translation.transpose(skel->JointList[i].translation);
    }

}
