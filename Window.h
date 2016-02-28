#ifndef WINDOW_H
#define WINDOW_H

#define INSIDE 0	//0000
#define LEFT 1		//0001
#define RIGHT 2		//0010
#define BOTTOM 4	//0100
#define TOP 8		//1000

#include "Line.h"
#include "Polygon.h"
#include <vector>

class Window {
	public:
		Polygon square;
		vector<Line> lines;

		Window();
		Point getMidPoint();
		Point getTopLeft();
		Point getTopRight();
		Point getBottomRight();
		Point getBottomLeft();
		int computeEndpoint(Point P);
		void lineClipping(Line line);
		void polygonClipping(Polygon P);
		void clipAllPolygon(vector<Polygon> pols);
		void zoomIn(float k);
		void zoomOut(float k);
		void moveUp(float k);
		void moveDown(float k);
		void moveRight(float k);
		void moveLeft(float k);
};

#endif