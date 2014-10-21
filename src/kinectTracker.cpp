//
//  kinectTracker.cpp
//  ofOscKinect
//
//  Created by martial on 10/10/2014.
//
//


#include "kinectTracker.h"

kinectTracker::kinectTracker() {
    
}

kinectTracker::~kinectTracker() {
    kinect.close();
}

void kinectTracker::setup() {
    
    kinect.init(true, true, true); // set first value to true to show IR image of false for RGB
    kinect.setDepthClipping(NEAR_CLIP, FAR_CLIP);
    kinect.open();
    kinect.setLed(ofxKinect::LED_OFF);
    
    depthImage.allocate(kinect.width, kinect.height);
//    threshFar.allocate(kinect.width, kinect.height);
//    threshNear.allocate(kinect.width, kinect.height);
    thresholdImage.allocate(kinect.width, kinect.height);
    
    //threshold = 10;
    threshold = 10;
//    nearThreshValue = 10;
//    farThreshValue = 1000;

    minBlobSize = 5000.f;
    bDilate = false;
    bErode = false;
    
    pos = ofVec3f(0);

    nbPass = 1;
    
    roi.x = 0;
    roi.y = 0;
    roi.width = CAM_WIDTH;
    roi.height = CAM_HEIGHT;
}

void kinectTracker::update() {
    
    kinect.update();
    
    if(kinect.isFrameNew())
    {
        
        
        
        //////////////////////////////////////////
        // TODO : threshNear, threshFar, mirror //
        //////////////////////////////////////////
        
        
        
        // load grayscale depth image from the kinect source
        depthImage.setFromPixels(kinect.getDepthPixels(), kinect.width, kinect.height);
//      depthImage.mirror(false, true);
        
        // ---------- ROI -----------
        ofRectangle roiMat = ofRectangle(roi.x, roi.y, roi.width, roi.height);
        depthImage.setROI(roiMat);
        thresholdImage.setROI(roiMat);
//        threshNear.setROI(roiMat);
//        threshFar.setROI(roiMat);
        
        // threshold
//        threshNear = depthImage;
//        threshFar = depthImage;
//        threshNear.threshold(nearThreshValue);
//        threshFar.threshold(farThreshValue, true);
        thresholdImage = depthImage;
        thresholdImage.threshold(threshold);
        
        // combine thresholded images
        //cvAnd(threshNear.getCvImage(), threshFar.getCvImage(), depthImage.getCvImage(), NULL);
        
        // Optimize blob options
        if (bDilate){
            for(int i = 0; i < nbPass; i++){
                depthImage.dilate();
            }
        }
        
        if (bErode) {
            for(int i = 0; i < nbPass; i++){
                depthImage.erode();
            }
        }
        
        // update the cv images
        depthImage.flagImageChanged();
        
        //find blob in ROI
        contourFinder.findContours(depthImage, minBlobSize, roi.width*roi.height, 1, false);
    }
    
    if (contourFinder.nBlobs > 0 && contourFinder.blobs[0].area > minBlobSize)
    {
        pos = contourFinder.blobs.at(0).centroid;
        pos.z = kinect.getDistanceAt(pos.x, pos.y);
    }
    else
    {
        pos = ofVec3f(0);
    }

}

void kinectTracker::draw() {
    
}

void kinectTracker::draw(float _x, float _y, float _w, float _h) {
    ofPushMatrix();
    ofTranslate(_x, _y);
    ofScale(_w/CAM_WIDTH, _h/CAM_HEIGHT);
    depthImage.draw(0, 0);
    roi.draw(0, 0);
    
    if(contourFinder.nBlobs > 0 && contourFinder.blobs[0].area > minBlobSize) {
        ofTranslate(roi.x, roi.y);
        contourFinder.draw(0,0);
        
        // Draw blob centroid
        ofPushStyle();
        ofSetColor(0, 255, 0);
        ofCircle(pos.x, pos.y, 3);
        ofPopStyle();
    }
    
    ofPopMatrix();
}

void kinectTracker::drawDepth(float _x, float _y, float _w, float _h) {
    ofPushMatrix();
    ofTranslate(_x, _y);
    ofScale(_w/CAM_WIDTH, _h/CAM_HEIGHT);
    kinect.drawDepth(0, 0);
    //roi.draw(0, 0);
    if(contourFinder.nBlobs > 0 && contourFinder.blobs[0].area > minBlobSize) {
        contourFinder.draw(roi.x, roi.y);
    }
    ofPopMatrix();
}
