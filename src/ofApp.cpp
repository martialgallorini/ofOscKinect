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
    
    setupMode = false;
    
    setupUI();
    
}


//--------------------------------------------------------------

void ofApp::setupUI() {
    
    // Config Panel GUI
    
    configUI = new ofxUICanvas(10, 389, CANVAS_WIDTH, CANVAS_HEIGHT);
    configUI->setName("CONFIG PANEL");
    configUI->setWidgetSpacing(10);
    
    configUI->addLabel("PARAMETRES");
    configUI->addSpacer();
    configUI->addSlider("ROI - position X", 0.f, 640.f, &cvKinect.roi.x);
    configUI->addSlider("ROI - position Y", 0.f, 480.f, &cvKinect.roi.y);
    configUI->addSlider("ROI - largeur", 0.f, 640.f, &cvKinect.roi.width);
    configUI->addSlider("ROI - hauteur", 0.f, 480.f, &cvKinect.roi.height);
    configUI->addSlider("Taille min du blob", 0.f, 20000.f, &cvKinect.minBlobSize);
    configUI->addSlider("Sensibilite", 0.f, 255.f, &cvKinect.threshold);
    configUI->addLabel("Adresse / Port OSC");
    configUI->addWidgetRight(new ofxUITextInput("OSC IP", oscHost, 100));
    configUI->addWidgetRight(new ofxUITextInput("OSC PORT", ofToString(oscPort), 100));
    configUI->addSpacer(20);
    configUI->addWidgetDown(new ofxUILabelButton("SAVE", false, 150));
    configUI->addWidgetRight(new ofxUILabelButton("LOAD", false, 150));
    configUI->addWidgetRight(new ofxUILabelButton("LOAD DEFAULTS", false, 150));
    
    configUI->loadSettings("config.xml");
    
    // Help GUI
    helpUI = new ofxUICanvas(517, 10, CANVAS_WIDTH, CANVAS_HEIGHT);
    helpUI->setName("HELP PANEL");
    helpUI->setWidgetSpacing(10);
    helpUI->setWidgetFontSize(OFX_UI_FONT_SMALL);
    helpUI->addLabel("PARAMETRES OSC");
    helpUI->addSpacer();
    helpUI->addLabel("distance de l'objet (float) : /vidMap/kinect/distance");
    helpUI->addLabel("position x de l'objet (float) : /vidMap/kinect/x");
    helpUI->addLabel("position y de l'objet (float) : /vidMap/kinect/y");
    helpUI->addLabel("clip suivant : /vidMap/clip/next");
    helpUI->addLabel("active / desactive effet numero N (booleen) : /vidMap/fx/N");
    helpUI->setVisible(false);
    
    // Kinect Infos GUI
    
    kinectUI = new ofxUICanvas(517, 10, CANVAS_WIDTH, CANVAS_HEIGHT);
    kinectUI->setWidgetSpacing(10);
    kinectUI->setName("KINECT INFOS");
    kinectUI->addLabel("CAPTEUR");
    kinectUI->addSpacer();
    kinectUI->add2DPad("POSITION", ofPoint(250, 500), ofPoint(285, 400), &cvKinect.pos, 480, 210);
    kinectUI->addSlider("DISTANCE", 500.f, 1100.f, &cvKinect.pos.z);
    
    // Effects panel
    
    effects.push_back("CITY LIGHTS");
    effects.push_back("HEXAGONAL PIXELLATE");
    effects.push_back("TRACER");
    effects.push_back("GLITCH FBO COMPOSITE");
    effects.push_back("RUTT ETRA");
    effects.push_back("CUBE MAP");
    effects.push_back("LINE SCREEN");
    effects.push_back("DENT");
    
    effectsUI = new ofxUICanvas(517, 389, CANVAS_WIDTH, CANVAS_HEIGHT);
    effectsUI->setName("EFFECTS PANEL");
    effectsUI->setWidgetSpacing(10);
    effectsUI->addLabel("EFFETS");
    effectsUI->addSpacer();
    effectsRadio = effectsUI->addRadio("TYPE INTERFERENCE", effects, OFX_UI_ORIENTATION_VERTICAL);
    effectsRadio->activateToggle(effects.at(effectNumber));
    effectsUI->addSlider("TRACEUR 1", 0, 1, &cvKinect.pos.z);
    effectsUI->addSlider("TRACEUR 2", 0, 1, &cvKinect.pos.x);
    effectsUI->addSlider("TRACEUR 3", 0, 1, &cvKinect.pos.y);
    
}


//--------------------------------------------------------------
void ofApp::update(){
    
    cvKinect.update();
    
    if ( ofGetElapsedTimeMillis() - lastTimeCheck > TIMEOUT)
    {
        sendOsc("/vidMap/fx/" + ofToString(effectNumber + 1), 0);
        if (effectNumber < 7)
        {
            effectNumber++;
        }
        else
        {
            effectNumber = 0;
        }
        ofxUIRadio *sel = (ofxUIRadio *)effectsUI->getWidget("TYPE INTERFERENCE");
        sel->activateToggle(effects.at(effectNumber));
        lastTimeCheck = ofGetElapsedTimeMillis();
    }
    
    // Send OSC values
    sendOsc("/vidMap/kinect/distance", ofMap(cvKinect.pos.z, 1100, 500, 1, 0));
    sendOsc("/vidMap/kinect/x", ofMap(cvKinect.pos.x, 250, 500, 0, 1));
    sendOsc("/vidMap/kinect/y", ofMap(cvKinect.pos.y, 285, 380, 0, 1));
}

//--------------------------------------------------------------
void ofApp::draw()
{
    cvKinect.draw(10, 10, CANVAS_WIDTH, CANVAS_HEIGHT);
}

void ofApp::exit()
{
    //cvKinect.close();
    configUI->saveSettings("config.xml");
}

void ofApp::guiEvent(ofxUIEventArgs &e)
{
    string name = e.getName();
    if (name == "SAVE") {
        ofxUILabelButton *button = (ofxUILabelButton *) e.widget;
        configUI->saveSettings("config.xml");
    }
    else if (name == "LOAD") {
        ofxUILabelButton *button = (ofxUILabelButton *) e.widget;
        configUI->loadSettings("config.xml");
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
            configUI->saveSettings("config.xml");
            break;
        case 'd':
            loadDefaultConfig();
            break;
        case 'h':
            //configUI->toggleVisible();
            //effectsUI->toggleVisible();
            kinectUI->toggleVisible();
            helpUI->toggleVisible();
            setupMode = !setupMode;
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
    cvKinect.pos = ofVec3f(0,0,0);
    
    cvKinect.roi.x = 0;
    cvKinect.roi.y = 0;
    cvKinect.roi.width = 640;
    cvKinect.roi.height = 480;
    
    cvKinect.threshold = 0;
    cvKinect.minBlobSize = 5000.f;
    
    oscHost = "192.168.5.81";
    oscPort = 3333;
    
    ofxUITextInput *sel = (ofxUITextInput *)configUI->getWidget("OSC IP");
    sel->setTextString(oscHost);
    sel = (ofxUITextInput *)configUI->getWidget("OSC PORT");
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
