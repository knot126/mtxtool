#include <cinttypes>
#include <jpeglib.h>

class QiJpegDecoder {
private:
	jpeg_decompress_struct decompressInfo;
	jpeg_error_mgr jpegError;
	
public:
	void init();
	char* decode(char* input, int32_t size);
	~QiJpegDecoder();
};