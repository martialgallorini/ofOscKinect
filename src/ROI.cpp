//
//  ROI.cpp
//  ofOscKinect
//
//  Created by Martial on 01/09/2015.
//
//

#include "ROI.h"

ROI::ROI() {
    
    posX.addListener(this, &ROI::changeRoiX);
    posY.addListener(this, &ROI::changeRoiY);
    width.addListener(this, &ROI::changeRoiWidth);
    height.addListener(this, &ROI::changeRoiHeight);
    
    parameters.setName("ROI Settings");
    parameters.add(posX.set("position x", 0, 0, CAM_WIDTH));
    parameters.add(posY.set("position y", 0, 0, CAM_HEIGHT));
    parameters.add(width.set("width", 0, 0, CAM_WIDTH));
    parameters.add(height.set("height", 0, 0, CAM_HEIGHT));
    
}

ROI::~ROI() {
    posX.removeListener(this, &ROI::changeRoiX);
    posY.removeListener(this, &ROI::changeRoiY);
    width.removeListener(this, &ROI::changeRoiWidth);
    height.removeListener(this, &ROI::changeRoiHeight);
}


float ROI::getWidth()  {
    return CAM_WIDTH;
}

float ROI::getHeight()  {
    return CAM_HEIGHT;
}

void ROI::draw() {
    
    ofPushMatrix();
    
    ofPushStyle();
    ofSetColor(255, 0, 0);
    ofNoFill();
    ofSetLineWidth(2);
    ofRect(*this);
    ofPopStyle();
    
    ofPopMatrix();
}

void ROI::draw(float _x,float _y) {
    draw(_x, _y, CAM_WIDTH, CAM_HEIGHT);
}

void ROI::draw(float _x,float _y,float w, float h)  {
    
    ofPushMatrix();
    
    ofTranslate(_x, _y);
    ofScale(w/CAM_WIDTH, h/CAM_HEIGHT);
    
    ofPushStyle();
    ofSetColor(255, 0, 0);
    ofNoFill();
    ofSetLineWidth(2);
    ofRect(*this);
    ofPopStyle();
    
    ofPopMatrix();
}

void ROI::changeRoiX(float &posX) {
    this->setX(posX);
}

void ROI::changeRoiY(float &posY) {
    this->setY(posY);
}

void ROI::changeRoiWidth(float &width) {
    this->setWidth(width);

}

void ROI::changeRoiHeight(float &height) {
    this->setHeight(height);
}
