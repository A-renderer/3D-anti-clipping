#ifndef THREEDIMENSION_H
#define THREEDIMENSION_H

#include "Polygon.h"
#include "Line.h"
#include "Point.h"
#include <vector>


class ThreeDimension {
	public:
		Polygon frontside;
		Polygon backside;
		vector<Polygon> p;
		vector<Line> lines;

		ThreeDimension();
		ThreeDimension(vector<Point> pol);
		ThreeDimension(vector<Point> pol, int d);
		ThreeDimension(const ThreeDimension& p);
		ThreeDimension& operator=(const ThreeDimension& p);
		void moveLeft (float dx);
		void moveRight (float dx);
		void moveUp (float dy);
		void moveDown (float dy);
		void scale(float k);
		void rotate(float k);
};

#endif