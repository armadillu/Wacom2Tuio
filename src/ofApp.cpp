#include "ofApp.h"


void ofApp::setup(){

	ofSetFrameRate(240);
	ofSetVerticalSync(false);
	ofBackground(0);
	ofHideCursor();

	fingers = new Finger[MAX_TOUCHES];
	for( int i = 0; i < MAX_TOUCHES; i++ ){
		fingers[i].active = false;
		fingers[i].angle  = 0.0f;
	}
	
	wacomTablet.init();
	wacomTablet.listAttachedDevices();
	//wacomTablet.setCoordinateDimensions(1920, 1080); //cintiq HD

	ofxXmlSettings xml;
	xml.load("settings.xml");
	xml.pushTag("settings");
	IP = xml.getValue("IP", "127.0.0.1");
	port = xml.getValue("port", 3333);

	server.setCanvasSize(ofGetWidth(), ofGetHeight());
	server.setVerbose(true);
	server.start((char*)IP.c_str(), port);

	touchScreenDesktopRect.x = -3360;
	touchScreenDesktopRect.y = 980;
	touchScreenDesktopRect.width = 1920;
	touchScreenDesktopRect.height = 1080;

	xml.pushTag("touchScreenDesktopRect");
	touchScreenDesktopRect.x = xml.getValue("x", 0);
	touchScreenDesktopRect.y = xml.getValue("y", 0);
	touchScreenDesktopRect.width = xml.getValue("width", 1920);
	touchScreenDesktopRect.height = xml.getValue("height", 1080);
	xml.popTag();

	debug = xml.getValue("debug", 0);
	ofLogNotice() << "Touch screen desktop units coords: [" << touchScreenDesktopRect << "]";
}


void ofApp::update(){

	server.run();
}


void ofApp::draw(){

	float dimensionScale = 1.0f;

	for( int i = 0; i < MAX_TOUCHES; i++ ){
		if( fingers[i].active ){
			// Scale the width and height up a little bit
			float tmpW = fingers[i].dimensions.x * dimensionScale;
			float tmpH = fingers[i].dimensions.y * dimensionScale;
			ofSetColor(fingers[i].color);
			ofDrawEllipse(fingers[i].pos, tmpW, tmpH );
			ofDrawBitmapString( ofToString(i), fingers[i].pos.x + tmpW * 0.7, fingers[i].pos.y + 5 );
		}
	}
	ofSetColor(255);
	ofDrawBitmapString("Tuio > " + IP + " : "+ofToString(port), 20, 20);
}


void ofApp::keyPressed(int key){
	if(key != OF_KEY_ESC){
		ofToggleFullscreen();
	}
}


void ofApp::keyReleased(int key){

}


void ofApp::mouseMoved(int x, int y ){

}


void ofApp::mouseDragged(int x, int y, int button){

}


void ofApp::mousePressed(int x, int y, int button){

}


void ofApp::mouseReleased(int x, int y, int button){

}


void ofApp::windowResized(int w, int h){
	server.setCanvasSize(w, h);
	wacomTablet.setCoordinateDimensions(w, h);
}


void ofApp::gotMessage(ofMessage msg){

}


void ofApp::dragEvent(ofDragInfo dragInfo){
	
}


//--------------------------------------------------------------
void ofApp::touchDown(ofTouchEventArgs & touch)
{
	if( touch.id < MAX_TOUCHES ){
		float xx = ofMap(touch.x, touchScreenDesktopRect.x, touchScreenDesktopRect.getRight(), 0, ofGetWidth());
		float yy = ofMap(touch.y, touchScreenDesktopRect.y, touchScreenDesktopRect.getBottom(), 0, ofGetHeight());
		if(debug) cout << "Touch DOWN " <<  touch.id << " : [" << touch.x << ", "<<  touch.y << "]~[ " << xx << ", " << yy << "]" <<  endl;
		int i = touch.id;
		fingers[i].active = true;
		fingers[i].angle  = 0.0f;
		fingers[i].color.setHsb(ofRandom(255.0f), 255.0f, 255.0f);
		fingers[i].pos.set(xx, yy);

		if(cursors[i] == NULL){
			cursors[i] = server.addCursor(xx, yy);
		}else{
			ofLogError() << "wtf touch already was down!";
		}
		server.run();
	}
}

//--------------------------------------------------------------
void ofApp::touchMoved(ofTouchEventArgs & touch)
{
	if( touch.id < MAX_TOUCHES ){
		int i = touch.id;
		float xx = ofMap(touch.x, touchScreenDesktopRect.x, touchScreenDesktopRect.getRight(), 0, ofGetWidth());
		float yy = ofMap(touch.y, touchScreenDesktopRect.y, touchScreenDesktopRect.getBottom(), 0, ofGetHeight());
		if(debug) cout << "Touch MOVED " <<  touch.id << " : [" << touch.x << ", "<<  touch.y << "]~[ " << xx << ", " << yy << "]" <<  endl;

		fingers[i].pos.set(xx, yy);
		fingers[i].dimensions.set( 10, 10 );
		fingers[i].angle = touch.angle;

		if(cursors[i] == NULL){
			ofLogError() << "wtf! touch move but was never down!";
		}else{
			server.updateCursor(cursors[i], xx, yy);
		}
		server.run();
	}
}

//--------------------------------------------------------------
void ofApp::touchUp(ofTouchEventArgs & touch)
{
	//cout << "Touch UP " <<  touch.id << endl;
	if( touch.id < MAX_TOUCHES )
	{

		float xx = ofMap(touch.x, touchScreenDesktopRect.x, touchScreenDesktopRect.getRight(), 0, ofGetWidth());
		float yy = ofMap(touch.y, touchScreenDesktopRect.y, touchScreenDesktopRect.getBottom(), 0, ofGetHeight());
		if(debug) cout << "Touch MOVED " <<  touch.id << " : [" << touch.x << ", "<<  touch.y << "]~[ " << xx << ", " << yy << "]" <<  endl;

		int i = touch.id;
		fingers[i].active = false;

		if (cursors[i] != NULL){
			server.removeCursor(cursors[i]);
			//NSLog(@"remove cursor %d (%d)", i, cursors[i]->getCursorID());
			cursors[i] = NULL;
		}
		server.run();
	}
}


