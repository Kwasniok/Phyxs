//
//  grid_to_bmp.hpp
//  Phyxs
//
//  Created by Jens Kwasniok on 11.06.16.
//  Copyright © 2016 Jens Kwasniok. All rights reserved.
//

#ifndef grid_to_bmp_hpp
#define grid_to_bmp_hpp

#include "Grid.hpp"

void grid_to_bmp(Grid& g, const char* path, unsigned long t);
void grid_to_bmp_bw(Grid& grid, const char* p, unsigned long t);

#endif /* grid_to_bmp_hpp */
