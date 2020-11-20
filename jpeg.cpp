#include <iostream>
#include <cstdio>
#include <cinttypes>
#include <cstdlib>
#include <jpeglib.h>
#include <cstring>

#include "jpeg.hpp"

void QiJpegDecoder::init() {
	decompressInfo.err = jpeg_std_error(&jpegError);
	jpeg_create_decompress(&decompressInfo);
}

char* QiJpegDecoder::decode(char* input, int32_t size) {
	/** @see https://gist.github.com/PhirePhly/3080633 */
	int status;
	unsigned char* output;
	
	jpeg_mem_src(&decompressInfo, (const unsigned char*) input, size);
	
	status = jpeg_read_header(&decompressInfo, TRUE);
	
	if (status != 1) {
		std::cout << "Error opening JPEG header. Is this a valid MTX file?" << std::endl;
		exit(1);
	}
	
	jpeg_start_decompress(&decompressInfo);
	
	uint32_t width = decompressInfo.output_width;
	uint32_t height = decompressInfo.output_height;
	uint32_t compos = decompressInfo.output_components;
	
	output = new unsigned char[width * height * compos];
	
	int rowSize = width * compos;
	
	while (decompressInfo.output_scanline < decompressInfo.output_height) {
		unsigned char* bufferArray[1];
		bufferArray[0] = output + decompressInfo.output_scanline * rowSize;
		jpeg_read_scanlines(&decompressInfo, bufferArray, 1);
	}
	
	jpeg_finish_decompress(&decompressInfo);
	
	return (char*) output;
}

QiJpegDecoder::~QiJpegDecoder() {
	jpeg_destroy_decompress(&decompressInfo);
}