#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
	//ofSetWindowShape(2160, 1440);
	ofSetWindowShape(1000, 1000);
	maxteslaX = maxteslaY = 15; //myu
	minteslaX = minteslaY = -15;
	maxteslaZ = 15;
	minteslaZ = -15;

	kijunX = 0; kijunY = 0; kijunZ = 0;

	Hx = Hy = Hz = 0;
	w = ofGetWidth();
	h = ofGetHeight();

	magSizeX = 50.0;
	magSizeY = 50.0;
	magSizeZ = 50.0;

	rangeXYZ = 20;

	receiver.setup(3333);

	inX = 0, inY = 0, inZ = 0;

	cam.setFov(80.0f);                 // カメラの水平視野角を８０度に設定
	cam.setDistance(1.0f);          // カメラと見ているものの距離を1mに設定
	cam.setPosition(0, 0, 200); // カメラの位置を設定
	//cam.setTarget(model.getPosition()); // カメラが見る対象物を設定
	//cam.lookAt(model.getPosition(), ofVec3f(0, -1, 0)); // 見る対象物の位置と、上向き方向を設定
}

//--------------------------------------------------------------
void ofApp::update(){
	while (receiver.hasWaitingMessages())
	{
		ofxOscMessage m;
		//cout << m.getAddress() << endl;
		receiver.getNextMessage(&m);
		/*if (m.getAddress() == "/compassData") {
			inX = m.getArgAsDouble(0);
			inY = m.getArgAsDouble(1);
			inZ = m.getArgAsDouble(2);
		}*/
		if (m.getAddress() == "/node/heading") {
			//heading = m.getArgAsFloat(0);
		}
		if (m.getAddress() == "/node/raw") {
			inX = m.getArgAsInt(0);
			inY = m.getArgAsInt(1);
			inZ = m.getArgAsInt(2);
		}
	}
	float minXYZ;
	minx = miny = minz = minXYZ = INT_MAX;
	//Hx = ofMap(inX, 0, w, minteslaX, maxteslaX);
	//Hy = ofMap(inY, 0, h, minteslaY, maxteslaY);
	//Hz = ofMap(inZ, 0, 0, minteslaZ, maxteslaZ);
	Hx = inX - kijunX;
	Hy = inY - kijunY;
	Hz = inZ - kijunZ;
	cout << "inx:" << Hx << ",iny:" << Hy << ",inz:" << Hz << endl;
	//-------------------
	float n = 0.8; //interval
	mx = my = mz = 0.0;
	for (float x = -rangeXYZ; x <= rangeXYZ; x = x + n) {
		for (float y = -rangeXYZ; y <= rangeXYZ; y = y + n) {
			for (float z = -rangeXYZ; z <= rangeXYZ; z = z + n) {
				float tmpX = 0;
				float tmpY = 0;
				float tmpZ = 0;
				/*if (tmpX < minx) {
					minx = tmpX;
					mx = x;
				}
				if (tmpY < miny) {
					miny = tmpY;
					my = y;
				}
				if (tmpZ < minz) {
					minz = tmpZ;
					mz = y;
				}*/
				try {
					tmpX = calcTeslaX(x, y, z) - calcTeslaX(x, y, z - magSizeZ);
					tmpY = calcTeslaY(x, y, z) - calcTeslaY(x, y, z - magSizeZ);
				}
				catch (const char * str)
				{
					cout << "Caught exception: " << str << endl;
				}
				
				try {
					tmpZ = calcTeslaZ(x, y, z) - calcTeslaZ(x, y, z - magSizeZ);
				}
				catch (const char * str)
				{
					cout << "Caught exception: " << str << endl;
				}
				//cout << "tmpx:" << tmpX << ",tmpy:" << tmpY << ",tmpz:" << tmpZ << endl;
				/*if (abs(tmpX - Hx) < minx)	
				if (abs(tmpY - Hy) < miny) 
				if (abs(tmpZ - Hz) < minz)*/
				if(pow(abs(tmpX - Hx), 2) + pow(abs(tmpY - Hy), 2) + pow(abs(tmpZ - Hz), 2) < minXYZ){
					mx = x;
					my = y;
					mz = z;
					minXYZ = pow(abs(tmpX - Hx), 2) + pow(abs(tmpY - Hy), 2) + pow(abs(tmpZ - Hz), 2);
					minx = abs(tmpX - Hx);
					miny = abs(tmpY - Hy);
					minz = abs(tmpZ - Hz);
				}
			}
		}
	}
	//cout << "tethlaX:" << calcTeslaX(14.0, 0, 0) << ",:" << Hx << endl;
	cout << "x:" << mx << ",y:" << my << ",z:" << mz<<",minXYZ:" <<minXYZ<< endl;
	cout << "minx:" << minx << ",miny:" << miny << ",minz:" << minz << endl;
}

float ofApp::boolCalc(float in1, float in2, bool add) {
	if (add) {
		return in1 + in2;
	}
	else {
		return in1 - in2;
	}
}

float ofApp::calcTeslaX(float tX, float tY, float tZ) {
	return log((tmpCalcY(tX, tY, tZ, false, true)*tmpCalcY(tX, tY, tZ, true, false)) / (tmpCalcY(tX, tY, tZ, true, true)*tmpCalcY(tX, tY, tZ, false, false)));
}
float ofApp::calcTeslaY(float tX, float tY, float tZ) {
	return log((tmpCalcX(tX, tY, tZ, true, false)*tmpCalcX(tX, tY, tZ, false, true)) / (tmpCalcX(tX, tY, tZ, true, true)*tmpCalcX(tX, tY, tZ, false, false)));
}
float ofApp::calcTeslaZ(float tX, float tY, float tZ) {
	return (tmpCalcZ(tX, tY, tZ, true, true) - tmpCalcZ(tX, tY, tZ, true, false)) - (tmpCalcZ(tX, tY, tZ, false, true) - tmpCalcZ(tX, tY, tZ, false, false));
}
float ofApp::tmpCalcY(float tX, float tY, float tZ, bool xSign, bool ySign) {
	//return boolCalc(tX, magSizeX, xSign);
	return boolCalc(tY, magSizeY, ySign) + tmpRoot(tX, tY, tZ, xSign, ySign);
}
float ofApp::tmpCalcX(float tX, float tY, float tZ, bool xSign, bool ySign) {
	//return boolCalc(tY, magSizeY, ySign);
	return boolCalc(tX, magSizeX, xSign) + tmpRoot(tX, tY, tZ, xSign, ySign);
}

float ofApp::tmpCalcZ(float tX, float tY, float tZ, bool xSign, bool ySign) {
	float tmp = boolCalc(tX, magSizeX, xSign)*boolCalc(tY, magSizeY, ySign);
	return atan2(tmp, (tmpRoot(tX, tY, tZ, xSign, ySign)*tZ));
}
float ofApp::tmpRoot(float tX, float tY, float tZ, bool xSign, bool ySign) {
	return sqrt(pow(boolCalc(tX, magSizeX, xSign), 2) + pow(boolCalc(tY, magSizeY, ySign), 2) + pow(tZ, 2));
}

//--------------------------------------------------------------
void ofApp::draw(){
	ofBackground(255);
	
	cam.begin();
	
	//ofDrawCircle(ofGetWidth()/2.0 + mx *4.0, ofGetHeight()/2.0 + my * 4.0, 20);
	//ofTranslate(w / 2, h / 2, -10);
	/*ofRotateX(20);
	ofRotateY(40);
	ofRotateZ(-5);*/
	ofSetColor(20, 70, 5);
	//ofDrawCircle(mx *4.0, my * 4.0, 20);
	ofPushMatrix();
	int boxSize = 100;
	ofTranslate(ofMap(mx, -rangeXYZ, rangeXYZ, -boxSize/2.0, boxSize/2.0), ofMap(my, -rangeXYZ, rangeXYZ, -boxSize / 2.0, boxSize / 2.0), ofMap(mz, -rangeXYZ, rangeXYZ, -boxSize / 2.0, boxSize / 2.0));
	ofSphere(5);
	ofPopMatrix();
	
	ofSetColor(0, 0, 0, 30);
	box.set(boxSize);
	box.drawWireframe();
	//ofBox(boxSize);
	cam.end();
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
	kijunX = inX;
	kijunY = inY;
	kijunZ = inZ;
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
void ofApp::mouseEntered(int x, int y){

}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y){

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
