#include <iostream>
#include <cinttypes>
#include <cstdlib>
#include <zlib.h>

#include "zlib.hpp"

void ZlibWrapper::init() {
	streamInfo.zalloc = Z_NULL;
	streamInfo.zfree = Z_NULL;
	streamInfo.opaque = Z_NULL;
	streamInfo.avail_in = 0;
	streamInfo.next_in = Z_NULL;
}

uint32_t ZlibWrapper::decompress(char* input, char* output, uint32_t datalen, uint32_t outlen) {
	status = inflateInit(&streamInfo);
	if (status != Z_OK) {
		std::cout << "Error with inflateInit(): " << status << std::endl;
		exit(1);
	}
	
	streamInfo.avail_in = datalen;
	streamInfo.avail_out = outlen;
	streamInfo.next_in = (Bytef*) input;
	streamInfo.next_out = (Bytef*) output;
	
	status = inflate(&streamInfo, Z_FINISH);
	if (status != Z_OK && status != Z_STREAM_END) {
		std::cout << "Warning: Zlib inflate returned " << status << " instead of Z_OK." << std::endl;
	}
	
	inflateEnd(&streamInfo);
	
	return streamInfo.total_out;
}

uint32_t ZlibWrapper::compress(char* input, char* output, uint32_t datalen) {
	status = deflateInit(&streamInfo, 6);
	if (status != Z_OK) {
		std::cout << "Error with deflateInit(): " << status << std::endl;
		exit(1);
	}
	
	streamInfo.avail_in = datalen;
	streamInfo.avail_out = datalen;
	streamInfo.next_in = (Bytef*) input;
	streamInfo.next_out = (Bytef*) output;
	
	status = deflate(&streamInfo, Z_FINISH);
	
	if (status != Z_OK && status != Z_STREAM_END) {
		std::cout << "Warning: deflate() returned " << status << " instead of Z_OK." << std::endl;
	}
	
	status = deflateEnd(&streamInfo);
	
	if (status == Z_DATA_ERROR) {
		std::cout << "Warning: deflateEnd() returned Z_DATA_ERROR." << std::endl;
	}
	
	outputLength = streamInfo.total_out;
	return streamInfo.total_out;
}

uint32_t ZlibWrapper::getLength() { return outputLength; }