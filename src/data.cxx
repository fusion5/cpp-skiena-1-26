#include "data.h"

Edge::Edge(Point *p1, Point *p2, float distance) {
	this->p1 = p1;
	this->p2 = p2;
	this->distance = distance;
}