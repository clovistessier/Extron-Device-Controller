#pragma once

#include "ofMain.h"
#include "ofxGui.h"
#include "ofxMidi.h"
#include "ofxOsc.h"
#include "Extron.h"
#include "USP405.h"

//listening port
#define PORT 7001

#define PANEL_W 200
#define PANEL_MARGIN 10

class ofApp : public ofBaseApp, public ofxMidiListener
{
public:
	void setup();
	void update();
	void draw();
	void exit();

	void keyPressed(int key);
	void keyReleased(int key);
	void mouseMoved(int x, int y);
	void mouseDragged(int x, int y, int button);
	void mousePressed(int x, int y, int button);
	void mouseReleased(int x, int y, int button);
	void mouseEntered(int x, int y);
	void mouseExited(int x, int y);
	void windowResized(int w, int h);
	void dragEvent(ofDragInfo dragInfo);
	void gotMessage(ofMessage msg);

	void newMidiMessage(ofxMidiMessage &eventArgs);

	ofxMidiIn midiIn;
	std::queue<ofxMidiMessage> midiMessages;
	std::size_t maxMidiMessages = 10;

	ofxOscReceiver receiver;

	ExtronDevice dsc_301;
    USP_405 usp_405;

	// ExtronDevice USP_405;

	ofxPanel dsc_gui;
    ofxPanel usp_gui;
};
