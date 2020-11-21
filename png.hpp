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
	uint32_t outputWidth;
	uint32_t outputHeight;
	
public:
	char* encode(char* input, uint32_t width, uint32_t height);
	char* decode(char* input, uint32_t size);
	uint32_t getLength();
	uint32_t getHeight();
	uint32_t getWidth();
};