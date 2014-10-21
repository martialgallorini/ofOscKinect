//
//  kinectTracker.h
//  ofOscKinect
//
//  Created by martial on 10/10/2014.
//
//

#ifndef __ofOscKinect__kinectTracker__
#define __ofOscKinect__kinectTracker__

#define NEAR_CLIP 500
#define FAR_CLIP 1200

#include "ofxOpenCv.h"
#include "ofxKinect.h"
#include "ROI.h"

class kinectTracker {
    
public:
    
    kinectTracker();
    ~kinectTracker();
                                                                                    
    void setup();
    void update();
    void draw();
    void draw(float _x, float _y, float _w, float _h);
    void drawDepth(float _x, float _y, float _w, float _h);
    //void close();

    ROI roi;
    
    //---------- Parameters ----------//
    
    //float threshold;
//    float nearThreshValue;
//    float farThreshValue;
    float threshold;
    float minBlobSize;
    bool bDilate;
    bool bErode;
    int nbPass;
    
    ofVec3f pos; // x, y and z position of blob
    
private:
    
    ofxKinect kinect;
    
    //void fillBlobPoly();
    
    //---------- blob tracking with ofxKinect and ofxOpenCV ----------//
    
    ofxCvGrayscaleImage depthImage; // grayscale depth image
    //ofxCvGrayscaleImage threshNear;
    //ofxCvGrayscaleImage threshFar;
    ofxCvGrayscaleImage thresholdImage;
    ofxCvContourFinder contourFinder; // blob detection
    //ofxCvGrayscaleImage roiImage;

    
};

#endif /* defined(__ofOscKinect__ROI__) */

