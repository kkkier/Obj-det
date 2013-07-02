/*============================================================
 * This program is intended to test the sift detector functions.
 * The interest point detector of Hession Laplacian is used to 
 * get all the interest points. Test image is test.jpg, result
 * image is stored as result.jpg.
 *
 * Author: Zhang Yuren
 * Date:   2013.6.16
 * ===========================================================*/
#include <math.h>

#include <vector>
#include <iostream>
#include <fstream>
#include <string>
#include <utility>

#include "siftdetector.hh"
#include "qimage.h"

using namespace std;
const double PI = 3.1415926535;
PointVector generateInterestPoints(int width, int height)
{
	int space = 100;
	PointVector interestPointList;
	for(int i=1; i<width/space ;i++)
		for(int j=1; j<height/space; j++)
		{
			InterestPoint ip;
			ip.x = i*space;
			ip.y = j*space;
			ip.scale = 20;
			interestPointList.push_back(ip);
		}
	return interestPointList;
}

void drawCircle(QImage& image, int x, int y, double radii)
{
	/*
	int divides = radii;
	for(int i=0; i<divides; i++)
	{
		int xx = floor( x + radii*cos(static_cast<double>(i)/divides*PI*2));
		int yy = floor( y + radii*sin(static_cast<double>(i)/divides*PI*2));
		image.setPixel(xx, yy, qRgb(255,0,0));
	}
	*/
	
	int thick = 3;
	for(int xx=static_cast<int>(x-radii-thick) ;
			xx<static_cast<int>(x+radii+thick);
			xx++)
		for(int yy=static_cast<int>(y-radii-thick);
				yy<static_cast<int>(y+radii+thick);
				yy++)
		{
			if(fabs(sqrt(pow(static_cast<double>(xx-x),2)+pow(static_cast<double>(yy-y),2)) - radii) < thick)
				image.setPixel(xx, yy, qRgb(255,0,0));
		}
}

double distance_P2L(int xx, int yy, int x1, int y1, int x2, int y2)
{
	/*
	cout<<"("<<xx<<","<<yy<<")"<<endl;
	cout<<"("<<x1<<","<<y1<<")"<<endl;
	cout<<"("<<x2<<","<<y2<<")"<<endl;*/
	double dist = fabs((static_cast<double>(x1-xx) * (y2-yy)) - ((y1-yy) * (x2-xx))) / sqrt(pow(static_cast<double>(x1-x2),2)+pow(static_cast<double>(y1-y2),2));
//	cout<<"dist = "<<dist<<endl;
	return(dist);
}

void drawLine(QImage& image, int x1, int y1, int x2, int y2)
{
	/*
	int length = min(abs(y1-y2), abs(x1-x2));
	double xstep = static_cast<double>(x2-x1)/length;
	double ystep = static_cast<double>(y2-y1)/length;
	for(int i=0; i<length; i++)
	{
		int xx = floor( x1 + xstep*i );
		int yy = floor( y1 + ystep*i );
		image.setPixel(xx, yy, qRgb(255,0,0));
	}*/
	int thick = 3;
	int xx1, xx2, yy1, yy2;
	if(x1 > x2) 
	{
		xx1 = x2;
		xx2 = x1;
	}
	else
	{
		xx1 = x1;
		xx2 = x2;
	}
	if(y1 > y2) 
	{
		yy1 = y2;
		yy2 = y1;
	}
	else
	{
		yy1 = y1;
		yy2 = y2;
	}


	for(int xx=xx1-thick ;
			xx<xx2+thick;
			xx++)
		for(int yy=yy1-thick;
				yy<yy2+thick;
				yy++)
		{
			if( distance_P2L(xx, yy, x1, y1, x2, y2) < thick)
				image.setPixel(xx, yy, qRgb(255,0,0));
		}

}
void plotSIFTonImage(vector<VlSiftKeypoint>& KeyPointList, vector<double>& OrientationList)
{
	QImage image("test.png");
	for(int i=0; i<OrientationList.size(); i++)
	{
		drawCircle(image, KeyPointList[i].x, KeyPointList[i].y, KeyPointList[i].sigma);
		drawLine(image, KeyPointList[i].x, KeyPointList[i].y, KeyPointList[i].x + cos(OrientationList[i])*KeyPointList[i].sigma, KeyPointList[i].y + sin(OrientationList[i])*KeyPointList[i].sigma);
	}
	image.save("result.jpg");
}

void writeDescrToFile(vector< vector<float> >& descr, const char* filename)
{
	fstream fs;
	fs.open(filename, fstream::out);
	for(int i=0; i<descr.size(); i++)
	{
		for(int j=0; j<descr[i].size(); j++)
		{
			fs<<descr[i][j]<<" "; 
		}
		fs<<endl;
	}
	fs.close();
}

int main()
{	
	PGMImage pgmimage("test.png");
	PointVector interestPointList;
	interestPointList = generateInterestPoints(pgmimage.width(), pgmimage.height());
	SiftDetector siftdetector(pgmimage, interestPointList);
	siftdetector.initializeAllPreliminaryData();
	siftdetector.calculateAllSIFTDescriptor();
	vector< vector<float> > DescrList 		= siftdetector.getDescr();
	vector<VlSiftKeypoint> 	KeyPointList 	= siftdetector.getKeyPointList();
	vector<double> 			OrientationList = siftdetector.getOrientationList();	 

	cout<<"********************Report*********************"<<endl;
	cout<<" Number of descriptor extracted:"<<DescrList.size()<<endl;
	cout<<" Patch size to scale factor:"<< siftdetector.getPatchScaleRatio()<<endl;
	cout<<" Scale value"<<KeyPointList[0].sigma<<endl;

	plotSIFTonImage(KeyPointList, OrientationList);
	cout<<" SIFT ploted on result.jpg"<<endl;
	writeDescrToFile(DescrList,"descr.txt");
	cout<<" Descriptor saved in descr.txt"<<endl;
	return 0;
}
