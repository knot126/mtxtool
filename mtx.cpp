#include <iostream>
#include <fstream>
#include <cinttypes>
#include <cstdlib>
#include <cstring>
#include <zlib.h>
#include <jpeglib.h>
#include <png.h>

#include "jpeg.hpp"
#include "zlib.hpp"
#include "mtx.hpp"
#include "png.hpp"

void MtxImage::decodeMtx(std::fstream& file) {
	// Get file length
	file.seekg(0, file.end);
	len = file.tellg();
	file.seekg(0, file.beg);
	
	// Will throw an exception so there is no need to check
	content = new char[len];
	file.read(content, len);
	
	// Get width, height, and length of JPEG image
	// NOTE: Again, a better way to do this would be nice...
	width = *(int *)(content + 16);
	height = *(int *)(content + 20);
	jpegLength = *(int *)(content + 24);
	uint32_t alphaStart = jpegLength + 32;
	
	// Create RGB/A data in memory
	rgbData = new char[width * height * 3];
	alphaData = new char[width * height];
	
	// Decode JPEG file
	JpegWrapper jpegw;
	char* jptr = content + 28; // do it better in the future
	rgbData = jpegw.decode(jptr, jpegLength);
	
	// Decompress alpha data
	ZlibWrapper zlibw;
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
	// Get file length
	file.seekg(0, file.end);
	len = file.tellg();
	file.seekg(0, file.beg);
	
	// Read file content to memory
	content = new char[len];
	file.read(content, len);
	
	// Decode PNG image
	PngWrapper pngw;
	char* temp = pngw.decode(content, len);
	delete[] content;
	content = temp;
	
	// Set width and height
	width = pngw.getWidth();
	height = pngw.getHeight();
}

void MtxImage::encodeMtx(std::fstream& file) {
	rgbData = new char[width * height * 3];
	alphaData = new char[width * height];
	char* compressedAlpha = new char[width * height];
	
	if (!content) {
		std::cout << "Error: content is null pointer. Did PNG decode fail?" << std::endl;
		exit(1);
	}
	
	for (uint32_t i; i < width * height; i++) {
		rgbData[i*3+0] = content[i*4+0];
		rgbData[i*3+1] = content[i*4+1];
		rgbData[i*3+2] = content[i*4+2];
		alphaData[i]   = content[i*4+3];
	}
	
	delete[] content;
	content = nullptr;
	
	ZlibWrapper zlibw;
	zlibw.init();
	uint32_t alphaLen = zlibw.compress(alphaData, compressedAlpha, width * height);
	
	JpegWrapper jpegw;
	char* jpegData = jpegw.encode(rgbData, width, height);
	uint32_t jpegLen = jpegw.getLength();
	
	if (!jpegData) {
		std::cout << "JPEG encode failed." << std::endl;
	}
	
	len = 28 + 4 + jpegLen + 4 + alphaLen;
	
	// Since I really can't be bother to figure out another way to do this in
	// "the C++ way" at the moment, I'm just implementing my tape for now.
	// I will fix this :-)
	
	const uint8_t header[16] = {
		0x01, 0x00, 0x00, 0x00,
		0xFF, 0xFF, 0xFF, 0xFF,
		0xFF, 0xFF, 0xFF, 0xFF,
		0x01, 0x00, 0x00, 0x00
	};
	
	std::cout << width << "w," << height << "h," << jpegLen << "jl," << alphaLen << "al" << std::endl;
	
	if (jpegLen > width * height * 3) {
		std::cout << "Error: JPEG would be larger than the orginial, uncompressed image data." << std::endl;
	}
	
	file.write((const char*) header, 16);
	file.write((const char*) &width, 4);
	file.write((const char*) &height, 4);
	file.write((const char*) &jpegLen, 4);
	file.write((const char*) jpegData, jpegLen);
	file.write((const char*) &alphaLen, 4);
	file.write((const char*) compressedAlpha, alphaLen);
	
	delete[] jpegData;
	delete[] compressedAlpha;
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

namespace util {
	char getFirstByte(std::fstream& file) {
		char b = 0;
		file.read(&b, 1);
		file.seekg(0, file.beg);
		return b;
	}
}