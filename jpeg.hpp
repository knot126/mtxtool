#include <cinttypes>
#include <jpeglib.h>

class JpegWrapper {
private:
	jpeg_decompress_struct dcInfo;
	jpeg_compress_struct coInfo;
	jpeg_error_mgr jpegError;
	uint32_t outputWidth;
	uint32_t outputHeight;
	uint32_t outputLength;
	
public:
	char* encode(char* input, uint32_t width, uint32_t height);
	char* decode(char* input, uint32_t size);
	uint32_t getWidth();
	uint32_t getHeight();
	uint32_t getLength();
};