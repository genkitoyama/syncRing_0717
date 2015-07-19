//
//  Firework.h
//  hanabi2
//
//  Created by Toyama Genki on 2015/07/18.
//
//

#pragma once

#include "ofMain.h"

class Firework{
    
public:
    float x, y, oldX,oldY, ySpeed, targetX, targetY, explodeTimer, flareWeight, flareAngle;
    int flareAmount, duration;
    bool launched,exploded,hidden,once;
    ofColor flare[7] = {ofColor(255,0,0),ofColor(0,255,0),ofColor(0,0,255),ofColor(255,255,0),
        ofColor(255,0,255),ofColor(0,255,255),ofColor(255,145,0)};
    int flareNum;
    
    Firework();
    
    void setup(int num);
    void draw();
    void launch();
    void launchMaths();
    void explode();
    void explodeMaths();
    void hide();
};