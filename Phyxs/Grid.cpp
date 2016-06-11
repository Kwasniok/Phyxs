//
//  Grid.cpp
//  Phyxs
//
//  Created by Jens Kwasniok on 07.06.16.
//  Copyright © 2016 Jens Kwasniok. All rights reserved.
//

#include "Grid.hpp"
#include <map>

Grid::Grid(index_t size_x, index_t size_y) {
	
	if (size_x <= 0) size_x = 1;
	if (size_y <= 0) size_y = 1;
	
	_size_x = size_x;
	_size_y = size_y;
	
	_cells = std::vector<Cell>(size_x * size_y);
}

void Grid::set_cell(index_t x, index_t y, const Cell &c) {
	
	index_t p = pos(x, y);
	if (c.is_alive() && !_cells[p].is_alive()) ++_alive;
	_cells[p] = c;
}

Cell& Grid::get_cell(index_t x, index_t y) {
	
	return _cells[pos(x, y)];
}

bool Grid::has_same_alive_pattern(const Grid &rhs) {
	if (_size_x != rhs._size_x || _size_y != rhs._size_y) return false;
	
	for (index_t i = 0; i < _size_y*_size_y; ++i) {
		if (_cells[i].is_alive() != rhs._cells[i].is_alive()) return false;
	}
	return  true;
}

int Grid::update() {
	std::vector<Cell> prev = _cells;
	_alive = 0;
	
	int changes = 0;
	
	for (int x=0; x < _size_x; ++x) {
		for (int y=0; y < _size_y; ++y) {
			

			Cell& current_cell = _cells[pos(x,y)];
			if (current_cell.is_obstacle()) continue;
			Cell& previous_cell = prev[pos(x,y)];
			std::map<Cell::colony_t, int> live;
			
			live[prev[pos(x+1, y)].get_colony()] += prev[pos(x+1, y)].is_alive();
			live[prev[pos(x-1, y)].get_colony()] += prev[pos(x-1, y)].is_alive();
			
			live[prev[pos(x, y+1)].get_colony()] += prev[pos(x, y+1)].is_alive();
			live[prev[pos(x, y-1)].get_colony()] += prev[pos(x, y-1)].is_alive();
			
			live[prev[pos(x+1, y+1)].get_colony()] += prev[pos(x+1, y+1)].is_alive();
			live[prev[pos(x+1, y-1)].get_colony()] += prev[pos(x+1, y-1)].is_alive();
			
			live[prev[pos(x-1, y+1)].get_colony()] += prev[pos(x-1, y+1)].is_alive();
			live[prev[pos(x-1, y-1)].get_colony()] += prev[pos(x-1, y-1)].is_alive();

			
			int live_total = 0;
			Cell::colony_t cny = 0;
			int best_live_cny = 0;
			for (auto it = live.begin(); it!=live.end(); ++it) {
				live_total += it->second;
				if (it->second > best_live_cny) {
					best_live_cny = it->second;
					cny = it->first;
				}
			}
			
			if ((!previous_cell.is_alive() && live_total == 3)) {
				current_cell.set_alive();
				current_cell.inrecease_livetime();
				current_cell.set_colony(cny);
				++_alive;
			}
			else if ((previous_cell.is_alive() && (live_total == 2  || live_total == 3))) {
				current_cell.inrecease_livetime();
				++_alive;
			}
			else
				current_cell.set_dead();
			
			if (current_cell.is_alive() != previous_cell.is_alive())
				++changes;
			
		}
	}
	
	return changes;

}
