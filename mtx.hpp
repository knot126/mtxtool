#include <iostream>
#include <fstream>
#include <cinttypes>
#include <cstdlib>
#include <zlib.h>
#include <jpeglib.h>
#include <png.h>

class MtxImage {
	/* Wrapper around image handling, including MTX encoding and decoding. */
private:
	char* content = nullptr;
	char* rgbData = nullptr;
	char* alphaData = nullptr;
	uint32_t len;
	uint32_t jpegLength;
	uint32_t width;
	uint32_t height;
	uint32_t dataType;
	
public:
	void decodeMtx(std::fstream& file); // load as mtx
	void decodePng(std::fstream& file); // load as png
	void encodeMtx(std::fstream& file); // save as mtx
	void encodePng(std::fstream& file); // save as png
	void showFileInfo();
	~MtxImage();
};

namespace util {
	char getFirstByte(std::fstream& file);
}