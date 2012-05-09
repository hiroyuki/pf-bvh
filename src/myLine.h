//
//  Line.h
//  perfume-bvh-voronoi
//
//  Created by 宏行 堀 on 5/7/12.
//  Copyright 2012 rhizomatiks. All rights reserved.
//

#ifndef perfume_bvh_voronoi_Line_h
#define perfume_bvh_voronoi_Line_h
#include "ofMain.h"
class myLine 
{
public:
    ofPoint a, b;
    bool isPartnerSwapped, isSwappedPartnerSwapped;
    float myAngle;
    myLine(){}
    
    myLine(const ofPoint& _a, const ofPoint& _b):
    a(_a), b(_b), partnerFixed( false ),isEnd( false ),
    isPartnerSwapped(false), isSwappedPartnerSwapped(false),
    regularPoints(NULL), reversePoints( NULL ),
    partner(NULL), swapingPartner(NULL),
    regularShapeComplete( false ), reverseShapeComplete( false )
                                {
//        if (_a.x - _b.x == 0) {
//            if ( _a.y - _b.y > 0)
//            {
//                myAngle = - PI / 2;
//            }
//            else
//            {
//                myAngle = PI / 2;
//            }
//        }
//        else
//        {
//            myAngle = atan2( (_b.y - _a.y), (float)(_b.x - _a.x) );
//        }
    }
    virtual ~myLine(){
        
    }
    
    bool operator==(myLine &obj)
    {
        return((this->a.x == obj.a.x && this->a.y == obj.a.y
                && this->b.x == obj.b.x && this->b.y == obj.b.y) ||
               (this->a.x == obj.b.x && this->a.y == obj.b.y
                && this->b.x == obj.a.x && this->b.y == obj.a.y));
    }
    
    bool operator!=(myLine &obj)
    {
        return(!((this->a.x == obj.a.x && this->a.y == obj.a.y
                  && this->b.x == obj.b.x && this->b.y == obj.b.y) ||
                 (this->a.x == obj.b.x && this->a.y == obj.b.y
                  && this->b.x == obj.a.x && this->b.y == obj.a.y)));
    }
    
    float swapPos();
    
    myLine* partner;//順向きのライン
    myLine* swapingPartner;//逆向きのライン
    vector<ofPoint> * regularPoints;//順向きの図形を構成する座標群
    vector<ofPoint> * reversePoints;//逆向きの図形を構成する座標群
    bool regularShapeComplete, reverseShapeComplete;
    //上記アルゴリズムではなく、座標のベクターを２つ（順方向と逆方向）を保持し、同じ図形を保持するライン間で共有する
    bool partnerFixed;
    bool isEnd;
    void fixPartner();
    void setMySource();
    bool findPartner(myLine *line);
    bool findPartnerNormal(myLine *line);
    bool findPartnerSwap(myLine *line);
    void getShape( vector< ofPolyline *>& shapes );
    bool checkLeftAngle(const ofPoint a, const ofPoint b);
    bool isSamePoint( const ofPoint p1, const ofPoint p2 )
    {
        return abs( p1.x - p2.x + p1.y - p2.y) < 0.1;
    }
    
};

#endif
