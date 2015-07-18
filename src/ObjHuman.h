//
//  object.h
//  bapa_visualization
//
//  Created by Toyama Genki on 2015/06/30.
//
//

#pragma once
#include "ofMain.h"


#define KETYA_NUM 3000

class KetyaBillboard {
    ofShader billboardShader_k;
    ofImage texture_k;
    ofVbo billboards_k;
    vector<ofVec3f> billboardVels_kp;//position
    vector<ofVec3f> billboardVels_kv;//vel
    vector<ofVec3f> billboardVels_kc;//center of fire
    vector<ofVec3f> billboardVels_ks;//size
    vector<ofFloatColor> billboardVels_kcolor;//color
    vector<int> billboardVels_klt;//left time
    float size;
    int count;
    int lt=40;
public:
    KetyaBillboard(){};
    KetyaBillboard(float x, float y);
    void setup();
    void update();
    void draw();
    ofVec3f pos; // 炎の位置
};


/*
class KetyaParticle{
private:
    ofPoint p; // 位置
    ofPoint v; // 速度
    ofPoint c; // 炎自体の中心位置
    float size; // 炎の大きさ
    int lt=40; //消えるまでの時間
    
public:
    static ofImage img; // パーティクルの画像を読み込む
    KetyaParticle(){};
    KetyaParticle(ofPoint _p, float _size);
    static void setup();
    void update();
    void draw();
    bool isDead();
};

class KetyaBall {
    vector<KetyaParticle> ps; // パーティクルの配列
    ofPoint pos; // 炎の位置
    float size; // 炎の大きさ
public:
    KetyaBall(){};
    KetyaBall(float x, float y);
    void setup();
    void update();
    void draw();
};*/






class ObjHuman {
    
public:
    ObjHuman();
    
    void setup(float positionX, float positionY,int positionZ,int _speed,int _id,int _mouseStd,int _objMissThr);
    void update();
    void draw();
    
    ofVec2f position;
    int positionz;
    int speed;
    int humanid;//動体検出時のid
    int humanStd;//平均動きからのズレ量
    int count;
    float radius;
    float length;
    int objMissThr;
    
    int width;
    int height;
};

//ライブハウスオブジェクト
class ObjFrame{
public:
    ObjFrame();
    void setup(int _scalex,int _scaley,int _scalez,int _xoffset,int _yoffset);
    void update();
    void draw();
    vector <ofVec3f> posFrom;
    vector <ofVec3f> posTo;
};

//道オブジェクト
class ObjRoad{
public:
    ObjRoad();
    ofVec4f getLeftPos(int idx);//(leftx,lefty,leftz,alpha)
    int getIdxStart();
    int getIdxEnd();
    int getRoadWidth(int idx);//x軸方向の広がり
    void update();
    
    int offsetZ;
    int count;//
    float speed;
    int width;//描画短冊の幅
    int Ythr1;//消え始めるY閾値
    int Ythr2;//完全に消えるY閾値
    
};


class ObjWhiteWall{
public:
    ObjWhiteWall();
    void setup(int y);
    int width;
    int height;
    int offsetZ;
};


class ObjSimple{
public:
    ObjSimple();
    void setup(int _w,int _h,int _x,int _y,int _z);
    void settex(int _i,int _j);
    bool visible(float);
    bool killmyself(float);
    int width;
    int height;
    int x,y,z;
    int visibleyrange;
    void draw(ofImage tex);
    int texidi = 0;
    int texidj = 0;
};
