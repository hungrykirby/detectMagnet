#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
	//ofSetWindowShape(2160, 1440);
	ofSetWindowShape(400, 400);
	maxteslaX = maxteslaY = 15; //myu
	minteslaX = minteslaY = -15;
	maxteslaZ = 0;
	minteslaZ = 0;

	Hx = Hy = Hz = 0;
	w = ofGetWidth();
	h = ofGetHeight();

	magSizeX = 5;
	magSizeY = 4;
	magSizeZ = 2;

	rangeXYZ = 15;
}

//--------------------------------------------------------------
void ofApp::update(){
	minx = miny = minz = 50000;
	Hx = ofMap(mouseX, 0, w, minteslaX, maxteslaX);
	Hy = ofMap(mouseY, 0, h, minteslaY, maxteslaY);
	//cout << "Hx:" << Hx << ",Hy:" << Hy << endl;
	Hz = ofMap(0, 0, 0, minteslaZ, maxteslaZ);
	//-------------------
	float n = 3.0; //interval
	float mx, my, mz = 0.0; //memorize x, y, z
	for (float x = -rangeXYZ; x <= rangeXYZ; x = x + n) {
		for (float y = -rangeXYZ; y <= rangeXYZ; y = y + n) {
			for (float z = -rangeXYZ; z <= rangeXYZ; z = z + n) {
				float tmpX, tmpY, tmpZ = 0;
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
					tmpX = calcTeslaX(x, y, z);
					tmpY = calcTeslaY(x, y, z);
				}
				catch (const char * str)
				{
					cout << "Caught exception: " << str << endl;
				}
				
				try {
					tmpZ = calcTeslaZ(x, y, z);
				}
				catch (const char * str)
				{
					cout << "Caught exception: " << str << endl;
				}
				//cout << "tmpx:" << tmpX << ",tmpy:" << tmpY << ",tmpz:" << tmpZ << endl;
				if (abs(tmpX - Hx) < minx) {
					if (abs(tmpY - Hy) < miny) {
						if (abs(tmpZ - Hz) < minz) {
							mx = x;
							my = y;
							mz = z;
							minx = abs(tmpX - Hx);
							miny = abs(tmpY - Hy);
							minz = abs(tmpZ - Hz);
						}
					}
				}
			}
		}
	}
	//cout << "tethlaX:" << calcTeslaX(14.0, 0, 0) << ",:" << Hx << endl;
	cout << "x:" << mx << ",y:" << my << ",z:" << mz << endl;
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
	return (atan(tmpCalcZ(tX, tY, tZ, true, true)) - atan(tmpCalcZ(tX, tY, tZ, true, false)) - (atan(tmpCalcZ(tX, tY, tZ, false, true)) - tmpCalcZ(tX, tY, tZ, false, false)));
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
	return tmp / (tmpRoot(tX, tY, tZ, xSign, ySign)*tZ);
}
float ofApp::tmpRoot(float tX, float tY, float tZ, bool xSign, bool ySign) {
	return sqrt(pow(boolCalc(tX, magSizeX, xSign), 2) + pow(boolCalc(tY, magSizeY, ySign), 2) + pow(tZ, 2));
}

//--------------------------------------------------------------
void ofApp::draw(){

}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){

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
