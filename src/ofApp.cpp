#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    
    ofDisableSmoothing();
    ofDisableAlphaBlending();
    //glEnable(GL_LIGHTING);
    
    ofSetFrameRate(FRAMERATE);
    //ofBackground(0, 0, 250);
    
    cvKinect.setup();
    
//    lastTimeCheck = ofGetElapsedTimeMillis();
//    standByTime = ofGetElapsedTimeMillis();
//    effectNumber = 0;
    
    oscHost = "192.168.5.96";
    oscPort = 3333;
    oscSender.setup(oscHost, oscPort);
    
    bSetupMode = false;
    
    scaledDistance = 0;
    scaledPosX = 0;
    scaledPosY = 0;
    
    setupUI();
}


//--------------------------------------------------------------

void ofApp::setupUI() {
    
    // Kinect Infos GUI
    
    kinectUI = new ofxUICanvas(517, 10, CANVAS_WIDTH, CANVAS_HEIGHT);
    kinectUI->setWidgetSpacing(10);
    kinectUI->setName("KINECT INFO");
    kinectUI->addLabel("SENSOR");
    kinectUI->addSpacer();
    kinectUI->add2DPad("POSITION", ofPoint(cvKinect.roi.x, 230), ofPoint(cvKinect.roi.y, 300), &cvKinect.pos, 480, 210);
    kinectUI->addSlider("DISTANCE", NEAR_CLIP, FAR_CLIP, &cvKinect.pos.z);
    
    // Help GUI
    helpUI = new ofxUICanvas(517, 10, CANVAS_WIDTH, CANVAS_HEIGHT);
    helpUI->setName("HELP PANEL");
    helpUI->setWidgetSpacing(10);
    helpUI->setWidgetFontSize(OFX_UI_FONT_SMALL);
    helpUI->addLabel("OSC PARAMETERS");
    helpUI->addSpacer();
    helpUI->addLabel("distance from objet (float) : /vidMap/kinect/distance");
    helpUI->addLabel("position x of objet (float) : /vidMap/kinect/x");
    helpUI->addLabel("position y of objet (float) : /vidMap/kinect/y");
//    helpUI->addLabel("next clip : /vidMap/clip/next");
//    helpUI->addLabel("activate / deactivate effect number N (boolean) : /vidMap/fx/N");
    helpUI->setVisible(false);
    
    // Config GUI 1
    
    configUI1 = new ofxUICanvas(10,  389, ofGetWidth()/3 - 15, CANVAS_HEIGHT);
    configUI1->setName("CONFIG1");
    configUI1->setWidgetSpacing(10);
    
    configUI1->addLabel("ROI PARAMETERS");
    configUI1->addSpacer();
    configUI1->addSlider("ROI - position X", 0.f, CAM_WIDTH, &cvKinect.roi.x);
    configUI1->addSlider("ROI - position Y", 0.f, CAM_HEIGHT, &cvKinect.roi.y);
    configUI1->addSlider("ROI - width", 0.f, CAM_WIDTH, &cvKinect.roi.width);
    configUI1->addSlider("ROI - height", 0.f, CAM_HEIGHT, &cvKinect.roi.height);
    configUI1->addLabel("OSC CONFIGURATION");
    configUI1->addSpacer();
    configUI1->addLabel("IP Address : ");
    configUI1->addWidgetRight(new ofxUITextInput("OSC IP", oscHost, 100));
    configUI1->addLabel("Port : ");
    configUI1->addWidgetRight(new ofxUITextInput("OSC PORT", ofToString(oscPort), 100));
    
    configUI1->loadSettings("config1.xml");
    
    // Config GUI 2
    configUI2 = new ofxUICanvas(configUI1->getRect()->getX() + configUI1->getRect()->getWidth() + 5, 389, (ofGetWidth() - 20) / 3, CANVAS_HEIGHT);
    configUI2->setName("CONFIG2");
    configUI2->setWidgetSpacing(10);
    
    configUI2->addLabel("BLOB DETECTION");
    configUI2->addSpacer();
    configUI2->addSlider("Min blob size", 0.f, 20000.f, &cvKinect.minBlobSize);
    //configUI2->addSlider("Threshold", 0.f, 255.f, &cvKinect.threshValue);
    configUI2->addRangeSlider("Threshold", 0, 300, &cvKinect.farThreshValue, &cvKinect.nearThreshValue);
    configUI2->addLabel("OPTIMIZE");
    configUI2->addSpacer();
    configUI2->addToggle("Dilate", &cvKinect.bDilate, 17, 17);
    configUI2->addWidgetRight(new ofxUIIntSlider("Nb pass dilate", 0, 50, &cvKinect.nbDilate, 250, 17));
    configUI2->addToggle("Erode", &cvKinect.bErode, 17, 17);
    configUI2->addWidgetRight(new ofxUIIntSlider("Nb pass erode", 0, 50, &cvKinect.nbErode, 248, 17));
    configUI2->addSpacer();
    configUI2->addLabelButton("SAVE", false);
    configUI2->addLabelButton("LOAD", false);
    configUI2->addLabelButton("LOAD DEFAULTS", false);
    
    configUI2->loadSettings("config2.xml");
    
    // Effects panel
    
//    effects.push_back("TRACER");
//    effects.push_back("HATCHED SCREEN");
//    effects.push_back("DENT");
    
    effectsUI = new ofxUICanvas(configUI2->getRect()->getX() + configUI2->getRect()->getWidth() +5, 389, (ofGetWidth() - 20) / 3, CANVAS_HEIGHT);
    effectsUI->setName("EFFECTS PANEL");
    effectsUI->setWidgetSpacing(10);
    effectsUI->addLabel("EFFECTS");
    effectsUI->addSpacer();
//    effectsRadio = effectsUI->addRadio("INTERFERENCE TYPE", effects, OFX_UI_ORIENTATION_VERTICAL);
//    effectsRadio->activateToggle(effects.at(effectNumber));
    effectsUI->addSlider("TRIGGER 1", 0, 1, &scaledDistance);
    effectsUI->addSlider("TRIGGER 2", 0, 1, &scaledPosX);
    effectsUI->addSlider("TRIGGER 3", 0, 1, &scaledPosY);
}


//--------------------------------------------------------------
void ofApp::update(){
    
    cvKinect.update();
    
//    if (ofGetElapsedTimeMillis() - lastTimeCheck > TIMEOUT / 2 || cvKinect.getNbBlobs() > 1)
//    {
//        // Every 35 seconds OR when 2 blobs detected, change effect
//        sendOsc("/vidMap/fx/" + ofToString(effectNumber + 1), 0);
//        if (effectNumber < 2)
//        {
//            effectNumber++;
//        }
//        else
//        {
//            effectNumber = 0;
//        }
//        ofxUIRadio *sel = (ofxUIRadio *)effectsUI->getWidget("INTERFERENCE TYPE");
//        sel->activateToggle(effects.at(effectNumber));
//        lastTimeCheck = ofGetElapsedTimeMillis();
//    }
    
    if (cvKinect.getNbBlobs() > 0)
    {
        sendOsc("/vidMap/fx/1", 1);
    }
    else
    {
        sendOsc("/vidMap/fx/1", 0);
    }
    
    // Update normalized values
    ofxUISlider *slider = (ofxUISlider *)kinectUI->getWidget("DISTANCE");
    scaledDistance = slider->getNormalizedValue();
    
    ofxUI2DPad *pad = (ofxUI2DPad *)kinectUI->getWidget("POSITION");
    scaledPosX = pad->getValue().x;
    scaledPosY = pad->getValue().y;
    
    // Send OSC normalized values
    
//    if (ofGetElapsedTimeMillis() - standByTime > TIMEOUT)
//    {
//        // Every 1m14s play next clip
//        sendOsc("/vidMap/clip/next", 1);
//        sendOsc("/vidMap/clip/next", 0);
//        standByTime = ofGetElapsedTimeMillis();
//    }
    
    sendOsc("/vidMap/kinect/distance", scaledDistance);
    sendOsc("/vidMap/kinect/x", scaledPosX);
    sendOsc("/vidMap/kinect/y", scaledPosY);
}

//--------------------------------------------------------------
void ofApp::draw()
{
    cvKinect.draw(10, 10, CANVAS_WIDTH, CANVAS_HEIGHT);
}

void ofApp::exit()
{
    //configUI1->saveSettings("config1.xml");
    //configUI2->saveSettings("config2.xml");
}

void ofApp::guiEvent(ofxUIEventArgs &e)
{
    string name = e.getName();
    if (name == "SAVE") {
        ofxUILabelButton *button = (ofxUILabelButton *) e.widget;
        configUI1->saveSettings("config1.xml");
        configUI2->saveSettings("config2.xml");
    }
    else if (name == "LOAD") {
        ofxUILabelButton *button = (ofxUILabelButton *) e.widget;
        configUI1->loadSettings("config1.xml");
        configUI2->loadSettings("config2.xml");
    }
    else if (name == "LOAD DEFAULTS") {
        ofxUILabelButton *button = (ofxUILabelButton *) e.widget;
        loadDefaultConfig();
    }
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key)
{
    switch (key) {
        case 'f':
            ofToggleFullscreen();
            break;
        case 's':
            configUI1->saveSettings("config1.xml");
            configUI2->saveSettings("config2.xml");
            break;
        case 'd':
            loadDefaultConfig();
            break;
        case 'h':
            kinectUI->toggleVisible();
            helpUI->toggleVisible();
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
void ofApp::loadDefaultConfig()
{
    cvKinect.pos = ofVec3f(0);
    
    cvKinect.roi.x = 0;
    cvKinect.roi.y = 0;
    cvKinect.roi.width = 640;
    cvKinect.roi.height = 480;
    
    cvKinect.nearThreshValue = 0;
    cvKinect.farThreshValue = 300;
    
    cvKinect.minBlobSize = 5000.f;
    
    cvKinect.nbDilate = 0;
    cvKinect.nbErode = 0;
    
    oscHost = "192.168.5.96";
    oscPort = 3333;
    
    
    ofxUITextInput *sel = (ofxUITextInput *)configUI1->getWidget("OSC IP");
    sel->setTextString(oscHost);
    sel = (ofxUITextInput *)configUI1->getWidget("OSC PORT");
    sel->setTextString(ofToString(oscPort));
    oscSender.setup(oscHost, oscPort);
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
