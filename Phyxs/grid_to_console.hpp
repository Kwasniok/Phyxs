//
//  grid_to_console.hpp
//  Phyxs
//
//  Created by Jens Kwasniok on 11.06.16.
//  Copyright © 2016 Jens Kwasniok. All rights reserved.
//

#ifndef grid_to_console_hpp
#define grid_to_console_hpp

#include <iostream>
#include "Grid.hpp"

//! supports only up to 8 different colonies (0 to 7)
void grid_to_console(Grid& grid, std::ostream& out, int t);

#endif /* grid_to_console_hpp */
