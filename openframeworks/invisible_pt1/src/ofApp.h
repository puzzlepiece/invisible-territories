#pragma once

#include "ofMain.h"
#include "ofxOpenCv.h"
#include "ofxXmlSettings.h"
//#include "ofxPS3EyeGrabber.h"
//#include "ofxSyphon.h"
#include "ofxOsc.h"
#include "ofxKinect.h"

//#define HOST "192.168.0.106"
#define PORT 8000
#define PORT2 9000


class ofApp : public ofBaseApp{

	public:
		void setup();
		void update();
		void draw();
        void setupCam();
        bool areAllTrue(vector<bool> array);
		void keyPressed(int key);
        void drawCam(ofEventArgs & args);
        void growLine(ofPoint a, ofPoint b);
    void exit();
    
    void drawPointCloud();

    ofxKinect kinect;
    
    ofxCvColorImage colorImg;
    
    ofxCvGrayscaleImage grayImage; // grayscale depth image
    ofxCvGrayscaleImage grayThreshNear; // the near thresholded image
    ofxCvGrayscaleImage grayThreshFar; // the far thresholded image
    
    ofxCvContourFinder contourFinder;
    
    bool bThreshWithOpenCV;
    bool bDrawPointCloud;
    
    int nearThreshold;
    int farThreshold;
    
    int angle;
    
    // used for viewing the point cloud
    ofEasyCam easyCam;
    
    
        ofImage path;
    
        ofxXmlSettings XML;
        int pathIndex = 0;
        int numPath;
        int numVert;
        int tagLevel;
    
        int vertX;
        int vertY;
        int vertX2;
        int vertY2;
    
        int camWidth = 640;
        int camHeight = 480;
        int exposure, gain;
    
        vector<ofPoint> vert;
        vector <bool> visited;
        bool nextMap = false;
        bool allVisited = false;

        ofSoundPlayer  alert;
        ofTrueTypeFont myfont;
        ofFbo fboCam;
        bool touch;

        ofxOscSender serverSender;
        ofxOscReceiver serverReceiver;
        bool endingIsPlay = false;
        bool startPlay = false;
    
    
    bool		bSendSerialMessage;			// a flag for sending serial
    char		bytesRead[3];				// data from serial, we will be trying to read 3
    char		bytesReadString[4];			// a string needs a null terminator, so we need 3 + 1 bytes
    int			nBytesRead;					// how much did we read?
    int			nTimesRead;					// how many times did we read?
    float		readTime;					// when did we last read?
    
    ofSerial	serial;
    int lightOn = 0;
        

};
