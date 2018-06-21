/*
 TUIO Server Wrapper for OpenFrameworks
 Copyright (c) 2009 Matthias Dörfelt <info@mokafolio.de>
 Based on the TUIO SimpleSimulator by Martin Kaltenbrunner:

 Copyright (c) 2005-2009 Martin Kaltenbrunner <mkalten@iua.upf.edu>

 This program is free software; you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by
 the Free Software Foundation; either version 2 of the License, or
 (at your option) any later version.

 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.

 You should have received a copy of the GNU General Public License
 along with this program; if not, write to the Free Software
 Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */

#include "ofxTuioServer.h"

ofxTuioServer::ofxTuioServer() {
	canvasW = 1024;
	canvasH = 768;
}

void ofxTuioServer::setCanvasSize(float width, float height){
	canvasH = height;
	canvasW = width;
}

//start Server (host, port);
void  ofxTuioServer::start(char * host, int port){
	verbose = false;
//	if ((strcmp(host,"default")==0) && (port==0)) tuioServer = new TuioServer(new UdpSender);
//	else tuioServer = new TuioServer(new UdpSender(host, port));
	tuioServer = new TuioServer(host, port);
	currentTime = TuioTime::getSessionTime();
}
//add cursor, object
TuioCursor * ofxTuioServer::addCursor(float _x, float _y){
	//if(verbose) cout<<"ofxTuioServer: TUIO Cursor added at x: "+ofToString(_x)+" y: "+ofToString(_y)<<endl;
	return tuioServer->addTuioCursor(_x/canvasW, _y/canvasH);
}

TuioObject * ofxTuioServer::addObject(int _sid, float _x, float _y, float _a){
	return tuioServer->addTuioObject(_sid, _x/canvasW, _y/canvasH, _a);
}

//update them
void ofxTuioServer::updateCursor(TuioCursor * _tcur, float _nx, float _ny){
	//if(verbose) cout<<"ofxTuioServer: update TUIO Cursor " << _tcur->getCursorID() << " to x: "+ofToString(_nx)+" y: "+ofToString(_ny)<<endl;
	tuioServer->updateTuioCursor(_tcur, _nx/canvasW, _ny/canvasH);
}

void ofxTuioServer::updateObject(TuioObject * _tobj, float _nx, float _ny, float _ang){
	tuioServer->updateTuioObject(_tobj, _nx/canvasW, _ny/canvasH, _ang);
}

//remove them
void ofxTuioServer::removeCursor(TuioCursor * _tcur){
	tuioServer->removeTuioCursor(_tcur);
}

void ofxTuioServer::removeObject(TuioObject * _tobj){
	tuioServer->removeTuioObject(_tobj);
}

//Getters
TuioTime ofxTuioServer::getCurrentTime(){
	return tuioServer->getFrameTime();
}

void ofxTuioServer::setVerbose(bool _b){
	verbose = _b;
}

//send the OSC messages
void ofxTuioServer::run() {
	//this is weird, it should actually be something like....
//	tuioServer->stopUntouchedMovingCursors();
//	tuioServer->commitFrame();
//	currentTime = TuioTime::getSessionTime();
//	tuioServer->initFrame(currentTime);
	//...this:

	currentTime = TuioTime::getSessionTime();
	tuioServer->initFrame(currentTime);
	tuioServer->stopUntouchedMovingCursors();
	tuioServer->commitFrame();

	//propably due to the order of which OF handles mouse and keyboard events
}
