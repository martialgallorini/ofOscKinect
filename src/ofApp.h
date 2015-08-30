#pragma once

#define TIMEOUT 30000 // in millisecond = 1m 14s
#define FRAMERATE 30

#include "ofMain.h"
#include "ofxGui.h"
#include "ofxOsc.h"
#include "kinectTracker.h"


class ofApp : public ofBaseApp{
    
public:
    void setup();
    void update();
    void draw();
    void exit();
    
    void keyPressed(int key);
    void keyReleased(int key);
    void mouseMoved(int x, int y );
    void mouseDragged(int x, int y, int button);
    void mousePressed(int x, int y, int button);
    void mouseReleased(int x, int y, int button);
    void windowResized(int w, int h);
    void dragEvent(ofDragInfo dragInfo);
    void gotMessage(ofMessage msg);
    
    kinectTracker cvKinect;
    
    string oscHost;
    int oscPort;
    
    //---------- GUI ----------//
    
    ofxPanel gui;
    ofParameterGroup parameters;
    
    
    
//    // BLOB position
//    ofParameter<float> posX;
//    ofParameter<float> posY;
//    ofParameter<float> dist;
//    
//    // BLOB detection
//    
//    ofParameter<float> blobMinSize;
//    ofParameter<float> farThresh;
//    ofParameter<float> nearThresh;
//    ofParameter<bool> bDilate;
//    ofParameter<float> nbDilate;
//    ofParameter<bool> bErode;
//    ofParameter<float> nbErode;



    
    
    // ROI
    ofParameter<float> roiX;
    ofParameter<float> roiY;
    ofParameter<float> roiW;
    ofParameter<float> roiH;
    
    
    
    bool bSetupMode;
    
    //---------- OSC ----------//
    
    ofxOscSender oscSender;
    void sendOsc(string key, float val);
    
};
