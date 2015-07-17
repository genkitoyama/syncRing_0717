//
//  object.h
//  bapa_visualization
//
//  Created by Toyama Genki on 2015/06/30.
//
//

#pragma once
#include "ofMain.h"

class Obj {
    
public:
    Obj();
    
    void setup1(ofVec2f _position, float _velocity);
    void setup2(ofVec2f _position, float _velocity);
    void setup3(ofVec2f _position, float _velocity);
    void setup4(ofVec2f _position, float _velocity);
    void setup(float positionX, float positionY, float velocity);
    void setupBezier(ofVec2f _position);
    void setDrawMethod(int a);
    void setupLong(ofVec2f _position, float _velocity, float _length);
    
    void updatePos();
    void update();
    void draw();
    void draw1();
    void draw2();
    void draw3();
    void draw4();
    void drawLong();
    void drawBig();
    void timeReset();
    void fillRed();
    
    float objVelocity;
    
    ofImage img1;
    ofImage img2;
    ofImage img3;
    ofImage img4;
    
    ofVec2f position;
    ofVec2f velocity;
    float radius;
    float radius_ori;
    bool bFixed;
    
    int alphaPoint = 128;//点の不透明度
    
    float length;
    
    float startTime;
    float posX;
    
    //タイムライン描画方法
    int timelineMethod = 0;
    int nowCount;
    int frightCount;//到達点にたどり着くまでの時間（フレーム数）
    //参考URL http://blog.sigbus.info/2011/10/bezier.html
    bool bbezier = false;//移動にベジェ曲線を使うかどうか
    ofVec2f positionStart;
    ofVec2f positionEnd;
    ofVec2f positionMid1;//ビジェ曲線を引くための中間点
    ofVec2f positionMid2;//ビジェ曲線を引くための中間点
    ofVec2f pbufM1,pbufM2,pbufM3,pbufB1,pbufB2;
    void setStartPosition(float positionX, float positionY);
    void setEndPosition(float positionX, float positionY);
    void setMidPosition(float positionX1, float positionY1,float positionX2, float positionY2);
    void calcPosition(int count);
    ofVec2f calcBuf(ofVec2f start,ofVec2f end,int count);
};