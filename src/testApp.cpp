#include "testApp.h"
int mode;

//--------------------------------------------------------------
void testApp::setup(){
	ofSetFrameRate(60);
    ofSetVerticalSync(true);
    ofBackgroundHex(0xC52050);
    
    mode = 0;
    gui.addTitle("setting");
    gui.addSlider("mode", mode, 0, 2);
    
	cam.cacheMatrices();
	
	bvh.resize(3);
    voronoi.setMinDistance(1);
	// setup bvh
	bvh[0].load("bvhfiles/aachan.bvh");
	bvh[1].load("bvhfiles/kashiyuka.bvh");
	bvh[2].load("bvhfiles/nocchi.bvh");
	
	for (int i = 0; i < bvh.size(); i++)
	{
		bvh[i].setFrame(1);
	}
	
	track.loadSound("Perfume_globalsite_sound.wav");
	track.play();
	track.setLoop(true);
	voronoi.setMinDistance(0.1f);
	for (int i = 0; i < bvh.size(); i++)
	{
		bvh[i].play();
		//bvh[i].setLoop(true);
	}
}

//--------------------------------------------------------------
void testApp::update()
{
	for (int i = 0; i < bvh.size(); i++)
	{
		bvh[i].update();
	}
    voronoi.clear();
    ofSetWindowTitle(ofToString(ofGetFrameRate()));
    voronoi.addPoint(10, 10);voronoi.addPoint(10, ofGetHeight()-10);voronoi.addPoint(ofGetWidth()-10, 10);voronoi.addPoint(ofGetWidth()-10, ofGetHeight()-10);
}

//--------------------------------------------------------------
void testApp::draw(){
	//glEnable(GL_DEPTH_TEST);
	
	ofEnableBlendMode(OF_BLENDMODE_ALPHA);
    
	cam.begin();
    for (int i = 0; i < bvh.size(); i++)
	{
		ofxBvh &b = bvh[i];
		if ( mode == 0 || mode == 1)
        {
            for (int n = 0; n < b.getNumJoints(); n++)
            {
                const ofxBvhJoint *o = b.getJoint(n);
                ofVec3f pos = cam.worldToScreen( o->getPosition() );
                voronoi.addPoint(pos.x, pos.y );

            }
        }
        else
        {
            bvh[i].draw(); 
        }
	}
	
	cam.end();
    if (mode == 0 || mode == 1)
    {
        ofSetHexColor(0xcccccc);
        bool bMade = voronoi.generateVoronoi();
        for(int i=0; i<voronoi.edges.size(); i++) {
            ofVec2f a = voronoi.edges[i].a;
            ofVec2f b = voronoi.edges[i].b;;
            ofLine(a, b);
            ofDrawBitmapString(ofToString(i), ofPoint( ( a.x + b.x ) / 2, ( a.y + b.y ) / 2));
        }
        if (mode == 1)
        {
            vector<ofPoint> points = voronoi.getPoints();
            for( int i = 0; i < points.size(); i++ )
            {
                ofCircle(points[i].x, points[i].y, 1);
            }
        }
    }
    gui.draw();

}

//--------------------------------------------------------------
void testApp::keyPressed(int key){
    if (key == 9)
        gui.toggleDraw();
    else if ( key == ' ' )
        for( int i = 0; i < bvh.size(); i++ )
            bvh[ i ].stop();
}

//--------------------------------------------------------------
void testApp::keyReleased(int key){

}

//--------------------------------------------------------------
void testApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void testApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void testApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void testApp::mouseReleased(int x, int y, int button){
}

//--------------------------------------------------------------
void testApp::windowResized(int w, int h){
}

//--------------------------------------------------------------
void testApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void testApp::dragEvent(ofDragInfo dragInfo){ 
}
