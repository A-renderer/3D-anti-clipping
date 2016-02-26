#include "Window.h"

Window::Window() {
	vector<Point> e;
	e.push_back(Point(2,2));
	e.push_back(Point(2,252));
	e.push_back(Point(252,252));
	e.push_back(Point(252,2));

	square = Polygon(e);
}

Point Window::getMidPoint() {
	return Point(square.getMidX(), square.getMidY());
}

Point Window::getTopLeft() {
	return square.e[0];
}

Point Window::getTopRight() {
	return square.e[1];
}

Point Window::getBottomRight() {
	return square.e[2];
}

Point Window::getBottomLeft() {
	return square.e[3];
}

int Window::computeEndpoint(Point P) {
	int result = INSIDE;
 
	if (P.x < getTopLeft().x) {
		result = result|LEFT;
	}
	else if (P.x > getBottomRight().x)  {
		result = result|RIGHT;
	}

	if (P.y < getTopLeft().y) {
		result = result|BOTTOM;
	}
	else if (P.y > getBottomRight().y) {
		result = result|TOP;
	}
 
	return result;
}



void Window::lineClipping(Line line) {
	int endPoint1 = computeEndpoint(line.src);
	int endPoint2 = computeEndpoint(line.dest);
	bool valid = false;

	while (true) {
		if (!(endPoint1|endPoint2)) { //kedua titik line didalam (0000)
			valid = true;
			break;
		} else if (endPoint1 & endPoint2) { //dua2 diluar
			break;
		} else {
			float x, y;
			int point = endPoint1 ? endPoint1 : endPoint2;
			if (point & TOP) {
				x = line.src.x + (line.dest.x - line.src.x) * (getBottomRight().y - line.src.y) / (line.dest.y - line.src.y);
				y = getBottomRight().y;
			} else if (point & BOTTOM) {
				x = line.src.x + (line.dest.x - line.src.x) * (getTopLeft().y - line.src.y) / (line.dest.y - line.src.y);
				y = getTopLeft().y;
			} else if (point & RIGHT) {
				y = line.src.y + (line.dest.y - line.src.y) * (getBottomRight().x - line.src.x) / (line.dest.x - line.src.x);
				x = getBottomRight().x;
			} else if (point & LEFT) {
				y = line.src.y + (line.dest.y - line.src.y) * (getTopLeft().x - line.src.x) / (line.dest.x - line.src.x);
				x = getTopLeft().x;
			}

			if (point == endPoint1) {
				line.src.x = x;
				line.src.y = y;
				endPoint1 = computeEndpoint(line.src);
			} else {
				line.dest.x = x;
				line.dest.y = y;
				endPoint2 = computeEndpoint(line.dest);
			}
		}

	}
	if (valid) {
		lines.push_back(line);
	}
}

void Window::zoomIn(float k) {
	int midX = getMidPoint().x;
	int midY = getMidPoint().y;

	for(int i=0; i<square.e.size(); i++) {
		square.e[i].x = (square.e[i].x - midX) * k + midX;
		square.e[i].y = (square.e[i].y - midY) * k + midY;
	}
}

void Window::zoomOut(float k) {
	int midX = getMidPoint().x;
	int midY = getMidPoint().y;

	for(int i=0; i<square.e.size(); i++) {
		square.e[i].x = (square.e[i].x - midX) / k + midX;
		square.e[i].y = (square.e[i].y - midY) / k + midY;
	}
}

void Window::polygonClipping(Polygon P) {
	for(int j=0; j<P.e.size()-1; j++) {
		lineClipping(Line(Point(P.e[j].x,P.e[j].y),Point(P.e[j+1].x,P.e[j+1].y)));
	}
	lineClipping(Line(Point(P.e[P.e.size()-1].x,P.e[P.e.size()-1].y),Point(P.e[0].x,P.e[0].y)));
}

void Window::clipAllPolygon(vector<Polygon> pols) {
	int c = 1;
	if(!pols.empty()) {
		for(int i=0; i<pols.size(); i++) {
			polygonClipping(pols[i]);
		}
	}
}

void Window::moveUp(float k) {
	square.e[0] = Point(square.e[0].x,square.e[0].y-=k);
	square.e[1] = Point(square.e[1].x,square.e[1].y-=k);
	square.e[2] = Point(square.e[2].x,square.e[2].y-=k);
	square.e[3] = Point(square.e[3].x,square.e[3].y-=k);
}

void Window::moveDown(float k) {
	square.e[0] = Point(square.e[0].x,square.e[0].y+=k);
	square.e[1] = Point(square.e[1].x,square.e[1].y+=k);
	square.e[2] = Point(square.e[2].x,square.e[2].y+=k);
	square.e[3] = Point(square.e[3].x,square.e[3].y+=k);
}

void Window::moveLeft(float k) {
	square.e[0] = Point(square.e[0].x-=k,square.e[0].y);
	square.e[1] = Point(square.e[1].x-=k,square.e[1].y);
	square.e[2] = Point(square.e[2].x-=k,square.e[2].y);
	square.e[3] = Point(square.e[3].x-=k,square.e[3].y);
}

void Window::moveRight(float k) {
	square.e[0] = Point(square.e[0].x+=k,square.e[0].y);
	square.e[1] = Point(square.e[1].x+=k,square.e[1].y);
	square.e[2] = Point(square.e[2].x+=k,square.e[2].y);
	square.e[3] = Point(square.e[3].x+=k,square.e[3].y);
}