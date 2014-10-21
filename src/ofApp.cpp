#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    
    ofDisableSmoothing();
    ofDisableAlphaBlending();
    //glEnable(GL_LIGHTING);
    
    ofSetFrameRate(FRAMERATE);
    //ofBackground(0, 0, 250);
    
    cvKinect.setup();
    
    lastTimeCheck = ofGetElapsedTimeMillis();
    effectNumber = 0;
    
    oscHost = "192.168.4.190";
    oscPort = 3333;
    oscSender.setup(oscHost, oscPort);
    
    bSetupMode = false;
    
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
    //kinectUI->add2DPad("POSITION", ofPoint(0, cvKinect.roi.width), ofPoint(0, cvKinect.roi.height), &cvKinect.pos, 480, 210);
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
    helpUI->addLabel("next clip : /vidMap/clip/next");
    helpUI->addLabel("activate / deactivate effect number N (boolean) : /vidMap/fx/N");
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
    configUI2->addSlider("Threshold", 0.f, 255.f, &cvKinect.threshold);
    //configUI2->addRangeSlider("Threshold", 0, 1500, &cvKinect.nearThreshValue, &cvKinect.farThreshValue);
    
    configUI2->addLabel("OPTIMIZE");
    configUI2->addSpacer();
    configUI2->addToggle("Dilate", &cvKinect.bDilate, 20, 20);
    configUI2->addToggle("Erode", &cvKinect.bErode, 20, 20);
    configUI2->addIntSlider("Pass number", 0, 50, &cvKinect.nbPass);
    configUI2->addSpacer();
    configUI2->addLabelButton("SAVE", false);
    configUI2->addLabelButton("LOAD", false);
    configUI2->addLabelButton("LOAD DEFAULTS", false);

    configUI2->loadSettings("config2.xml");
    
    // Effects panel
    
    effects.push_back("CITY LIGHTS");
    effects.push_back("HEXAGONAL PIXELLATE");
    effects.push_back("TRACER");
    effects.push_back("GLITCH FBO COMPOSITE");
    effects.push_back("RUTT ETRA");
    effects.push_back("CUBE MAP");
    effects.push_back("LINE SCREEN");
    effects.push_back("DENT");
    
    effectsUI = new ofxUICanvas(configUI2->getRect()->getX() + configUI2->getRect()->getWidth() +5, 389, (ofGetWidth() - 20) / 3, CANVAS_HEIGHT);
    effectsUI->setName("EFFECTS PANEL");
    effectsUI->setWidgetSpacing(10);
    effectsUI->addLabel("EFFECTS");
    effectsUI->addSpacer();
    effectsRadio = effectsUI->addRadio("INTERFERENCE TYPE", effects, OFX_UI_ORIENTATION_VERTICAL);
    effectsRadio->activateToggle(effects.at(effectNumber));
    effectsUI->addSlider("TRIGGER 1", 0, 1, &cvKinect.pos.z);
    effectsUI->addSlider("TRIGGER 2", 0, 1, &cvKinect.pos.x);
    effectsUI->addSlider("TRIGGER 3", 0, 1, &cvKinect.pos.y);
    
}


//--------------------------------------------------------------
void ofApp::update(){
    
    cvKinect.update();
    
//    if ( ofGetElapsedTimeMillis() - lastTimeCheck > TIMEOUT)
//    {
//        sendOsc("/vidMap/fx/" + ofToString(effectNumber + 1), 0);
//        if (effectNumber < 7)
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
//    
//    // Send OSC values
//    sendOsc("/vidMap/kinect/distance", ofMap(cvKinect.pos.z, 1100, 500, 1, 0));
//    sendOsc("/vidMap/kinect/x", ofMap(cvKinect.pos.x, 250, 500, 0, 1));
//    sendOsc("/vidMap/kinect/y", ofMap(cvKinect.pos.y, 285, 380, 0, 1));
}

//--------------------------------------------------------------
void ofApp::draw()
{
    cvKinect.draw(10, 10, CANVAS_WIDTH, CANVAS_HEIGHT);
    //cvKinect.drawDepth(10, 10, CANVAS_WIDTH, CANVAS_HEIGHT);
}

void ofApp::exit()
{
    //cvKinect.close();
    configUI1->saveSettings("config1.xml");
    configUI2->saveSettings("config2.xml");
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
            configUI1->saveSettings("config.xml");
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
    
//    cvKinect.nearThreshValue = 10;
//    cvKinect.farThreshValue = 1500;
    cvKinect.threshold = 10;
    
    cvKinect.minBlobSize = 5000.f;
    
    cvKinect.nbPass = 1;
    
    oscHost = "192.168.5.81";
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
