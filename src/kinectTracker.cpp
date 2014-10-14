//
//  kinectTracker.cpp
//  ofOscKinect
//
//  Created by martial on 10/10/2014.
//
//

#define NEAR_CLIP 500
#define FAR_CLIP 1200

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
    thresholdImage.allocate(kinect.width, kinect.height);

    threshold = 10;
    minBlobSize = 5000.f;
    
    pos = ofVec3f(0,0,0);
    
    roi.x = 0;
    roi.y = 0;
    roi.width = 640;
    roi.height = 480;
    
}

void kinectTracker::update() {

    kinect.update();
    
    if(kinect.isFrameNew())
    {
        // load grayscale depth image from the kinect source
        depthImage.setFromPixels(kinect.getDepthPixels(), kinect.width, kinect.height);
        
        // ------- ROI ----------
        CvRect cvROI = cvRect(roi.x,roi.y,roi.width,roi.height);
        cvSetImageROI(depthImage.getCvImage(),cvROI);
        
        // retrieve average depth
        thresholdImage = depthImage;
        thresholdImage.threshold(threshold);
        
        //find blob in ROI
        contourFinder.findContours(thresholdImage, minBlobSize, roi.width*roi.height, 1, false);
        
        //fillBlobPoly();
        
        // -------- END ROI -----------
        
        // update the cv images
        depthImage.flagImageChanged();
        contourFinder.findContours(depthImage, 100, (kinect.width*kinect.height)/2, 5, false);
    }
    
    if (contourFinder.nBlobs > 0)
    {
        pos = contourFinder.blobs.at(0).centroid;
        pos.z = kinect.getDistanceAt(pos.x, pos.y);
    }
    
}

void kinectTracker::draw() {
    kinect.draw(0, 0, CAM_WIDTH, CAM_HEIGHT);
    roi.draw(0, 0);
}

void kinectTracker::draw(float _x, float _y, float _w, float _h) {
    kinect.draw(_x, _y, _w, _h);
    roi.draw(_x, _y, _w, _h);
}

void kinectTracker::drawContour(float _x, float _y, float _w, float _h) {
    contourFinder.draw(_x, _y, _w, _h);
}

void kinectTracker::drawDepth(float _x, float _y, float _w, float _h) {
    kinect.drawDepth(_x, _y, _w, _h);
    roi.draw(0, 0);
}
