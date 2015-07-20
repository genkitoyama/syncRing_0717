//
//  Fireworks.cpp
//  hanabi2
//
//  Created by Toyama Genki on 2015/07/18.
//
//

#include "Firework.h"

Firework::Firework(){
    launched = false;
    exploded = false;
    hidden = true;
    once = false;
}

void Firework::setup(int num){
    if(num > 7) return;
    flareNum = num;
}

void Firework::draw(){
    if((launched)&&(!exploded)&&(!hidden)){
        launchMaths();
//        ofSetLineWidth(1);
//        ofSetColor(255);
//        ofLine(x,y,oldX,oldY);
    }
    if((!launched)&&(exploded)&&(!hidden)){
        explodeMaths();
        ofSetColor(flare[flareNum]);
        for(int i = 0; i < flareAmount + 1; i++){
            ofPushMatrix();
            ofTranslate(x,y);
            float posX = sin(ofDegToRad(i*flareAngle))*explodeTimer*3;
            float posY = cos(ofDegToRad(i*flareAngle))*explodeTimer*3;
            ofCircle(posX, posY, flareWeight*1.2);
            ofSetLineWidth(flareWeight);
            ofLine(posX*0.9,posY*0.9,posX*0.4,posY*0.4);
            //ofLine(posX*0.4,posY*0.4,posX*0.6,posY*0.6);
            ofPopMatrix();
        }
    }
    if((!launched)&&(!exploded)&&(hidden)){
        //do nothing
    }
}

void Firework::launch(){
    x = oldX = ofGetWidth()/2 + ((ofRandom(5)*10) - 25);
    y = oldY = ofGetHeight()/2;
    targetX = ofGetWidth()/2+ofRandomf()*300;
    targetY = ofGetHeight()/2+ofRandomf()*300;
    ySpeed = ofRandom(3) + 2;
//    flare = ofColor(ofRandom(2)*100+55,ofRandom(2)*100+55,ofRandom(2)*100+55);
    flareAmount = 30;
    flareWeight = 2+ceil(ofRandom(3));
    duration = ceil(ofRandom(4))*10 + 40;
    flareAngle = 360/flareAmount;
    launched = true;
    exploded = false;
    hidden = false;
}

void Firework::launchMaths(){
    oldX = x;
    oldY = y;
    if(ofDist(x,y,targetX,targetY) > 6){
        x += (targetX - x);
        y += -ySpeed*2;
    }else{
        explode();
    }
}

void Firework::explode(){
    explodeTimer = 0;
    launched = false;
    exploded = true;
    hidden = false;
}

void Firework::explodeMaths(){
    if(explodeTimer < duration){
        explodeTimer+=3;
    }else{
        hide();
    }
}

void Firework::hide(){
    launched = false;
    exploded = false;
    hidden = true;
    once = true;
}
