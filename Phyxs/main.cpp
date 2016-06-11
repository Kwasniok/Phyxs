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

using namespace std;

void test_seed(ofstream& of_p, unsigned long seed, int size_x, int size_y, int t_max);
void random_init(Grid& grid);
void seeded_init(Grid& grid, unsigned long seed);
char repr(const Cell& c);
void grid_to_bmp(Grid& g, const char* path, unsigned long t);


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
		//cout << "t = " << t << endl;
		
		/*
		 for (int i=0; i < size_x; ++i) {
			for (int j=0; j < size_y; ++j) {
		 Cell& c = grid.get_cell(i, j);
		 if (c.get_colony() != 0)
		 cout << "\33[" << 30 + c.get_colony() << 'm';
		 cout << repr(c) << ' ';
		 if (c.get_colony() != 0)
		 cout << "\33[0m";
			}
			cout << endl;
		 }
		 cout << endl;
		 cout.flush();
		 this_thread::sleep_for(chrono::milliseconds(long(1000.0/10.0)));
		 */
	 
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
		if ((seed >> i) & 0x1) grid.get_cell(pos_x + i / 8, pos_y + i % 8).set_alive();
		//else grid.get_cell(pos_x + i / 8, pos_y + i % 8).set_dead();
	}
}


char repr(const Cell& c) {
	
	if (c.is_obstacle()){
		if(c.is_alive()) return 'O';
		else return 'o';
	}
	
	switch (c.get_livetime()) {
		case 0:
			return ' ';
		case 1:
			return '1';
		case 2:
			return '2';
		case 3:
			return '3';
		case 4:
			return '4';
		case 5:
			return '5';
		case 6:
			return '6';
		case 7:
			return '7';
		case 8:
			return '8';
		case 9:
			return '9';
		default:
			return 'X';
	}
}

void grid_to_bmp(Grid& grid, const char* p, unsigned long t)  {
	
	int w = grid.get_size_y();
	int h = grid.get_size_x();

	FILE *f;
	unsigned char *img = NULL;
	int filesize = 54 + 3*w*h;  //w is your image width, h is image height, both int
	
	img = (unsigned char *)malloc(filesize);
	memset(img,0,filesize);
	
	unsigned char r,g,b;
	int x,y;
	for(int i=0; i<w; i++)
	{
		for(int j=0; j<h; j++)
		{
			x=i; y=j;
			
			Cell& c = grid.get_cell(j, i);
			
			if (c.is_obstacle()) {r=128; b=128; g=128;}
			else if (c.is_alive()) {r=255; b=255; g=255;}
			else {r=0; b=0; g=0;}
			
			img[(x+y*w)*3+2] = r;
			img[(x+y*w)*3+1] = g;
			img[(x+y*w)*3+0] = b;
		}
	}
	
	unsigned char bmpfileheader[14] = {'B','M', 0,0,0,0, 0,0, 0,0, 54,0,0,0};
	unsigned char bmpinfoheader[40] = {40,0,0,0, 0,0,0,0, 0,0,0,0, 1,0, 24,0};
	unsigned char bmppad[3] = {0,0,0};
	
	bmpfileheader[ 2] = (unsigned char)(filesize    );
	bmpfileheader[ 3] = (unsigned char)(filesize>> 8);
	bmpfileheader[ 4] = (unsigned char)(filesize>>16);
	bmpfileheader[ 5] = (unsigned char)(filesize>>24);
	
	bmpinfoheader[ 4] = (unsigned char)(       w    );
	bmpinfoheader[ 5] = (unsigned char)(       w>> 8);
	bmpinfoheader[ 6] = (unsigned char)(       w>>16);
	bmpinfoheader[ 7] = (unsigned char)(       w>>24);
	bmpinfoheader[ 8] = (unsigned char)(       h    );
	bmpinfoheader[ 9] = (unsigned char)(       h>> 8);
	bmpinfoheader[10] = (unsigned char)(       h>>16);
	bmpinfoheader[11] = (unsigned char)(       h>>24);
	
	std::string path = p;
	path += to_string(t);
	path += ".bmp";
	f = fopen(path.c_str(),"wb");
	fwrite(bmpfileheader,1,14,f);
	fwrite(bmpinfoheader,1,40,f);
	for(int i=0; i<h; i++)
	{
		fwrite(img+(w*(h-i-1)*3),3,w,f);
		fwrite(bmppad,1,(4-(w*3)%4)%4,f);
	}
	fclose(f);

}
