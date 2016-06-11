//
//  main.cpp
//  Phyxs
//
//  Created by Jens Kwasniok on 07.06.16.
//  Copyright © 2016 Jens Kwasniok. All rights reserved.
//

#include <iostream>
#include <fstream>
#include <math.h>
#include <thread>
#include <limits>
#include "Grid.hpp"
#include "grid_to_bmp.hpp"
#include "grid_to_console.hpp"

using namespace std;

void test_seed(ofstream& of_p, unsigned long seed, int size_x, int size_y, int t_max);
void random_init(Grid& grid);
void seeded_init(Grid& grid, unsigned long seed);
char repr(const Cell& c);


int main(int argc, const char * argv[]) {
	
	int size_x = 100, size_y = 100;
	cin >> size_x >> size_y;
	unsigned int t_max;
	cin >> t_max;
	unsigned long seed_start, seed_end;
	cin >> seed_start >>seed_end;
	
	ofstream of_p = ofstream("./plots/protocol.txt");
	if (!of_p.good()) return  EXIT_FAILURE;
	
	of_p << "size_x = " << size_x << ", size_y = " << size_y << ", t_max = " << t_max << endl
	     << "seed_start = " << seed_start << ", seed_end = " << seed_end << endl << endl;
	
	of_p << "seed\tend_state\ttime" << endl;
	for (unsigned long seed = seed_start; seed <= seed_end; ++seed) {
		test_seed(of_p, seed, size_x, size_y, t_max);
		if (seed % 0xFF == 0) {
			of_p.flush();
			cout.flush();
		}
	}
	
	of_p.close();
	
    return EXIT_SUCCESS;
}

void test_seed(ofstream& of_p, unsigned long seed, int size_x, int size_y, int t_max) {
	
	of_p << "0x" << std::hex << seed << std::dec << "\t";
	cout << "0x" << std::hex << seed << std::dec << "\t";
	
	Grid grid {size_x, size_y};
	std::vector<std::pair<Grid, int>> history;
	seeded_init(grid, seed);
	
	int up_count;
	history.push_back({grid, up_count});
	//grid_to_bmp(grid, 0);
	bool cont = true;
	for (int t=0; cont && t <= t_max; ++t) {
	 
		up_count = grid.update();
		//cout << up_count << endl;
		
		if (grid.get_number_of_living_cells() == 0) {
			of_p << "extincts \t" << t + 1;
			cont = false;
		}
		
		else if (up_count == 0) {
			of_p << "stops    \t" << t + 1;
			cont = false;
		}
		
		
		else for (int i=t; i >= 0; --i) {
			if (history[i].second == up_count && history[i].first.has_same_alive_pattern(grid)) {
				of_p << "loops    \t" << t+1 << "-" << i;
				cont = false;
				break;
			}
		}
		
		history.push_back({grid, up_count});
		//grid_to_bmp(grid, t);
		
		if (t == t_max) {
			of_p << "continues\t" << t;
			grid_to_bmp(grid, "./plots/continues/", seed);
		}
		

	}
	
	of_p << (endl);
	

	/*
	cout << "printing ... " << endl;
	for (int i=0; i<history.size(); ++i) {
		grid_to_bmp(history[i].first, i);
	}
	*/
	
	//grid_to_bmp(grid, seed);
}

inline bool alive_cond(int i, int j, int size_x, int size_y) {
	//return double(rand()) * (1- (sqrt(sqrt(pow((i - size_x/2.0),4.0) + pow((j - size_y/2.0),4.0)) / sqrt(sqrt(pow((size_x),4.0)) + pow((size_y),4.0))))) >= double(RAND_MAX)/2.0;
	return rand() <= RAND_MAX/4;
}

inline bool obstacle_cond(int i, int j) {
	return false;//rand() <= RAND_MAX/1000;
}

void random_init(Grid& grid) {
	for (int i=0; i < grid.get_size_x(); ++i) {
		for (int j=0; j < grid.get_size_y(); ++j) {
			Cell c;
			if (alive_cond(i, j, grid.get_size_y(), grid.get_size_y())) {c.set_alive(); c.inrecease_livetime(); c.set_colony(rand() % 4 + 1);}
			else if (obstacle_cond(i, j)) c.set_obstacle();
			grid.set_cell(i, j, c);
			
		}
	}
}

void seeded_init(Grid& grid, unsigned long seed) {
	static_assert(sizeof(seed) == 8, "sizeof(long) muste be 8");
	
	int pos_x = grid.get_size_x() / 2 - 4;
	int pos_y = grid.get_size_y() / 2 - 4;
	
	for (int i=0; i < 64; ++i) {
		if ((seed >> i) & 0x1) grid.get_cell(pos_x + i % 8, pos_y + i / 8).set_alive();
		//else grid.get_cell(pos_x + i / 8, pos_y + i % 8).set_dead();
	}
}

