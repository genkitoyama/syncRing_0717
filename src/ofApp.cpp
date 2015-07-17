#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    //ofSetVerticalSync(true);
    ofSetFrameRate(30);
    ofSetCircleResolution(128);
    
    ofSetWindowPosition(1921, 0);
    ofSetFullscreen(true);
    
    img.loadImage("sora.jpg");
    
    for(int i=0;i<4;i++){
        char c1[32];
        sprintf(c1, "slides/s_%04d.png", i+1);
        slide[i].loadImage(c1);
    }
    currentSlide = 0;
    
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
    gui.add(missThr.set("missThr", 100,1,200));//失敗と判定する閾値
    gui.add(scalex.set("3d scale x", 30,1,256));
    gui.add(scaley.set("3d scale y", 30,1,256));
    gui.add(scalez.set("3d scale z", 1,1,256));
    gui.add(humanscale.set("size scale", 2,1,10));
    gui.add(humansizeoffset.set("size offset", 5,1,256));
    gui.add(timelineMethod.set("Timeline Draw Method", 0,0,3));
    
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
    
    //timeline.loadMovie("timeline_0716.mov");      //音あり
    timeline.loadMovie("tl_0716.mp4");              //音なし
    timeline.play();
    timeline.setPaused(true);
    
    //2D関連
    bDraw2d = false;
    
    //ここから3D CG
    bDraw3d = true;
    
    ofDisableArbTex();//画像サイズが２のべき乗でないといけないのを無効化
    ofLoadImage(texture, "textures/dot.png");
    ofLoadImage(texture2, "textures/sensu.png");
    
    //3D Object
    cameraId = 4;
    objFrame.setup(scalex, scaley, scalez, objFrameOffsetx, objFrameOffsety);
    
    texTorii.loadImage("textures/torii.png");
    objTorii.setup(1200, 1200, 0, 800, 300);
    
    
    texCloud.loadImage("textures/cloud1.png");
    for (int i = 0;i<100;i++){
        ObjSimple objbuf;
        objbuf.setup(1000, 1000, 0, i*2000, 0);
        objClouds.push_back(objbuf);
    }
    
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
    
    for (int i = 0; i < 3 ;i++){
        for(int j = 0;j < 2; j++){
            for(int k =0; k < 2; k++){
                ofImage bufimage;
                char bufchar2[40] = "";
                strcat(bufchar2,"textures/");
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
                objbuf2.setup(200, 200, buf_x, buf_y, buf_z);
                objTexts.push_back(objbuf2);
            }
        }
    }
    
    // upload the data to the vbo
    int total = (int)points.size();
    vbo.setVertexData(&points[0], total, GL_STATIC_DRAW);
    vbo.setNormalData(&sizes[0], total, GL_STATIC_DRAW);
    
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
    billboards.getVertices().resize(NUM_BILLBOARDS);
    billboards.getColors().resize(NUM_BILLBOARDS);
    billboards.getNormals().resize(NUM_BILLBOARDS,ofVec3f(0));
    
    // ------------------------- billboard particles
    for (int i=0; i<NUM_BILLBOARDS; i++) {
        billboardVels[i].set(ofRandomf(), -1.0, ofRandomf());
        billboards.getVertices()[i].set(ofRandom(-800, 800),
                                        ofRandom(0, 100000),
                                        ofRandom(-500, 500));
        //billboards.getColors()[i].set(ofColor(255));
        //billboards.getColors()[i].set(ofColor::fromHsb(160, 255, 255));
        billboardSizeTarget[i] = ofRandom(64, 128);
    }
    billboards.setUsage( GL_DYNAMIC_DRAW );
    billboards.setMode(OF_PRIMITIVE_POINTS);
    billboardShader.load("shadersGL2/Billboard");
    
    // we need to disable ARB textures in order to use normalized texcoords
    //ofDisableArbTex();
    texture_.loadImage("textures/matsu.png");
    ofEnableAlphaBlending();
    //
    //
    //---------------------------------------------------------------------
    
    
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
    objRoad.update();
    
    timeline.update();
    timer = ofGetElapsedTimeMillis()-startTime;
    
    //松
    for (int i=0; i<NUM_BILLBOARDS; i++) {
        billboards.setNormal(i,ofVec3f(12 + billboardSizeTarget[i],0,0));
    }
    
    //テクスチャライブラリ
    if(texflag){
        ObjSimple objbuf;
        objbuf.settex(texlibnum,texid);
        objbuf.setup(100, 100, ofRandom(-500,500), cameraMoving.y+800, ofRandom(-100,400));
        objLibs.push_back(objbuf);
        texflag = 0;
    }
    
    //---------------------------------------------
    //    時間制御
    //---------------------------------------------
    if(timer < 70942){     //1番
        texlibnum=0;
        objectPct=20;
    }else if(timer < 101119){   //1サビ
        texlibnum=1;
        objectPct=30;
    }else if(timer < 139857){   //2番
        texlibnum=1;
        objectPct=20;
    }else if(timer < 194000){   //よいよい・ケチャ
        texlibnum=2;
        objectPct=25;
    }else if (timer < 224000){
        texlibnum=2;            //ラスト
        objectPct=40;
        bRainbow = true;
    }else{
    }
    if(timeline.isPlaying()){
        if(ofRandom(0, 100)<objectPct){
            ObjSimple objbuf;
            int id = ((int)ofRandom(0, 10)%10)+1;
            objbuf.settex(texlibnum,id);
            objbuf.setup(100, 100, ofRandom(-500,500), cameraMoving.y+800, ofRandom(-100,400));
            objLibs.push_back(objbuf);
        }
    }
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
    //ofEnableDepthTest();
    
    points.clear();
    sizes.clear();
    points2.clear();
    sizes2.clear();
    
    
    //カメラ設定
    int cx,cy;
    cameraCount++;
    cameraMoving = ofVec3f(0,objRoad.count*objRoad.speed,0);
    camera2.setPosition(camera.getPosition()+cameraMoving);
    camera2.lookAt(cameraMoving,ofVec3f(0,0,1));
    
    switch (cameraId) {
        case 1:
            camera.setPosition(0, -1000, 400);
            camera.lookAt(ofVec3f(0,0,0),ofVec3f(0,0,1));
            break;
        case 2:
            camera.setPosition(800, -800, 300);
            //camera.setPosition(1500, -3000, 800);
            camera.lookAt(ofVec3f(0,0,0),ofVec3f(0,0,1));
            break;
        case 3:
            camera.setPosition(0, 1500, 500);
            camera.lookAt(ofVec3f(0,0,0),ofVec3f(0,0,1));
            break;
        case 4:
            cx = 1200*cos( cameraCount/200.0 );
            cy = 1200*sin( cameraCount/200.0 );
            camera.setPosition(cx, cy, 400);
            camera.lookAt(ofVec3f(0,0,0),ofVec3f(0,0,1));
            break;
        case 5:
            cx = -1200*cos( cameraCount/100.0 )*(cos(cameraCount/100.0)+2)/3;
            cy = 1200*sin( cameraCount/100.0 )*(cos(cameraCount/100.0)+2)/3;
            camera.setPosition(cx, cy, 400*(cos(cameraCount/100.0)+2)/3);
            camera.lookAt(ofVec3f(0,0,0),ofVec3f(0,0,1));
            break;
        default:
            break;
    }
    
    glDepthMask(GL_FALSE);//デプスバッファに書き込むのを禁止する
    
    ofEnableBlendMode(OF_BLENDMODE_ADD);//加算描画 this makes everything look glowy
    ofEnablePointSprites();
    
    
    //道路表示
    //shader2.begin();
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
            ofSetColor(0,200,0,bufpos.w);
        }
        ofBoxPrimitive buf_box;
        buf_box = ofBoxPrimitive(roadwidth, objRoad.width, 1);
        buf_box.setPosition(bufpos_1f.x, bufpos_1f.y+objRoad.width/2, bufpos.z);
        buf_box.draw();
        if(i%20==0){
            ofSetColor(255,0,0,255);
            ofSpherePrimitive a;
            a = ofSpherePrimitive(10, 20);
            a.setPosition(bufpos_1f.x, bufpos_1f.y, 0);
            a.draw();
            ofBoxPrimitive b;
            b = ofBoxPrimitive(roadwidth, 3, 20);
            b.setPosition(bufpos_1f.x, bufpos_1f.y, bufpos.z);
            b.draw();
        }
        bufpos_1f = bufpos;
    }
    camera2.end();
    //shader2.end();
    
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
            buf_speed = (int)(ObjHumans[i].speed*humanscale/50+humansizeoffset);
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
    
    glPointSize(10);
    
    //観客ノード
    texture.bind();
    //texture2.bind();
    //ofSetColor(255);
    ofSetColor(0, 100, 255);
    int total = (int)points.size();
    vbo.setVertexData(&points[0], total, GL_STATIC_DRAW);
    vbo.setNormalData(&sizes[0], total, GL_STATIC_DRAW);
    vbo.draw(GL_POINTS, 0, (int)points.size());
    
    ofSetColor(255, 100, 90);
    total = (int)points2.size();
    vbo.setVertexData(&points2[0], total, GL_STATIC_DRAW);
    vbo.setNormalData(&sizes2[0], total, GL_STATIC_DRAW);
    vbo.draw(GL_POINTS, 0,(int)points2.size());
    texture.unbind();
    //texture2.unbind();
    
    //ライブハウスグリッド描画
    objFrame.draw();
    
    camera.end();
    shader.end();
    
    ofDisablePointSprites();
    ofDisableBlendMode();
    ofDisableAlphaBlending();
    
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
    
    camera2.begin();
    objTorii.draw(texTorii);
    for(int i =0 ;i< objClouds.size();i++){
        if(objClouds[i].visible(cameraMoving.y)){
            objClouds[i].draw(texCloud);
        }
    }
    
    
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
                    objTexts[i*4+j*2+k].draw(texTexts[i*4+j*2+k]);
                }
            }
        }
    }
    camera.end();
    ofDisableAlphaBlending();
    
    
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
    
    //ここから3D CG
    draw3d();
    //ここまで3D CG
    
    //    ofEnableAlphaBlending();
    //    ofSetColor(200,100);
    //    ofRect(0, 0, ofGetWidth(), judgeLine_yoko*2);
    
    //タイムライン描画
    if(currentSlide>=4){
        ofSetColor(255);
        timeline.draw(240, 0);
    }
    
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
    
    //シンクロ率表示
    //ofSetColor(255);
    if(cameraCount%2==0){
        if(sizes.size() || sizes2.size()){
            syncScore = ((int)(sizes.size()*100/(sizes.size()+sizes2.size())) + syncScore*3)/4;
            if(syncScore > 98){
                syncScore = 100;
            }
        }
    }
    if(currentSlide>=4){
        if(syncScore<90){
            ofSetColor(255);
        }else{
            float hue = fmodf(ofGetElapsedTimef()*30,255);
            ofColor c;
            c.setHsb(hue, 230, 230);
            ofSetColor(c);
        }
        font.drawString(ofToString(syncScore),ofGetWidth()/2-100,ofGetHeight()/2+150);
        font2.drawString("%",ofGetWidth()/2+250,ofGetHeight()/2+150);
    }
    
    
    //FPS等の情報
    string info = "FPS: "+ofToString(ofGetFrameRate(), 3);
    //info += "\nObjects num: "+ofToString(Objects.size());
    //info += "\nlongObjects num: "+ofToString(longObjects.size());
    //info += "\npress z: clap x: right c: left";
    info += "\nelapsed time: "+ofToString(ofGetElapsedTimeMillis());
    info += "\ntimer: "+ofToString(timer)+" ms";
    info += "\nParticle Matsu: "+ofToString(NUM_BILLBOARDS);
    info += "\nTexture LineID: "+ofToString(texlibnum);
    
    
    //両側の黒部分
    ofSetColor(0);
    ofFill();
    ofRect(0, 0, 240, 1080);
    ofRect(1680, 0, 240, 1080);
    
    ofSetColor(255);
    if(!bHideGui) gui.draw();                                          //本番時は ! とる
    if(!bHideInfo) ofDrawBitmapString(info, 20, ofGetHeight()-100);
    
    if(currentSlide < 4){                                              //スライド透過表示
        ofEnableAlphaBlending();
        slide[currentSlide].draw(240,0);
        ofDisableAlphaBlending();
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
        Obj o;
        o.setDrawMethod(timelineMethod);
        o.setup4(ofVec2f(ofGetWidth(),judgeLine_yoko), objVelocity);
        Objects4.push_back(o);
    }
    else if(key == 'x'){
        Obj o;
        o.setDrawMethod(timelineMethod);
        o.setup2(ofVec2f(ofGetWidth(),judgeLine_yoko), objVelocity);
        Objects2.push_back(o);
    }
    else if(key == 'c'){
        Obj o;
        o.setDrawMethod(timelineMethod);
        o.setup3(ofVec2f(ofGetWidth(),judgeLine_yoko), objVelocity);
        Objects3.push_back(o);
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
        if(texlibnum<3){
            texlibnum++;
        }
    }
    else if(key == OF_KEY_DOWN){
        if(texlibnum>0){
            texlibnum--;
        }
    }
    else if(key == OF_KEY_RIGHT){       //左右でスライド切り替え
        currentSlide++;
    }
    else if(key == OF_KEY_LEFT){
        currentSlide--;
        if(currentSlide<0) currentSlide=0;
    }
    else if(key == OF_KEY_RETURN){      //enterで曲スタート
        timeline.setPaused(false);
        startTime = ofGetElapsedTimeMillis();
    }
    else if(key == 's') {
        gui.saveToFile("settings.xml");
    }
    else if(key == 'l') {
        gui.loadFromFile("settings.xml");
        
    }else if(key == '1'){
        bFogSw = true;//白霧を切り替えるスイッチ
    }else if(key == '2'){
        bDraw2d = !bDraw2d;
    }else if(key == '3'){
        bDraw3d = !bDraw3d;
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
        timeline.setPosition(0.0);      //最初に戻る
        timeline.play();
        timeline.setPaused(true);
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
