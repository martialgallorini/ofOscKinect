#pragma once

#include "ofMain.h"
#include "ofxKinect.h"
#include "ofxUI.h"
#include "ofxOpenCv.h"
#include "ofxOsc.h"
#include "ofxXmlSettings.h"


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
    
    ofxKinect kinect;
    
    //---------- blob tracking with ofxOpenCV ----------//
    
    ofxCvGrayscaleImage grayImage; // grayscale depth image
	ofxCvGrayscaleImage grayThreshNear; // the near thresholded image
	ofxCvGrayscaleImage grayThreshFar; // the far thresholded image
	
	ofxCvContourFinder contourFinder;
    
    //---------- DATA ----------//
    
    float nearThreshold;
	float farThreshold;
    string isEffectActive;
    string isDetectionActive;
    float effectForce;
    float dist;
    ofxUIVec3f pos;
    string oscHost;
    int oscPort;
    float test;
    
    
    //---------- GUI ----------//
    
    int canvasTheme;
    
    ofxUICanvas *cartel;
    ofxUICanvas *kinectInfos;
    ofxUICanvas *configPanel;

    void guiEvent(ofxUIEventArgs &e);

    
    //---------- OSC ----------//
    
    ofxOscSender oscSender;
    void sendOsc(string key, float val);
    
    //---------- CONFIGURATION ----------//

    //ofxXmlSettings settings;
    void loadDefaultConfig();
		
};
