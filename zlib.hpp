#pragma once

#include <cinttypes>
#include <zlib.h>
 
class ZlibWrapper {
	/* Simple zlib wrapper
	 * For now, everything is being done at once; however, I plan to change
	 * this to use buffers in the future. 
	 */
private:
	z_stream streamInfo;
	int32_t status;
	int32_t outputLength;
	
public:
	void init();
	uint32_t decompress(char* input, char* output, uint32_t datalen, uint32_t outlen);
	uint32_t compress(char* input, char* output, uint32_t datalen);
	uint32_t getLength();
};
