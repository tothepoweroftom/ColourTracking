#pragma once

#include "ofMain.h"
#include "ofxOpenCv.h"
#include "ofxCv.h"


class ofApp : public ofBaseApp{
    
public:
    void setup();
    void update();
    void draw();
    
    void keyPressed  (int key);
    void keyReleased(int key);
    void mouseMoved(int x, int y );
    void mouseDragged(int x, int y, int button);
    void mousePressed(int x, int y, int button);
    void mouseReleased(int x, int y, int button);
    void windowResized(int w, int h);
    
    int wWidth;
    int wHeight;
    
    int hue, sat, val;
    bool drawEnabled;
    
    ofVideoGrabber		wCam;
    ofxCvColorImage		origOutput;
    ofxCvColorImage		origOutputHSV;
    
    ofxCvGrayscaleImage hueOutput;
    ofxCvGrayscaleImage satOutput;
    ofxCvGrayscaleImage briOutput;
    
    unsigned char *		lockedOnPixels;
    ofTexture			lockedOnTexture;
    
    ofxCvGrayscaleImage	lockedOutput;
    ofxCvContourFinder	lockedContours;
    
    ofxCv::KalmanPosition kalman;
    
    ofMesh predicted, line, estimated;
    ofVec2f point;
    float speed;
    
    
//    unsigned char *		scribblePixels;
//    ofTexture			scribbleOutput;
};
