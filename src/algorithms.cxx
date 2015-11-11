#include "algorithms.h"

#define ALGO_TRACE

using namespace std;

float point_distance (Point *p1, Point *p2) {
	// Pythagoras
	return sqrt (pow (p1->x - p2->x, 2) + pow (p1->y - p2->y, 2));
}

vector<Edge*> compute_closest_pairs_tour_heuristic(list<Point*> ps1) {

	vector<Edge*> edges; 

	if (ps1.empty()) return edges;

	// Keep a set of chains in the graph and merge them until 
	// a single, shortest path chain is obtained...
	// To store chains, we need fast inserts, random deletes, and iteration.
	// Unordered set seems to be the best data structure in STL for this
	// (note: it requires c++11 to be enabled)
	unordered_set<list<Point*>*> chains;

	for (	list<Point*>::iterator it = ps1.begin(); 
		it != ps1.end();
		++it
	) {
		// We need the ability to reverse chains and to merge them 
		// to form bigger ones. A list seems good because reverse is
		// O(1), and concatenation is also O(1).
		list <Point*> *l = new list<Point*>();
		l->push_back(*it);
		// chains.push_back(l);
		chains.insert(l);
	}

	int k = 0;
	float dmin;
	int reverse;
	list<Point*> *chain1;
	list<Point*> *chain2;
	unordered_set<list<Point*>*>::iterator it_chain2;

	while (chains.size() > 1) {
		
		dmin = numeric_limits<float>::max();

		// Given 2 chains, there are 4 ways to connect them with an edge:
		// begin-begin, begin-end, end-begin and end-end.
		// The algorithm tries all distinct chain pairs in all 4 ways and 
		// chooses the shortest edge that connects 2 of the chains

		unordered_set<list<Point*>*>::iterator from = chains.begin();

		k = 0;

		for (from; from != chains.end(); from++) {
			unordered_set<list<Point*>*>::iterator to = from; to++;
			for (to; to != chains.end(); to++) {

				assert (!(*from)->empty());
				assert (!(*to)->empty());

				Point *from_begin = (*from)->front();
				Point *from_end   = (*from)->back();
				Point *to_begin	  = (*to)->front();
				Point *to_end     = (*to)->back();
				
				float d;

				d = point_distance (from_begin, to_begin);
				if (d < dmin) {
					dmin       	= d;
					reverse		= 1;
					chain1 		= *from;
					chain2		= *to;
					it_chain2	= to;
				}
				
				d = point_distance (from_begin, to_end);
				if (d < dmin) {
					dmin 	   	= d;
					reverse		= 0;
					chain1		= *to;
					chain2		= *from;
					it_chain2	= from;
				}

				d = point_distance (from_end, to_begin);
				if (d < dmin) {
					dmin       	= d;
					reverse    	= 0;
					chain1		= *from;
					chain2		= *to;
					it_chain2 	= to;
				}
				
				d = point_distance (from_end, to_end);
				if (d < dmin) {
					dmin 	     	= d;
					reverse		= 2;
					chain1		= *to;
					chain2		= *from;
					it_chain2	= from;
				}
				k++;
			}
		}

		assert (chain1->size() > 0);
		assert (chain2->size() > 0);
		assert (chain1 != chain2);

		assert (k == chains.size() * (chains.size() - 1) / 2);

		if (reverse == 1) chain1->reverse();
		if (reverse == 2) chain2->reverse();

		assert (chain1->back() != chain2->front());

		// The distance between chain1's last element and chain2's 1st
		// must equal dmin...
		assert ( point_distance (chain1->back(), chain2->front()) - dmin 
		       < 0.00001 );

		chain1->splice (chain1->end(), *chain2);

		assert (chain2->empty());
		chains.erase (it_chain2); 
	}

	// Format the final chain list as a vector of edges...

	list<Point*> *last_chain = *(chains.begin());
	list<Point*>::iterator j = last_chain->begin();
	for (j; j != last_chain->end(); ++j) {

		list <Point*>::iterator next = j;
		if (++next == last_chain->end()) continue;

		Edge *e = new Edge (*j, *next, 0);
		edges.push_back(e);
	}

	// Close the tour
	Edge *e = new Edge (*(last_chain->begin()), *(--last_chain->end()), 0);
	edges.push_back(e);

	return edges;
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
		) if (point_distance(p, *it) < min) {
			pnext = *it;
			min = point_distance(p, *it);
			to_visit = it;
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
};
