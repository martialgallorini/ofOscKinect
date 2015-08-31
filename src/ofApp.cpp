#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    
    ofDisableSmoothing();
    ofDisableAlphaBlending();
    
    ofSetFrameRate(FRAMERATE);
    
    cvKinect.setup();
    
    oscSender.setup("127.0.0.1", 3333);
    
    bSetupMode = false;
    
    parameters.setName("SETTINGS");
    parameters.add(cvKinect.parameters);
    gui.setup(parameters);
    
    gui.loadFromFile("settings.xml");
}

//--------------------------------------------------------------
void ofApp::update()
{
    cvKinect.update();
}

//--------------------------------------------------------------
void ofApp::draw()
{
    cvKinect.draw();
    if (bSetupMode) {
        gui.draw();
    }
    if (cvKinect.getNbBlobs() > 0) {
        sendOsc("/kinect/x", ofMap(cvKinect.pos->x, 0, 640, 0, 1));
        sendOsc("/kinect/y", ofMap(cvKinect.pos->y, 0, 480, 0, 1));
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
