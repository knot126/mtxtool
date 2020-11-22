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

int main(int argc, char* argv[]) {
	if (argc != 4) {
		std::cout << "Error: Please specify input and output file argument.\n";
		exit(1);
	}
	
	std::fstream ifile;
	std::fstream ofile;
	
	ifile.open(argv[2], std::ios::in | std::ios::binary);
	ofile.open(argv[3], std::ios::out | std::ios::binary);
	
	MtxImage image;
	
#ifndef CONVERT_PNG_ONLY
	if (!strcmp("import", argv[1])) {
#endif
		std::cout << "Converting MTX to PNG..." << std::endl;
		image.decodeMtx(ifile);
		image.encodePng(ofile);
#ifndef CONVERT_PNG_ONLY
	} else if (!strcmp("export", argv[1])) {
		std::cout << "Converting PNG to MTX..." << std::endl;
		image.decodePng(ifile);
		image.encodeMtx(ofile);
	}
#endif
	
	ifile.close();
	ofile.close();
	
	return 0;
}