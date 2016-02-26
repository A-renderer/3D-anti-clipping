#include "View.h"

View::View() {
	P1 = Point(620,1);
	P2 = Point(1020,401);

	vector<Point> e;
	e.push_back(Point(620,1));
	e.push_back(Point(1020,1));
	e.push_back(Point(1020,401));
	e.push_back(Point(620,401));

	pol = Polygon(e);
}

View::View(Point P1, Point P2) {
	this->P1.x = P1.x;
	this->P1.y = P1.y;
	this->P2.x = P2.x;
	this->P2.y = P2.y;
}

void View::setViewLines(Window window) {
	if(!window.lines.empty()) {
		for(int i=0; i<window.lines.size(); i++) {
			lines.push_back(window.lines[i]);
		}

		float dx = (float) (P2.x - P1.x)/(window.getBottomRight().x - window.getTopLeft().x);
		float dy = (float) (P2.y - P1.y)/(window.getBottomRight().y - window.getTopLeft().y);
		
		for(int i=0; i<lines.size(); i++) {
			lines[i].src.x = dx * (lines[i].src.x - window.getTopLeft().x) + P1.x;
			lines[i].src.y = dy * (lines[i].src.y - window.getTopLeft().y) + P1.y;
			lines[i].dest.x = dx * (lines[i].dest.x - window.getTopLeft().x) + P1.x;
			lines[i].dest.y = dy * (lines[i].dest.y - window.getTopLeft().y) + P1.y;
		}
	}
}