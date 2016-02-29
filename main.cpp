#include "FrameBuffer.cpp"
#include <cstring>
#include <termios.h>
#include <fstream>
#include "assets.h"
#include "ThreeDimension.h"

using namespace std;

FrameBuffer FB;
bool quit = false;
vector<ThreeDimension> peta;
int key;

int kbhit(void);
vector<Point> matrixToPolygon(int object[][2], int col);
void drawMap();
void redraw();
void move(int key);

Polygon map_border(matrixToPolygon(border,sizeof(border)/sizeof(*border)));
ThreeDimension p_sumatra(matrixToPolygon(sumatra,sizeof(sumatra)/sizeof(*sumatra)));
ThreeDimension p_kalimantan(matrixToPolygon(kalimantan,sizeof(kalimantan)/sizeof(*kalimantan)));
ThreeDimension p_sulawesi(matrixToPolygon(sulawesi,sizeof(sulawesi)/sizeof(*sulawesi)));
ThreeDimension p_papua(matrixToPolygon(papua,sizeof(papua)/sizeof(*papua)));
ThreeDimension p_jawa(matrixToPolygon(jawa,sizeof(jawa)/sizeof(*jawa)));
ThreeDimension kotak3D (matrixToPolygon(kotak,sizeof(kotak)/sizeof(*kotak)), 30);

int main() {
	// Adjust positions of the islands
	system("clear");
	FB.rasterScan(map_border,135, 206, 235, 0);
	FB.draw3D(kotak3D,0,0,0,0);
	FB.scanLine3D(kotak3D,0,50,0,0);

	int rotation = 0;
	while(!quit){
		if(kbhit()){
			FB.rasterScan(map_border,135, 206, 235, 0);
			FB.draw3D(kotak3D,0,0,0,0);
			FB.scanLine3D(kotak3D,0,50,0,0);
			
			key=getchar();
			//PANGGIL FUNGSI UNTUK REDRAW MOVEMENT
			if(key=='a' && rotation <= 10) {
				FB.rasterScan(map_border,135, 206, 235, 0);
				FB.draw3D(kotak3D,0,0,0,0);
				FB.scanLine3D(kotak3D,0,50,0,0);
				kotak3D.rotate(2);
				rotation++;
			}
			else if(key=='d' && rotation > 0) {
				FB.rasterScan(map_border,135, 206, 235, 0);
				FB.draw3D(kotak3D,0,0,0,0);
				FB.scanLine3D(kotak3D,0,50,0,0);
				kotak3D.rotate(-2);
				rotation--;
			}
			else if(key=='q') {
				quit = true;
			}
		}
	}

	

	return 0;
}

int kbhit(void) {
	struct termios oldt, newt;
	int ch;
	int oldf;

	tcgetattr(STDIN_FILENO, &oldt);
	newt = oldt;
	newt.c_lflag &= ~(ICANON | ECHO);
	tcsetattr(STDIN_FILENO, TCSANOW, &newt);
	oldf = fcntl(STDIN_FILENO, F_GETFL, 0);
	fcntl(STDIN_FILENO, F_SETFL, oldf | O_NONBLOCK);

	ch = getchar();

	tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
	fcntl(STDIN_FILENO, F_SETFL, oldf);

	if(ch != EOF) {
		ungetc(ch, stdin);
		return 1;
	}

	return 0;
}

vector<Point> matrixToPolygon(int object[][2], int col) {
	vector<Point> points;
	points.clear();
	for(int i=0;i<col;i++) {
		points.push_back(Point(object[i][0],object[i][1]));
	}
	return points;
}

void drawMap() {
	FB.drawPolygon(map_border,0, 255, 255,0);
	FB.rasterScan(map_border,135, 206, 235, 0);
	FB.draw3D(p_sumatra,0,100,0,0);
	//FB.scanLine3D(p_sumatra,0,50,0,0);
	FB.draw3D(p_kalimantan,0,100,0,0);
	FB.scanLine3D(p_kalimantan,0,50,0,0);
	FB.draw3D(p_sulawesi,0,100,0,0);
	//FB.scanLine3D(p_sulawesi,0,50,0,0);
	FB.draw3D(p_papua,0,100,0,0);
	//FB.scanLine3D(p_papua,0,50,0,0);
	FB.draw3D(p_jawa,0,100,0,0);
	FB.scanLine3D(p_jawa,0,50,0,0);
}

void move(int key) {
	/*int i = 0;
	if(key=='w'){
		while(i < 10 && window.square.getMinY() > 0) {
			window.moveUp(1);
			i++;
		}
	}
	else if(key=='a'){
		while(i < 10 && window.square.getMinX() > 0) {
			window.moveLeft(1);
			i++;
		}
	}
	else if(key=='d'){
		while(i < 10 && window.square.getMaxX() < 599) {
			window.moveRight(1);
			i++;
		}
	}
	else if(key=='s'){
		while(i < 10 && window.square.getMaxY() < 400) {
			window.moveDown(1);
			i++;
		}
	}
	else if(key=='q') {
		// OTHER KEYS
		quit=true;
		system("clear");
	}

	if (key=='a' || key=='s' || key=='d' || key=='w'){
		//menggambar ulang peta
		drawMap();

		//menggambar ulang window & view
		FB.cleararea(view.P1.x,view.P1.y,view.P2.x,view.P2.y);
	}*/
}