#include "ThreeDimension.h"

ThreeDimension::ThreeDimension(){
	frontside = Polygon();
	backside = Polygon();
}

ThreeDimension::ThreeDimension(vector<Point> pol) {
	frontside = Polygon(pol);

	backside = Polygon(pol);
	backside.moveUp(10);
	backside.moveRight(10);

	for (int i=0; i<pol.size()-1; i++) {
		lines.push_back(Line(frontside.e[i], backside.e[i]));

		vector<Point> temp;
		if (i==pol.size()-1) {
			temp.push_back(frontside.e[i]);
			temp.push_back(frontside.e[0]);
			temp.push_back(backside.e[0]);
			temp.push_back(backside.e[i]);
		} else {
			temp.push_back(frontside.e[i]);
			temp.push_back(frontside.e[i+1]);
			temp.push_back(backside.e[i+1]);
			temp.push_back(backside.e[i]);
		}
		p.push_back(Polygon(temp));
	}
}

ThreeDimension::ThreeDimension(vector<Point> pol, int d) {
	frontside = Polygon(pol);

	backside = Polygon(pol);
	backside.moveUp(d);
	backside.moveRight(d);

	for (int i=0; i<pol.size()-1; i++) {
		lines.push_back(Line(frontside.e[i], backside.e[i]));

		vector<Point> temp;
		if (i==pol.size()-1) {
			temp.push_back(frontside.e[i]);
			temp.push_back(frontside.e[0]);
			temp.push_back(backside.e[0]);
			temp.push_back(backside.e[i]);
		} else {
			temp.push_back(frontside.e[i]);
			temp.push_back(frontside.e[i+1]);
			temp.push_back(backside.e[i+1]);
			temp.push_back(backside.e[i]);
		}
		p.push_back(Polygon(temp));
	}
}

ThreeDimension::ThreeDimension(const ThreeDimension& ob) {
	frontside = ob.frontside;
	backside = ob.backside;
	lines = ob.lines;
	p = ob.p;
}

ThreeDimension& ThreeDimension::operator=(const ThreeDimension& ob) {
	frontside = ob.frontside;
	backside = ob.backside;
	lines = ob.lines;
	p = ob.p;

	return *this;
}

void ThreeDimension::moveLeft (float dx) {
	int bound = 10;
	frontside.moveLeft(dx);
	backside.moveLeft(dx);
	for(int i = 0; i<lines.size(); i++){
		lines[i].moveLeft(dx);
		p[i].moveLeft(dx);
	}
}

void ThreeDimension::moveRight (float dx) {
	int bound = 390;
	frontside.moveRight(dx);
	backside.moveRight(dx);
	for(int i = 0; i<lines.size(); i++){
		lines[i].moveRight(dx);
		p[i].moveRight(dx);
	}
}

void ThreeDimension::moveUp (float dy) {
	int bound = 10;
	frontside.moveUp(dy);
	backside.moveUp(dy);
	for(int i = 0; i<lines.size(); i++){
		lines[i].moveUp(dy);
		p[i].moveUp(dy);
	}
}

void ThreeDimension::moveDown (float dy) {
	int bound = 489;
	frontside.moveDown(dy);
	backside.moveDown(dy);
	for(int i = 0; i<lines.size(); i++){
		lines[i].moveDown(dy);
		p[i].moveDown(dy);
	}
}

void ThreeDimension::scale (float k) {
	frontside.scale(k);
	backside.scale(k);

	p.clear();
	lines.clear();

	for (int i=0; i<frontside.e.size()-1; i++) {
		lines.push_back(Line(frontside.e[i], backside.e[i]));

		vector<Point> temp;
		if (i==frontside.e.size()-1) {
			temp.push_back(frontside.e[i]);
			temp.push_back(frontside.e[0]);
			temp.push_back(backside.e[0]);
			temp.push_back(backside.e[i]);
		} else {
			temp.push_back(frontside.e[i]);
			temp.push_back(frontside.e[i+1]);
			temp.push_back(backside.e[i+1]);
			temp.push_back(backside.e[i]);
		}
		p.push_back(Polygon(temp));
	}
}

void ThreeDimension::rotate(int k){
	int fcenter = frontside.getMidX();
	int bcenter = backside.getMidX();
	
	for (int i=0; i<frontside.e.size(); i++){
		if (frontside.e[i].x < fcenter){
			frontside.e[i].x += k;
			frontside.e[i].y -= k/2;
			backside.e[i].x += k;
			backside.e[i].y -= k/2;
		} else if (frontside.e[i].x > fcenter) {
			frontside.e[i].x -= k;
			frontside.e[i].y += k/2;
			backside.e[i].x -= k;
			backside.e[i].y += k/2;
		} else { //frontside.e[i].x == fcenter
			// do nothing
		}
	}

	lines.clear();
	p.clear();

	for (int i=0; i<frontside.e.size()-1; i++) {
		lines.push_back(Line(frontside.e[i], backside.e[i]));

		vector<Point> temp;
		if (i==frontside.e.size()-1) {
			temp.push_back(frontside.e[i]);
			temp.push_back(frontside.e[0]);
			temp.push_back(backside.e[0]);
			temp.push_back(backside.e[i]);
		} else {
			temp.push_back(frontside.e[i]);
			temp.push_back(frontside.e[i+1]);
			temp.push_back(backside.e[i+1]);
			temp.push_back(backside.e[i]);
		}
		p.push_back(Polygon(temp));
	}
}