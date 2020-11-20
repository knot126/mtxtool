#include <iostream>
#include <fstream>
#include <cinttypes>
#include <cstdlib>
#include <zlib.h>
#include <jpeglib.h>
#include <png.h> 

class PngWrapper {
private:
	png_image png;
	uint32_t length;
	
public:
	char* encode(char* input, uint32_t width, uint32_t height);
	uint32_t getLength();
};