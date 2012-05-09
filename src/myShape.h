//
//  myShape.h
//  perfume-bvh-voronoi
//
//  Created by 宏行 堀 on 5/9/12.
//  Copyright 2012 rhizomatiks. All rights reserved.
//

#ifndef perfume_bvh_voronoi_myShape_h
#define perfume_bvh_voronoi_myShape_h

struct myPoint {
	ofPoint point;
	ofPoint center;
};

class myShape
{
public:
    myShape(){}
    vector< myPoint > drawPoints;
    vector< ofPoint > points;
    ofPoint center;
    ofColor color;
    void setCenter( const ofPoint c )
    {
        center = c;
    }
    
    void setColor( const ofColor col )
    {
        color = col;
    }
    
    void addVertex( const ofPoint p )
    {
        int counter = 0;
        for( int i = 0; i< points.size() && counter == 0; i++ )
        {
            if ( points[ i ].x == p.x && points[ i ].y == p.y )
            {
                counter++;
            }
        }
        if ( counter == 0 ) points.push_back( p );
        if ( counter == 1 )
        {
            myPoint mp;
            mp.point = p; mp.center = center;
            drawPoints.push_back(mp);
        }
    }
    
    static bool sort_by_angle( const myPoint& left, const myPoint& right ) {
        return atan2(left.center.y - left.point.y, left.center.x - left.point.x) < atan2(left.center.y - right.point.y, left.center.x - right.point.x);
    }
    
    void draw()
    {
        if (drawPoints.size() > 0)
            std::sort(drawPoints.begin(), drawPoints.end(), sort_by_angle);
        else
            return;
        glBegin(GL_TRIANGLE_FAN);
        
        glVertex2f( center.x, center.y);
        for ( int i = 0; i < drawPoints.size(); i++ )
        {
            glVertex2f(drawPoints[ i ].point.x, drawPoints[ i ].point.y);   
        }
        glVertex2f(drawPoints[ 0 ].point.x, drawPoints[ 0 ].point.y);   
        glEnd();
    }
};


#endif
