#include <cstdlib>
#include <iostream>
#include <sstream>
#include <list>

#include <set>
#include <queue>

#include <gtkmm.h>

#include "data.h"
#include "algorithms.h"
#include "plotter.h"

#define READ_OK 1
#define READ_ERR_EOF 2
#define READ_ERR_PARSE 3 

using namespace std;

/*
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
*/

/*
void generate_pairs (int num_pairs) {
	// Generate pairs as input for the main program...

	srand(time(0));

	int x;
	int y;

	for (int i = 0; i < num_pairs; i++) {
		x = rand() % num_pairs + 1;
		y = rand() % num_pairs + 1;
		printf ("%d %d \n", x, y);		
	}
}
*/

/*
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

		cout 	
			<< "Please enter a point as x and y coordinates, or EOF to end the "
			<< "input..." << endl;

		try {
			cin_read_integer (&(p->x));
			cin_read_integer (&(p->y));
		} catch (int e) {
			switch (e) {
				case READ_ERR_EOF:
					break;
				case READ_ERR_PARSE: {
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
*/

int main(int argc, char *argv[])
{
	// Nearest neighbour heuristic
	// Input: ps1, a list of n points on a plane coming from stdin
	// Output: The shortest cyclic tour that visits each point in the list 
	// Algorithm: Find the nearest 2 points and connect them. Repeat until there
	// are no more points (this is a greedy heuristic).

	
	// if (argc > 1) {
		// To generate some random pairs, we could pass an integer as argument 
		// Parse integer from argv[1]...
	//	int num_pairs = 10;
	//	istringstream sstream(argv[1]);
	//	sstream >> num_pairs;
		
	//	generate_pairs (num_pairs);
	//	return EXIT_SUCCESS;	 
	// }

	// Keep 2 lists of points because of the need to iterate in 2 nested loops.
	// and when iterating, p1,p2 = p2,p1, i.e. iterating through
	// unordered pairs.
	list<Point*> ps1;

	// stdin input, but this is a GUI app so it's disabled 
	// read_input (&ps1);
	// if (ps1.empty()) {
	//	cout << "No input detected!" << endl;
	//	return EXIT_SUCCESS;
	//}

	vector<Edge*> es;

	es = compute_shortest_distances_tour_heuristic (ps1);
	// es = compute_shortest_distances(ps1);

	Gtk::Main kit(argc, argv);

	Plotter* main_win = new Plotter ();
	main_win->set_edges  (es);
	main_win->set_points (ps1);
	if (main_win) kit.run(*main_win);
	
	return EXIT_SUCCESS;
}
