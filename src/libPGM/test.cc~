/*=======================================
 * This file is intended to test pgm.so
 * Author: Zhang Yuren
 * Date 2013.6.15
 * =====================================*/
#include <stdio.h>
#include <iostream>
#include "mypgm.hh"

using namespace std;
int main()
{
	cout<<"*****************Create from raw data******************"<<endl;
	unsigned char data[4] = {0, 2, 3, 255};
	unsigned char *outputdata_uchar;
	float *outputdata_float;

	PGMImage image(data, 2, 2, 255);
	cout<< "Image width: \t"<< image.width() <<endl;
	cout<< "Image height:\t"<< image.height() <<endl;
	cout<< "Buffer size: \t"<<image.getBufferSize()<<endl;

	outputdata_uchar = image.ucharRawData();
	cout<<"Read Raw data(int):"<<endl;
	printf("[%d %d %d %d]\n", outputdata_uchar[0], outputdata_uchar[1], outputdata_uchar[2], outputdata_uchar[3]);
	
	outputdata_float = image.floatRawData();
	cout<<"Read Raw data(float)"<<endl;
	printf("[%f %f %f %f]\n", outputdata_float[0], outputdata_float[1], outputdata_float[2], outputdata_float[3]);

	cout<<"*****************Create from Image******************"<<endl;
	PGMImage anotherimage("test.png");
	cout<< "Image width: \t"<< anotherimage.width() <<endl;
	cout<< "Image height:\t"<< anotherimage.height() <<endl;
	// saveImage uses the data stored in the object, hence saving the 
	// image verifies the function.
	anotherimage.saveImage("result.jpg");

	
	return 0;

}
