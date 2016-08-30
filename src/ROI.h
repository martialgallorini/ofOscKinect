//
//  ROI.h
//  ofOscKinect
//
//  Created by martial on 13/10/2014.
//
//

#include <ofMain.h>

#ifndef ofOscKinect_ROI_h
#define ofOscKinect_ROI_h

#define CAM_WIDTH 640
#define CAM_HEIGHT 480

class ROI : public ofBaseDraws, public ofRectangle {
    
public:
    ROI();
    ~ROI();
    void draw() const;
    void draw(float _x,float _y) const;
    void draw(float _x,float _y,float w, float h) const;
    float getWidth() const;
    float getHeight()const;
    
    ofParameterGroup parameters;
    ofParameter<float> posX;
    ofParameter<float> posY;
    ofParameter<float> width;
    ofParameter<float> height;
        
    void changeRoiX(float &posX);
    void changeRoiY(float &posY);
    void changeRoiWidth(float &width);
    void changeRoiHeight(float &height);
};

#endif
