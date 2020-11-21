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

int main(int argc, char* argv[]) {
	if (argc != 3) {
		std::cout << "Error: Please specify input and output file argument.\n";
		exit(1);
	}
	
	std::cout << "mtxtool v1.0\n";
	
	std::fstream ifile;
	std::fstream ofile;
	
	ifile.open(argv[1], std::ios::in | std::ios::binary);
	ofile.open(argv[2], std::ios::out | std::ios::binary);
	
	MtxImage image;
	char firstByte = util::getFirstByte(ifile);
	
	if (firstByte < 2) {
		std::cout << "Detected MTX image; convert to PNG." << std::endl;
		image.decodeMtx(ifile);
		image.encodePng(ofile);
	} else {
		std::cout << "Detected PNG image; convert to MTX." << std::endl;
		image.decodePng(ifile);
		image.encodeMtx(ofile);
	}
	
	ifile.close();
	ofile.close();
	
	return 0;
}