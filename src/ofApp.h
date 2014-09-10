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
    
    //---------- blob tracking with ofxKinect and ofxOpenCV ----------//
    
    ofxCvGrayscaleImage grayImage; // grayscale depth image
	ofxCvGrayscaleImage grayThreshNear; // the near thresholded image
	ofxCvGrayscaleImage grayThreshFar; // the far thresholded image
	ofxCvGrayscaleImage flippedDepthImg; // mirrored depth image (horizontal flip)
	ofxCvGrayscaleImage testImg; // to DEL
	
	ofxCvContourFinder contourFinder;
    
    //---------- DATA ----------//
    
    float nearThreshold;
	float farThreshold;
    float nDist; // normalized kinect distance value (float between 0 and 1)
    float dist;
    ofxUIVec3f pos;
    string oscHost;
    int oscPort;
    
    int frameRate;
    float lastTimeCheck;
    float timeOut;
    
    vector<string> effects;
        
    int effectNumber;
    
    
    
    //---------- GUI ----------//
    
    int canvasTheme;
    
    ofxUICanvas *cartel;
    ofxUICanvas *kinectInfos;
    ofxUICanvas *configPanel;
    ofxUICanvas *effectsPanel;
    
    ofxUIRadio* effectsRadio;
    
    int canvasWidth;
    int canvasHeight;

    void guiEvent(ofxUIEventArgs &e);

    
    //---------- OSC ----------//
    
    ofxOscSender oscSender;
    void sendOsc(string key, float val);
    
    //---------- CONFIGURATION ----------//

    //ofxXmlSettings settings;
    void loadDefaultConfig();
		
};
