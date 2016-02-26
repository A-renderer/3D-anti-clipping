#ifndef POINT_H
#define POINT_H

#include "header.h"

class Point {
public:
	Point();

	Point(int x, int y);

	Point(const Point& p);

	Point& operator=(const Point& p);

	~Point();

	int x;
	int y;

private:

};

#endif
