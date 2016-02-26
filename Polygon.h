#ifndef POLYGON_H
#define POLYGON_H

#include "Point.h"
#include <string>
#include "math.h"

class Polygon {
public:
	Polygon();
	
	Polygon(string str);

	Polygon(vector<Point> P);

	Polygon& operator<<(const vector<Point>& P);
	
	void insert(Point point);

	float getMaxX();

	float getMinX();

	float getMaxY();

	float getMinY();

	float getMidY();

	float getMidX();

	void moveRight(float dx);

	void moveLeft(float dx);

	void moveUp(float dy);

	void moveDown(float dy);
	
	void rotate(double dg);

	void rotateCenter(double dg);

	void scale(float size);

	void zoomIn(float k);

	void zoomOut(float k);

	int n; //jumlah titik
	vector<Point> e; //titik2 poligon
	Point firePoint;

private:

};

#endif
