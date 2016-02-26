#include "Line.h"

Line::Line() { // CTOR

}

Line::Line(Point src, Point dest) { // CTORPARAM
	this->src = src;
	this->dest = dest;
}

Line::Line(const Line& l) { // CCTOR
	src = l.src;
	dest = l.dest;
}

Line& Line::operator=(const Line& l) { // OPERATOR=
	src = l.src;
	dest = l.dest;

	return *this;
}

Line::~Line() { // DTOR

}

void Line::moveRight(float dx) {
	src.x += dx;
	dest.x += dx;
}

void Line::moveLeft(float dx) {
	src.x -= dx;
	dest.x -= dx;
}

void Line::moveUp(float dy) {
	src.y -= dy;
	dest.y -= dy;
}

void Line::moveDown(float dy) {
	src.y += dy;
	dest.y += dy;
}

void Line::setLine(Point _src, Point _dest) {
	src.x = _src.x;
	src.y = _src.y;
	dest.x = _dest.x;
	dest.y = _dest.y;
}