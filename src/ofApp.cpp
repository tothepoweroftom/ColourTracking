#include "ofApp.h"

//--------------------------------------------------------------

//------ - - - - - - - - - -Simple Color Tracker - - - - - - - - - - - - - - - - -?
void ofApp::setup(){
    
    ofSetVerticalSync(true);
    
    kalman.init(1/100000., 1/10.); // inverse of (smoothness, rapidness)
    
    line.setMode(OF_PRIMITIVE_LINE_STRIP);
    predicted.setMode(OF_PRIMITIVE_LINE_STRIP);
    estimated.setMode(OF_PRIMITIVE_LINE_STRIP);
    
    speed = 0.f;
    
    wWidth  = 320;
    wHeight = 240;
    
    hue = 0;
    sat = 0;
    val = 0;
    
    drawEnabled = true;
    
    
    //Video Grabber from webcam
    wCam.initGrabber(wWidth, wHeight);
    
    
    //Allocate memory for outputs
    origOutput.allocate(wWidth, wHeight);
    origOutputHSV.allocate(wWidth, wHeight);
    
    hueOutput.allocate(wWidth, wHeight);
    satOutput.allocate(wWidth, wHeight);
    briOutput.allocate(wWidth, wHeight);
    
    
    //Tracked pixels
    lockedOnPixels = new unsigned char [wWidth * wHeight];
    lockedOnTexture.allocate(wWidth, wHeight, GL_LUMINANCE);
    lockedOutput.allocate(wWidth, wHeight);
    
}

//--------------------------------------------------------------
void ofApp::update(){
    
    

    
    wCam.update();
    if (wCam.isFrameNew()) {
//        origOutput.setFromPixels(wCam.getPixels(), 320, 240);
                string info = "fps: " + ofToString(ofGetFrameRate(),2);
        ofDrawBitmapString(info, 10, ofGetHeight()-40);
        
        origOutput.setFromPixels(wCam.getPixels());
        origOutputHSV = origOutput;
        origOutputHSV.convertRgbToHsv();
        
        origOutputHSV.convertToGrayscalePlanarImages(hueOutput, satOutput, briOutput);
        
        hueOutput.flagImageChanged();
        satOutput.flagImageChanged();
        briOutput.flagImageChanged();
        
        ofPixels huePixels = hueOutput.getPixels();
        ofPixels satPixels = satOutput.getPixels();
        ofPixels briPixels = briOutput.getPixels();
        
        //This is a flat array of the pixel values that we iterate through. Set to white the pixels that are there.
        for (int i = 0; i < (wWidth * wHeight); i++) {
            if ((huePixels[i] >= 9  && huePixels[i] <= 22) &&
                (satPixels[i] >= 130)) {
                lockedOnPixels[i] = 255;
            } else {
                lockedOnPixels[i] = 0;
            }
        }
        
        lockedOnTexture.loadData(lockedOnPixels, wWidth, wHeight, GL_LUMINANCE);
        lockedOutput.setFromPixels(lockedOnPixels, wWidth, wHeight);
        
        lockedContours.findContours(lockedOutput, 160, (wWidth * wHeight) / 3, 1, false, true);

        
        if (lockedContours.blobs.size() > 0) {
            
            ofVec2f curPoint(lockedContours.blobs.at(0).centroid.x, lockedContours.blobs.at(0).centroid.y);
            line.addVertex(curPoint);
            
            
            kalman.update(curPoint); // feed measurement
            
            point = kalman.getPrediction(); // prediction before measurement
            predicted.addVertex(point);
            estimated.addVertex(kalman.getEstimation()); // corrected estimation after measurement
            
            speed = kalman.getVelocity().length();
            int alpha = ofMap(speed, 0, 20, 50, 255, true);
            line.addColor(ofColor(255, 255, 255, alpha));
            predicted.addColor(ofColor(255, 0, 0, alpha));
            estimated.addColor(ofColor(0, 255, 0, alpha));
        }
        
        
    }
}

//--------------------------------------------------------------
void ofApp::draw(){
    ofBackground(100, 100, 100);
    
    ofSetColor(0xffffff);
    
    wCam.draw(0,0);
//    origOutputHSV.draw(360, 0);
    lockedContours.draw();
    
    lockedOnTexture.draw(720, 0);
    
    
    char tmpStr[255];
    sprintf(tmpStr, "h: %i\ns: %i\nv: %i", hue, sat, val);
    ofDrawBitmapString(tmpStr, 20, 260);
    
//    line.draw();
    
    predicted.draw();
    ofPushStyle();
    ofSetColor(ofColor::red, 128);
    ofFill();
    ofDrawCircle(point, speed * 2);
    ofPopStyle();
    
    estimated.draw();

}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){

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
    ofPixels huePixels = hueOutput.getPixels();
    ofPixels satPixels = satOutput.getPixels();
    ofPixels briPixels = briOutput.getPixels();
    
    hue = huePixels[x + (y * hueOutput.width)];
    sat = satPixels[x + (y * satOutput.width)];
    val = briPixels[x + (y * briOutput.width)];
}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){
    
}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){
    
}
