#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    //ofSetVerticalSync(true);
    ofSetFrameRate(30);
    ofSetCircleResolution(32);
    
    //ofSetWindowPosition(1921, 0);
    //ofSetFullscreen(true);
    
    img.loadImage("sora.jpg");
    
    myClearSound.loadSound("clear.mp3"); //サウンドファイルの読込み
    myClearSound.setLoop(false); //ループ再生をONに
    myClearSound.setVolume(0.7);
    
    mySnareSound.loadSound("snareroll.mp3");
    mySnareSound.setLoop(false);
    mySnareSound.setVolume(0.7);
    
    mySound.loadSound("../../../kirakirabgm.mp3"); //サウンドファイルの読込み
    mySound.setLoop(true); //ループ再生をONに
    mySound.setVolume(0.2);
    mySound.play(); //サウンド再生開始
    
    for(int i=0;i<4;i++){
        char c1[32];
        sprintf(c1, "slides/s_%04d.png", i+1);
        slide[i].loadImage(c1);
    }
    currentSlide = 0;

    for(int i=0;i<6;i++){
        char c2[32];
        sprintf(c2, "haikei/h_%04d.png", i+1);
        haikei[i].loadImage(c2);
    }
    currentHaikei = 0;
    
    bHideImage = false;
    bBlack = false;
    bHideGui = false;
    bHideInfo = false;
    bMusicStop = false;
    bMusicPlay = false;
    bMusicReset = false;
    
    judgeLine = 280;
    judgeLine_yoko = 100;
    score = 0;
    
    objVelocity = 3.9355f;
    //objVelocity = 3.8925f;
    //objVelocity = 4.0655f;
    
    gui.setup("panel");
    gui.add(missThr.set("missThr", 30,1,200));//失敗と判定する閾値
    gui.add(scalex.set("3d scale x", 29,1,80));
    gui.add(scaley.set("3d scale y", 29,1,80));
    gui.add(scalez.set("3d scale z", 1,1,80));
    gui.add(humanscale.set("size scale", 4,1,10));
    gui.add(humansizeoffset.set("size offset", 18,1,256));
    gui.add(timelineMethod.set("Timeline Draw Method", 0,0,3));
    gui.add(scorespeedthr.set("SyncScore speed thr", 3,0,80));
    gui.add(scorespeedthr2.set("SyncScore Moving Ratio thr", 20,0,100));
    missThr.addListener(this, &ofApp::valChanged);
    scalex.addListener(this, &ofApp::valChanged);
    scaley.addListener(this, &ofApp::valChanged);
    scalez.addListener(this, &ofApp::valChanged);
    
    startTime = ofGetElapsedTimeMillis();
    
    ofShowCursor();
    
    receiver.setup(12345);
    sender.setup(HOST, 12346);
    
    font.loadFont("parukana_herf.ttf", 300);
    font2.loadFont("parukana_herf.ttf", 150);
    //VALkana/valkana_standard.otf
    timeline.loadMovie("../../../0720.mov");//音あり
    //timeline.loadMovie("tl_0716.mp4");              //音なし
    timeline.play();
    timeline.setPaused(true);
    
    sakura.loadMovie("../../../sakura.mov");
    sakura.play();
    sakura.setPaused(true);
    
    texGameStart.loadImage("tex/start_01.png");
    texZukkyun.loadImage("tex/zukkyun.png");
    texNainai.loadImage("tex/nainai.png");
    texHajimaruyo.loadImage("tex/hajimaruyo.png");
    texKawaii.loadImage("tex/kawaii.png");
    texDokkyun.loadImage("tex/dokkyun.png");
    texYoiyoi.loadImage("tex/yoiyoi.png");
    texDodon.loadImage("tex/dodon.png");
    texZokkon.loadImage("tex/zokkon.png");
    texBakkyun.loadImage("tex/bakkyun.png");
    
    posGameStart = ofPoint(240,0);
    posZukkyun = ofPoint(240,0);
    posNainai = ofPoint(240,200);
    posHajimaruyo = ofPoint(240,0);
    posKawaii = ofPoint(240,0);
    posDokkyun = ofPoint(240,0);
    posYoiyoi = ofPoint(240,0);
    posDodon = ofPoint(240,0);
    posZokkon = ofPoint(240,0);
    posBakkyun = ofPoint(240,0);
    
    //2D関連
    bDraw2d = false;
    
    //ここから3D CG
    bDraw3d = true;
    
    ofDisableArbTex();//画像サイズが２のべき乗でないといけないのを無効化
    ofLoadImage(texture, "textures/dot.png");
    ofLoadImage(texture2, "textures/sensu.png");
    ofLoadImage(texture3, "textures/snow.png");
    ofLoadImage(texturehito, "textures/hito_col.png");
    ofLoadImage(texturehito2, "textures/hito_gl.png");
    
    //3D Object
    cameraId = 4;
    objFrame.setup(scalex, scaley, scalez, objFrameOffsetx, objFrameOffsety);
    
    texTorii.loadImage("textures/torii.png");
    objTorii.setup(1200, 1200, 0, 800, 300);
    
    //ここからケチャ
    /*for(int i =0; i<300 ; i++){
        //Ketyas.push_back(KetyaBall(ofRandom(-100,100), ofRandom(-100,100)));
        Ketyas_billboard.push_back(KetyaBillboard(ofRandom(-500,500), ofRandom(-500,500)));
    }*/
    Ketyas_billboard.push_back(KetyaBillboard(0,0));
    
    for (int i=0;i<Ketyas_billboard.size();i++){
        Ketyas_billboard[i].setup();
    }
    
    //飛行雲
    texCloud.loadImage("textures/cloud1.png");
    for (int i = 0;i<100;i++){
        ObjSimple objbuf;
        objbuf.setup(1000, 1000, 0, i*2000, 0);
        objClouds.push_back(objbuf);
    }
    
    //ランダム飛行物体のテクスチャ
    for (int i = 0;i<TEXLIBLINE;i++){
        for(int j = 0;j<TEXLIBNUM;j++){
            ofImage bufimage;
            char bufchar[40] = "";
            strcat(bufchar,"textures/");
            strcat(bufchar,texlib[i][j]);
            strcat(bufchar,".png");
            bufimage.loadImage(bufchar);
            texLibs.push_back(bufimage);
        }
    }
    
    //応援コメント
    for (int i = 0; i < 3 ;i++){
        for(int j = 0;j < 2; j++){
            for(int k =0; k < 2; k++){
                ofImage bufimage;
                char bufchar2[40] = "";
                strcat(bufchar2,"textures/hyouka");
                strcat(bufchar2,texlib2[i]);
                strcat(bufchar2,texlib3[j]);
                strcat(bufchar2,texlib4[k]);
                strcat(bufchar2,".png");
                bufimage.loadImage(bufchar2);
                texTexts.push_back(bufimage);
                ObjSimple objbuf2;
                
                int buf_x,buf_y,buf_z;
                buf_x = ((commentxpos[i] - objFrameOffsetx)*scalex)>>5; //32等倍
                buf_y = ((commentypos[j] - objFrameOffsety)*scaley)>>5; //32等倍
                buf_z = (300 * scalez) >>5;//32等倍
                objbuf2.setup(400, 400, buf_x, buf_y, buf_z);
                objTexts.push_back(objbuf2);
            }
        }
    }
    
    // load the shader
    
    //道路
    shader2.setGeometryInputType(GL_LINES);
    shader2.setGeometryOutputType(GL_TRIANGLE_STRIP);
    shader2.setGeometryOutputCount(4);
    
    
#ifdef TARGET_OPENGLES
    shader.load("shaders_gles/shader");
#else
    shader.load("shaders/shader");
#endif
    
    //-----------------------------------------------------------------
    //    松を500個ほど配置
    //
//    billboards.getVertices().resize(NUM_BILLBOARDS);
//    billboards.getColors().resize(NUM_BILLBOARDS);
//    billboards.getNormals().resize(NUM_BILLBOARDS,ofVec3f(0));
//    
//    // ------------------------- billboard particles
//    for (int i=0; i<NUM_BILLBOARDS; i++) {
//        billboardVels[i].set(ofRandomf(), -1.0, ofRandomf());
//        billboards.getVertices()[i].set(ofRandom(-800, 800),
//                                        ofRandom(0, 100000),
//                                        ofRandom(-500, 500));
//        //billboards.getColors()[i].set(ofColor(255));
//        //billboards.getColors()[i].set(ofColor::fromHsb(160, 255, 255));
//        billboardSizeTarget[i] = ofRandom(64, 128);
//    }
//    billboards.setUsage( GL_DYNAMIC_DRAW );
//    billboards.setMode(OF_PRIMITIVE_POINTS);
//    billboardShader.load("shadersGL2/Billboard");
//    
//    // we need to disable ARB textures in order to use normalized texcoords
//    //ofDisableArbTex();
//    texture_.loadImage("textures/matsu.png");
//    ofEnableAlphaBlending();
    //
    //
    //---------------------------------------------------------------------
    
    //成績発表テクスチャ
    seiseki1.loadImage("textures/seiseki_kai.png");
    seiseki2.loadImage("textures/seiseki_syncr_new.png");
    
    
}   //setupここまで

void ofApp::valChanged(int &val){
    objFrame.setup(scalex, scaley, scalez, objFrameOffsetx, objFrameOffsety);
}


//--------------------------------------------------------------
void ofApp::update(){
    
    while (receiver.hasWaitingMessages()) {
        //メッセージを取得
        ofxOscMessage m;
        receiver.getNextMessage(&m);
        
        if(m.getAddress() == "/bang/left"){ //名前をチェック
            Obj o;
            o.setup3(ofVec2f(ofGetWidth(),judgeLine_yoko), objVelocity);
            Objects3.push_back(o);
            //cout << "bang fired" << endl;
        }
        else if(m.getAddress() == "/bang/up"){ //名前をチェック
            Obj o;
            o.setup1(ofVec2f(ofGetWidth(),judgeLine_yoko), objVelocity);
            Objects1.push_back(o);
        }
        else if(m.getAddress() == "/bang/right"){ //名前をチェック
            Obj o;
            o.setup2(ofVec2f(ofGetWidth(),judgeLine_yoko), objVelocity);
            Objects2.push_back(o);
        }
        else if(m.getAddress() == "/bang/clap"){ //名前をチェック
            Obj o;
            o.setup4(ofVec2f(ofGetWidth(),judgeLine_yoko), objVelocity);
            Objects4.push_back(o);
        }
        else if(m.getAddress() == "/mouse/position2"){ //名前をチェック
            getMessage2(m);
        }
        else if(m.getAddress() == "/mouse/position4"){ //名前をチェック
            getMessage4(m);
        }
        else if(m.getAddress() == "/mouse/position22"){ //名前をチェック
            getMessage22(m);
        }
    }
    
    for (int i = 0; i < Objects1.size(); i++){
        if(Objects1[i].nowCount > Objects1[i].frightCount){
            Objects1.erase(Objects1.begin()+i);
        }
        if(Objects1[i].position.x < judgeLine+1){
            Objects1.erase(Objects1.begin()+i);
        }
        Objects1[i].update();
    }
    
    for (int i = 0; i < Objects2.size(); i++){
        if(Objects2[i].nowCount > Objects2[i].frightCount){
            Objects2.erase(Objects2.begin()+i);
        }
        if(Objects2[i].position.x < judgeLine+1){
            Objects2.erase(Objects2.begin()+i);
        }
        Objects2[i].update();
    }
    
    for (int i = 0; i < Objects3.size(); i++){
        if(Objects3[i].nowCount > Objects3[i].frightCount){
            Objects3.erase(Objects3.begin()+i);
        }
        if(Objects3[i].position.x < judgeLine+1){
            Objects3.erase(Objects3.begin()+i);
        }
        Objects3[i].update();
    }
    
    for (int i = 0; i < Objects4.size(); i++){
        if(Objects4[i].nowCount > Objects4[i].frightCount){
            Objects4.erase(Objects4.begin()+i);
        }
        if(Objects4[i].position.x < judgeLine+1){
            Objects4.erase(Objects4.begin()+i);
        }
        Objects4[i].update();
    }
    
    for (int i = 0; i < longObjects.size(); i++){
        if(longObjects[i].position.x < -(longObjects[i].radius+longObjects[i].length)){
            longObjects.erase(longObjects.begin()+i);
        }
        longObjects[i].update();
    }
    
    for (int i = 0; i < bigObjects.size(); i++){
        if(bigObjects[i].nowCount > bigObjects[i].frightCount){
            bigObjects.erase(bigObjects.begin()+i);
        }
        if(bigObjects[i].position.x < judgeLine+1){
            bigObjects.erase(bigObjects.begin()+i);
            //score -= 100;
        }
        bigObjects[i].update();
    }
    
    for (int i = 0; i < ObjHumans.size(); i++){
        if(ObjHumans[i].count > 100){
            ObjHumans.erase(ObjHumans.begin()+i);
        }
        ObjHumans[i].update();
    }
    
    if(cameraMode==2){
        objRoad.update();
    }
    
    //ケチャ
    /*for (vector<KetyaBall>::iterator it=Ketyas.begin(); it!=Ketyas.end(); ++it) {
        it->update();
    }*/
    
    /*for (int i=0;i<Ketyas_billboard.size();i++){
        Ketyas_billboard[i].update();
    }*/
    if(bKetya){
//        Ketyas_billboard[0].add(ofVec3f(0,0,0));
  //      Ketyas_billboard[0].add(ofVec3f(100,0,0));
   //     Ketyas_billboard[0].add(ofVec3f(-100,0,0));
        
        for(int i = 0; i< points.size();i++){
            Ketyas_billboard[0].add(points[i]);
        }
        for(int i = 0; i< points2.size();i++){
            Ketyas_billboard[0].add(points2[i]);
        }
        Ketyas_billboard[0].updateonly();

    }
    
    timeline.update();
    timer = ofGetElapsedTimeMillis()-startTime;
    
    sakura.update();
    if(bPausedSakura){
        sakura.setPaused(false);
        bPausedSakura=false;
    }
    
    for (int i = 0; i < fs.size(); i++){
        if((fs[i].hidden)&&(!fs[i].once)){
            fs[i].launch();
        }else if((fs[i].hidden)&&(fs[i].once)){
            fs.erase(fs.begin()+i);
        }
    }
    
//    //松
//    for (int i=0; i<NUM_BILLBOARDS; i++) {
//        billboards.setNormal(i,ofVec3f(12 + billboardSizeTarget[i],0,0));
//    }
    
    if(syncScoreShowFlag && (syncScoreShow==100)){
        for(int i=0;i<2;i++){
            Firework f;
            f.setup((int)ofRandom(7));
            fs.push_back(f);
        }
    }
    
    //テクスチャライブラリ
    if(texflag){
        ObjSimple objbuf;
        objbuf.settex(texlibnum,texid);
        objbuf.setup(100, 100, ofRandom(-500,500), cameraMoving.y+800, ofRandom(-100,400));
        objLibs.push_back(objbuf);
        texflag = 0;
    }
    
    //背景のクロスフェード
    if(countHaikei){
        countHaikei++;
        if (countHaikei==countHaikeiMax) {
            countHaikei=0;
            preHaikei=currentHaikei;
        }
    }else{
        preHaikei=currentHaikei;
    }

    if(timeline.isPlaying()){
        //timer = timer *30;
        //timer = timer *3;
        //---------------------------------------------
        //    時間制御
        //---------------------------------------------
        if(timer < 57000){//1Aまで
            if(timer < 21096){//説明画面中
                sceneId = 1;
                cameraId = 1;
                if(!cameraModeForce)cameraMode=2;
                texlibnum=0;
                objectPct=0;
            }
            else if(timer < 29373){//ぐるぐる
                sceneId = 12;
                cameraId = 5;//ぐるぐるアングル
                if(!cameraModeForce)cameraMode=2;
                objectPct=20;
                
                if(timer > 21500 && timer < 24500){//はじまるよ
                    bHajimaruyo = true;
                }else{
                    bHajimaruyo = false;
                }
            }
            else if(timer < 32064){//止まってスタート
                bGameStart = true;
                sceneId = 13;
                cameraId = 1;//ストップアングル
                if(!cameraModeForce)cameraMode=2;
                objectPct=20;
            }else{//１Aメロの間は上昇
                bGameStart = false;
                sceneId = 14;
                cameraId = 1;
                if(!cameraModeForce)cameraMode=2;
                objectPct=20;
                
                if(timer > 34232 && timer < 35008){     //ずっきゅん
                    bZukkyun = true;
                }else{
                    bZukkyun = false;
                }
                
                if(timer > 41500 && timer < 43176){     //ないない
                    bNainai = true;
                }else{
                    bNainai = false;
                }
                
                if(timer > 45160 && timer < 46000){     //ドッキュン
                    bDokkyun = true;
                }else{
                    bDokkyun = false;
                }
                
            }
        }else if(timer < 70942){     //1B
            sceneId = 15;
            bShowScore=false;
        }else if(timer < 101119){   //1サビ
            bShowScore = true;
            sceneId = 2;
            texlibnum=1;
            objectPct=30;
        }else if(timer < 125878){   //2A
            sceneId = 3;
            texlibnum=1;
            objectPct=20;
            
            if(timer > 103100 && timer < 103930){     //ぞっこん
                bZokkon = true;
            }else{
                bZokkon = false;
            }
            
            if(timer > 110210 && timer < 112091){     //かわいい
                bKawaii = true;
            }else{
                bKawaii = false;
            }
            
            if(timer > 114126 && timer < 114767){     //ばっきゅん
                bBakkyun = true;
            }else{
                bBakkyun = false;
            }
            
        }else if(timer < 139680){   //2B
            sceneId = 31;
            bShowScore = false;
        }else if(timer < 169783){   //2番サビ
            bShowScore = true;
            sceneId = 32;
            texlibnum=1;
            objectPct=30;
        }else if(timer < 182275){   //よいよい
            sceneId = 4;
            texlibnum=2;
            objectPct=20;
            
            if(timer < 173621){
                if(timer < 171741){
                    bYoiyoi=true;
                    bDodon = false;
                }else{
                    bYoiyoi = false;
                    bDodon = true;
                }
            }else if (timer<177029){
                if(timer < 175156){
                    bYoiyoi = true;
                    bDodon = false;
                }else{
                    bYoiyoi = false;
                    bDodon = true;
                }
            }else if(timer < 179909){
                if(timer < 178619){
                    bYoiyoi = true;
                    bDodon = false;
                }else{
                    bYoiyoi=false;
                    bDodon = true;
                }
            }else{
                bYoiyoi = false;
                bDodon = false;
            }
            
        }else if(timer < 194000){   //ケチャ
            sceneId = 42;
            texlibnum=2;
            objectPct=0;
            bShowScore = false;
        }else if (timer < 224000){
            bShowScore = true;
            bPausedSakura = true;
            sceneId = 5;
            texlibnum=2;            //ラスト
            objectPct=35;
        }else if (timer < 230535){
            sceneId = 6;
        }else if (timer < 235535){
            sceneId = 7;
        }else{
            sceneId = 8;
        }
        if(sceneId!=sceneId_1f){
            bsceneChange = true;
            sceneId_1f = sceneId;
        }
        if(bsceneChange){//シーンが変わった瞬間だけ立つ
            switch (sceneId) {
                case 1://説明画面中
                    cameraId = 1;
                    hitoTex = 0;
                    bKetya = false;
                    currentHaikei=6;
                    countHaikei=1;
                    break;
                case 12://ぐるぐる
                    hitoTex = 1;
                    bKetya = false;
                    cameraId = 5;//ぐるぐるアングル
                    currentHaikei=1;
                    countHaikei=1;
                    break;
                case 13://止まってスタート
                    cameraId = 1;//ストップアングル
                    bKetya = false;
                    currentHaikei=1;
                    countHaikei=1;
                    break;
                case 14://１Aメロの間は上昇
                    cameraId = 1;
                    bKetya = false;
                    currentHaikei=1;
                    countHaikei=1;
                    break;
                case 15://１B
                    cameraId = 6;
                    bKetya = false;
                    break;
                case 2://1サビ
                    bcameraRandom = true;
                    bKetya = false;
                    currentHaikei=2;
                    countHaikei=1;
                    break;
                case 3://２番
                    bcameraRandom = false;
                    cameraId = 1;
                    bKetya = false;
                    currentHaikei=1;
                    countHaikei=1;
                    break;
                case 31://２B
                    bcameraRandom = false;
                    cameraId = 6;
                    bKetya = false;
                    currentHaikei=1;
                    countHaikei=1;
                    break;
                case 32://２番サビ
                    bcameraRandom = true;
                    bKetya = false;
                    currentHaikei=2;
                    countHaikei=1;
                    break;
                case 4://よいよい
                    bcameraRandom = false;
                    cameraId = 4;
                    bKetya = false;
                    currentHaikei=3;
                    countHaikei=1;
                    break;
                case 42://ケチャ
                    hitoTex = 2;
                    cameraId = 4;
                    bKetya = true;
                    currentHaikei=4;
                    countHaikei=1;
                    break;
                case 5://ラスト
                    hitoTex = 1;
                    bcameraRandom = true;
                    bKetya = false;
                    bRainbow = true;
                    currentHaikei=5;
                    countHaikei=1;
                    break;
                case 6://早めに流れ止める
                    break;
                case 7://終わった瞬間
                    bfinish = true;
                    break;
                case 8://成績発表
                    bHappyou = true;
                    countHappyou = 0;
                    break;
                default:
                    break;
            }
            bsceneChange=false;
        }
        if(bcameraRandom){
            cameraRandom++;
            if(cameraRandom%80==0){
                cameraRandom=0;
                cameraId = (int)ofRandom(1,10.9);
                if(cameraId == 6){
                    cameraId = 7;
                }
            }
        }
        
        if(ofRandom(0, 100)<objectPct){
            ObjSimple objbuf;
            int id = ((int)ofRandom(0, 10)%10)+1;
            objbuf.settex(texlibnum,id);
            objbuf.setup(100, 100, ofRandom(-500,500), cameraMoving.y+800, ofRandom(-100,400));
            objLibs.push_back(objbuf);
        }
    }
    
    
//    if(bZukkyun){
//        texZukkyun.draw(240, 0);
//    }else if (bNainai){
//        texNainai.draw(240, 200);
//    }else if(bKawaii){
//        texKawaii.draw(240, 0);
//    }else if(bZokkon){
//        texZokkon.draw(240, 0);
//    }else if(bDokkyun){
//        texDokkyun.draw(240, 0);
//    }else if(bGameStart){
//        posGameStart.x += 20;
//    }else if(bHajimaruyo){
//        posHajimaruyo.y += 20;
//    }else if(bBakkyun){
//        texBakkyun.draw(240, 0);
//    }else if(bYoiyoi){
//        texYoiyoi.draw(240,0);
//    }else if(bDodon){
//        texDodon.draw(240, 0);
//    }

}

//--------------------------------------------------------------
void ofApp::addPoint(float x, float y, float z,float size) {
    ofVec3f p(x, y, z);
    points.push_back(p);
    
    // we are passing the size in as a normal x position
    sizes.push_back(ofVec3f(size));
}

void ofApp::addPoint2(float x, float y, float z,float size) {
    ofVec3f p(x, y, z);
    points2.push_back(p);
    
    // we are passing the size in as a normal x position
    sizes2.push_back(ofVec3f(size));
}

//--------------------------------------------------------------
void ofApp::draw3d(){
    points.clear();
    sizes.clear();
    points2.clear();
    sizes2.clear();

    //カメラ設定
    int cx,cy;
    cameraCount++;
    switch(cameraMode){
        case 0:
            break;
        case 1:
            //if(bcameradown){
            //    cameraZpos = -300;
            //    bcameradown=false;
            //}
            //if(cameraZpos<0 && cameraZpos>-20){
            //}else{
            cameraZpos += 100;
            //}
            cameraMoving = ofVec3f(0,objRoad.count*objRoad.speed,cameraZpos);
            break;
        case 2:
            cameraMoving = ofVec3f(0,objRoad.count*objRoad.speed,0);
            break;
    }
    camera2.setPosition(camera.getPosition()+cameraMoving);
    camera2.lookAt(cameraMoving,ofVec3f(0,0,1));
    
    switch (cameraId) {
        case 1:
            camera.setPosition(0, -1000, 200);
            camera.lookAt(ofVec3f(0,0,0),ofVec3f(0,0,1));
            break;
        case 2:
            camera.setPosition(800, -800, 300);
            //camera.setPosition(1500, -3000, 800);
            camera.lookAt(ofVec3f(0,0,0),ofVec3f(0,0,1));
            break;
        case 3:
            camera.setPosition(200, -800, 300);
            camera.lookAt(ofVec3f(0,0,0),ofVec3f(0,0,1));
            break;
        case 4:
            cx = 1200*cos( cameraCount/200.0 );
            cy = 1200*sin( cameraCount/200.0 );
            camera.setPosition(cx, cy, 400);
            camera.lookAt(ofVec3f(0,0,0),ofVec3f(0,0,1));
            break;
        case 5:
            cx = -1200*cos( cameraCount/20.0 )*(cos(cameraCount/20.0)+2)/3;
            cy = 1200*sin( cameraCount/20.0 )*(cos(cameraCount/20.0)+2)/3;
            camera.setPosition(cx, cy, 400*(cos(cameraCount/20.0)+2)/3);
            camera.lookAt(ofVec3f(0,0,0),ofVec3f(0,0,1));
            break;
        case 6:
            camera.setPosition(0, -300, 1000);
            camera.lookAt(ofVec3f(0,0,0),ofVec3f(0,0,1));
            break;
        case 7:
            camera.setPosition(-200, -800, 200);
            camera.lookAt(ofVec3f(0,0,0),ofVec3f(0,0,1));
            break;
        case 8:
            cx = -1200*cos( cameraCount/20.0 );
            cy = 1200*sin( cameraCount/20.0 );
            camera.setPosition(cx, cy, 400*(cos(cameraCount/20.0)+2)/3);
            camera.lookAt(ofVec3f(0,0,0),ofVec3f(0,0,1));
        case 9:
            cx = -1200*cos( cameraCount/40.0 )*(cos(cameraCount/40.0)+2)/3;
            cy = 1200*sin( cameraCount/40.0 )*(cos(cameraCount/40.0)+2)/3;
            camera.setPosition(cx, cy, 1600);
            camera.lookAt(ofVec3f(0,0,0),ofVec3f(0,0,1));
            break;
        case 10:
            cx = 1200*cos( cameraCount/40.0 )*(cos(cameraCount/40.0)+2)/3;
            cy = 1200*sin( cameraCount/40.0 )*(cos(cameraCount/40.0)+2)/3;
            camera.setPosition(cx, cy, 400*(cos(cameraCount/40.0)+2)/3);
            camera.lookAt(ofVec3f(0,0,0),ofVec3f(0,0,1));
            break;
        /*case 11:
            cx = 0;
            cy = -1200+cameraCount*30;
            camera.setPosition(cx, cy, 200);
            camera.lookAt(ofVec3f(0,4000,200),ofVec3f(0,0,1));
            break;*/
        default:
            break;
    }
    
    glDepthMask(GL_FALSE);//デプスバッファに書き込むのを禁止する
    
    ofEnableBlendMode(OF_BLENDMODE_ALPHA);//加算描画 this makes everything look glowy
    ofEnablePointSprites();
    
    //道路表示
    camera2.begin();
    ofVec4f bufpos,bufpos_1f;
    for(int i=objRoad.getIdxStart();i<objRoad.getIdxEnd();i++){
        bufpos = objRoad.getLeftPos(i);
        int roadwidth = objRoad.getRoadWidth(i);
        if(i==objRoad.getIdxStart()){
            continue;
        }
        if(bRainbow){
            ofSetColor(rainbow[(i/10)%7][0],rainbow[(i/10)%7][1],rainbow[(i/10)%7][2],bufpos.w);    //虹色
        }else{
            ofSetColor((int)roadColorNow.x,(int)roadColorNow.y,(int)roadColorNow.z,((int)(bufpos.w*roadAlphaNow)>>8));
        }
        ofBoxPrimitive buf_box;
        buf_box = ofBoxPrimitive(roadwidth, objRoad.width, 1);
        buf_box.setPosition(bufpos_1f.x, bufpos_1f.y+objRoad.width/2, bufpos.z);
        buf_box.draw();
        bufpos_1f = bufpos;
    }
    camera2.end();
    
    ofDisablePointSprites();
    ofDisableBlendMode();
    
    glDepthMask(GL_TRUE);
    
    //観客描画
    //ofEnableBlendMode(OF_BLENDMODE_ADD);//加算描画 this makes everything look glowy
    if(bDraw3d){
        int buf_x,buf_y,buf_z,buf_speed;
        for (int i = 0; i < ObjHumans.size(); i++) {
            buf_x = ObjHumans[i].position.x;
            buf_x = ((buf_x - 512 - objFrameOffsetx)*scalex)>>5; //32等倍
            buf_y = ObjHumans[i].position.y;
            buf_y = ((buf_y - 512 - objFrameOffsety)*scaley)>>5; //32等倍
            buf_z = (ObjHumans[i].positionz * scalez) >>5;//32等倍
            //buf_speed = (int)(ObjHumans[i].speed*humanscale/50+humansizeoffset);
            buf_speed = (int)(humanscale/50+humansizeoffset);
            if(ObjHumans[i].humanStd <= ObjHumans[i].objMissThr){
                
                if(ObjHumans[i].humanStd>=0){
                    addPoint(buf_x, buf_y, buf_z,buf_speed*2);
                }
                addPoint(buf_x, buf_y, buf_z,buf_speed*2);
                boxScale.push_back(buf_speed);
            }else{
                addPoint2(buf_x, buf_y, buf_z,buf_speed*2);
                boxScale2.push_back(buf_speed);
            }
        }
    }
    glDepthMask(GL_FALSE);

    
    
    
    // this makes everything look glowy :)
    ofEnablePointSprites();
    ofEnableBlendMode(OF_BLENDMODE_ADD);
    ofEnableAlphaBlending();
    shader.begin();
    camera.begin();
    
    //観客ノード
    glPointSize(10);
    switch (hitoTex) {
        case 0:
            texture.bind();
            ofSetColor(0, 100, 255);
            break;
        case 1:
            texturehito.bind();
            ofSetColor(255, 255, 255);
            break;
        case 2:
            break;
        default:
            break;
    }
    int total = (int)points.size();
    if(total){
        vbo.setVertexData(&points[0], total, GL_STATIC_DRAW);
        vbo.setNormalData(&sizes[0], total, GL_STATIC_DRAW);
        vbo.draw(GL_POINTS, 0, (int)points.size());
    }
    if(hitoTex==1){
        texturehito.unbind();
        texturehito2.bind();
    }
    if(hitoTex==0)ofSetColor(255, 100, 90);
    total = (int)points2.size();
    if(total){
        vbo.setVertexData(&points2[0], total, GL_STATIC_DRAW);
        vbo.setNormalData(&sizes2[0], total, GL_STATIC_DRAW);
        vbo.draw(GL_POINTS, 0,(int)points2.size());
    }
    if(hitoTex==1)texturehito2.unbind();
    if(hitoTex==0)texture.unbind();
    //ケチャ
    if(bKetya){
        /*for(int i = 0; i< points.size();i++){
            Ketyas_billboard[0].update(points[i]);
        }
        for(int i = 0; i< points2.size();i++){
            Ketyas_billboard[0].update(points2[i]);
        }*/
        texture.bind();
        //ofSetColor(230, 230, 255);
        vector<ofVec3f> sizebuf;
        vector<ofVec3f> posbuf;
        vector<ofFloatColor> colorbuf;
        for(int i = 0;i<Ketyas_billboard.size();i++){
            posbuf = Ketyas_billboard[i].getpos();
            sizebuf = Ketyas_billboard[i].getsize();
            colorbuf = Ketyas_billboard[i].getcolor();
            total = (int)sizebuf.size();
            vbo2.setVertexData(&posbuf[0], total, GL_STATIC_DRAW);
            vbo2.setNormalData(&sizebuf[0], total, GL_STATIC_DRAW);
            vbo2.setColorData(&colorbuf[0], total, GL_STATIC_DRAW);
            vbo2.draw(GL_POINTS, 0,(int)posbuf.size());
        }
        texture.unbind();
    }
    camera.end();
    shader.end();
    ofDisablePointSprites();
    ofDisableBlendMode();
    ofDisableAlphaBlending();
    
    
    //ライブハウスグリッド描画
    camera.begin();
    if(currentHaikei==6 || preHaikei==6){
        if(currentHaikei == preHaikei){
            ofSetColor(255);
        }else if(currentHaikei==6){
            ofSetColor((int)(255.0*countHaikei/countHaikeiMax));
        }else{
            ofSetColor((int)(255.0*(countHaikeiMax-countHaikei)/countHaikeiMax));
        }
    }else{
        ofSetColor(0);
    }
    objFrame.draw();
    camera.end();


    //基準座標
    camera2.begin();
    ofSetLineWidth(1);
    
    /*ofSetColor(255,0,0);
     ofLine(ofVec3f(0,0,0), ofVec3f(300,0,0));
     ofSetColor(0,255,0);
     ofLine(ofVec3f(0,0,0), ofVec3f(0,300,0));
     ofSetColor(0,0,255);
     ofLine(ofVec3f(0,0,0), ofVec3f(0,0,300));*/
    //ここから松
    //    ofEnableAlphaBlending();
    //    // bind the shader so that wee can change the
    //    // size of the points via the vert shader
    //    billboardShader.begin();
    //    ofEnablePointSprites(); // not needed for GL3/4
    //    texture_.getTextureReference().bind();
    //    billboards.draw();
    //    texture_.getTextureReference().unbind();
    //    ofDisablePointSprites(); // not needed for GL3/4
    //    billboardShader.end();
    //    ofDisableAlphaBlending();
    //ここまで松
    camera2.end();
    
    /*float boxSize = 100;
     ofFill();
     ofSetColor(255);
     ofDrawBox(boxSize);
     ofNoFill();*/
    //ba.drawNormals(1000);
    //texture3.getTextureReference().unbind();
    ofEnableAlphaBlending();
    
    if(sceneId==1)return;//sceneが説明なら表示終わる
    
    camera2.begin();
    objTorii.draw(texTorii);
    for(int i =0 ;i< objClouds.size();i++){
        if(objClouds[i].visible(cameraMoving.y)){
            objClouds[i].draw(texCloud);
        }
    }
    
    //飛んでくるオブジェクト
    for(int i =0 ;i< objLibs.size();i++){
        if(objLibs[i].visible(cameraMoving.y)){
            objLibs[i].draw(texLibs[objLibs[i].texidi*TEXLIBNUM+objLibs[i].texidj]);
        }else{
            if(objLibs[i].killmyself(cameraMoving.y)){
                objLibs.erase(objLibs.begin()+i);
            }
        }
    }
    
    camera2.end();
    
    //ここから応援コメント
    camera.begin();
    ofSetColor(255,255,255);
    for (int i = 0; i < 3 ;i++){
        for(int j = 0;j < 2; j++){
            for(int k =0; k < 2; k++){
                if(commentdraw[i][j][k]){
                    objTexts[i*4+j*2+k].drawYoko(texTexts[i*4+j*2+k]);
                }
            }
        }
    }
    //ここからケチャ
    ofEnableAlphaBlending();
    
    /*KetyaParticle::img.bind();
    for (vector<KetyaBall>::iterator it=Ketyas.begin(); it!=Ketyas.end(); ++it) {
        it->draw();
    }
    KetyaParticle::img.unbind();*/
    /*texture.bind();
    for (int i=0;i<Ketyas_billboard.size();i++){
        Ketyas_billboard[i].draw();
    }
    texture.unbind();*/
    
    ofDisableAlphaBlending();
    camera.end();
    
    glDepthMask(GL_TRUE);
}

//--------------------------------------------------------------
void ofApp::draw(){
    ofBackground(200,200,170);
    //ofBackground(30);
    //ofBackgroundGradient(ofColor(255), ofColor(214, 240, 214));
    //ofBackgroundGradient(ofColor(255), ofColor(127, 127, 255));
    
    
    //最悪の時用画像
    ofSetColor(255);
    if(bBlack){
        img.draw(0,0,ofGetWidth(),ofGetHeight());
        return;
    }

    //背景表示、　同時に道路の色も決定する
    ofSetColor(255,255,255,255);
    if(currentHaikei==preHaikei && currentHaikei>0){
        ofEnableAlphaBlending();
        haikei[currentHaikei-1].draw(240,0);
        ofDisableAlphaBlending();
        //道路の色
        roadColorNow = ofVec3f(roadColor[currentHaikei][0],roadColor[currentHaikei][1],roadColor[currentHaikei][2]);
        roadAlphaNow = roadAlpha[currentHaikei];
    }else{
        if(currentHaikei < 7 && currentHaikei>0){
            ofEnableBlendMode(OF_BLENDMODE_ALPHA);
            ofEnableAlphaBlending();
            haikei[preHaikei-1].draw(240,0);
            ofSetColor(255,255,255,(int)(255.0*countHaikei/countHaikeiMax));
            haikei[currentHaikei-1].draw(240,0);
            ofDisableAlphaBlending();
            ofDisableBlendMode();
            //道路の色
            ofVec3f bufa,bufb;
            bufb = ofVec3f(roadColor[preHaikei][0],roadColor[preHaikei][1],roadColor[preHaikei][2]);
            bufa = ofVec3f(roadColor[currentHaikei][0],roadColor[currentHaikei][1],roadColor[currentHaikei][2]);
            roadColorNow = (bufa * countHaikei + bufb * (countHaikeiMax-countHaikei))/countHaikeiMax;
            roadAlphaNow = (int)((roadAlpha[currentHaikei] * countHaikei + roadAlpha[preHaikei] * (countHaikeiMax-countHaikei))/countHaikeiMax);
        }
    }
    
    //桜
    if(currentHaikei == 5){
        ofSetColor(255);
        sakura.draw(240, 0);
    }
    
    if(currentSlide>=4){
        ofSetColor(255);
        timeline.draw(240, 0);
    }
        
    //ここから3D CG
    draw3d();
    //ここまで3D CG
    
    //    ofEnableAlphaBlending();
    //    ofSetColor(200,100);
    //    ofRect(0, 0, ofGetWidth(), judgeLine_yoko*2);
    
    //タイムライン描画
    
    //    if(timelineMethod<=1){
    //    ofSetLineWidth(2);
    //    ofLine(0, judgeLine_yoko, ofGetWidth(), judgeLine_yoko);
    //    ofLine(judgeLine, 0, judgeLine, judgeLine_yoko*2);
    //    ofNoFill();
    //    ofCircle(judgeLine, judgeLine_yoko, 40);
    //    ofFill();
    //    for(int i=0;i<16;i++){
    //        ofLine(i*80+40, judgeLine_yoko-5, i*80+40, judgeLine_yoko+5);
    //        }
    //    }
    
    //タイムラインのガイドライン
    //    if(timelineMethod>1){
    //        ofSetLineWidth(1);
    //        Obj buf_o;
    //        buf_o.setDrawMethod(timelineMethod);
    //        buf_o.setup(0,0,0);
    //        ofVec2f p1,p2;
    //        p1=buf_o.positionStart;
    //        for(int i=0;i<buf_o.frightCount;i++){
    //            buf_o.update();
    //            p2=buf_o.position;
    //            ofLine(p1,p2);
    //            p1=p2;
    //        }
    //        ofNoFill();
    //        ofCircle(buf_o.positionEnd, 35);
    //        ofFill();
    //    }
    
    ofEnableAlphaBlending();
    ofSetColor(255);
    for (int i = 0; i < Objects1.size(); i++) {
        Objects1[i].draw1();
    }
    for (int i = 0; i < Objects2.size(); i++) {
        Objects2[i].draw2();
    }
    for (int i = 0; i < Objects3.size(); i++) {
        Objects3[i].draw3();
    }
    for (int i = 0; i < Objects4.size(); i++) {
        Objects4[i].draw4();
    }
    //ofSetColor(0, 255, 0);
    for (int i = 0; i < longObjects.size(); i++) {
        longObjects[i].drawLong();
    }
    for (int i = 0; i < bigObjects.size(); i++) {
        bigObjects[i].drawBig();
    }
    ofDisableAlphaBlending();
    
    //ofSetColor(255);
    //ofRect(0, 0, judgeLine-80, 250);
    
    //    float timer = ofGetElapsedTimeMillis() - startTime;
    //    int x = ofGetWidth() - timer/3.561f;
    //    int y = ofGetHeight()/2;
    //
    //    ofSetColor(0, 0, 255);
    //    ofCircle(x,y,30);
    
    //こっから動体描画
    if(bDraw2d){
        for (int i = 0; i < ObjHumans.size(); i++) {
            ObjHumans[i].draw();
        }
    }
    
    ofEnableAlphaBlending();
    if(bZukkyun){
        texZukkyun.draw(posZukkyun);
    }else if (bNainai){
        texNainai.draw(posNainai);
    }else if(bKawaii){
        texKawaii.draw(posKawaii);
    }else if(bZokkon){
        texZokkon.draw(posZokkon);
    }else if(bDokkyun){
        texDokkyun.draw(posDokkyun);
    }else if(bGameStart){
        texGameStart.draw(posGameStart);
    }else if(bHajimaruyo){
        texHajimaruyo.draw(posHajimaruyo);
    }else if(bBakkyun){
        texBakkyun.draw(posBakkyun);
    }else if(bYoiyoi){
        texYoiyoi.draw(posYoiyoi);
    }else if(bDodon){
        texDodon.draw(posDodon);
    }
    ofDisableAlphaBlending();
    
    //シンクロ率表示
    //ofSetColor(255);
    if((sizes.size() + sizes2.size()) > 20){
        syncScoreBuf = (int)(sizes.size()*100/(sizes.size()+sizes2.size()));
        syncScoreBuf+=scoreOffSet;//魔法のオフセット
        if(syncScoreBuf>100)syncScoreBuf=100;
        if(syncScoreBuf<0)syncScoreBuf=0;
        if(velx_ave*velx_ave+vely_ave*vely_ave > scorespeedthr){
            syncScore = syncScoreBuf;
            syncScoreMoving++;
        }else{
            syncScoreMovingNot++;
        }
    }
    if(cameraCount%30==0){
    //if(cameraCount%1==0){
        syncScoreShow = syncScore;
        //syncScore = ((int)(sizes.size()*100/(sizes.size()+sizes2.size())) + syncScore*3)/4;
        if(syncScoreShow > 99){
            syncScoreShow = 100;
        }
        if(!bfinish)scoreLog.push_back(syncScoreShow);//終わっていなければログに追加
        if(syncScoreMovingNot || syncScoreMoving){
            if((syncScoreMoving*100.0/(syncScoreMoving+syncScoreMovingNot)) > scorespeedthr2){
                syncScoreShowFlag = true;
            }else{
                syncScoreShowFlag = false;
            }
        }
        syncScoreMoving2 = syncScoreMoving;
        syncScoreMovingNot2 = syncScoreMovingNot;
        syncScoreMoving=0;
        syncScoreMovingNot=0;
    }
    //if(!bfinish)scoreLog.push_back(syncScoreShow);//終わっていなければログに追加
    if(currentSlide>=4 && syncScoreShowFlag && (!bfinish)){
        if(syncScoreShow<90){
            ofSetColor(255);
        }else{
            float hue = fmodf(ofGetElapsedTimef()*30,255);
            ofColor c;
            c.setHsb(hue, 230, 230);
            ofSetColor(c);
        }
        if(bShowScore&&timeline.isPlaying()){
        font.drawString(ofToString(syncScoreShow),ofGetWidth()/2-100,ofGetHeight()/2+150);
        if(syncScoreShow==100){
            font2.drawString("%",ofGetWidth()/2+350,ofGetHeight()/2+150);
        }else{
            font2.drawString("%",ofGetWidth()/2+250,ofGetHeight()/2+150);
        }
        }
    }
    
    //FPS等の情報
    string info = "FPS: "+ofToString(ofGetFrameRate(), 3);
    //info += "\nObjects num: "+ofToString(Objects.size());
    //info += "\nlongObjects num: "+ofToString(longObjects.size());
    //info += "\npress z: clap x: right c: left";
    info += "\nelapsed time: "+ofToString(ofGetElapsedTimeMillis());
    info += "\ntimer: "+ofToString(timer)+" ms";
    info += "\nTexture LineID: "+ofToString(texlibnum);
    info += "\nHaikei: "+ofToString(currentHaikei)+":"+ofToString(countHaikei);
    info += "\nScene: "+ofToString(sceneId)+":"+ofToString(sceneId_1f);
    info += "\nvel_ave: "+ofToString(velx_ave)+":"+ofToString(vely_ave);
    info += "\nsyncscore: "+ofToString(syncScoreBuf)+":"+ofToString(syncScore)+":"+ofToString(syncScoreShow);
    info += "\nScoreMoving"+ofToString(syncScoreMovingNot2)+":"+ofToString(syncScoreMoving2);
    info += "\nScoreLogSize "+ofToString(scoreLog.size());

    //両側の黒部分
    ofSetColor(0);
    ofFill();
    ofRect(0, 0, 240, 1080);
    ofRect(1680, 0, 240, 1080);
    
    ofSetColor(255);
    if(bHideGui) gui.draw();                                          //本番時は ! とる
    if(bHideInfo) ofDrawBitmapString(info, 20, ofGetHeight()-300);
    
    if(currentSlide < 4){                                              //スライド透過表示
        ofEnableAlphaBlending();
        slide[currentSlide].draw(240,0);
        ofDisableAlphaBlending();
    }
    
    if(bHappyou){
        countHappyou++;
        int happyounum;
        float histwidth;
        ofEnableAlphaBlending();
        ofEnableBlendMode(OF_BLENDMODE_ALPHA);
        ofSetColor(255,255,255,200);
        //haikei[5].draw(240,0);
        ofFill();
        ofRect(0,0,ofGetWidth(),ofGetHeight());
        ofSetColor(255,255,255,255);
        //seiseki1.draw(1200, 600, 90, 90);//回
        //seiseki2.draw(300, 500, 430, 195);//100%なった回数のテキスト
        seiseki1.draw(1200, 200, 90, 90);//回
        seiseki2.draw(300, 100, 430, 195);//100%なった回数のテキスト
        ofSetLineWidth(25);
        if(countHappyou>30 && (scoreLog.size()>0)){
            if(countHappyou==31){
                mySnareSound.play();
                timeline.stop();
            }
            if(countHappyou==(scoreLog.size()+30)){
                myClearSound.play();
                mySnareSound.stop();
                
                for(int i=0;i<40;i++){
                    Firework f;
                    f.setup((int)ofRandom(7));
                    fs.push_back(f);
                }
            }
            happyounum=MIN((int)((countHappyou-30)),scoreLog.size()-1);
            histwidth = (900.0/scoreLog.size());
            //ofSetColor(255,255,255,128);
            //ofLine(20,700,820,700);
            //ofLine(40,100,40,720);
            ofColor c;
            ofFill();
            for(int i =0;i<happyounum;i++){
                //if(i==0)continue;
                c.setHsb(255*i/scoreLog.size(), 255, 255);
                ofSetColor((int)c.r,(int)c.g,(int)c.b,255);
                ofRect((int)(280+i*histwidth), 900-4*scoreLog[i], histwidth,scoreLog[i]*4);
                //ofLine((int)(280+i*histwidth),500-4*scoreLog[i-1],(int)(280+(i+1)*histwidth),500-4*scoreLog[i]);
            }
            if(happyounum!=(scoreLog.size()-1)){
                //font.drawString(ofToString(scoreLog[happyounum]),780,300);//フレームごとのスコアログ
                font2.drawString(ofToString(scoreLog[happyounum]),780,700);//フレームごとのスコアログ
                //font2.drawString("%",1150,300);
                font2.drawString("%",1100,700);
            }if(countHappyou-30 <= scoreLog.size()){
                if(scoreLog[happyounum]==100){
                    count100p++;
                }
            }
            //font.drawString(ofToString(count100p),780,750);//100％になった回数
            ofSetColor(255,0,0,255);
            font.drawString(ofToString(count100p),780,350);//100％になった回数
        }
        ofDisableAlphaBlending();
    }
    
    //花火
    for (int i = 0; i < fs.size(); i++){
        fs[i].draw();
    }

}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    if(key == 'b') bBlack = !bBlack;
    else if(key == 'h') bHideImage = !bHideImage;
    else if(key == 'g') {
        bHideGui = !bHideGui;
        bHideInfo = !bHideInfo;
    }
    else if(key == 'z'){
        scoreOffSet++;
    }
    else if(key == 'x'){
        scoreOffSet--;
    }
    else if(key == 'c'){
        scoreOffSet=0;
    }
    else if(key == 'v'){
        scoreOffSet=100;
    }
    
    /*else if(key == 'p'){
     ofxOscMessage m;
     m.setAddress("/duration/play");
     sender.sendMessage(m);
     bMusicStop = false;
     bMusicReset = false;
     bMusicPlay = true;
     }
     else if(key == 's'){
     ofxOscMessage m;
     m.setAddress("/duration/stop");
     sender.sendMessage(m);
     bMusicPlay = false;
     bMusicReset = false;
     bMusicStop = true;
     score = 0;
     }
     else if(key == 'r'){
     ofxOscMessage m;
     m.setAddress("/duration/stop");
     sender.sendMessage(m);
     bMusicPlay = false;
     bMusicStop = false;
     bMusicReset = true;
     objVelocity = 3.9095f;
     score = 0;
     }*/
    else if(key == OF_KEY_UP){
        //if(texlibnum<3){
        //    texlibnum++;
        //}
        missThr++;
        cout<<missThr<<endl;
    }
    else if(key == OF_KEY_DOWN){
        //if(texlibnum>0){
        //    texlibnum--;
        //}
        missThr--;
        cout<<missThr<<endl;
    }
    else if(key == OF_KEY_RIGHT){       //左右でスライド切り替え
        currentSlide++;
        if(currentSlide>=4 && currentSlide <= 7){
            if(currentSlide==4){
                mySound.stop();
                currentHaikei=6;
                countHaikei=countHaikeiMax-1;
                cameraId = 1;
            }
            sceneId = 1;
        }
    }
    else if(key == OF_KEY_LEFT){
        currentSlide--;
        if(currentSlide==3){
            mySound.play();
            currentHaikei=0;
            countHaikei=countHaikeiMax-1;
            cameraId = 4;
        }
        if(currentSlide<0) currentSlide=0;
    }
    else if(key == OF_KEY_RETURN && (currentSlide>=4)){      //enterで曲スタート
        timeline.setPaused(false);
        startTime = ofGetElapsedTimeMillis();
        scoreLog.clear();
    }
    else if(key == 's') {
        gui.saveToFile("settings.xml");
    }
    else if(key == 'l') {
        gui.loadFromFile("settings.xml");
        
    }else if(key == '1'){
        bFogSw = true;//白霧を切り替えるスイッチ
    }else if(key == '2'){
        //bDraw2d = !bDraw2d;
        cameraCount = 0;
        cameraId--;
    }else if(key == '3'){
        cameraCount = 0;
        cameraId++;
        //bDraw3d = !bDraw3d;
    }else if(key == '4') {
        points.clear();
    }else if(key == '5') {
        cameraCount = 0;
        cameraId = 1;
    }else if(key == '6') {
        cameraCount = 0;
        cameraId = 2;
    }else if(key == '7') {
        cameraCount = 0;
        cameraId = 3;
    }else if(key == '8') {
        cameraCount = 0;
        cameraId = 4;
    }else if(key == '9') {
        cameraCount = 0;
        cameraId = 5;
    }else if(key == ' '){       //松
        //        billboards.getVertices().resize(NUM_BILLBOARDS);
        //        billboards.getColors().resize(NUM_BILLBOARDS);
        //        billboards.getNormals().resize(NUM_BILLBOARDS,ofVec3f(0));
        //
        //        // ------------------------- billboard particles
        //        for (int i=0; i<NUM_BILLBOARDS; i++) {
        //
        //            billboardVels[i].set(ofRandomf(), -1.0, ofRandomf());
        //            billboards.getVertices()[i].set(camera2.getPosition()+
        //                                            ofVec3f(ofRandom(-800, 800),
        //                                            ofRandom(0, 100000),
        //                                            ofRandom(-1000, 0)));
        //
        //            //billboards.getColors()[i].set(ofColor(255));
        //            //billboards.getColors()[i].set(ofColor::fromHsb(160, 255, 255));
        //            billboardSizeTarget[i] = ofRandom(64, 128);
        //
        //        }
        
        timeline.setPaused(true);       //曲ストップ
//        timeline.setPosition(0.0);      //最初に戻る
//        timeline.play();
//        timeline.setPaused(true);
    }
    else if(key == 'q'){
        texflag = 1;
        texid = 1;
    }
    else if(key == 'w'){
        texflag = 1;
        texid = 2;
    }
    else if(key == 'e'){
        texflag = 1;
        texid = 3;
    }
    else if(key == 'r'){
        texflag = 1;
        texid = 4;
    }
    else if(key == 't'){
        texflag = 1;
        texid = 5;
    }
    else if(key == 'y'){
        texflag = 1;
        texid = 6;
    }
    else if(key == 'u'){
        texflag = 1;
        texid = 7;
    }
    else if(key == 'i'){
        texflag = 1;
        texid = 8;
    }
    else if(key == 'o'){
        texflag = 1;
        texid = 9;
    }
    else if(key == 'p'){
        texflag = 1;
        texid = 10;
    }
    else if(key == ';'){
        commentdraw[2][0][(int)commentsw]= !commentdraw[2][0][(int)commentsw];
    }
    else if(key == ':'){
        commentdraw[1][0][(int)commentsw]= !commentdraw[1][0][(int)commentsw];
    }
    else if(key == ']'){
        commentdraw[0][0][(int)commentsw]= !commentdraw[0][0][(int)commentsw];
    }
    else if(key == '.'){
        commentdraw[2][1][(int)commentsw]= !commentdraw[2][1][(int)commentsw];
    }
    else if(key == '/'){
        commentdraw[1][1][(int)commentsw]= !commentdraw[1][1][(int)commentsw];
    }
    else if(key == '_'){
        commentdraw[0][1][(int)commentsw]= !commentdraw[0][1][(int)commentsw];
    }
    else if(key == '['){
        commentsw= !commentsw;
    }
    else if(key == '-'){
        //変わりきる前にコマンド押すと背景がパカッて変わるので背景が全部変わってから次の背景チェンジを行うこと
        if(currentHaikei<6){
            currentHaikei++;
            countHaikei=1;
        }
    }
    else if(key == '^'){
        if(currentHaikei>0){
            currentHaikei--;
            countHaikei=1;
        }
    }
    else if(key == '0'){
        currentHaikei=0;
    }
    else if(key == '@'){
        if(cameraMode<2){
            cameraMode++;
        }else{
            cameraMode=0;
        }
    }
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){
}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){
    
}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){
    
}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){
    for(int i=0;i<20;i++){
        Firework f;
        f.setup((int)ofRandom(7));
        fs.push_back(f);
    }
}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){
    
}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){
    
}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){
    
}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){
    
}

//--------------------------------------------------------------
void ofApp::getMessage2(ofxOscMessage m){
    ObjHumans.clear();
    int mousenum;
    mousenum = m.getNumArgs();
    for (int j=0 ; j < mousenum/4 ; j++){
        //叩かれた座標
        mouseX = m.getArgAsInt32(0+j*3);
        mouseY = m.getArgAsInt32(1+j*3);
        //物体検出id
        int mouseID;
        mouseID = m.getArgAsInt32(2+j*3);
        //平均動きからのズレ
        int mouseStd;
        mouseStd = m.getArgAsInt32(j+mousenum/4*3);
        ObjHuman o;
        o.setup(mouseX,mouseY,0,5,mouseID,mouseStd,missThr);
        ObjHumans.push_back(o);
    }
}

//--------------------------------------------------------------
void ofApp::getMessage4(ofxOscMessage m){
    ObjHumans.clear();
    int mousenum;
    mousenum = m.getNumArgs();
    for (int j=0 ; j < mousenum/5 ; j++){
        //叩かれた座標
        mouseX = m.getArgAsInt32(0+j*5);
        mouseY = m.getArgAsInt32(1+j*5);
        //物体検出id
        int mouseZ;
        mouseZ = m.getArgAsInt32(2+j*5);
        int mouseSpeed;
        mouseSpeed = m.getArgAsInt32(3+j*5);
        //平均動きからのズレ
        int mouseStd;
        mouseStd = m.getArgAsInt32(4+j*5);
        //cout << ofToString(mouseStd) << endl;
        ObjHuman o;
        o.setup(mouseX,mouseY,mouseZ,mouseSpeed,0,mouseStd,missThr);
        ObjHumans.push_back(o);
    }
}

//--------------------------------------------------------------
void ofApp::getMessage22(ofxOscMessage m){
    int msgnum;
    msgnum = m.getNumArgs();
    if(msgnum==2){
        velx_ave = m.getArgAsInt32(0);
        vely_ave = m.getArgAsInt32(1);
    }
}
