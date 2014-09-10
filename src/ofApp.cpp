#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){

    ofDisableSmoothing();
    ofDisableAlphaBlending();
    //glEnable(GL_LIGHTING);
    
    frameRate = 30;
    ofSetFrameRate(frameRate);
    ofBackground(0, 0, 250);
    
    canvasWidth = 497;
    canvasHeight = 369;
    
    lastTimeCheck = ofGetElapsedTimeMillis();
    timeOut = 120000; // in millisecond = 2 min
    effectNumber = 0;
    
    kinect.init(true, false, true);
    kinect.setDepthClipping(500, 1200);
    kinect.open();
    kinect.setLed(ofxKinect::LED_OFF);
    
    grayImage.allocate(kinect.width, kinect.height);
    grayThreshNear.allocate(kinect.width, kinect.height);
    grayThreshFar.allocate(kinect.width, kinect.height);
    flippedDepthImg.allocate(kinect.width, kinect.height);
    
    nearThreshold = 400;
	farThreshold = 50;
    nDist = 0;
    dist = 0;
    pos = ofPoint(0,0);
    oscHost = "192.168.4.190";
    oscPort = 3333;
        
    oscSender.setup(oscHost, oscPort);
    
    canvasTheme = OFX_UI_THEME_DEFAULT;
    //canvasTheme = OFX_UI_THEME_MINYELLOW;
    //canvasTheme = OFX_UI_THEME_MINBLACK;
    //canvasTheme = OFX_UI_THEME_HIPSTER;
    //canvasTheme = OFX_UI_THEME_HACKER;
    
    
    // cartel GUI
    
    cartel = new ofxUICanvas(10, 389, canvasWidth, canvasHeight);
    cartel->setTheme(canvasTheme);
    cartel->setWidgetSpacing(10);
    cartel->addLabel("INSTRUCTIONS");
    cartel->addSpacer();
    cartel->addSpacer();
    cartel->addLabel("- Placer votre main au desus de la zone du capteur");
    cartel->addLabel("- Un effet est actif");
    cartel->addLabel("- Deplacer votre main de haut en bas pour doser l'effet");
    cartel->addLabel("- Observer l'effet sur la video");
    cartel->addLabel("- Retirer votre main de la zone active puis replacez la");
    cartel->addLabel("  pour changer la video");
    cartel->addLabel("- Le type d'effet change toutes les 2 minutes");
    
    // Kinect Infos GUI
    
    kinectInfos = new ofxUICanvas(517, 10, canvasWidth, canvasHeight);
    kinectInfos->setTheme(canvasTheme);
    kinectInfos->setWidgetSpacing(10);
    kinectInfos->setName("kinectInfos");
    kinectInfos->addLabel("CAPTEUR");
    kinectInfos->addSpacer();
    kinectInfos->addSpacer();
    kinectInfos->addLabel("DETECTION","ETAT : Inactif");
    kinectInfos->add2DPad("POSITION", ofPoint(0, canvasWidth), ofPoint(0, canvasHeight), &pos, 480, 210);
    kinectInfos->addSlider("DISTANCE", 500, 1100, &dist);

    // Effects panel
    
	effects.push_back("DOTS");
	effects.push_back("RGB");
	effects.push_back("NEON");
	effects.push_back("ASCII");
	effects.push_back("CITY LIGHTS");
	effects.push_back("GLOW");
	effects.push_back("INVERT");
    
    effectsPanel = new ofxUICanvas(517, 389, canvasWidth, canvasHeight);
    effectsPanel->setTheme(canvasTheme);
    effectsPanel->setWidgetSpacing(10);
    effectsPanel->addLabel("INTERFERENCES");
    effectsPanel->addSpacer();
    effectsPanel->addSpacer();
    effectsPanel->addLabel("FX STATE", "ETAT : Inactif");
	effectsRadio = effectsPanel->addRadio("TYPE INTERFERENCE", effects, OFX_UI_ORIENTATION_VERTICAL);
    effectsRadio->activateToggle(effects.at(effectNumber));
    effectsPanel->addSlider("INTENSITE", 0, 1, nDist);
    effectsPanel->addSlider("VAR 1", 0, 1, pos.x);
    effectsPanel->addSlider("VAR 2", 0, 1, pos.y);

    
    // Config Panel GUI
    
    
    configPanel = new ofxUICanvas(517, 389, canvasWidth, canvasHeight);
    //configPanel->setTheme(OFX_UI_THEME_MAROON);
    configPanel->setVisible(false);
    configPanel->setWidgetSpacing(10);
    configPanel->addLabel("CONFIGURATION");
    configPanel->addSpacer();
    configPanel->addSpacer();
    configPanel->addRangeSlider("ZONE DE DETECTION", 0, 1100, &farThreshold, &nearThreshold);
    configPanel->addSpacer();
    configPanel->addLabel("Adresse IP OSC");
    configPanel->addWidgetRight(new ofxUITextInput("OSC IP", oscHost, 100));
    configPanel->addLabel("Port OSC");
    configPanel->addWidgetRight(new ofxUITextInput("PORT", ofToString(oscPort), 100));
    //configPanel->setWidgetSpacing(2);
    configPanel->setWidgetFontSize(OFX_UI_FONT_SMALL);
    configPanel->addLabel("distance de l'objet (float) : /vidMap/kinect/distance");
    configPanel->addLabel("position x de l'objet (float) : /vidMap/kinect/x");
    configPanel->addLabel("position y de l'objet (float) : /vidMap/kinect/y");
    configPanel->addLabel("effet actif (float) : /vidMap/fx/on");
    configPanel->addLabel("numero de l'effet actif (float) : /vidMap/fx/active");
    configPanel->addSpacer(20);
    configPanel->addWidgetDown(new ofxUILabelButton("SAVE", false, 150));
    configPanel->addWidgetRight(new ofxUILabelButton("LOAD", false, 150));
    configPanel->addWidgetRight(new ofxUILabelButton("LOAD DEFAULTS", false, 150));
    
    // Load settings from previously saved setup from XML file
    configPanel->loadSettings("config.xml");
}

//--------------------------------------------------------------
void ofApp::update(){
    
    kinect.update();
    
    if(kinect.isFrameNew())
    {
		// load grayscale depth image from the kinect source
		grayImage.setFromPixels(kinect.getDepthPixels(), kinect.width, kinect.height);
		    
        //grayImage.mirror(false, true);
        //flippedDepthImg = grayImage;
        
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
    
    // find contours which are between the size of 50 pixels and 1/2 the w*h pixels.
    // also, find holes is set to true so we will get interior contours as well....
    contourFinder.findContours(grayImage, 50, (kinect.width*kinect.height)/2, 5, false);
    
    if (contourFinder.nBlobs > 0)
    {
        // TEST - solution 1 pour trouver le centre de la forme dŽtectŽe
        //ofRectangle bRect = contourFinder.blobs.at(0).boundingRect;
        //pos.x = bRect.x + (bRect.width / 2);
        //pos.y = bRect.y + (bRect.height / 2);
        // END TEST
        
        // TEST - solution 2 pour trouver le centre de la forme dŽtectŽe
        pos = contourFinder.blobs.at(0).centroid;
        // END TEST
        
        dist = kinect.getDistanceAt(pos);

        // update detected and effect state
        ofxUILabel *sel = (ofxUILabel *)kinectInfos->getWidget("DETECTION");
        sel->setLabel("ETAT : Presence active !");
        sel = (ofxUILabel *)effectsPanel->getWidget("FX STATE");
        sel->setLabel("ETAT : Actif !");
    }
    else
    {
        dist = 0;
        pos = ofPoint(0,0);
        
        // update detected and effect state
        ofxUILabel *sel = (ofxUILabel *)kinectInfos->getWidget("DETECTION");
        sel->setLabel("ETAT : Inactif");
        sel = (ofxUILabel *)effectsPanel->getWidget("FX STATE");
        sel->setLabel("ETAT : Inactif");
    }
    
    // update effect intensity slider value
    ofxUISlider* slider = (ofxUISlider *)effectsPanel->getWidget("INTENSITE");
    slider->setValue(ofMap(dist, 1100, 500, 1, 0));
    
    // update effect variable 1 slider value with kinect position x
    slider = (ofxUISlider *)effectsPanel->getWidget("VAR 1");
    slider->setValue(ofMap(pos.x, 0, canvasWidth, 0, 1));
    
    // update effect variable 2 slider value with kinect position y
    slider = (ofxUISlider *)effectsPanel->getWidget("VAR 2");
    slider->setValue(ofMap(pos.y, 0, canvasHeight, 0, 1));
    
    
    if ( ofGetElapsedTimeMillis() - lastTimeCheck > timeOut)
    {
        if (effectNumber < 6)
        {
            effectNumber++;
        }
        else
        {
            effectNumber = 0;
        }
        ofxUIRadio *sel = (ofxUIRadio *)effectsPanel->getWidget("TYPE INTERFERENCE");
        sel->activateToggle(effects.at(effectNumber));
        lastTimeCheck = ofGetElapsedTimeMillis();
    }

    // Send OSC values
    sendOsc("/vidMap/kinect/distance",  nDist);
    sendOsc("/vidMap/kinect/x", ofMap(pos.x, 0, canvasWidth, 0, 1));
    sendOsc("/vidMap/kinect/y", ofMap(pos.y, 0, canvasHeight, 0, 1));
    sendOsc("/vidMap/fx/on", contourFinder.nBlobs);
    sendOsc("/vidMap/fx/active", effectNumber);
    }

//--------------------------------------------------------------
void ofApp::draw()
{
    ofSetColor(255, 255, 255);
    kinect.drawDepth(10, 10, 497, 369);
    //flippedDepthImg.draw(10, 10, canvasWidth, canvasHeight);
    contourFinder.draw(10, 10, canvasWidth, canvasHeight);
    //flippedDepthImg.draw(10, 10, 640, 480);
    //contourFinder.draw(10, 10, 640, 480);
    ofSetColor(255, 0, 0); //stroke color
    ofPoint newPos(ofMap(pos.x,0,kinect.width,0,canvasWidth),ofMap(pos.y,0,kinect.height,0,canvasHeight));
    ofCircle(newPos, 3);
}

void ofApp::exit()
{
    kinect.close();
}

void ofApp::guiEvent(ofxUIEventArgs &e)
{
	string name = e.getName();
	int kind = e.getKind();
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key)
{
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
        case 'c':
            configPanel->toggleVisible();
            effectsPanel->toggleVisible();
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
    cout << val << endl;
    m.addFloatArg(val);
    oscSender.sendMessage(m);
}

//--------------------------------------------------------------
void ofApp::loadDefaultConfig()
{
    nearThreshold = 400;
	farThreshold = 50;
    nDist = 0;
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
