#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    
    ofDisableSmoothing();
    ofDisableAlphaBlending();
    
    ofSetFrameRate(FRAMERATE);
    
    cvKinect.setup();
    
    oscSender.setup("127.0.0.1", 9001);
    
    bSetupMode = false;
    
    parameters.setName("SETTINGS");
    parameters.add(cvKinect.parameters);
    parameters.add(cvKinect.roi.parameters);
    gui.setup(parameters);
    gui.add(bVerticalFlip.set("Flip vertical values", false));
    gui.add(bHorizontalFlip.set("Flip horizontal values", false));
    
    gui.loadFromFile("settings.xml");
    
    cvKinect.bBlobs.addListener(this, &ofApp::onBlobsDetected);
    
    bHelp = false;
}

//--------------------------------------------------------------
void ofApp::onBlobsDetected(bool &val) {
    if (val) {
        sendOsc("/kinect/detected", 1);
    }
    else {
        sendOsc("/kinect/detected", 0);
    }
}

//--------------------------------------------------------------
void ofApp::update()
{
    cvKinect.update();
    
    if (cvKinect.bBlobs) {
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
        sendOsc("/kinect/z", ofMap(cvKinect.pos->z, cvKinect.pos.getMin().z, cvKinect.pos.getMax().z, 0, 1));
    }
}

//--------------------------------------------------------------
void ofApp::draw()
{
    cvKinect.draw();
    if (bSetupMode) {
        gui.draw();
    }
    
    if (bHelp) {
        ofDrawBitmapString("setup : TAB", 300, 20);
        ofDrawBitmapString("fulscreen : 'f'", 300, 30);
        ofDrawBitmapString("Kinect tilt up : key up", 300, 40);
        ofDrawBitmapString("Kinect tilt down : key down", 300, 50);
        ofDrawBitmapString("OSC sender address : localhost", 300, 60);
        ofDrawBitmapString("OSC out port : 9001", 300, 70);
        ofDrawBitmapString("Kinect x position : /kinect/x", 300, 80);
        ofDrawBitmapString("Kinect y position : /kinect/y", 300, 90);
        ofDrawBitmapString("Kinect z position : /kinect/z", 300, 100);
        ofDrawBitmapString("Kinect blob detected : /kinect/detected", 300, 110);
        
    }
}

void ofApp::exit()
{
    cvKinect.bBlobs.removeListener(this, &ofApp::onBlobsDetected);
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key)
{
    switch (key) {
        case 'f':
            ofToggleFullscreen();
            break;
        case 'h':
            bHelp = !bHelp;
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
