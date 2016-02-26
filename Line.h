#ifndef LINE_H
#define LINE_H

#include "Point.h"

class Line {
public:
	Line();

	Line(Point src, Point dest);

	Line(const Line& l);

	Line& operator=(const Line& l);

	~Line();

	void moveRight(float dx);
	
	void moveLeft(float dx);
	
	void moveUp(float dy);
	
	void moveDown(float dy);

	void setLine(Point _src, Point _dest);

	Point src, dest;

private:

};

#endif
