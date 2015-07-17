//
//  object.cpp
//  bapa_visualization
//
//  Created by Toyama Genki on 2015/06/30.
//
//

#include "Obj.h"

Obj::Obj(){
    radius = 40;
    radius_ori = radius;
    bFixed = false;
    startTime = ofGetElapsedTimeMillis();
    frightCount = (int)((ofGetWidth()-280)/3.8925);
    nowCount=0;
}

void Obj::setup1(ofVec2f _position, float _velocity){
    position = _position;
    objVelocity = _velocity;
    startTime = ofGetElapsedTimeMillis();
    setupBezier(_position);
    img1.loadImage("textures/tebyousi.png");
}

void Obj::setup2(ofVec2f _position, float _velocity){
    position = _position;
    objVelocity = _velocity;
    startTime = ofGetElapsedTimeMillis();
    setupBezier(_position);
    img2.loadImage("textures/migi.png");
}

void Obj::setup3(ofVec2f _position, float _velocity){
    position = _position;
    objVelocity = _velocity;
    startTime = ofGetElapsedTimeMillis();
    setupBezier(_position);
    img3.loadImage("textures/hidari.png");
}

void Obj::setup4(ofVec2f _position, float _velocity){
    position = _position;
    objVelocity = _velocity;
    startTime = ofGetElapsedTimeMillis();
    setupBezier(_position);
    img4.loadImage("textures/tebyousi.png");
}


void Obj::setDrawMethod(int a){
    timelineMethod = a;
    //0:従来通りのタイムライン、1:上部、2:縦、3:曲線
}

void Obj::setup(float positionX, float positionY, float velocity){
    position = ofVec2f(positionX, positionY);
    objVelocity = velocity;
    startTime = ofGetElapsedTimeMillis();
    setupBezier(ofVec2f(positionX,positionY));
}


void Obj::setupBezier(ofVec2f _position){
    if (timelineMethod>0) {
        switch (timelineMethod) {
            case 1:
                setStartPosition(_position.x, _position.y);
                setEndPosition(280, _position.y);
                break;
            case 2:
                setStartPosition(ofGetWidth()/2, 0);
                setEndPosition(ofGetWidth()/2, ofGetHeight());
                break;
            default:
                setStartPosition(ofGetWidth()*3/4, ofGetWidth()/20);
                setEndPosition(ofGetWidth()/4, ofGetHeight()*9/10);
                setMidPosition(ofGetWidth()/4, 200, 0, 400);
                break;
        }
    }
}

void Obj::setupLong(ofVec2f _position, float _velocity, float _length){
    position = _position;
    objVelocity = _velocity;
    length = _length;
    startTime = ofGetElapsedTimeMillis();
}

void Obj::updatePos(){
    if (!bFixed) {
        position += velocity;
    }
}

void Obj::update(){
    //updatePos();
    float timer = ofGetElapsedTimeMillis() - startTime;
    position.x = ofGetWidth() - timer/objVelocity;
    if(timelineMethod){
        nowCount++;
        calcPosition(nowCount);
        radius=radius_ori*nowCount/frightCount;
    }
}

void Obj::draw1(){
    img1.draw(position+ofVec2f(-35, -35),70,70);
}
void Obj::draw2(){
    img2.draw(position+ofVec2f(-60, -60),120,120);
}
void Obj::draw3(){
    img3.draw(position+ofVec2f(-60, -60),120,120);
}
void Obj::draw4(){
    img4.draw(position+ofVec2f(-60, -60),120,120);
}

void Obj::draw(){
    ofSetColor(255,alphaPoint);
    ofCircle(position, radius);
    ofSetColor(0, 0, 255,alphaPoint);
    ofCircle(position, radius*0.85);
    ofSetColor(255);
    //ofLine(position.x,position.y-radius, position.x,position.y+radius);
}

void Obj::drawLong(){
    ofSetColor(255,alphaPoint);
    ofCircle(position, radius);
    ofRect(position.x, position.y-radius, length, radius*2);
    ofCircle(position+ofVec2f(length, 0),radius);
    ofSetColor(0, 255, 0,alphaPoint);
    ofCircle(position, radius*0.85);
    ofRect(position.x, position.y-(radius*0.85), length, (radius*0.85)*2);
    ofCircle(position+ofVec2f(length, 0),radius*0.85);
}

void Obj::drawBig(){
    ofSetColor(255,alphaPoint);
    ofCircle(position, radius);
    ofSetColor(255, 0, 0,alphaPoint);
    ofCircle(position, radius*0.85);
}

void Obj::timeReset(){
    startTime = ofGetElapsedTimeMillis();
}

void Obj::fillRed(){
    ofSetColor(255,0,0,200);
    ofCircle(position, radius);
    ofRect(position.x, position.y-radius, length, radius*2);
    ofCircle(position+ofVec2f(length, 0),radius);
}


void Obj::setStartPosition(float positionX, float positionY){
    positionStart=ofVec2f(positionX,positionY);
}

void Obj::setEndPosition(float positionX, float positionY){
    positionEnd=ofVec2f(positionX,positionY);
}

void Obj::setMidPosition(float positionX1, float positionY1,float positionX2, float positionY2){
    positionMid1=ofVec2f(positionX1,positionY1);
    positionMid2=ofVec2f(positionX2,positionY2);
    bbezier = true;
}

void Obj::calcPosition(int count){
    if(bbezier){
        pbufM1=calcBuf(positionStart,positionMid1,count);
        pbufM2=calcBuf(positionMid1,positionMid2,count);
        pbufM3=calcBuf(positionMid2,positionEnd,count);
        pbufB1=calcBuf(pbufM1,pbufM2,count);
        pbufB2=calcBuf(pbufM2,pbufM3,count);
        position=calcBuf(pbufB1, pbufB2, count);
    }else{
        position=calcBuf(positionStart,positionEnd, count);
    }
}

ofVec2f Obj::calcBuf(ofVec2f start,ofVec2f end,int count){
    if(frightCount){
        return (start * (frightCount-count)+ end * count)/frightCount;
    }else{
        return end;
    }
}

