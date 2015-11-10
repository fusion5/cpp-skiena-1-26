#ifndef SKIENA_1_26_DATA_INCLUDED
#define SKIENA_1_26_DATA_INCLUDED

struct Point {
	int x;
	int y;	
	Point (int a, int b) : x(a), y(b) {}
};

struct PointWithDegree {
	Point *p;
	int   degree;
	PointWithDegree (Point *p, int degree) : p(p), degree(degree) {}
};

class Edge {
	public:
		Point *p1;
		Point *p2;
		float distance;	// Distance between p1 and p2
		Edge();
		Edge(Point *p1, Point *p2, float distance);
};

#endif
