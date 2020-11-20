#include <iostream>
#include <fstream>
#include <cinttypes>
#include <cstdlib>
#include <zlib.h>
#include <jpeglib.h>
#include <png.h>
#include <cstring>

#include "png.hpp"

char* PngWrapper::encode(char* input, uint32_t width, uint32_t height) {
	memset(&png, 0, sizeof(png_image));
	
	png.opaque = nullptr;
	png.version = PNG_IMAGE_VERSION;
	png.width = width;
	png.height = height;
	png.format = PNG_FORMAT_RGBA;
	
	char* output = new char[width * height * 4]; // reasonable guess to max PNG size for now
	char* opc = output;
	
	png_alloc_size_t sizet = width * height * 4;
	bool result = png_image_write_to_memory(&png, nullptr, &sizet, 0, input, 0, nullptr);
	
	if (!result) {
		std::cout << "Libpng error.\n";
	}
	
	result = png_image_write_to_memory(&png, output, &sizet, 0, input, 0, nullptr);
	
	return output;
}

uint32_t PngWrapper::getLength() {
	return length;
}