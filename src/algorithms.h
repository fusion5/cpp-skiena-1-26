#ifndef SKIENA_1_26_ALGORITHMS_INCLUDED
#define SKIENA_1_26_ALGORITHMS_INCLUDED

#include "data.h"

#include <vector>
#include <list>
#include <cmath>
#include <iostream>
#include <limits>
#include <set>
#include <unordered_set>
#include <queue>
#include <cassert>

std::vector<Edge*> compute_shortest_distances_tour_heuristic (
	std::list<Point*> ps1);
std::vector<Edge*> compute_closest_pairs_tour_heuristic (
	std::list<Point*> ps1);
std::vector<Edge*> compute_shortest_distances(
	std::list<Point*> ps1);

#endif
