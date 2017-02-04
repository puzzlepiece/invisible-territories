#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    serverSender.setup("localhost", PORT);
    serverReceiver.setup( PORT2 );
    myfont.loadFont("Arial.ttf", 50);
    alert.load("alert_3s.wav");
    ofBackground(255);
    ofSetBackgroundAuto(false);
    
    fboCam.allocate(camWidth, camHeight, GL_RGBA);
      /*---------------kinect----------------*/
    ofSetLogLevel(OF_LOG_VERBOSE);
    
    // enable depth->video image calibration
    kinect.setRegistration(true);
    
    kinect.init();
    //kinect.init(true); // shows infrared instead of RGB video image
    //kinect.init(false, false); // disable video image (faster fps)
    
    kinect.open();		// opens first available kinect
    //kinect.open(1);	// open a kinect by id, starting with 0 (sorted by serial # lexicographically))
    //kinect.open("A00362A08602047A");	// open a kinect using it's unique serial #
    
    // print the intrinsic IR sensor values
    if(kinect.isConnected()) {
        ofLogNotice() << "sensor-emitter dist: " << kinect.getSensorEmitterDistance() << "cm";
        ofLogNotice() << "sensor-camera dist:  " << kinect.getSensorCameraDistance() << "cm";
        ofLogNotice() << "zero plane pixel size: " << kinect.getZeroPlanePixelSize() << "mm";
        ofLogNotice() << "zero plane dist: " << kinect.getZeroPlaneDistance() << "mm";
    }
    
    colorImg.allocate(kinect.width, kinect.height);
    grayImage.allocate(kinect.width, kinect.height);
    grayThreshNear.allocate(kinect.width, kinect.height);
    grayThreshFar.allocate(kinect.width, kinect.height);
    
    nearThreshold = 230;
    farThreshold = 70;
    bThreshWithOpenCV = true;
    
    ofSetFrameRate(60);
    
    // zero the tilt on startup
    angle = 0;
    kinect.setCameraTiltAngle(angle);
    
    // start from the front
    bDrawPointCloud = false;
    
    /*---------------XML----------------*/
    XML.loadFile("territory.xml");
    //don't use  int numPath = XML.getNumTags("territory:path");
    XML.pushTag("territory", 0);
    numPath = XML.getNumTags("path");
    //cout << "numPath = " << numPath << endl;
    tagLevel = XML.getPushLevel();
    if(tagLevel < 2){
        XML.pushTag("path", pathIndex);
        numVert = XML.getNumTags("vertex");
       // cout << "numVertex = " << numVert << endl;
    }
    
    /*---------------map------------------*/
    //re-assign the vertex position value in the array
    vert.resize(numVert);
    for (int j = 0; j < numVert; j++){
        
        vertX = XML.getAttribute("vertex", "x", 0, j);
        vertY = XML.getAttribute("vertex", "y", 0, j);
        vert[j] = ofPoint(vertX, vertY);
    }
    
    visited.resize(numVert-1);
    for (int j = 0; j < numVert-1; j++){
        visited[j] = false; //path visited instead of points visited
    }
    
    /*---------------serial------------------*/
    bSendSerialMessage = false;
    ofBackground(255);
    ofSetLogLevel(OF_LOG_VERBOSE);
    
    serial.listDevices();
    vector <ofSerialDeviceInfo> deviceList = serial.getDeviceList();
    int baud = 9600;
    //serial.setup(0, baud); //open the first device
//	serial.setup("/dev/tty.usbmodem1421", baud); // mac osx example
    serial.setup(0, baud); //open the first device
}

//--------------------------------------------------------------

void ofApp::update(){
    
    /*------ Update kinect------*/
    ofBackground(100, 100, 100);
    
    kinect.update();
    
    // there is a new frame and we are connected
    if(kinect.isFrameNew()) {
        
        // load grayscale depth image from the kinect source
        grayImage.setFromPixels(kinect.getDepthPixels());
        
        // we do two thresholds - one for the far plane and one for the near plane
        // we then do a cvAnd to get the pixels which are a union of the two thresholds
        if(bThreshWithOpenCV) {
            grayThreshNear = grayImage;
            grayThreshFar = grayImage;
            grayThreshNear.threshold(nearThreshold, true);
            grayThreshFar.threshold(farThreshold);
            cvAnd(grayThreshNear.getCvImage(), grayThreshFar.getCvImage(), grayImage.getCvImage(), NULL);
        } else {
            
            // or we do it ourselves - show people how they can work with the pixels
            ofPixels & pix = grayImage.getPixels();
            int numPixels = pix.size();
            for(int i = 0; i < numPixels; i++) {
                if(pix[i] < nearThreshold && pix[i] > farThreshold) {
                    pix[i] = 255;
                } else {
                    pix[i] = 0;
                }
            }
        }
        
        // update the cv images
        grayImage.flagImageChanged();
        
        // find contours which are between the size of 20 pixels and 1/3 the w*h pixels.
        // also, find holes is set to true so we will get interior contours as well....
        contourFinder.findContours(grayImage, 10, (kinect.width*kinect.height)/2, 20, false);
    }
    
    //osc receives animation playing condition
    while(serverReceiver.hasWaitingMessages()){
                // get the next message
        ofxOscMessage m;
        serverReceiver.getNextMessage(&m);
        if(m.getAddress() == "/endingplay"){
            endingIsPlay = m.getArgAsBool(0);
            //cout<<"endingIsPlay: "<< endingIsPlay <<endl;
        }
    }
    
    if(nextMap == true && endingIsPlay == true){
        startPlay = true;
    }
    
    if(startPlay == true && endingIsPlay == false){
        nextMap = false;
    }
    
     /*------ next map in XML------*/
   

    //cout<<"points all touched: "<< nextMap <<endl;
 
    if (startPlay == true && nextMap == false) {
        startPlay = false;
            
        if( pathIndex < numPath-1 ){
        pathIndex++;
        }else if(pathIndex >= numPath-1){
        pathIndex = 0;
        }
        //ofClear(255);
        tagLevel = XML.getPushLevel();
        if(tagLevel >= 2){
            XML.popTag();
            XML.pushTag("path", pathIndex);
            numVert = XML.getNumTags("vertex");
                    
        }
       
        //re-assign the vertex position value in the array
        vert.clear();
        vert.resize(numVert);
        for (int j = 0; j < numVert; j++){
            
        vertX = XML.getAttribute("vertex", "x", 0, j);
        vertY = XML.getAttribute("vertex", "y", 0, j);
        vert[j] = ofPoint(vertX, vertY);
            
        }
        
        visited.clear();
        visited.resize(numVert-1);
        for (int j = 0; j < numVert-1; j++){
            visited[j] = false;
        }
    }
    
    //this is purely workaround for a mysterious OSCpack bug on 64bit linux
    // after startup, reinit the receivers
    // must be a timing problem, though - in debug, stepping through, it works.
    if(ofGetFrameNum() == 60){
        serverReceiver.setup(PORT2);
    }
    
}

//--------------------------------------------------------------
void ofApp::draw(){

    setupCam();
    
    std::stringstream map;
//    map << "Vert Sum: " << numVert << std::endl;
//    map << "Visited Sum: " << visited.size() << std::endl;
    
    for(int i=0; i<visited.size(); i++){
        map << "visited vert: " << i <<": "<< visited[i] << std::endl;
    }
    map << "Map No. "<< pathIndex << std::endl;
    map << "Total Map "<< numPath << std::endl;
    map << "all visited "<< nextMap << std::endl;
    
    ofDrawBitmapStringHighlight(map.str(), ofPoint(660, 80));
    
    // This block plays when the ending is NOT playing.
    //  In it, we:
    //    - calculate blobs from the camera stream
    //    - identify collisions between our blobs and our walls
    //    - if we have a collision
    //       - figure out if all the walls have been touched
    //       - send serial data to Arduino to blink lights
    //       - send an OSC message to reveal the vertex in
    //           our other program displaying stuff, and to
    //           play the ending video
    //       - play the alarm sound
    if(endingIsPlay == false){
    /*------ detection------*/
    //draw boundingRect and collision detection
          std::stringstream ss;
        ss << "cam1 blob: " << contourFinder.blobs.size() << std::endl;
        ofDrawBitmapStringHighlight(ss.str(), ofPoint(660, 15));
        
    for(int i = 0; i < contourFinder.blobs.size(); i++) {//all blobs of each cam
        ofRectangle r = contourFinder.blobs.at(i).boundingRect;

        for (int j = 0; j < numVert-1; j++){//all vertex of the map
        
            
                touch = r.intersects(vert[j], vert[j+1]);
            
            
            cout<< "touch = " << touch << endl;
            if(touch){
                if(visited[j] == false){
                    serial.writeByte(1);
                    cout<<"write serial"<<endl;
                    visited[j] = true;
                    allVisited = areAllTrue(visited);
                    if(allVisited == true){
                        nextMap = true;
                    }
                    
                    
                    ofxOscMessage m;
                    m.setAddress("/vertex");
                    m.addIntArg(vert[j].x);
                    m.addIntArg(vert[j].y);
                    m.addIntArg(vert[j+1].x);
                    m.addIntArg(vert[j+1].y);
                    m.addIntArg(nextMap);
                    serverSender.sendMessage(m);
                    cout<<"sender vert1= ("<< m.getArgAsInt(0) <<","<<m.getArgAsInt(1)<<")" <<endl;
                    cout<<"sender vert2= ("<< m.getArgAsInt(2) <<","<<m.getArgAsInt(3)<<")" <<endl;
                    cout<<"sender nextMap= "<< m.getArgAsInt(4) <<endl;
                    
                    touch = false;
                    if(alert.isPlaying() == false){
                        alert.play();
                    }//if alert
                    
                }//if visited
          }//if(touch)
            else{
           //     serial.writeByte(0);
            }
        }//for(j)
      }//for(i)
    }//for(z)
    }//endingplay
    




//--------------------------------------------------------------
void ofApp::setupCam(){
    
    int x = 0;
    int y = 0;
    
    fboCam.begin();
    ofClear(255,255,255, 0);
    
    ofSetColor(255, 255, 255);
    
    if(bDrawPointCloud) {
        easyCam.begin();
        drawPointCloud();
        easyCam.end();
    } else {
        // draw from the live kinect
       // kinect.drawDepth(10, 10, 400, 300);
        //kinect.draw(420, 10, 400, 300);
        
        grayImage.draw(0, 0, camWidth, camHeight);
        contourFinder.draw(0, 0, camWidth, camHeight);
    }
    fboCam.end();
    fboCam.draw(0, 0, camWidth, camHeight);
    
}

//--------------------------------------------------------------
void ofApp::drawPointCloud() {
    ofMesh mesh;
    mesh.setMode(OF_PRIMITIVE_POINTS);
    int step = 2;
    for(int y = 0; y < camHeight; y += step) {
        for(int x = 0; x < camWidth; x += step) {
            if(kinect.getDistanceAt(x, y) > 0) {
                mesh.addColor(kinect.getColorAt(x,y));
                mesh.addVertex(kinect.getWorldCoordinateAt(x, y));
            }
        }
    }
    glPointSize(3);
    ofPushMatrix();
    // the projected points are 'upside down' and 'backwards'
    ofScale(1, -1, -1);
    ofTranslate(0, 0, -1000); // center the points a bit
    ofEnableDepthTest();
    mesh.drawVertices();
    ofDisableDepthTest();
    ofPopMatrix();
}

//--------------------------------------------------------------
void ofApp::exit() {
    kinect.setCameraTiltAngle(0); // zero the tilt on exit
    kinect.close();
    
}

//--------------------------------------------------------------
bool ofApp::areAllTrue(vector<bool> array)
{
    for(bool b : array) if(!b) return false;
    return true;
}

//--------------------------------------------------------------
//void ofApp::growLine(ofPoint a, ofPoint b){

//}



//--------------------------------------------------------------
void ofApp::keyPressed(int key){

      //  bLearnBackground = true;
}

//--------------------------------------------------------------
void ofApp::drawCam(ofEventArgs & args){
    setupCam();
}
