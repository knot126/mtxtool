#include <cinttypes>
#include <zlib.h>
 
class ZlibCompress {
	/* Simple zlib wrapper
	 * For now, everything is being done at once; however, I plan to change
	 * this to use buffers in the future. 
	 */
private:
	z_stream streamInfo;
	int32_t status;
	
public:
	void init();
	uint32_t decompress(char* input, char* output, uint32_t datalen, uint32_t outlen);
	uint32_t compress(char* input, char* output, uint32_t datalen);
};