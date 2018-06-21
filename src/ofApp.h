#pragma once

#include "ofMain.h"
#include "ofxWacomFeelMultiTouch.h"
#include "ofxTuioServer.h"
#include "ofxXmlSettings.h"

#define MAX_TOUCHES					1024

class Finger{

public:
	ofVec2f pos;
	ofVec2f dimensions;
	float	angle;
	ofColor	color;
	bool 	active;
};


class ofApp : public ofBaseApp{

public:
	void setup();
	void update();
	void draw();

	void keyPressed(int key);
	void keyReleased(int key);
	void mouseMoved(int x, int y );
	void mouseDragged(int x, int y, int button);
	void mousePressed(int x, int y, int button);
	void mouseReleased(int x, int y, int button);
	void windowResized(int w, int h);
	void dragEvent(ofDragInfo dragInfo);
	void gotMessage(ofMessage msg);

	// WACOM /////////////////////////////////////////////
	void touchDown(ofTouchEventArgs & touch);
	void touchMoved(ofTouchEventArgs & touch);
	void touchUp(ofTouchEventArgs & touch);



	ofxWacomFeelMultiTouch	wacomTablet;

	Finger*					fingers;

	ofxTuioServer			server;
	TuioCursor*				cursors[MAX_TOUCHES];

	string IP;
	int port;

	ofRectangle touchScreenDesktopRect;
	bool debug = false;
};
