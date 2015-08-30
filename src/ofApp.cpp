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
void ofApp::update(){
    
    cvKinect.update();
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
