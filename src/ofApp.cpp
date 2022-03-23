#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup()
{
    ofSetFrameRate(60);

    dsc_301.setup();
    usp_405.setup();

    // print input ports to console
    midiIn.listInPorts();

    // open port by number (you may need to change this)
    midiIn.openPort(1);
    //midiIn.openPort("IAC Pure Data In");	// by name
    //midiIn.openVirtualPort("ofxMidiIn Input"); // open a virtual port

    // don't ignore sysex, timing, & active sense messages,
    // these are ignored by default
    midiIn.ignoreTypes(false, false, false);

    // add ofApp as a listener
    midiIn.addListener(this);

    // print received messages to the console
    midiIn.setVerbose(true);

    // Listen on the given port
    ofLog() << "listening for osc messages on port " << PORT;
    receiver.setup(PORT);


    dsc_gui.setup();
    dsc_301.parameters.setName("DSC 301 HD");
    dsc_gui.add(dsc_301.parameters);
    dsc_gui.setPosition(PANEL_MARGIN, PANEL_MARGIN);
    
    usp_gui.setup();
    usp_gui.add(usp_405.parameters);
    usp_gui.setPosition(PANEL_MARGIN + PANEL_W + PANEL_MARGIN, PANEL_MARGIN);
}

//--------------------------------------------------------------
void ofApp::update()
{
    ExtronDevice* pdev;
    
    while (!midiMessages.empty())
    {
        ofxMidiMessage &message = midiMessages.front();
        
        switch (message.channel){
            case 1: pdev = &dsc_301;
                break;
            case 2: pdev = &usp_405;
                break;
            default: pdev = nullptr;
                break;
        }
        
        if (message.status == MIDI_CONTROL_CHANGE)
        {
            if (pdev != nullptr){
                if( pdev->isMidiCC(message.control) == true)
                {
                    pdev->updateFromMidi(message.control, message.value);
                }
            }
        }
        midiMessages.pop();
    }

    // Check for waiting messages
    while (receiver.hasWaitingMessages())
    {
        // get the next message
        ofxOscMessage m;
        receiver.getNextMessage(m);

        if (dsc_301.isOscAddress(m.getAddress()) == true)
        {
            dsc_301.updateFromOsc(m.getAddress(), m.getArgAsFloat(0));
        }
        
        if(usp_405.isOscAddress(m.getAddress()) == true)
        {
            usp_405.updateFromOsc(m.getAddress(), m.getArgAsFloat(0));
        }
    }

    dsc_301.update();
    usp_405.update();
}

    

//--------------------------------------------------------------
void ofApp::draw()
{
    dsc_gui.draw();
    usp_gui.draw();
}

//--------------------------------------------------------------
void ofApp::exit()
{
    // clean up
    midiIn.closePort();
    midiIn.removeListener(this);
}
//--------------------------------------------------------------
void ofApp::keyPressed(int key)
{
    Direction d;
    switch (key)
    {
        case OF_KEY_UP: d = UP; break;
        case OF_KEY_DOWN: d = DOWN; break;
        case OF_KEY_LEFT: d = LEFT; break;
        case OF_KEY_RIGHT: d = RIGHT; break;
    }
    if (d) {usp_405.adjustCentering(d);}
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key)
{
}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y)
{
}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button)
{
}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button)
{
}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button)
{
}

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y)
{
}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y)
{
}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h)
{
}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg)
{
}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo)
{
}

//--------------------------------------------------------------
void ofApp::newMidiMessage(ofxMidiMessage &msg)
{
    // Add the latest message to the message queue
    midiMessages.push(msg);

    // Remove any old messages if we have too many
    while (midiMessages.size() > maxMidiMessages)
    {
        midiMessages.pop();
    }
}
