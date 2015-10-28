#include <cstdlib>
#include <iostream>
#include <list>
#include <cmath>
#include <set>
#include <limits>
#include "pair_generator.h"

#define READ_OK 1
#define READ_ERR_EOF 2
#define READ_ERR_PARSE 3 

using namespace std;

struct Point {
	int x;
	int y;		 
};

struct Edge {
	Point *p1;
	Point *p2;
	float distance;	// Distance between p1 and p2
};

bool operator < (const Edge &e1, const Edge &e2) {
	return e1.distance < e2.distance;				 
};

bool compare_edges (Edge *e1, Edge *e2) {
	return *e1 < *e2;
}

int cin_read_integer(int *v)
{
	cin >> *v;
	if (cin.eof()) {
		cout << "End of file!" << endl;
		throw READ_ERR_EOF;
	}
	if (!cin.good()) {
		cout << "Parse error, expecting <int><whitespace><int>" << endl;
		throw READ_ERR_PARSE;
	}
	return READ_OK;
}

float point_distance (Point *p1, Point *p2) {
	// Pythagoras
	return sqrt(pow ((float) p1->x - (float) p2->x, 2) 
			+ pow ((float) p1->y - (float) p2->y, 2));
}

void read_input (list<Point *> *ps) {
	// Parse the input of:
	// line  := int<whitespace>int
	// input := line<EOL>+
	// This method could probably be tested for correctness some more.

	int x   = 0; 
	int y   = 0;

	do {	        
		Point *p;
		p = new Point;

		p->x = 0;
		p->y = 0;

		cout 	<< "Please enter a point as x and y coordinates, or EOF to end the "
			<< "input..." << endl;

		try {
			cin_read_integer (&(p->x));
			cin_read_integer (&(p->y));
		} catch (int e) {
			switch (e) {
				case READ_ERR_EOF:
					break;
				case READ_ERR_PARSE: {
							     system("PAUSE");
							     exit (EXIT_FAILURE);
						     }
			}
		}

		if (!cin.eof()) {
			cout << "Entered: " << p->x << " " << p->y << endl;
			ps->push_back (p);
		}

	} while (!cin.eof()); 
}

int main(int argc, char *argv[])
{
	// Nearest neighbour heuristic
	// Input: ps, a list of n points on a plane coming from stdin
	// Output: The shortest cyclic tour that visits each point in the list 
	// Algorithm: Find the nearest 2 points and connect them. Repeat until there
	// are no more points (this is a greedy heuristic).

	// TODO: The nearest 2 points would be more efficiently computed using 
	// Voronoi Diagrams instead of the two nested "for" loops. The nested
	// loops produce n squared set insert operations giving us at least n^2 
	// operations.

	if (argc > 1) {
		// To generate some random pairs, we pass any argument in the CLI. 
		generate_pairs();
		return EXIT_SUCCESS;	 
	}

	list<Point*> ps;
	read_input (&ps);

	if (ps.empty()) {
		cout << "No input detected!" << endl;
		system("PAUSE");
		return EXIT_SUCCESS;
	}

	float min = INT_MAX;
	Point *min_from;
	Point *min_to;

	set<pair<Point*,Point*> > visited_edges;

	bool (*fn_pt)(Edge*, Edge*) = compare_edges;
	set<Edge*, bool(*)(Edge*, Edge*)> smallest_distances (fn_pt);

	int steps = 0;

	// Need to store the Top N-1 smallest edges and their distances...

	min = INT_MAX;

	for (list<Point*>::iterator i = ps.begin(); i != ps.end(); ++i)
		for (list<Point*>::iterator j = ps.begin(); j != ps.end(); ++j) {
			if (*i == *j) continue;

			float d = point_distance(*i, *j);
			steps++;

			Edge *e = new Edge;
			e->p1 				= *i;
			e->p2 				= *j;
			e->distance 	= d;

			if (smallest_distances.size() < ps.size()) {
				smallest_distances.insert(e);
			} else {
				set <Edge *>::iterator biggest_edge;
				biggest_edge = smallest_distances.end();
				biggest_edge--;
				if (e->distance < (*biggest_edge)->distance) {
					smallest_distances.insert(e);
					smallest_distances.erase(biggest_edge);
				}
			}
		}

	cout 	<< "We have the smallest " << smallest_distances.size() << " edges." 
		<< endl;

	for (set<Edge*>::iterator it = smallest_distances.begin(); 
			it != smallest_distances.end(); 
			++it)
		cout << (*it)->p1->x << ", " << (*it)->p1->y << " -> " << (*it)->p2->x
			<< ", " << (*it)->p2->y << " distance " << (*it)->distance << endl;

	cout << ps.size() << " points completed in " << steps << " steps" << endl;

	system("PAUSE");
	return EXIT_SUCCESS;
}
