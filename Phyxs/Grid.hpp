//
//  Grid.hpp
//  Phyxs
//
//  Created by Jens Kwasniok on 07.06.16.
//  Copyright © 2016 Jens Kwasniok. All rights reserved.
//

#ifndef Grid_hpp
#define Grid_hpp

#include <vector>
#include "Cell.hpp"

class Grid {
public:
	typedef int index_t;
	
private:
	std::vector<Cell> _cells;
	index_t _size_x, _size_y;
	int _alive;
	
	inline index_t pos(index_t x, index_t y) const {
		if (x < 0) return pos(x + _size_x, y);
		if (x >= _size_x) x = x % _size_x;
		if (y < 0) return pos(x, y + _size_y);
		if (y >= _size_y) y = y % _size_y;
		
		return x * _size_y + y;
	}
	
public:
	
	Grid(index_t size_x, index_t size_y);
	Grid(const Grid& rhs) : _size_x(rhs._size_x), _size_y(rhs._size_y), _cells(rhs._cells) { };
	Grid(Grid&& rhs) : _size_x(rhs._size_x), _size_y(rhs._size_y), _cells(std::move(rhs._cells)) { };
	
	void operator=(const Grid& rhs) {_size_x = rhs._size_x; _size_y = rhs._size_y; _cells = rhs._cells;}
	
	index_t get_size_x() const {return _size_x;}
	index_t get_size_y() const {return _size_y;}
	
	void set_cell(index_t x, index_t y, const Cell& c);
	Cell& get_cell(index_t x, index_t y);
	
	int get_number_of_living_cells() const {return _alive;}
	
	bool has_same_alive_pattern(const Grid& rhs);
	
	int update();
};

#endif /* Grid_hpp */
