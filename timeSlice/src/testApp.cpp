#include "testApp.h"

void testApp::setup(){
    ofSetFrameRate(60);
    scanner.setup();
    ofAddListener(scanner.lineEvent, this, &testApp::lineEvent);
    ofBackground(0, 0, 0);
    offset = 0;
    screenImage.allocate(ofGetWidth(), ofGetHeight(), OF_IMAGE_COLOR);
    width = ofGetWidth();
    height = ofGetHeight();
    saveFrame = false;
}

void testApp::update(){
}

void testApp::draw(){
    ofSetColor(255, 255, 255);
    screenImage.update();
    screenImage.draw(0, 0);

    if(saveFrame) {
        ofSaveFrame();
        saveFrame = false;
    }
}

void testApp::mousePressed(int x, int y, int button){
    scanner.scan();
}

void testApp::keyPressed(int key) {
    if(key == ' ')
        scanner.stop();
}

void testApp::lineEvent(lineEventArgs& event) {
    // nn interpolation
    unsigned char* pixels = screenImage.getPixels();
    int cur = offset * width * 3;
    for(int i = 0; i < width; i++) {
        int j = (i * event.size) / (width * 3);
        j *= 3;
        pixels[cur + 0] = event.line[j + 0];
        pixels[cur + 1] = event.line[j + 1];
        pixels[cur + 2] = event.line[j + 2];
        cur += 3;
    }
    unsigned offsetBefore = offset;
    offset = (offset + 1) % height;
    if(offsetBefore > offset)
        saveFrame = true;
}
