#pragma once

#include "ofMain.h"
#include "math.h"

class ofApp : public ofBaseApp{

	public:
		void setup();
		void update();
		void draw();

		void keyPressed(int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y );
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void mouseEntered(int x, int y);
		void mouseExited(int x, int y);
		void windowResized(int w, int h);
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);

		float calcTeslaX(float tX, float tY, float tZ);
		float calcTeslaY(float tX, float tY, float tZ);
		float calcTeslaZ(float tX, float tY, float tZ);
		float tmpCalcX(float tX, float tY, float tZ, bool xSign, bool ySign);
		float tmpCalcY(float tX, float tY, float tZ, bool xSign, bool ySign);
		float tmpCalcZ(float tX, float tY, float tZ, bool xSign, bool ySign);
		float tmpRoot(float tX, float tY, float tZ, bool xSign, bool ySign);
		float boolCalc(float in1, float in2, bool add);

		int w, h;
		int magSizeX, magSizeY, magSizeZ;
		float maxteslaX, minteslaX;
		float maxteslaY, minteslaY;
		float maxteslaZ, minteslaZ;

		float Hx, Hy, Hz;

		float minx, miny, minz;

		float teslaX, teslaY, teslaZ;
		
		float rangeXYZ;
};
