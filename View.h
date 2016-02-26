#ifndef VIEW_H
#define VIEW_H

#include "Window.h"
#include <vector>

class View {
	public:
		Point P1; //top left
		Point P2; //bottom right
		vector<Line> lines;
		Polygon pol;

		View();
		View(Point P1, Point P2);
		void setViewLines(Window window);
};

#endif