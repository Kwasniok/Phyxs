//
//  grid_to_console.cpp
//  Phyxs
//
//  Created by Jens Kwasniok on 11.06.16.
//  Copyright © 2016 Jens Kwasniok. All rights reserved.
//

#include "grid_to_console.hpp"

using namespace std;

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

void grid_to_console(Grid& grid, ostream& out, int t) {
	
	out << "t = " << t << endl;
	
	for (int x=0; x < grid.get_size_x(); ++x) {
		for (int y=0; y < grid.get_size_y(); ++y) {
			
		 Cell& c = grid.get_cell(x, y);
		 
		 if (c.get_colony() != 0 && c.get_colony() < 7)
			 out << "\33[" << 30 + c.get_colony() << 'm';
		 out << repr(c) << ' ';
		 if (c.get_colony() != 0)
			 out << "\33[0m";
		 
		}
		out << endl;
	}
	out << endl;
	out.flush();
	
}
