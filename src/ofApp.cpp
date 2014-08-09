#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){

    //ofDisableSmoothing();
    //ofDisableAlphaBlending();
    //glEnable(GL_LIGHTING);
    
    ofSetFrameRate(60);
    
    kinect.init(true, false, true);
    kinect.setDepthClipping(500, 1200);
    kinect.open();
    
    grayImage.allocate(kinect.width, kinect.height);
	grayThreshNear.allocate(kinect.width, kinect.height);
	grayThreshFar.allocate(kinect.width, kinect.height);
    
    nearThreshold = 400;
	farThreshold = 50;
    isEffectActive = "inactif";
    isDetectionActive = "Aucune presence";
    effectForce = 0;
    dist = 0;
    pos = ofPoint(0,0);
    oscHost = "192.168.5.81";
    oscPort = 3333;
        
    oscSender.setup(oscHost, oscPort);
    
    //canvasTheme = OFX_UI_THEME_DEFAULT;
    canvasTheme = OFX_UI_THEME_MINYELLOW;
    

    
    // cartel GUI
    
    cartel = new ofxUICanvas(10, 389, 497, 369);
    cartel->setTheme(canvasTheme);
    cartel->addLabel("INSTRUCTIONS");
    cartel->addSpacer();
    cartel->addSpacer();
    cartel->addLabel("- Placer votre main au desus de la zone du capteur");
    cartel->addLabel("- Un effet est actif");
    cartel->addLabel("- Deplacer votre main de haut en bas pour doser l'effet");
    cartel->addLabel("- Observer l'effet sur la video");
    cartel->addLabel("- Retirer votre main de la zone active puis replacez la");
    cartel->addLabel("pour changer la video");
    
    // Kinect Infos GUI
    
    vector<string> effects;
	effects.push_back("DOTS");
	effects.push_back("RGB");
	effects.push_back("NEON");
	effects.push_back("ASCII");
	effects.push_back("CITY LIGHTS");
	effects.push_back("GLOW");
	effects.push_back("INVERT");
    
    kinectInfos = new ofxUICanvas(517, 10, 497, 369);
    kinectInfos->setTheme(canvasTheme);
    kinectInfos->setName("kinectInfos");
    kinectInfos->addLabel("CAPTEURS");
    kinectInfos->addSpacer();
    kinectInfos->addSpacer();
    kinectInfos->addLabel("DETECTION","DETECTION : " + isDetectionActive);
    kinectInfos->add2DPad("POSITION", ofPoint(0, 497), ofPoint(0,369), &pos, 260, 140);
    kinectInfos->addSlider("DISTANCE", 500, 1100, &dist);
    kinectInfos->addSpacer();
    kinectInfos->addLabel("INTERFERENCES", "INTERFERENCES : Inactif");
	kinectInfos->addRadio("TYPE INTERFERENCE", effects, OFX_UI_ORIENTATION_HORIZONTAL);
    kinectInfos->addSlider("INTENSITE", 0, 255, 40);
    
    // Config Panel GUI
    
    configPanel = new ofxUICanvas(517, 389, 497, 369);
    configPanel->setTheme(canvasTheme);
    configPanel->addLabel("CONFIGURATION");
    configPanel->addSpacer();
    configPanel->addSpacer();
    configPanel->addRangeSlider("ZONE DE DETECTION", 0, 1100, &farThreshold, &nearThreshold);
    configPanel->addSpacer();
    configPanel->addLabel("OSC");
    configPanel->addLabel("ADRESSE IP");
    configPanel->addTextInput("OSC IP", oscHost);
    configPanel->addLabel("PORT");
    configPanel->addTextInput("OSC PORT", ofToString(oscPort));
    
    configPanel->loadSettings("config.xml");

    
}

//--------------------------------------------------------------
void ofApp::update(){
    
    kinect.update();
    
    if(kinect.isFrameNew()) {
		
		// load grayscale depth image from the kinect source
		grayImage.setFromPixels(kinect.getDepthPixels(), kinect.width, kinect.height);
		
		// we do two thresholds - one for the far plane and one for the near plane
		// we then do a cvAnd to get the pixels which are a union of the two thresholds
        grayThreshNear = grayImage;
        grayThreshFar = grayImage;
        grayThreshNear.threshold(nearThreshold, true);
        grayThreshFar.threshold(farThreshold);
        cvAnd(grayThreshNear.getCvImage(), grayThreshFar.getCvImage(), grayImage.getCvImage(), NULL);
    }
    
    // update the cv images
    grayImage.flagImageChanged();
    
    // find contours which are between the size of 20 pixels and 1/3 the w*h pixels.
    // also, find holes is set to true so we will get interior contours as well....
    contourFinder.findContours(grayImage, 50, (kinect.width*kinect.height)/2, 5, false);

    
    if (contourFinder.nBlobs > 0)
    {
        ofBackground(255, 0, 0);
        ofPoint midPoint = contourFinder.blobs[0].centroid;
        dist = kinect.getDistanceAt(midPoint);
        pos = midPoint;
        
    }
    else
    {
        //ofBackground(200, 0, 0);
        ofBackground(50, 50, 50);
        dist = 0;
        //pos = ofPoint(248,184);
        pos = ofPoint(0,0);
    }
    
    sendOsc("/oF/distance", ofMap(dist, 500, 1100, 0, 1));
    sendOsc("/oF/x", ofMap(pos.x, 0, 497, 0, 1));
    sendOsc("/oF/y", ofMap(pos.y, 0, 369, 0, 1));
    sendOsc("/oF/on", contourFinder.nBlobs);
    
    }

//--------------------------------------------------------------
void ofApp::draw(){

    //kinect.drawDepth(10, 10, 492, 364);
    grayImage.draw(10, 10, 497, 369);
    contourFinder.draw(10, 10, 497, 369);
    
}

void ofApp::exit(){

    kinect.close();
    
}

void ofApp::guiEvent(ofxUIEventArgs &e)
{
	string name = e.getName();
	int kind = e.getKind();
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    
    switch (key) {
        case 'f':
            ofToggleFullscreen();
            break;
        case 's':
            configPanel->saveSettings("config.xml");
            break;
        case 'd':
            loadDefaultConfig();
            break;
        default:
            break;
    }
}

//--------------------------------------------------------------
void ofApp::sendOsc(string key, float val){
    ofxOscMessage m;
    m.setAddress(key);
    cout << val << endl;
    m.addFloatArg(val);
    oscSender.sendMessage(m);
}

//--------------------------------------------------------------
void ofApp::loadDefaultConfig(){
    nearThreshold = 400;
	farThreshold = 50;
    isEffectActive = "inactif";
    isDetectionActive = "Aucune presence";
    effectForce = 0;
    dist = 0;
    pos = ofPoint(0,0);
    oscHost = "192.168.5.81";
    oscPort = 3333;
    ofxUITextInput *sel = (ofxUITextInput *)configPanel->getWidget("OSC IP");
    sel->setTextString(oscHost);
    sel = (ofxUITextInput *)configPanel->getWidget("OSC PORT");
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
