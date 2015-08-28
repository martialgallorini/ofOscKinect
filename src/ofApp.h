#pragma once

#define TIMEOUT 30000 // in millisecond = 1m 14s
#define FRAMERATE 30
#define CANVAS_WIDTH 497
#define CANVAS_HEIGHT 369
#define CANVAS_THEME OFX_UI_THEME_DEFAULT

#include "ofMain.h"
#include "ofxUI.h"
#include "ofxOsc.h"
#include "ofxXmlSettings.h"
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
    
    void setupUI();
    
    kinectTracker cvKinect;
    
    string oscHost;
    int oscPort;
    
    bool bSetupMode;
    
    vector<string> effects;
    
    float scaledDistance;
    float scaledPosX;
    float scaledPosY;
    
    //---------- GUI ----------//
    
    int canvasTheme;
    
    ofxUICanvas *kinectUI;
    ofxUICanvas *configUI1;
    ofxUICanvas *configUI2;
    ofxUICanvas *helpUI;
    
    
    void guiEvent(ofxUIEventArgs &e);
    
    //---------- OSC ----------//
    
    ofxOscSender oscSender;
    void sendOsc(string key, float val);
    
    //---------- XML SETTINGS ----------//
    void loadDefaultConfig();
    
};
