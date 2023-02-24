#pragma once

#include "ofMain.h"
#include "ofxSane.h"

class testApp : public ofBaseApp{
public:
    ~testApp() {
        scanner.stop();
    }

    void setup();
    void update();
    void draw();

    void mousePressed(int x, int y, int button);
    void keyPressed(int key);
    void lineEvent(lineEventArgs& event);

    int width, height;
    ofxSane scanner;
    ofImage screenImage;
    unsigned offset;
    bool saveFrame;
};
