#pragma once

#include "ofMain.h"
#include "Obj.h"
#include "ObjHuman.h"
#include "ofxGui.h"
#include "ofxOsc.h"

#define HOST "localhost"

#include "ofVbo.h"
#define NUM_BILLBOARDS 500  //松

#define TEXLIBLINE 4
#define TEXLIBNUM 10

#define FASTMODE

class ofApp : public ofBaseApp{
private:
    ofTrueTypeFont font;
    ofTrueTypeFont font2;
public:
    void setup();
    void update();
    void draw();
    void draw3d();
    
    void keyPressed(int key);
    void keyReleased(int key);
    void mouseMoved(int x, int y );
    void mouseDragged(int x, int y, int button);
    void mousePressed(int x, int y, int button);
    void mouseReleased(int x, int y, int button);
    void windowResized(int w, int h);
    void dragEvent(ofDragInfo dragInfo);
    void gotMessage(ofMessage msg);
    
    ofVideoPlayer timeline;
    bool bTimelinePaused = false;
    
    ofImage slide[4];
    int currentSlide;
    
    ofImage haikei[6];//(0:無し,1:)
    int currentHaikei;
    int preHaikei;//フェード前の背景インデックス
    int countHaikei;//フェード用のカウンタ
    int countHaikeiMax=30;//フェードにかける時間
    
    int sceneId = 0;//現在のシーンID
    int sceneId_1f = 0;//前フレームのシーンID
    bool bsceneChange = false;//シーンが変わったフラグ 前フレームとの比較で判断
    
    ofImage img;
    bool bHideImage;
    bool bBlack;
    bool bHideGui;
    bool bHideInfo;
    int judgeLine;
    int judgeLine_yoko;
    int score;
    
    int syncScore = 0;
    
    ofParameter<int> missThr;
    ofParameter<int> scalex;//8bit
    ofParameter<int> scaley;//8bit
    ofParameter<int> scalez;//8bit
    ofParameter<int> humanscale;
    ofParameter<int> humansizeoffset;
    ofParameter<int> timelineMethod;
    
    ofxPanel gui;
    
    void valChanged(int &val);
    
    vector<Obj> Objects1;
    vector<Obj> Objects2;
    vector<Obj> Objects3;
    vector<Obj> Objects4;
    vector<Obj> longObjects;
    vector<Obj> bigObjects;
    
    ofxOscReceiver receiver;
    ofxOscSender sender;
    
    
    bool bMusicStop;
    bool bMusicPlay;
    bool bMusicReset;
    
    float startTime;
    float timer;
    
    float objVelocity;
    vector<ObjHuman> ObjHumans;
    
    void getMessage2(ofxOscMessage m);
    void getMessage4(ofxOscMessage m);
    void getMessage22(ofxOscMessage m);
    int velx_ave,vely_ave;
    
    void addPoint(float x, float y, float z,float size);
    void addPoint2(float x, float y, float z,float size);
    
    //2D 関連
    bool bDraw2d;
    
    //3D CG 関連
    bool bDraw3d;
    
    bool bFogSw;
    vector <ofVec3f> points;
    vector <ofVec3f> sizes;
    vector <ofVec3f> points2;//間違ってるやつ
    vector <ofVec3f> sizes2;
    //3D Object
    ObjRoad objRoad;
    ObjFrame objFrame;
    int objFrameOffsetx = 0;
    int objFrameOffsety = 0;//1536;
    vector<float> boxScale;
    vector<float> boxScale2;
    
    ofVbo vbo;
    ofShader shader;
    ofShader shader2;
    
    //ofEasyCam camera;
    ofCamera camera;
    ofCamera camera2;
    
    float camDist;
    ofTexture texture;
    ofTexture texture2;
    ofTexture texture3;
    
    //カメラパラメータ
    int cameraCount;
    ofVec3f cameraMoving;//移動するカメラ原点
    int cameraZpos = 0;
    int cameraId;
    bool cameraModeForce = false;//キーコマンドによって強制すげ替え
    int cameraMode = 0;//上に行くか1 前に進むか2 止まるか0
    int bcameradown = false;//上昇するカメラを下に下げる
    
    // 鳥居logo
    ofImage texTorii;
    ObjSimple objTorii;
    
    //ケチャ
    //vector<KetyaBall> Ketyas;
    
    vector<KetyaBillboard> Ketyas_billboard;
    bool bKetya = false;
    
    // 雲たち
    ofImage texCloud;
    vector<ObjSimple> objClouds;
    
    
    vector<ofImage> texTexts;
    vector<ObjSimple> objTexts;
    
    ofImage texSetsumei1;
    ofImage texSetsumei2;
    ofImage texSetsumei3;
    ObjSimple objSetsumei1;
    ObjSimple objSetsumei2;
    ObjSimple objSetsumei3;
    
    
    //虹色
    int rainbow[7][3] = {{0xFF,0x00,0x00},
        {0xFF,0x80,0x00},
        {0xFF,0xFF,0x00},
        {0x00,0x99,0x00},
        {0x00,0x00,0xFF},
        {0x00,0x00,0xCC},
        {0x66,0x00,0xCC}};
    //道路の色
    int roadColor[7][3] = {
        {0x00,0x00,0x00},//
        {0x00,0xCC,0x33},//1A
        {0xCC,0x66,0x33},//1サビ
        {0x99,0x00,0x00},//yoiyoi
        {0x33,0xff,0xff},//ketya
        {0xff,0x66,0x33},//last
        {0xff,0xff,0xff}};//kuro
    
    int roadAlpha[7]={80,255,255,255,80,200,160
    };
    ofVec3f roadColorNow;
    int roadAlphaNow;
    
    //texture library
    //コマンドは　qwertyuiop　行は上下で変更
    char texlib[TEXLIBLINE][TEXLIBNUM][20]={
        {"1","2","3","4","5","6","7","8","9","10"},
        {"11","12","13","14","15","16","17","18","19","20"},
        {"21","22","23","24","matsu","1","2","3","4","5"},
        {"21","22","23","24","matsu","1","2","3","4","5"}};
    
    char texlib2[3][20]={"right","center","left"};
    char texlib3[2][20]={"front","back"};
    char texlib4[2][20]={"good","ng"};
    int commentxpos[3]={300,0,-300};
    int commentypos[2]={200,-200};
    bool commentdraw[3][2][2];//good or ngコメントを出すかどうか
    bool commentsw=false;//good ngどちらのコメントを出すかのスイッチ
    
    /*"7",			"leftfrontng"
     10.png			8.png			matsu.png
     11.png			9.png			midori.png
     13.png			centerbackgood.png	migi.png
     14.png			centerbackng.png	nainainai.png
     15.png			centerfrontgood.png	rightbackgood.png
     16.png			centerfrontng.png	rightbackng.png
     17.png			cloud1.png		rightfrontgood.png
     18.png			cloud2.png		rightfrontng.png
     19.png			dodongadon.png		sabihaikei.png
     2.png			dokkyun.png		sensu.png
     20.png			dot.png			tebyousi.png
     21.png			haikei.png		torii.png
     22.png			hidari.png		yoiyoiyoiyoi.png
     23.png			kawaii.png		yoiyoiyoiyoihaikei.png
     24.png			ketyahaikei.png		zokkon.png
     3.png			lasthaikei.png		zukkyun.png
     4.png			leftbackgood.png	zukkyun2.png
     5.png			leftbackng.png
     6.png			leftfrontgood.png*/
    
    vector<ObjSimple> objLibs;
    vector<ofImage> texLibs;//texlibの指定したファイルを一列にして保存 i*TEXLIBNUM+jでアクセス
    int texlibnum = 0;
    int texid;//
    int texflag;//ボタンが押されると立つ
    
    bool bRainbow = false;  //虹色フラグ
    int objectPct = 0;;     //オブジェクト出現率
    
    int commentId;
    
    // billboard particles（松）
    float billboardSizeTarget[NUM_BILLBOARDS];
    ofShader billboardShader;
    ofImage texture_;
    ofVboMesh billboards;
    ofVec3f billboardVels[NUM_BILLBOARDS];

};
