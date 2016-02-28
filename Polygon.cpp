#include "Polygon.h"

Polygon::Polygon() {
	n = 0;
}

Polygon::Polygon(vector<Point> P) {
	n = P.size();
	e = P;
}

Polygon& Polygon::operator<<(const vector<Point>& P) {
	n = P.size();
	e = P;
	e.push_back(e[0]);

	return *this;
}

void Polygon::insert(Point point) {
	e[n] = point;
	e.push_back(e[0]);
	++n;
}

float Polygon::getMaxX() {
	int i=1;
	float maxX=e[0].x;
	while(i<n) {
		if(e[i].x>maxX) {
			maxX=e[i].x;
		}
		i++;
	}
	return maxX;
}

float Polygon::getMinX() {
	int i=1;
	float minX=e[0].x;
	while(i<n) {
		if(e[i].x<minX) {
			minX=e[i].x;
		}
		i++;
	}
	return minX;
}

float Polygon::getMaxY() {
	int i=1;
	float maxY=e[0].y;
	while(i<n) {
		if(e[i].y>maxY) {
			maxY=e[i].y;
		}
		i++;
	}
	return maxY;
}

float Polygon::getMinY() {
	int i=1;
	float minY=e[0].y;
	while(i<n) {
		if(e[i].y<minY) {
			minY=e[i].y;
		}
		i++;
	}
	return minY;
}

float Polygon::getMidX() {
	int i=1;
	float maxX=e[0].x;
	while(i<n) {
		if(e[i].x>maxX) {
			maxX=e[i].x;
		}
		i++;
	}

	i=1;
	float minX=e[0].x;
	while(i<n) {
		if(e[i].x<minX) {
			minX=e[i].x;
		}
		i++;
	}

	return (maxX + minX)/2;
}

float Polygon::getMidY() {
	int i=1;
	float maxY=e[0].y;
	while(i<n) {
		if(e[i].y>maxY) {
			maxY=e[i].y;
		}
		i++;
	}

	i=1;
	float minY=e[0].y;
	while(i<n) {
		if(e[i].y<minY) {
			minY=e[i].y;
		}
		i++;
	}

	return (maxY + minY)/2;
}

void Polygon::moveRight(float dx) {
	int i=0;
	while(i<e.size())
	{
		e[i].x+=dx;
		i++;
	}
}

void Polygon::moveLeft(float dx) {
	int i=0;
	while(i<e.size())
	{
		e[i].x-=dx;
		i++;
	}
}


void Polygon::moveUp(float dy) {
	int i=0;
	while(i<e.size())
	{
		e[i].y-=dy;
		i++;
	}
}

void Polygon::moveDown(float dy) {
	int i=0;
	while(i<e.size())
	{
		e[i].y+=dy;
		i++;
	}
}

void Polygon::rotate(double deg) {
	int i=0;
	double cons=0.0174532925; double tempx,tempy;

	while(i < n) {
		tempx= ((cos(deg*cons)*(e[i].x - e[0].x)) - (sin(deg*cons)*(e[i].y - e[0].y)) + e[0].x)+0.5;
		tempy= ((sin(deg*cons)*(e[i].x - e[0].x)) + (cos(deg*cons)*(e[i].y - e[0].y)) + e[0].y)+0.5;
		e[i].x = tempx;
		e[i].y = tempy;
		i++;
	}
}

void Polygon::rotateCenter(double deg) {
	int i=0;
	double cons=0.0174532925; double tempx,tempy;

	float centerX=getMidX(), centerY=getMidY();
	//printf("%f\n", centerX); printf("%f\n", centerY);

	//TRANSLATE TO ORIGIN
	//double x1 = p1.getX() - center.x;
	//double y1 = p1.getY() - center.y;

	//APPLY ROTATION
	//double temp_x1 = x1 * Math.cos(angle) - y1 * Math.sin(angle));
	//double temp_y1 = x1 * Math.sin(angle) + y1 * Math.cos(angle));

	//TRANSLATE BACK
	//p1.setLocation(temp_x1 + center.x, temp_y1 + center.y);

	while(i < e.size()) {
		tempx= ((cos(deg*cons)*(e[i].x - centerX)) - (sin(deg*cons)*(e[i].y - centerY)) + centerX) + 0.5;
		tempy= ((sin(deg*cons)*(e[i].x - centerX)) + (cos(deg*cons)*(e[i].y - centerY)) + centerY) + 0.5;
		e[i].x = tempx;
		e[i].y = tempy;
		i++;
	}
}

void Polygon::scale(float size) {
	for (int i=0;i<e.size();i++) {
        e[i].x = e[i].x*size;
        e[i].y = e[i].y*size;
    }
}

void Polygon::zoomIn(float k){
	int i;
	float midX = getMidX();
	float midY = getMidY();

	for(i=0; i<e.size(); i++) {
		e[i].x = (e[i].x - midX) * k + midX;
		e[i].y = (e[i].y - midY) * k + midY;
	}
}
void Polygon::zoomOut(float k){
	int i;
	float midX = getMidX();
	float midY = getMidY();

	for(i=0; i<e.size(); i++) {
		e[i].x = (e[i].x - midX) / k + midX;
		e[i].y = (e[i].y - midY) / k + midY;
	}
}