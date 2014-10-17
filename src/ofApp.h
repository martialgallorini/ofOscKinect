#pragma once

#define TIMEOUT 30000 // in millisecond = 1m 14s
#define FRAMERATE 30
#define CANVAS_WIDTH 497
#define CANVAS_HEIGHT 369
#define CANVAS_THEME OFX_UI_THEME_DEFAULT
//canvasTheme = OFX_UI_THEME_MINYELLOW;
//canvasTheme = OFX_UI_THEME_MINBLACK;
//canvasTheme = OFX_UI_THEME_HIPSTER;
//canvasTheme = OFX_UI_THEME_HACKER;


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
    
    float lastTimeCheck;
    bool bSetupMode;
    
    vector<string> effects;
    
    int effectNumber;
    
    
    //---------- GUI ----------//
    
    int canvasTheme;
    
    ofxUICanvas *kinectUI;
    ofxUICanvas *configUI;
    ofxUICanvas *effectsUI;
    ofxUICanvas *helpUI;
    
    ofxUIRadio* effectsRadio;
    
    void guiEvent(ofxUIEventArgs &e);
    
    //---------- OSC ----------//
    
    ofxOscSender oscSender;
    void sendOsc(string key, float val);
    
    //---------- XML SETTINGS ----------//
    void loadDefaultConfig();
    
};
