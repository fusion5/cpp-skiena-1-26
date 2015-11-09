#include "algorithms.h"

// #define ALGO_TRACE

using namespace std;

float point_distance (Point *p1, Point *p2) {
	// Pythagoras
	return sqrt(pow ((float) p1->x - (float) p2->x, 2) 
			  + pow ((float) p1->y - (float) p2->y, 2));
}

std::vector<Edge*> compute_shortest_distances_tour_heuristic (
	std::list<Point*> ps1) {

	std::list<Point*>  unvisited;
	std::vector<Edge*> edges;
	
	if (ps1.empty()) return edges;

	unvisited = list<Point*> (ps1);

	Point *p;
	Point *pnext;
	Point *first;
	Point *last;
	
	p = unvisited.back();
	unvisited.pop_back();

	first = p;

	int min;
	list<Point*>::iterator to_visit;

	while(!unvisited.empty()) {
		// Find the shortest edge between p and unvisited edges...

		float min = std::numeric_limits<float>::max();
		
		for (
			list<Point*>::iterator it = unvisited.begin(); 
			it != unvisited.end();
			++it
		) {
			if (point_distance(p, *it) < min) {
				pnext = *it;
				min = point_distance(p, *it);
				to_visit = it;
			}
		}

		// Add the edge (p -> pnext)...
		Edge* e;
		e = new Edge(p, pnext, min);

#ifdef ALGO_TRACE
		std::cout << "Found a new edge of distance " << min 
			  << std::endl;
#endif
		
		edges.push_back(e);

		// Visit pnext, i.e. remove pnext from unvisited...
		unvisited.erase(to_visit);

		// p becomes pnext 
		p = pnext;
	}

	// Add one more edge to close the tour...
	Edge *e;
	e = new Edge(first, p, point_distance (p, first));

	edges.push_back (e);

	return edges;

}

bool operator < (const Edge &e1, const Edge &e2) {
	return e1.distance < e2.distance;				 
};

bool compare_edges (Edge *e1, Edge *e2) {
	return *e1 < *e2;
}

std::vector<Edge*> compute_shortest_distances(
	std::list<Point*> ps1) {
	/*
	 * TODO: The nearest 2 points would be more efficiently computed using 
	 * Voronoi Diagrams instead of the two nested "for" loops. The nested
	 * loops produce this many heap insert operations: 
	 *
	 *    n + (n - 1) + (n - 2) + ... + 1
	 *  = n^2 - (1 + 2 + ... + (n - 1))
	 *  = n^2 - (n - 1)(n - 2) / 2
	 *  = 1/2 * ( 2 * n^2 - (n^2 - 2n - n + 2) )
	 *  = 1/2 * ( n^2 + 3n - 2 )
	 */
	
	// Warning: This does not generate a closed tour, but rather partitions 
	// the network into close by "clusters"

	std::list<Point*> ps2;
	// ps2 is a copy of ps1 but it will help us to generate ps1's 
	// commutative pairs
	ps2 = std::list<Point*> (ps1);

	float min = std::numeric_limits<int>::max();
	Point *min_from;
	Point *min_to;
	
	bool (*fn_pt)(Edge*, Edge*) = compare_edges;
	std::priority_queue<Edge*, std::vector<Edge*>, bool(*)(Edge*, Edge*)> 
		smallest_distances (fn_pt);

	int steps = 0;

	// Need to store the Top N-1 smallest edges and their distances...

	min = std::numeric_limits<int>::max();
	
	std::list<Point*>::iterator to_erase;
	
	for (std::list<Point*>::iterator i = ps1.begin(); i != ps1.end(); ++i) {
		for (std::list<Point*>::iterator j = ps2.begin(); j != ps2.end(); ++j) {
			if (*i == *j) {
				// cout << *i << " " << *j << " equal" << endl;
				to_erase = j;
				continue;
			}
			
			float d = point_distance(*i, *j);
			steps++;

			Edge *e = new Edge(*i, *j, d);

			if (smallest_distances.size() < ps1.size()) {
				smallest_distances.push(e);
			} else {
				Edge *biggest_edge;
				biggest_edge = smallest_distances.top();
				if (e->distance < biggest_edge->distance) {
					smallest_distances.push(e);
					smallest_distances.pop();
				}
			}
		}
		// Remove that element from ps2: it helps us to avoid operations,
		// because we know that edge p1, p2 equals edge p2, p1.
		ps2.erase(to_erase);
	}

#ifdef ALGO_TRACE
	cout << "We have the smallest " 
		 << smallest_distances.size() 
		 << " edges." 
		 << endl;
#endif
	
	vector<Edge*> es;

	while (!smallest_distances.empty()) {
		Edge *e = smallest_distances.top();
		cout << e->p1->x << ", " << e->p1->y << " -> " 
		     << e->p2->x << ", " << e->p2->y << " distance " 
		     << e->distance << endl;
		
		es.push_back(e);
		smallest_distances.pop();
	}
	
#ifdef ALGO_TRACE
	cout << ps1.size() << " points completed in " 
	     << steps << " steps" << endl;
#endif
	
	return es;
}
