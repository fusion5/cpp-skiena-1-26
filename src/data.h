#ifndef SKIENA_1_26_DATA_INCLUDED
#define SKIENA_1_26_DATA_INCLUDED

struct Point {
	int x;
	int y;		 
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