#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    clientSender.setup("localhost", PORT2);
    clientReceiver.setup( PORT );
//   ofBackground(255);
    ofSetBackgroundAuto(false);
    ofEnableAlphaBlending();
//    startTime.push_back(ofGetElapsedTimeMillis());
    ofSetVerticalSync(true);
    myPlayer.load("ending1.mov");
    //myPlayer.setLoopState(OF_LOOP_NONE);
    lastFrame = myPlayer.getTotalNumFrames();
    
        /*---------------Syphon----------------*/
        // mainOutputSyphonServer.setName("Screen Output");
        individualTextureSyphonServer.setName("Characters");
        mClient.setup();
        //using Syphon app Simple Server, found at http://syphon.v002.info/
        mClient.set("","Simple Server");
        syphonTex.allocate(640, 480, GL_RGBA);
        syphonFbo.allocate(640, 480, GL_RGBA);

    
    syphonFbo.begin();
     ofBackground(255);
    syphonFbo.end();
//    mClient.draw(50, 50);
//    syphonTex = syphonFbo.getTextureReference();
//    ofEnableAlphaBlending();
//    individualTextureSyphonServer.publishTexture(& syphonTex);
}

//--------------------------------------------------------------
void ofApp::update(){
    // Video player code
    cout<< "all frames:" << lastFrame <<endl;
    cout<< "vid current position:" << myPlayer.getCurrentFrame() <<endl;
    if(myPlayer.getCurrentFrame() >= lastFrame-1 ){//receive new osc message
        videoPlay = false;
        myPlayer.stop();
        cout<< "video stop!"<<endl;
        ofBackground(255);
    }else{
        myPlayer.update();
    }
    
    //osc sender
    ofxOscMessage m;
    m.setAddress("/endingplay");
    m.addBoolArg(myPlayer.isPlaying());
    clientSender.sendMessage(m);
    cout<< "send:" << m.getArgAsBool(0) <<endl;
    //cout<< "video play:" << videoPlay <<endl;
   
   // if (videoPlay == false){
    
    //osc receiver
    cout<<"waiting message: "<<clientReceiver.hasWaitingMessages()<<endl;
    while(clientReceiver.hasWaitingMessages()){
        
        // get the next message
        ofxOscMessage m;
        clientReceiver.getNextMessage(m);
        if(m.getAddress() == "/vertex"){
            cout<<"this part working"<<endl;
            // both the arguments are int32's
            a = m.getArgAsInt(0);
            b = m.getArgAsInt(1);
            c = m.getArgAsInt(2);
            d = m.getArgAsInt(3);
            nextMap = m.getArgAsBool(4);
            cout<< "a:" << m.getArgAsInt(0) <<endl;
            cout<< "nextMap Instruction:" << m.getArgAsBool(4) <<endl;

            vert1.push_back(ofPoint(a, b));
            vert2.push_back(ofPoint(c, d));
            
        }//if address
    }//while
    
    //this is purely workaround for a mysterious OSCpack bug on 64bit linux
    // after startup, reinit the receivers
    // must be a timing problem, though - in debug, stepping through, it works.
    if(ofGetFrameNum() == 60){
        clientReceiver.setup(PORT);
    }
    
        /*------ Syphon Stuff------*/
        mClient.draw(50, 50);
        syphonTex = syphonFbo.getTextureReference();
        ofEnableAlphaBlending();
        individualTextureSyphonServer.publishTexture(& syphonTex);
   
}

//--------------------------------------------------------------
void ofApp::draw(){
    cout<< "videoPlay:" << videoPlay <<endl;
    syphonFbo.begin();
    // If the video is playing, do that.
    if(videoPlay == true){
      ofPushStyle();
      ofSetColor(255, 255, 255, vidTrans);
      myPlayer.draw(0,0);
        
      if(vidTrans < 255) {
        vidTrans= vidTrans + 5;
      }
      cout<< "trans: " << vidTrans <<endl;
      ofPopStyle();
    }
    
    else
    
    {
    /*when video not playing, draw lines*/
    //check if more vert is added in
    //if nothing more added, don't set new startTime point, just run the current line growing animation
    countNew = vert1.size();
    if(countNew > count){
        for(int a = 0; a < countNew - count; a++){
        startTime.push_back(ofGetElapsedTimeMillis());//mark a time code as the anim starting point of each line
        }
        count = countNew;
    }
      

    interpolate.resize(count);
    for(int i=0; i< count; i++){
    
    interpolate[i] = (float)(ofGetElapsedTimeMillis()-startTime[i])/growTime; //the proportion of time that has been spent for this particular line in the growTime duration
        
    //cout<< "interpolate=" << interpolate[i] <<endl;
        if(interpolate[i] < 1)
        {
            cout << "time duration of line[i]: " << interpolate[i] << endl;
            ofPushStyle();
            ofSetLineWidth(5);
            ofSetColor(255,0,0);
            ofDrawCircle(vert1[i].x, vert1[i].y, 2.5);
            ofDrawLine(vert1[i].x, vert1[i].y,
                       ( (1-interpolate[i]) * vert1[i].x + interpolate[i] * vert2[i].x ),
                       ( (1-interpolate[i]) * vert1[i].y + interpolate[i] * vert2[i].y ) );
            cout<<"drawing!"<<endl;
            
            //ofDrawCircle(vert2[i].x, vert2[i].y, 2.5);
             ofPopStyle();
        }
        else{
            interpolate[i] = 1;
            ofPushStyle();
            ofSetLineWidth(5);
            ofSetColor(255,0,0);
            ofDrawCircle(vert1[i].x, vert1[i].y, 2.5);
            ofDrawLine(vert1[i].x, vert1[i].y,
                       ( (1-interpolate[i]) * vert1[i].x + interpolate[i] * vert2[i].x ),
                       ( (1-interpolate[i]) * vert1[i].y + interpolate[i] * vert2[i].y ) );
            cout<<"drawing!"<<endl;
            
            //ofDrawCircle(vert2[i].x, vert2[i].y, 2.5);
            ofPopStyle();
            
        }
   
    
    }
  }//videoplay
    syphonFbo.end();
    
    if (nextMap) {//if map changes, play ending anim, empty out vert1,vert2
        myPlayer.play();
        videoPlay = true;
        nextMap = false;
        vidTrans = 0;
        vert1.clear();
        vert2.clear();
        startTime.clear();
        interpolate.clear();
        count = 0;
    }
}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}
