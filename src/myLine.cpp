//
//  Line.cpp
//  perfume-bvh-voronoi
//
//  Created by 宏行 堀 on 5/7/12.
//  Copyright 2012 rhizomatiks. All rights reserved.
//

#include <iostream>
#include "myLine.h"
bool myLine::findPartner(myLine *line)
{
    //順向きと逆向きでつながるラインを探す
    cout << "-" << endl;
//    cout << "start-> " << a << "/" << b << " // " << line->a << "/" << line->b<< endl;
    bool result = false;
    bool regularFound = false;
    bool reverseFound = false;
    result = findPartnerNormal(line);
    if ( result )
    {
        isPartnerSwapped = false;//順向きで相手も順向き
        regularFound = true;
    }
    
    result = findPartnerSwap( line );
    if ( result )
    {
        isSwappedPartnerSwapped = false;//逆向きで相手は順向き
        reverseFound = true;
    }
    
    line->swapPos();
    if ( !regularFound ) //すでにパートナー見つけてたら探さない
    {
        result = findPartnerNormal(line);//順向きで相手は逆向き
        if ( result )
        {
            isPartnerSwapped = true;
        }
    }
    
    if ( !reverseFound )
    {
        result = findPartnerSwap(line);//逆向きで相手も逆向き
        if ( result )
        {
            isSwappedPartnerSwapped = true;
        }
    }
    line->swapPos();
//    cout << "end" << endl;
}

//順向きで結合されるラインを探す
bool myLine::findPartnerNormal(myLine *line)
{
    cout << " ----> " << a << "/" << b << " // " << line->a << "/" << line->b << endl;    
    bool result = false;
    if ( isSamePoint( b, line->a) && checkLeftAngle(b - a, line->b - line->a))
    {
        if (partner != NULL )//すでにほかのラインが候補としてある場合
        {
            if (myAngle - line->myAngle > myAngle - partner->myAngle )
            {
                cout << "found "<< endl;
                partner = line;
                result = true;
            }
        } 
        else//はじめて結合ラインを見つけた場合
        {
            cout << "found first "<< endl;
            partner = line;
            result = true;
        }
    }
    return result;
}

//逆向きで結合されるラインを探す
bool myLine::findPartnerSwap(myLine *line)
{
    swapPos();
    bool result = false;
    cout << " ----> " << a << "/" << b << " // " << line->a << "/" << line->b << " " << endl;    
    if ( isSamePoint(b, line->a)&& checkLeftAngle(b - a, line->b - line->a))
    {
        if (partner != NULL )
        {
            if (myAngle - line->myAngle > myAngle - partner->myAngle )
            {
                cout << "found2 "<< endl;
                swapingPartner = line;
                result = true;
            }
        } 
        else 
        {
            cout << "found first2 "<< endl;
            swapingPartner = line;
            result = true;
        }
    }
    swapPos();
    return result;
    
}

void myLine::fixPartner()
{
    //partnerFixed = true;
}

void myLine::setMySource()
{
    if ( partner != NULL )
    {
        //座標fix 順向きの場合の結合
        if ( regularPoints == NULL ) //まだだれからもつながれていない場合
        {
            regularPoints = new vector<ofPoint>();
            regularPoints->push_back( a );
            regularPoints->push_back( b ); 
        }
        if ( isPartnerSwapped && !partner->reverseShapeComplete )
        {
            if ( partner->reversePoints != NULL) 
            {
                regularPoints->insert( regularPoints->end(), partner->reversePoints->begin(), partner->reversePoints->end() );
            }
            else if ( !isSamePoint( partner->a, (*regularPoints)[ regularPoints->size() - 1 ]))
            {
                regularPoints->push_back( partner->a );
            }
            partner->reversePoints=regularPoints;
        }
        else if ( !partner->regularShapeComplete )
        {
            if ( partner->regularPoints != NULL) 
            {
                regularPoints->insert( regularPoints->end(), partner->regularPoints->begin(), partner->regularPoints->end() );
            }
            else if ( !isSamePoint( partner->b ,(*regularPoints)[ regularPoints->size() - 1 ]))
            {
                regularPoints->push_back( partner->b );
            }
            partner->regularPoints=regularPoints;
        }
        for( int i=0; i<regularPoints->size(); i++)
        {
            printf( " %f %f %d",(*regularPoints)[ i ].x,(*regularPoints)[ i ].y, isPartnerSwapped);
        }
        cout << "<normal" << endl;        
    }
    //座標fix 逆向きの場合の結合
    if ( swapingPartner != NULL )
    {
        //座標fix 順向きの場合の結合
        if ( reversePoints == NULL ) //まだだれからもつながれていない場合
        {
            cout << "create point" << endl;
            reversePoints = new vector<ofPoint>();
            reversePoints->push_back( b );
            reversePoints->push_back( a ); 
        }
        if ( isSwappedPartnerSwapped && !swapingPartner->reverseShapeComplete)
        {
            
            if ( swapingPartner->reversePoints != NULL) 
            {
                cout << "concat" << endl;
                reversePoints->insert( reversePoints->end(), swapingPartner->reversePoints->begin(), swapingPartner->reversePoints->end() );
            }
            else if ( !isSamePoint( swapingPartner->a, (*reversePoints)[ reversePoints->size() - 1 ])) 
            {
                cout << "add" << endl;
                reversePoints->push_back(swapingPartner->a);
            }
            swapingPartner->reversePoints = reversePoints;
        }
        else if( !swapingPartner->regularShapeComplete )
        {
            if ( swapingPartner->regularPoints != NULL )
            {
                reversePoints->insert( reversePoints->end(), swapingPartner->regularPoints->begin(), swapingPartner->regularPoints->end() );
            }
            else if ( !isSamePoint( swapingPartner->b, (*reversePoints)[ reversePoints->size() - 1 ])) 
            {
                reversePoints->push_back(swapingPartner->b);
            }
            swapingPartner->regularPoints = reversePoints;
        }
        for( int i=0; i<reversePoints->size(); i++)
        {
            printf( " %f %f %d",(*reversePoints)[ i ].x,(*reversePoints)[ i ].y, isSwappedPartnerSwapped);
        }
        cout << "<reverse" << endl;
    }
    
}


void myLine::getShape( vector< ofPolyline *>& shapes )
{
    //順向きパートナーで一周した場合
    if ( reversePoints != NULL && isSamePoint((*reversePoints)[ 0 ], (*reversePoints)[ reversePoints->size() - 1 ]))
    {
        ofPolyline* shape = new ofPolyline();
        for( int i = 0; i < reversePoints->size(); i++)
        {
            shape->addVertex((*reversePoints)[ i ]);
        }
        reverseShapeComplete = true;
        if ( isSwappedPartnerSwapped ) swapingPartner->reverseShapeComplete = true;
        else swapingPartner->regularShapeComplete = true;
        cout << "shape complete" << endl;
        shapes.push_back(shape);
    }
    if ( regularPoints != NULL && isSamePoint((*regularPoints)[ 0 ], (*regularPoints)[ regularPoints->size() - 1 ]))
    {
        ofPolyline* shape = new ofPolyline();
        for( int i = 0; i < regularPoints->size(); i++)
        {
            shape->addVertex((*regularPoints)[ i ]);
        }
        regularShapeComplete = true;
        if ( isPartnerSwapped ) partner->reverseShapeComplete = true;
        else partner->regularShapeComplete = true;
        cout << "shape complete" << endl;
        shapes.push_back(shape);
    }
//        if ( (isPartnerSwapped && partner->shapeSwapSrc == partner->a) ||
//                (!isPartnerSwapped && partner->shapeSrc == partner->b)) 
//        {
//        cout << partner->b << "/" << shapeSrc << endl;
//        isEnd = true;
//        ofPolyline * shape = new ofPolyline();
//        myLine * pt = partner;
//        while( !( pt->isEnd ) )
//        {
//            shape->addVertex(pt->isPartnerSwapped ? pt->a : );
//            pt = pt->partner;
//            cout << "hello" << endl;
//        }
//        shape->addVertex(pt->b);
//        shapes.push_back(shape);
//        cout << "shape complete" << endl;
//        }
}


//http://okwave.jp/qa/q2463144.html
bool myLine::checkLeftAngle(const ofPoint _a, const ofPoint _b)
{
    return true;//(_a.x) * (_b.y) - (_b.x) * (_a.y ) < 0;
}

float myLine::swapPos()
{
    ofVec2f tmp = a;
    a = b;
    b = tmp;
//    myAngle = atan2( (b.y - a.y), (float)(b.x - a.x) );
    return 0.f;
}