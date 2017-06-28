#include "ofApp.h"

//--------------------------------------------------------------

//------ - - - - - - - - - -Simple Color Tracker - - - - - - - - - - - - - - - - -?
void ofApp::setup(){
    
    ofSetVerticalSync(true);
    
    ///GUI SETUP
    
    gui.setup();
    
    gui.add(lowHue.setup("lowHue", 32, 0, 360));
    gui.add(highHue.setup("highHue", 82, 0, 360));
    gui.add(lowSat.setup("lowSat", 100, 0, 360));
    gui.add(highSat.setup("highSat", 20, 0, 360));
    gui.add(rapidness.setup("Rapidness", 0.1, 0.000001, 1.0));
    gui.add(smoothness.setup("smoothness", 0.1, 0.000001, 1.0));


    kalman.init(1/10000., 1/100.); // inverse of (smoothness, rapidness)
    
    line.setMode(OF_PRIMITIVE_LINE_STRIP);
    predicted.setMode(OF_PRIMITIVE_LINE_STRIP);
    estimated.setMode(OF_PRIMITIVE_LINE_STRIP);
    
    speed = 0.f;
    myPlayer.load("/Users/tom.power/Documents/of_v0.9.8_osx_release/addons/ofxCv/ColourTracking/src/foosballsim.mov");
    
    
    wWidth  = myPlayer.getWidth();
    wHeight = myPlayer.getHeight();
    ofLog(OF_LOG_NOTICE, "Size w: " + ofToString(wWidth) + " h: " + ofToString(wHeight));

    hue = 0;
    sat = 0;
    val = 0;
    
    drawEnabled = true;
    
    
    //Video Grabber from webcam
//    wCam.initGrabber(wWidth, wHeight);


    myPlayer.play();
    
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
    
    

    
    myPlayer.update(); // get all the new frames
    if (myPlayer.isFrameNew()) {
        
        origOutput.setFromPixels(myPlayer.getPixels());
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
            if ((huePixels[i] >= lowHue  && huePixels[i] <= highHue) &&
                (satPixels[i] >= lowSat)) {
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
            estimated.addColor(ofColor(0, 255, 255, alpha));
        }
        
        
    }
}

//--------------------------------------------------------------
void ofApp::draw(){
    ofBackground(100, 100, 100);
//    ofSetColor(0xffffff);
    
    myPlayer.draw(0,0);
//    origOutputHSV.draw(360, 0);
    lockedContours.draw();
    
    lockedOnTexture.draw(200, 380);
    
    
    char tmpStr[255];
    sprintf(tmpStr, "h: %i\ns: %i\nv: %i", hue, sat, val);
    ofDrawBitmapString(tmpStr, 20, 260);
    
//    line.draw();
    
    predicted.draw();
    ofPushStyle();
//    ofSetColor(ofColor::red, 128);
    ofFill();
    ofDrawCircle(point, speed * 2);
    ofPopStyle();
    
    estimated.draw();
    gui.draw();

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
