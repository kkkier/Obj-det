#include <qapplication.h>
#include <qlabel.h>
#include <qimage.h>
#include <qcolor.h>
#include <iostream>
#include <vector>
using namespace std;
void dosome(vector<int>& v)
{
	v[5] = 10;
	v.push_back(2);
}
int main(int argc, char* argv[])
{
	QApplication app(argc, argv);
	QLabel *label = new QLabel("Hello world!",0);
	label->show();
	QImage img;
	for(int i = 1;i<=1;i++)
	{vector<int> v;
	dosome(v);
	for(vector<int>::iterator itr= v.begin();itr!=v.end();itr++)
	{
		cout<< *itr << ",";
	}
	cout<<endl;}
	img.create(100,100,8);
	img.fill(1);
	QRgb rgb = img.pixel(50,50);
	cout<<qBlue(rgb)<<","<<qRed(rgb)<<","<<qGreen(rgb)<<endl;
	return app.exec();
}
