#pragma once

#include "ofMain.h"
#include "ofxOsc.h"
#include "ofxSyphon.h"
//#define HOST "192.168.0.106"
#define PORT 8000
#define PORT2 9000

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
		void mouseEntered(int x, int y);
		void mouseExited(int x, int y);
		void windowResized(int w, int h);
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);
    
        ofxOscSender clientSender;
        ofxOscReceiver clientReceiver;
		//ofxOscMessage m;
        vector <ofPoint> vert1;
        vector <ofPoint> vert2;
        int vidTrans;
    
    int a, b, c, d, map, nextMap;
    
//    int startTime;
    int growTime = 200;
//    float interpolate = 0;
    vector <float> interpolate;
    vector <int> startTime;
    int count = 0;
    int countNew = 0;
    int numVert = 0;
    
    ofVideoPlayer myPlayer;
    bool videoPlay = false;
    int lastFrame;
    
    //declare Syphon
    ofTexture syphonTex;
    // ofxSyphonServer mainOutputSyphonServer;
    ofxSyphonServer individualTextureSyphonServer;
    ofxSyphonClient mClient;
    //draw texture inside FBO
    ofFbo syphonFbo;
};
