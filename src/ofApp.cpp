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
    parameters.add(cvKinect.roi.parameters);
    gui.setup(parameters);
    gui.add(bVerticalFlip.set("Flip vertical values", false));
    gui.add(bHorizontalFlip.set("Flip horizontal values", false));
    
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
        sendOsc("/kinect/detected", 1);
        if (bHorizontalFlip) {
            sendOsc("/kinect/x", ofMap(cvKinect.pos->x, 0, cvKinect.roi.width, 1, 0));
        }
        else
        {
            sendOsc("/kinect/x", ofMap(cvKinect.pos->x, 0, cvKinect.roi.width, 0, 1));
        }
        
        if (bVerticalFlip) {
            sendOsc("/kinect/y", ofMap(cvKinect.pos->y, 0, cvKinect.roi.height, 1, 0));
        }
        else
        {
            sendOsc("/kinect/y", ofMap(cvKinect.pos->y, 0, cvKinect.roi.height, 0, 1));
        }
    }
    else
    {
        sendOsc("/kinect/detected", 0);
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
        case OF_KEY_UP:
            cvKinect.tiltUp();
            break;
        case OF_KEY_DOWN:
            cvKinect.tiltDown();
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
