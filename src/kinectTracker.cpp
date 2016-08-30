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
    
#ifdef KINECT
    kinect.init(false, true, true); // set first value to true to show IR image or false for RGB
    //kinect.setDepthClipping(NEAR_CLIP, FAR_CLIP);
    kinect.open();
        
    kinect.setLed(ofxKinect::LED_OFF);
    
    depthImage.allocate(kinect.width, kinect.height);
    nearThresholdImage.allocate(kinect.width, kinect.height);
    farThresholdImage.allocate(kinect.width, kinect.height);
#else
    video.load("test_nb.mov");
    video.play();
    
    depthImage.allocate(video.getWidth(), video.getHeight());
    nearThresholdImage.allocate(video.getWidth(), video.getHeight());
    farThresholdImage.allocate(video.getWidth(), video.getHeight());

#endif
    
    // SETUP PARAMETERS
    
    parameters.setName("Kinect Parameters");
        
    parameters.add(nearThreshValue.set("near threshold", 258, 0, 400));
    parameters.add(farThreshValue.set("far threshold", 165, 0, 400));
    parameters.add(minBlobSize.set("min blob size", 5000, 0, 20000));

    parameters.add(bDilate.set("dilate", false));
    parameters.add(nbDilate.set("nb dilate pass", 1, 1, 50));
    parameters.add(bErode.set("erode", false));
    parameters.add(nbErode.set("nb erode pass", 1, 1, 50));
    
    parameters.add(pos.set("Blob position", ofVec3f(0,0,0), ofVec3f(0,0,0), ofVec3f(ofGetWidth(), ofGetHeight(), 200)));
    parameters.add(bKinectRgb.set("Kinect RGB Image", false));

    roi.x = 0;
    roi.y = 0;
    
#ifdef KINECT
    roi.width = kinect.width;
    roi.height = kinect.height;
#else
    roi.width = video.getWidth();
    roi.height = video.getHeight();
#endif
}

void kinectTracker::update() {

#ifdef KINECT
    kinect.update();
    
    if(kinect.isFrameNew())
    {
        // load grayscale depth image from the kinect source
        depthImage.setFromPixels(kinect.getDepthPixels());
        
//        // problem : unconsistent distance detection when flipping image
//        //depthImage.mirror(false, true);
//        
//        // Region of Interest
//        ofRectangle roiMat = ofRectangle(roi.x, roi.y, roi.width, roi.height);
//        depthImage.setROI(roiMat);
//        nearThresholdImage.setROI(roiMat);
//        farThresholdImage.setROI(roiMat);
//        
//        // Threshold
//        nearThresholdImage = depthImage;
//        farThresholdImage = depthImage;
//        nearThresholdImage.threshold(nearThreshValue, true);
//        farThresholdImage.threshold(farThreshValue);
//
//        // Combine thresholded images
//        cvAnd(nearThresholdImage.getCvImage(), farThresholdImage.getCvImage(), depthImage.getCvImage(), NULL);
//        
//        // Optimize blob filters
//        if (bDilate){
//            for(int i = 0; i < nbDilate; i++){
//                depthImage.dilate();
//            }
//        }
//        
//        if (bErode) {
//            for(int i = 0; i < nbErode; i++){
//                depthImage.erode();
//            }
        }
        
        // update the cv images
        depthImage.flagImageChanged();
        
        //find blob in ROI
        contourFinder.findContours(depthImage, minBlobSize, 640*480, 2, false);
        
        depthImage.resetROI();
        nearThresholdImage.resetROI();
        farThresholdImage.resetROI();

    
#else
    video.update();
    
    if(video.isFrameNew())
    {
        // load grayscale depth image from the kinect source
        colorImage.setFromPixels(video.getPixels());
        depthImage = colorImage;
        // problem : unconsistent distance detection when flipping image
        //depthImage.mirror(false, true);
        
        // Region of Interest
        ofRectangle roiMat = ofRectangle(roi.x, roi.y, roi.width, roi.height);
        depthImage.setROI(roiMat);
        nearThresholdImage.setROI(roiMat);
        farThresholdImage.setROI(roiMat);
        
        // Threshold
        nearThresholdImage = depthImage;
        farThresholdImage = depthImage;
        nearThresholdImage.threshold(nearThreshValue, true);
        farThresholdImage.threshold(farThreshValue);
        
        // Combine thresholded images
        cvAnd(nearThresholdImage.getCvImage(), farThresholdImage.getCvImage(), depthImage.getCvImage(), NULL);
        
        // Optimize blob filters
        if (bDilate){
            for(int i = 0; i < nbDilate; i++){
                depthImage.dilate();
            }
        }
        
        if (bErode) {
            for(int i = 0; i < nbErode; i++){
                depthImage.erode();
            }
        }
        
        // update the cv images
        depthImage.flagImageChanged();
        
        //find blob in ROI
        contourFinder.findContours(depthImage, minBlobSize, 640*480, 2, false);
        
        depthImage.resetROI();
        nearThresholdImage.resetROI();
        farThresholdImage.resetROI();
        
    }
#endif

    
    if (contourFinder.nBlobs > 0 && contourFinder.blobs[0].area > minBlobSize)
    {
        float maxBlobHeight = 150;
        if (contourFinder.blobs.at(0).boundingRect.getHeight() > maxBlobHeight) {
            contourFinder.blobs.at(0).boundingRect.setHeight(maxBlobHeight);
            contourFinder.blobs.at(0).centroid.y = contourFinder.blobs[0].boundingRect.getTop() + maxBlobHeight / 2;
        }
        
        // get new centroid
        ofVec3f newPos = contourFinder.blobs.at(0).centroid;
        
        // smoothing centroid position
        pos = pos.get() * 0.7 + newPos * 0.3;
        
        // blobs will be shifted by the ROI offset, so if the ROI starts at x = 100, then a blob that normally is at position x = 150
        // is now at position x = 50, so we need to add roi.x to blob x position to see it in its right place again.
        ofVec3f v = pos.get();
        
#ifdef KINECT
        v.z = kinect.getDistanceAt(pos.get().x + roi.x, pos.get().y + roi.y);
#endif

        pos.set(v);
    }
    else
    {
        pos = ofVec3f(ofGetWidth() / 2, ofGetHeight() / 2, 0);
    }
}

int kinectTracker::getNbBlobs() {
    return contourFinder.nBlobs;
}

void kinectTracker::draw() {
    //ofPushMatrix();
    if (bKinectRgb) {
        kinect.draw(0, 0);
    }
    else {
        depthImage.draw(0, 0);
    }
    
    roi.draw();
    
    if(contourFinder.nBlobs > 0 && contourFinder.blobs[0].area > minBlobSize) {
        ofPushMatrix();
        ofTranslate(roi.x, roi.y);
        contourFinder.draw(0,0);
        
        // Draw blob centroid
        ofPushStyle();
        ofSetColor(0, 255, 0);
        ofDrawCircle(pos.get().x, pos.get().y, 3);
        ofPopStyle();
        ofPopMatrix();
    }
    
    //ofPopMatrix();
}

void kinectTracker::draw(float _x, float _y, float _w, float _h) {
    ofPushMatrix();
    ofTranslate(_x, _y);
    ofScale(_w/CAM_WIDTH, _h/CAM_HEIGHT);
    depthImage.draw(0, 0);
    //roi.draw(0, 0);
    roi.draw();
    
    if(contourFinder.nBlobs > 0 && contourFinder.blobs[0].area > minBlobSize) {
        ofTranslate(roi.x, roi.y);
        contourFinder.draw(0,0);
        
        // Draw blob centroid
        ofPushStyle();
        ofSetColor(0, 255, 0);
        ofDrawCircle(pos.get().x, pos.get().y, 3);
        ofPopStyle();
    }
    
    ofPopMatrix();
}

void kinectTracker::drawDepth() {
    ofPushMatrix();
    kinect.drawDepth(0, 0);
    //roi.draw(0, 0);
    if(contourFinder.nBlobs > 0 && contourFinder.blobs[0].area > minBlobSize) {
        contourFinder.draw(roi.x, roi.y);
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

void kinectTracker::tiltUp() {
    kinect.setCameraTiltAngle(kinect.getCurrentCameraTiltAngle() + 1);
}

void kinectTracker::tiltDown() {
    kinect.setCameraTiltAngle(kinect.getCurrentCameraTiltAngle() - 1);
}

