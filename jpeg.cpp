#include <iostream>
#include <cstdio>
#include <cinttypes>
#include <cstdlib>
#include <jpeglib.h>
#include <cstring>

#include "jpeg.hpp"

char* JpegWrapper::decode(char* input, int32_t size) {
	/** @see https://gist.github.com/PhirePhly/3080633 */
	int status;
	unsigned char* output;
	
	dcInfo.err = jpeg_std_error(&jpegError);
	jpeg_create_decompress(&dcInfo);
	jpeg_mem_src(&dcInfo, (const unsigned char*) input, size);
	
	status = jpeg_read_header(&dcInfo, TRUE);
	
	if (status != 1) {
		std::cout << "Error opening JPEG header. Is this a valid MTX file?" << std::endl;
		exit(1);
	}
	
	jpeg_start_decompress(&dcInfo);
	
	uint32_t width = dcInfo.output_width;
	uint32_t height = dcInfo.output_height;
	uint32_t compos = dcInfo.output_components;
	
	output = new unsigned char[width * height * compos];
	
	int rowSize = width * compos;
	
	while (dcInfo.output_scanline < dcInfo.output_height) {
		unsigned char* bufferArray[1];
		bufferArray[0] = output + dcInfo.output_scanline * rowSize;
		jpeg_read_scanlines(&dcInfo, bufferArray, 1);
	}
	
	jpeg_finish_decompress(&dcInfo);
	jpeg_destroy_decompress(&dcInfo);
	
	outputWidth = width;
	outputHeight = height;
	
	return (char*) output;
}

char* JpegWrapper::encode(char* input, int32_t width, int32_t height) {
	int status, compos = 3;
	unsigned long int size = width * height * 3;
	unsigned char* output;
	
	coInfo.err = jpeg_std_error(&jpegError);
	jpeg_create_compress(&coInfo);
	jpeg_mem_dest(&coInfo, &output, &size);
	
	coInfo.image_width = width;
	coInfo.image_height = height;
	coInfo.input_components = compos;
	coInfo.in_color_space = JCS_RGB;
	
	jpeg_set_defaults(&coInfo);
	jpeg_start_compress(&coInfo, TRUE);
	
	output = new unsigned char[size];
	
	int rowSize = width * compos;
	
	while (coInfo.next_scanline < height) {
		unsigned char* bufferArray[1];
		bufferArray[0] = output + coInfo.next_scanline * rowSize;
		jpeg_write_scanlines(&coInfo, bufferArray, 1);
	}
	
	jpeg_finish_compress(&coInfo);
	
	outputLength = (size) - (coInfo.dest->free_in_buffer);
	
	jpeg_destroy_compress(&coInfo);
	
	return (char*) output;
}

uint32_t JpegWrapper::getWidth()  { return outputWidth; }
uint32_t JpegWrapper::getHeight() { return outputHeight; }
uint32_t JpegWrapper::getLength() { return outputLength; }