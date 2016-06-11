//
//  Cell.hpp
//  Phyxs
//
//  Created by Jens Kwasniok on 11.06.16.
//  Copyright © 2016 Jens Kwasniok. All rights reserved.
//

#ifndef Cell_hpp
#define Cell_hpp

#include <sys/types.h>

class Cell {
	int16_t value = 0x0000;
	int live_time = 0;
	
public:
	
	typedef int16_t value_t;
	typedef int8_t colony_t;
	
	const static value_t none = 0x0000;
	const static value_t alive = 0x0100;
	const static value_t obstacle = 0x0200;
private:
	const static value_t colony_maks = 0x00FF;
	
public:
	
	Cell() { }
	explicit Cell(value_t value) : value(value) { }
	
	inline bool is_alive() const {return value & alive;}
	inline void set_alive() {if (!is_obstacle()) {value |= alive;}}
	inline void set_dead() {if (!is_obstacle()) {value &= ~alive;} live_time = 0; clear_colony();}
	
	inline bool is_obstacle() const {return value & obstacle;}
	inline void set_obstacle() {value |= obstacle;}
	inline void unset_obstacle() {value &= ~obstacle;}
	
	inline int get_livetime() const {return live_time;}
	inline void inrecease_livetime() {++live_time;}
	
	inline colony_t get_colony() const {return value & colony_maks;}
	inline void set_colony(colony_t cny) {clear_colony(); value |= cny;}
	inline void clear_colony() {value &= ~colony_maks;}
};

#endif /* Cell_hpp */
