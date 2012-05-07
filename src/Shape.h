//
//  Shape.h
//  example-bvh
//
//  Created by 宏行 堀 on 5/7/12.
//  Copyright (c) 2012 rhizomatiks. All rights reserved.
//

#ifndef example_bvh_Shape_h
#define example_bvh_Shape_h
class Shape {
public:
    vector<ofPoint> points;
    
    bool addPoint( ofPoint p )
    {
        int len = points.size();
        bool isAddible = true;
        for ( int i = 0; i < len && isAddible; i++ )
        {
            isAddible = !( points[ i ].x == p.x && points[ i ].x == p.y );
        }
        if ( isAddible )
        {
            points.push_back( p );
        }
        return isAddible;
    }
    
    void draw( int r, int g, int b, int a )
    {
        
    }
    
    void debug()
    {
        cout << points.size() << endl;
    }
};


#endif
