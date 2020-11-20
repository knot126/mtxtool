#include <iostream>
#include <fstream>
#include <cinttypes>
#include <cstdlib>
#include <zlib.h>
#include <jpeglib.h>
#include <png.h>

#include "jpeg.hpp"
#include "zlib.hpp"
#include "mtx.hpp"
#include "png.hpp"

void MtxImage::decodeMtx(std::fstream& file) {
	file.seekg(0, file.end);
	len = file.tellg();
	file.seekg(0, file.beg);
	
	// Will throw an exception so there is no need to check
	content = new char[len];
	file.read(content, len);
	
	// NOTE: Again, a better way to do this would be nice...
	width = *(int *)(content + 16);
	height = *(int *)(content + 20);
	jpegLength = *(int *)(content + 24);
	uint32_t alphaStart = jpegLength + 32;
	
	// Create RGB/A data in memory
	rgbData = new char[width * height * 3];
	alphaData = new char[width * height];
	
	// Decode JPEG file
	QiJpegDecoder jpegw;
	jpegw.init();
	char* jptr = content + 28; // do it better in the future
	rgbData = jpegw.decode(jptr, jpegLength);
	
	// Decompress alpha data
	ZlibCompress zlibw;
	zlibw.init();
	zlibw.decompress(content+alphaStart, alphaData, len-alphaStart, width * height);
	
	// Load content as uncompressed RGBA data
	delete[] content;
	content = new char[width * height * 4];
	len = width * height * 4;
	
	for (uint32_t i = 0; i < width * height; i++)
	{
		content[i*4+0] = rgbData[i*3+0];
		content[i*4+1] = rgbData[i*3+1];
		content[i*4+2] = rgbData[i*3+2];
		content[i*4+3] = alphaData[i];
	}
}

void MtxImage::decodePng(std::fstream& file) {
	
}

void MtxImage::encodeMtx(std::fstream& file) {
	
}

void MtxImage::encodePng(std::fstream& file) {
	PngWrapper pngw;
	
	if (!content) {
		std::cout << "encodePng: There is no image data.";
		exit(1);
	}
	
	char* temp = pngw.encode(content, width, height);
	delete[] content;
	content = temp;
	
	file.write(content, sizeof(char)*width*height*4);
}

void MtxImage::showFileInfo() {
	std::cout << "Width: " << width << "\n" << "Height: " << height << "\n";
}

MtxImage::~MtxImage() {
	if (content) { delete[] content; }
	if (rgbData) { delete[] rgbData; }
	if (alphaData) { delete[] alphaData; }
}