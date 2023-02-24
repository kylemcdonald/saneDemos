#include "ofMain.h"
#include "testApp.h"
#include "ofAppGlutWindow.h"

int main() {
    ofAppGlutWindow window;
	ofSetupOpenGL(&window, 1440, 1050, OF_GAME_MODE);
	ofRunApp(new testApp());
}
