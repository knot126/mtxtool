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
	// Zero out png image struct
	memset(&png, 0, sizeof(png_image));
	
	// Set up png image struct
	png.opaque = nullptr;
	png.version = PNG_IMAGE_VERSION;
	png.width = width;
	png.height = height;
	png.format = PNG_FORMAT_RGBA;
	
	// Create space for output
	// reasonable guess to max PNG size for now
	char* output = new char[width * height * 4];
	
	// Set image size and initial test run
	png_alloc_size_t sizet = width * height * 4;
	bool result = png_image_write_to_memory(&png, nullptr, &sizet, 0, input, 0, nullptr);
	
	if (!result) {
		std::cout << "Initial test of libpng failed." << std::endl;
		return nullptr;
	}
	
	// Write the png image
	result = png_image_write_to_memory(&png, output, &sizet, 0, input, 0, nullptr);
	
	return output;
}

char* PngWrapper::decode(char* input, uint32_t size) {
	int result;
	
	// Zero out png image struct
	memset(&png, 0, sizeof(png_image));
	
	// Set up png image struct
	png.version = PNG_IMAGE_VERSION;
	
	result = png_image_begin_read_from_memory(&png, input, size);
	
	// Check for errors
	if (!result) {
		std::cout << "png_image_begin_read_from_memory failed, stat " << result << std::endl;
		return nullptr;
	}
	
	// Set properties
	int height = png.height;
	int width = png.width;
	char* output = new char[width * height * 4];
	int stride = width * 4;
	
	png.format = PNG_FORMAT_RGBA;
	
	// Finish the reading
	result = png_image_finish_read(&png, nullptr, output, stride, nullptr);
	
	if (!result) {
		std::cout << "png_image_finish_read failed, stat " << result << std::endl;
		return nullptr;
	}
	
	outputWidth = width;
	outputHeight = height;
	
	return output;
}

uint32_t PngWrapper::getLength() { return length; }
uint32_t PngWrapper::getHeight() { return outputHeight; }
uint32_t PngWrapper::getWidth()  { return outputWidth; }