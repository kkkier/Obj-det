/*=====================================================
 * This is a c++ wrapper of the pgm functions of the 
 * vlfeat library. The wapper is used to encapsue the
 * image data.
 * Author: Zhang Yuren
 * Date:   2013.6.14 Fri
 * ===================================================*/
#include "stdio.h"
#include "pgm.h"
#include "qimage.h"

const char* const TMPFILE = "~tmp";

class PGMImage
{
public:
	PGMImage(const char* filename);
	PGMImage(const QImage& qimage);
	PGMImage(void const *data, int width, int height, int max_value);
	PGMImage(const PGMImage& other);
	~PGMImage();
	inline int width() const{return Info.width;}
	inline int height() const {return Info.height;}
	inline float* floatRawData() const {return f_data;}
	inline unsigned char* ucharRawData() const {return int_data;}
	int getBufferSize();
	void saveImage(const char* filename);

	//void printFile();
private:
	void initializeWithQImage(const QImage& image);
	void makeImageInfo(int width, int height, int max_value);
	void getRawData_uint8(vl_uint8** data);
	void getRawData_f(float** data);
private:
	FILE* filePGMImage;
	VlPgmImage Info;
	float* f_data;
	vl_uint8* int_data;
};


