#include "testApp.h"
int mode;

//--------------------------------------------------------------
void testApp::setup(){
	ofSetFrameRate(60);
    ofSetVerticalSync(true);
    ofBackgroundHex(0xffffff);
    ofHideCursor();
    showAmount = 30;
    mode = 1;
    gui.addTitle( "setting" );
    gui.addSlider( "mode" , mode, 1, 2);
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
	voronoi.setMinDistance(1);
}

//--------------------------------------------------------------
void testApp::update()
{
	for (int i = 0; i < bvh.size(); i++)
	{
		bvh[i].update();
//        bvh[i].stop();
	}
    voronoi.clear();
    ofSetWindowTitle(ofToString(ofGetFrameRate()));
    voronoi.addPoint(-15, -15);voronoi.addPoint(-15, ofGetHeight()+15);voronoi.addPoint(ofGetWidth()+15, -15);voronoi.addPoint(ofGetWidth()+15, ofGetHeight()+15);
    voronoi.addPoint(-10, 10);voronoi.addPoint(-10, ofGetHeight()-10);voronoi.addPoint(ofGetWidth()+10, 10);voronoi.addPoint(ofGetWidth()+10, ofGetHeight()-10);
    voronoi.addPoint(10, -10);voronoi.addPoint(10, ofGetHeight()+10);voronoi.addPoint(ofGetWidth()-10, -10);voronoi.addPoint(ofGetWidth()-10, ofGetHeight()+10);
    voronoi.addPoint(10, 10);voronoi.addPoint(10, ofGetHeight()-10);voronoi.addPoint(ofGetWidth()-10, 10);voronoi.addPoint(ofGetWidth()-10, ofGetHeight()-10);
    frame.push_back(ofPoint(10, 10));frame.push_back(ofPoint(10, ofGetHeight()-10));
    frame.push_back(ofPoint(ofGetWidth()-10, 10));frame.push_back(ofPoint(ofGetWidth()-10, ofGetHeight()-10));
    
    for (int i = 0; i < bvh.size(); i++)
	{
		ofxBvh &b = bvh[i];
		if ( mode == 0 || mode == 1)
        {
            for (int n = 0; n < b.getNumJoints(); n++)
            {
                const ofxBvhJoint *o = b.getJoint(n);
                ofVec3f _pos = cam.worldToScreen( o->getPosition() );
                voronoi.addPoint(_pos.x, _pos.y );
                
            }
        }
    }
    if (mode == 0 || mode == 1)
    {
        bool bMade = voronoi.generateVoronoi();
        //unique pointの追加
        lines.clear();
        {
            for(int i=0; i< voronoi.edges.size(); i++) {
                ofVec2f a = voronoi.edges[i].a;
                ofVec2f b = voronoi.edges[i].b;
//                if ( voronoi.edges[i].a.x >= 0 && voronoi.edges[i].b.x >= 0 &&
//                    voronoi.edges[i].a.y >= 0 && voronoi.edges[i].b.y >= 0 &&
//                    voronoi.edges[i].a.x <= ofGetWidth() && voronoi.edges[i].b.x <= ofGetWidth() &&
//                    voronoi.edges[i].a.y <= ofGetHeight() && voronoi.edges[i].b.y <= ofGetHeight()
//                    && a != b 
//                    )
                {
                    lines.push_back(new myLine(a, b));
                }
//            addPoint(a);addPoint(b);
            }
            shapes.clear();
        
            for (int i = 0; i < bvh.size(); i++)
            {
                ofxBvh &b = bvh[i];
                if ( mode == 0 || mode == 1)
                {

                    for (int n = 0; n < b.getNumJoints()+4; n++)
                    {   
                        ofVec3f pos;
                        if ( n < b.getNumJoints() )
                        {
                            const ofxBvhJoint *o = b.getJoint(n);
                            pos = cam.worldToScreen( o->getPosition() );
                        }
                        else
                        {
                            int no = n - b.getNumJoints();
                            pos = ofVec3f( frame[ no ].x, frame[ no ].y, 0 );
                        }
                        int lineLength = sqrt( pow( (float)ofGetWidth(), 2 ) + pow( (float)ofGetHeight(), 2) );
                        float angle = 0;
                        myShape shape;

                        shape.setCenter(ofPoint( pos.x, pos.y ));
                        int counter = 0;
                        for ( angle = 0; angle < 2 * PI; angle += 0.087266 )
                        {
                            counter++;
//                            ofLineSegmentIntersection
                            int addx = cos(angle) * lineLength;
                            int addy = sin(angle) * lineLength;
                            ofPoint endPoint( pos.x + addx, pos.y + addy, 0);
                            int nearestDistance = lineLength;
                            bool isSet = false;
                            ofPoint intersect2;
                            ofPoint nearestA, nearestB;
                            for(int j=0; j<lines.size(); j++) {
                                ofPoint intersect;
                                bool isCross = ofLineSegmentIntersection(pos, endPoint, lines[ j ]->a, lines[ j ]->b, intersect);
                                if ( isCross )
                                {
                                    
                                    int distance = sqrt(pow((intersect.x - pos.x), 2) + pow( intersect.y - pos.y,2 ));
                                    if ( distance < nearestDistance)
                                    {
                                        intersect2 = intersect;
                                        nearestDistance = distance;
                                        nearestA = lines[ j ]->a;
                                        nearestB = lines[ j ]->b;
                                        isSet = true;
                                    }
                                }
                            }
//                            if (shapes.size() == 15 )
//                            {
//                                tmp_vec.push_back( intersect2 );
//                                cout << "_" << counter << " " << tmp_vc.size()<< endl;
//                            }
                            if ( isSet ) {
                                shape.addVertex(nearestA);
                                shape.addVertex(nearestB);
                            }
                        }
                        shapes.push_back(shape);
                    }
                }
            }
            while ( color.size() < shapes.size() )
            {
                color.push_back( ofColor( ofRandom(255), ofRandom(255),ofRandom(255) ) );
            }
            for ( int i = 0; i < shapes.size(); i++ )
            {
                shapes[ i ].setColor(color[i]);
            }
        }
    }
}

//--------------------------------------------------------------
void testApp::draw(){
	//glEnable(GL_DEPTH_TEST);
	
	ofEnableBlendMode(OF_BLENDMODE_ALPHA);
    
	cam.begin();
    for (int i = 0; i < bvh.size(); i++)
	{
		ofxBvh &b = bvh[i];
		if ( mode != 0 && mode != 1)
        {
            bvh[i].draw(); 
        }
	}
	
	cam.end();
    if (mode == 0 || mode == 1)
    {
        ofSetHexColor(0x0);
        if (mode == 1)
        {
            vector<ofPoint> points = voronoi.getPoints();
            for( int i = 0; i < points.size(); i++ )
            {
//                ofCircle(points[i].x, points[i].y, 1);
            }
        }
        
        ofEnableBlendMode( OF_BLENDMODE_ALPHA );
        
        for( int i = 0; i < shapes.size() && mode == 1; i++ ) 
        {
            ofSetColor(shapes[ i ].color.r, shapes[ i ].color.g, shapes[ i ].color.b, 150);
            shapes[ i ].draw();
        }
        ofSetHexColor(0);
        for(int i=0; i<lines.size(); i++) {
            //        for(int i=0; i<voronoi.edges.size();i++){
            //            ofVec2f a = voronoi.edges[ i ].a;
            //            ofVec2f b = voronoi.edges[ i ].b;
            
            ofVec2f a = lines[ i ]->a;
            ofVec2f b = lines[ i ]->b;
            
            ofLine(a, b);
            //            ofDrawBitmapString(ofToString(i), ofPoint( a.x+5, a.y));
        }
    }
    gui.draw();
}

//--------------------------------------------------------------
void testApp::keyPressed(int key){
    if (key == 9)
        gui.toggleDraw();
    if ( key == '1' ) mode = 0;
    if ( key == '2' ) mode = 1;
    if ( key == 's' ) 
    {
        track.play();
//        track.setLoop(true);
        for (int i = 0; i < bvh.size(); i++)
        {
            bvh[i].play();
            //bvh[i].setLoop(true);
        }
    }
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
