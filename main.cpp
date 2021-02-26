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

#ifndef CONVERT_PNG_ONLY
const int MT_ARGS_COUNT = 4;
const int MT_INFILE_LOCATION = 2;
const int MT_OUTFILE_LOCATION = 3;
#else
const int MT_ARGS_COUNT = 3;
const int MT_INFILE_LOCATION = 1;
const int MT_OUTFILE_LOCATION = 2;
#endif

int main(int argc, char* argv[]) {
	if (argc != MT_ARGS_COUNT) {
		std::cout << "Error: Please specify input and output file argument.\n";
		exit(1);
	}
	
	std::fstream ifile;
	std::fstream ofile;
	
	ifile.open(argv[MT_INFILE_LOCATION], std::ios::in | std::ios::binary);
	ofile.open(argv[MT_OUTFILE_LOCATION], std::ios::out | std::ios::binary);
	
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
