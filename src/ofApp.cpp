#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    
    ofDisableSmoothing();
    ofDisableAlphaBlending();
    
    ofSetFrameRate(FRAMERATE);
    
    cvKinect.setup();
    
    oscHost = "127.0.0.1";
    oscPort = 3333;
    oscSender.setup(oscHost, oscPort);
    
    bSetupMode = false;
    
    parameters.setName("SETTINGS");
    parameters.add(cvKinect.parameters);
    gui.setup(parameters);    
}


//--------------------------------------------------------------

//void ofApp::setupUI() {
//    
//    // Kinect Infos GUI
//    
//    kinectUI = new ofxUICanvas(517, 10, CANVAS_WIDTH, CANVAS_HEIGHT);
//    kinectUI->setWidgetSpacing(10);
//    kinectUI->setName("KINECT INFO");
//    kinectUI->addLabel("SENSOR");
//    kinectUI->addSpacer();
//    kinectUI->add2DPad("POSITION", ofPoint(cvKinect.roi.x, 230), ofPoint(cvKinect.roi.y, 300), &cvKinect.pos, 480, 210);
//    kinectUI->addSlider("DISTANCE", NEAR_CLIP, FAR_CLIP, &cvKinect.pos.z);
//    
//    // Config GUI 1
//    
//    configUI1 = new ofxUICanvas(10,  389, ofGetWidth()/3 - 15, CANVAS_HEIGHT);
//    configUI1->setName("CONFIG1");
//    configUI1->setWidgetSpacing(10);
//    
//    configUI1->addLabel("ROI PARAMETERS");
//    configUI1->addSpacer();
//    configUI1->addSlider("ROI - position X", 0.f, CAM_WIDTH, &cvKinect.roi.x);
//    configUI1->addSlider("ROI - position Y", 0.f, CAM_HEIGHT, &cvKinect.roi.y);
//    configUI1->addSlider("ROI - width", 0.f, CAM_WIDTH, &cvKinect.roi.width);
//    configUI1->addSlider("ROI - height", 0.f, CAM_HEIGHT, &cvKinect.roi.height);
//    
//    configUI1->loadSettings("config1.xml");
//    
//    // Config GUI 2
//    configUI2 = new ofxUICanvas(configUI1->getRect()->getX() + configUI1->getRect()->getWidth() + 5, 389, (ofGetWidth() - 20) / 3, CANVAS_HEIGHT);
//    configUI2->setName("CONFIG2");
//    configUI2->setWidgetSpacing(10);
//    
//    configUI2->addLabel("BLOB DETECTION");
//    configUI2->addSpacer();
//    configUI2->addSlider("Min blob size", 0.f, 20000.f, &cvKinect.minBlobSize);
//    configUI2->addRangeSlider("Threshold", 0, 300, &cvKinect.farThreshValue, &cvKinect.nearThreshValue);
//    configUI2->addLabel("OPTIMIZE");
//    configUI2->addSpacer();
//    configUI2->addToggle("Dilate", &cvKinect.bDilate, 17, 17);
//    configUI2->addWidgetRight(new ofxUIIntSlider("Nb pass dilate", 0, 50, &cvKinect.nbDilate, 250, 17));
//    configUI2->addToggle("Erode", &cvKinect.bErode, 17, 17);
//    configUI2->addWidgetRight(new ofxUIIntSlider("Nb pass erode", 0, 50, &cvKinect.nbErode, 248, 17));
//    configUI2->addSpacer();
//    configUI2->addLabelButton("SAVE", false);
//    configUI2->addLabelButton("LOAD", false);
//    configUI2->addLabelButton("LOAD DEFAULTS", false);
//    
//    configUI2->loadSettings("config2.xml");
//
//    
//    // Help GUI
//    
//    helpUI = new ofxUICanvas(configUI2->getRect()->getX() + configUI1->getRect()->getWidth() + 13, 389, (ofGetWidth() - 20) / 3, CANVAS_HEIGHT);
//    helpUI->setName("HELP PANEL");
//    helpUI->setWidgetSpacing(10);
//    helpUI->setWidgetFontSize(OFX_UI_FONT_SMALL);
//    helpUI->addLabel("OSC PARAMETERS");
//    helpUI->addSpacer();
//    helpUI->addLabel("IP Address : 127.0.0.1");
//    helpUI->addLabel("Port : 3333");
//    helpUI->addLabel("");
//    helpUI->addLabel("distance from objet (float) :");
//    helpUI->addLabel("/kinect/distance");
//    helpUI->addLabel("");
//    helpUI->addLabel("position x of objet (float) :");
//    helpUI->addLabel("/kinect/x");
//    helpUI->addLabel("");
//    helpUI->addLabel("position y of objet (float) :");
//    helpUI->addLabel("/kinect/y");
//}


//--------------------------------------------------------------
void ofApp::update(){
    
    cvKinect.update();
//    
//    // Update normalized values
//    ofxUISlider *slider = (ofxUISlider *)kinectUI->getWidget("DISTANCE");
//    scaledDistance = slider->getNormalizedValue();
//    
//    ofxUI2DPad *pad = (ofxUI2DPad *)kinectUI->getWidget("POSITION");
//    scaledPosX = pad->getValue().x;
//    scaledPosY = pad->getValue().y;
//    
//    sendOsc("/kinect/distance", scaledDistance);
//    sendOsc("/kinect/x", scaledPosX);
//    sendOsc("/kinect/y", scaledPosY);
}

//--------------------------------------------------------------
void ofApp::draw()
{
    cvKinect.draw(0, 0, 640, 480);
    if (bSetupMode) {
        gui.draw();
    }
}

void ofApp::exit()
{
    
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key)
{
    switch (key) {
        case 'f':
            ofToggleFullscreen();
            break;
        case 's':
            break;
        case 'd':
            loadDefaultConfig();
            break;
        case OF_KEY_TAB:
            bSetupMode = !bSetupMode;
            break;
        default:
            break;
    }
}

//--------------------------------------------------------------
void ofApp::sendOsc(string key, float val)
{
    ofxOscMessage m;
    m.setAddress(key);
    m.addFloatArg(val);
    oscSender.sendMessage(m);
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){
    
}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){
    
}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){
    
}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){
    
}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){
    
}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){
    
}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){
    
}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){
    
}
