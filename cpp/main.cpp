// main.cpp

#include <random>
#include <iostream>

#include "graph.h"
#include "superkiss64.h"

/*
int num_vertices = 21;
edge_list edges = {
	// A star on 21 vertices
	{0,1}, {0,2}, {0,3}, {0,4}, {0,5}, {0,6}, {0,7}, {0,8}, {0,9}, {0,10},
	{0,11}, {0,12}, {0,13}, {0,14}, {0,15}, {0,16}, {0,17}, {0,18}, {0,19}, {0,20}
};

int num_vertices = 17;
edge_list edges = {
	// Let H be C_4 with one additional diagonal edge (a,b).
	// Make 4 copies of H, each connected to a new vertex, v by edges (a,v), (b,v).
	{{1,2}, {1,3}, {1,4}, {2,4}, {3,4}, {0,1}, {0,4},
	{5,6}, {5,7}, {5,8}, {6,8}, {7,8}, {0,5}, {0,8},
	{9,10}, {9,11}, {9,12}, {10,12}, {11,12}, {0,9}, {0,12},
	{13,14}, {13,15}, {13,16}, {14,16}, {15,16}, {0,13}, {0,16}}
};
*/


int main()
{
	std::random_device rd;
	superkiss64 rng(rd(), rd(), rd());

	graph Hx4p1 (17,
				{{1,2}, {1,3}, {1,4}, {2,4}, {3,4}, {0,1}, {0,4},
				{5,6}, {5,7}, {5,8}, {6,8}, {7,8}, {0,5}, {0,8},
				{9,10}, {9,11}, {9,12}, {10,12}, {11,12}, {0,9}, {0,12},
				{13,14}, {13,15}, {13,16}, {14,16}, {15,16}, {0,13}, {0,16}});
				
	graph k4(4, {{0,1}, {1,2}, {0,2}, {2,3}, {0,3}, {1,3}});
	graph s_21(	21,
				{{0,1}, {0,2}, {0,3}, {0,4}, {0,5}, {0,6}, {0,7}, {0,8}, {0,9}, {0,10},
				{0,11}, {0,12}, {0,13}, {0,14}, {0,15}, {0,16}, {0,17}, {0,18}, {0,19}, {0,20}});

	
	//graph& g = Hx4p1;
	//g.display(std::cout) << std::endl;
	
	//std::pair<int,int> ve = g.get_ve();

	const int num_trials = 100;
	int smax = 6;
	for (int i = 1; i <= num_trials; ++i) {
		
		graph g = Hx4p1;
		std::pair<int,int> ve = g.get_ve();
		int iterations = 0;
		
		if (smax == 0) {
			smax = g.get_upper_bound_on_s_G();
		}
		
		for (;;) {	
			if (g.is_irregular()) {
				smax = g.s();
				g.display(std::cout) << std::endl;
				break;
			}
		
			//int edge_index = (int)(rng.rand01() * ve.second);
			int edge_index = g.compute_edge_priorities();
			//g.display(std::cout) << std::endl;
			
			int weight = 0;
			for (;;) {
				weight = (int)(1. + rng.rand01() * smax);
				if (weight != g.get_weight(edge_index)) {
					break;
				}
			}

			if (++iterations > 100) {
				std::cout << "...aborting...";
				break;
			}
			
			//std::cout << "Assigning edge " << edge_index << " weight " << weight << std::endl;
			g.adjust_weight(edge_index, weight);

		}
		std::cout << iterations << " iterations" << std::endl;
	}
}
