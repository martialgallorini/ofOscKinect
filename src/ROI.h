//
//  ROI.h
//  ofOscKinect
//
//  Created by martial on 13/10/2014.
//
//

#ifndef ofOscKinect_ROI_h
#define ofOscKinect_ROI_h

//#define CAM_WIDTH 640
//#define CAM_HEIGHT 480
#define CAM_WIDTH 497
#define CAM_HEIGHT 369

class ROI : public ofBaseDraws, public ofRectangle {
    
public:
    void draw(float _x,float _y) {
        draw(_x, _y, getWidth(), getHeight());
    }
    void draw(float _x,float _y,float w, float h)  {
        
        ofPushMatrix();
        ofTranslate(_x, _y);
        ofScale(w/CAM_WIDTH, h/CAM_HEIGHT);
        
        ofPushStyle();
        ofSetColor(255, 0, 0);
        ofNoFill();
        ofSetLineWidth(1);
        ofRect(*this);
        ofPopStyle();
        
        ofPopMatrix();
    }
    float getWidth()  {
        return CAM_WIDTH;
    }
    float getHeight()  {
        return CAM_HEIGHT;
    }
};

#endif
