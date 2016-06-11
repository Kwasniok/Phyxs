//
//  grid_to_bmp.cpp
//  Phyxs
//
//  Created by Jens Kwasniok on 11.06.16.
//  Copyright © 2016 Jens Kwasniok. All rights reserved.
//

#include "grid_to_bmp.hpp"
#include <string>
#include <fstream>

using namespace std;

#ifdef __LITTLE_ENDIAN__

struct bmpfileheader {
	char header_field[2] = {'B','M'}; // 'must' be
	uint32_t filesize = 0;
	uint16_t reserved_1 = 0;
	uint16_t reserved_2 = 0;
	uint32_t offset_to_data = 0;
} __attribute__((packed));

static_assert(sizeof(bmpfileheader) == 14, "packing of structs not supported");

struct bmpinfoheader {
	uint32_t header_size = 40; // must be
	int32_t bitmap_width = 0;
	int32_t bitmap_height = 0;
	uint16_t color_planes = 1; // must be
	uint16_t bits_per_pixel = 0;
	uint32_t compression_method = 0;
	uint32_t bitmap_size = 0;
	int32_t resolution_horizontal = 0;
	int32_t resolution_vertical = 0;
	uint32_t number_of_colors_in_palette = 0;
	uint32_t number_of_important_colors = 0; // generelly ignored
} __attribute((packed));

static_assert(sizeof(bmpinfoheader) == 40, "packing of structs not supported");

#else
#error system not little endian
#endif

void grid_to_bmp(Grid& grid, const char* p, unsigned long t)  {
	
	const int offset_to_data = sizeof(bmpfileheader) + sizeof(bmpinfoheader);
	
	int w = grid.get_size_x();
	int h = grid.get_size_y();
	
	int bytes_per_row = ((3*8 * w + 31)/32)*4; // rounding intended!, RGB 8-bit/pixel, includes padding
	int pixel_array_size = bytes_per_row * h;
	
	int filesize = offset_to_data + pixel_array_size;
	
	unsigned char *pixel_array = new unsigned char[pixel_array_size];
	memset(pixel_array, 0, pixel_array_size);
	
	unsigned char r,g,b;
	for(int y=0; y<h; ++y)
	{
		for(int x=0; x<w; ++x)
		{
			Cell& c = grid.get_cell(x, y);
			
			if (c.is_obstacle()) {r=128; b=128; g=128;}
			else if (c.is_alive()) {r=255; b=255; g=255;}
			else {r=0; b=0; g=0;}
			
			pixel_array[y * bytes_per_row + x*3 + 0] = b;
			pixel_array[y * bytes_per_row + x*3 + 1] = g;
			pixel_array[y * bytes_per_row + x*3 + 2] = r;
		}
	}
	
	
	
	bmpfileheader bmpfileheader;
	bmpfileheader.filesize = filesize;
	bmpfileheader.offset_to_data = offset_to_data;
	
	bmpinfoheader bmpinfoheader;
	bmpinfoheader.bits_per_pixel = 3*8; // RGB 3*8-bit/pixel
	bmpinfoheader.bitmap_width = w;
	bmpinfoheader.bitmap_height = -h; // default originis lower left corner hence the image is 'flipped' vertically to match prefered origin position at upper left corner
	
	std::string path = p;
	path += to_string(t);
	path += ".bmp";
	ofstream ofs (path);
	
	ofs.write(reinterpret_cast<const char*>(&bmpfileheader), 14);
	ofs.write(reinterpret_cast<const char*>(&bmpinfoheader), 40);
	ofs.write(reinterpret_cast<const char*>(pixel_array), pixel_array_size);
	
	ofs.close();
	
	delete[] pixel_array;
}

void grid_to_bmp_bw(Grid& grid, const char* p, unsigned long t)  {
	
	const int offset_to_data = sizeof(bmpfileheader) + sizeof(bmpinfoheader) + 2*4; // Clolor Table with 2 entries
	
	int w = grid.get_size_x();
	int h = grid.get_size_y();
	
	int bytes_per_row = ((1 * w + 31)/32)*4; // rounding intended!, BW 1-bit/pixel, includes padding
	int pixel_array_size = bytes_per_row * h;
	
	int filesize = offset_to_data + pixel_array_size;
	
	unsigned char *pixel_array = new unsigned char[pixel_array_size];
	memset(pixel_array, 0, pixel_array_size);
	
	bool bw;
	for(int y=0; y<h; ++y)
	{
		for(int x=0; x<w; ++x)
		{
			Cell& c = grid.get_cell(x, y);
			
			if (c.is_alive()) {bw = true;}
			else {bw = false;}
			
			pixel_array[y * bytes_per_row + (x/8)] |= bw << (7 - (x % 8));
		}
	}
	
	
	
	bmpfileheader bmpfileheader;
	bmpfileheader.filesize = filesize;
	bmpfileheader.offset_to_data = offset_to_data;
	
	bmpinfoheader bmpinfoheader;
	bmpinfoheader.bits_per_pixel = 1; // BW 1-bit/pixel
	bmpinfoheader.bitmap_width = w;
	bmpinfoheader.bitmap_height = -h; // default originis lower left corner hence the image is 'flipped' vertically to match prefered origin position at upper left corner
	bmpinfoheader.number_of_colors_in_palette = 2;
	unsigned char color0[4] = {0x00, 0x00, 0x00, 0x00}; // BGR-
	unsigned char color1[4] = {0xFF, 0xFF, 0xFF, 0x00}; // BGR-
	
	std::string path = p;
	path += to_string(t);
	path += ".bmp";
	ofstream ofs (path);
	
	ofs.write(reinterpret_cast<const char*>(&bmpfileheader), 14);
	ofs.write(reinterpret_cast<const char*>(&bmpinfoheader), 40);
	ofs.write(reinterpret_cast<const char*>(color0), 4);
	ofs.write(reinterpret_cast<const char*>(color1), 4);
	ofs.write(reinterpret_cast<const char*>(pixel_array), pixel_array_size);
	
	ofs.close();
	
	delete[] pixel_array;
}
