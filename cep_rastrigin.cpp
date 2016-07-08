#include <iostream>
#include <complex>
#include <cstdlib>
#include <cmath>
#include <vector>
#include <algorithm>
using namespace std;

#include "CImg.h"
using namespace cimg_library;


#ifndef PI
#define PI (3.141592653589793)
#endif

double max(double a, double b)
{
	return (a>b)?a:b;
}
double min(double a, double b)
{
	return (a<b)?a:b;
}

int scaling(double v, double a, double b, int pix)
{
//			3.1			3		4			1000
//			=> 100			
	return max(min(((v-a)/(b-a))*pix, pix-1),0);
}

double fRand(double fMin, double fMax)
{
    double f = (double)rand() / RAND_MAX;
    return fMin + f * (fMax - fMin);
}

double fitness(complex<double> C)
{
	
	double x=C.real(), y=C.imag();

	//return (1/sqrt(x*x+y*y))*cos(x*x+y*y);
	return 20+x*x-10*cos(2*PI*x)+y*y-10*cos(2*PI*y) ;
}

int main(void)
{
CImg<unsigned char> image(1024,1024);
CImgDisplay displ(image);


	vector< complex<double> > Pop;
	for (int i=0; i<100; i++)
		{
		complex<double> C(fRand(-5.12,5.12),fRand(-5.12,5.12)); 
		//cout << C.real() << " " << C.imag() << " " << fitness(C) << endl;
		Pop.push_back(C);
		}
	
	double delta=1;	
	for (int i=0; i<100000; i++)	
		{
	
	for (int i=0; i<image.width(); i++)
		for (int j=0; j<image.height(); j++)
		//	image(i,j)=0;
		//*
			if (image(i,j)!=0 )
				image(i,j)=image(i,j)-1;
		//*/
		
	for(int i=0; i<Pop.size(); i++)
		{
		int x, y;
		x=scaling(Pop[i].real(), -5.12, 5.12, image.width() );
		y=scaling(Pop[i].imag(), -5.12, 5.12, image.height() );
		
		const unsigned char white[] = { 255,255,255 };
		image.draw_circle(x,y,2,white);
		//image(x,y)= 255;
		
		}

//	image.blur(2);
	displ.display(image);
//	displ.wait(1);

		int i1, i2;
		i1 = rand()% Pop.size();
		i2 = rand()% Pop.size();
		
		if (fitness(Pop[i1])<fitness(Pop[i2]))
			Pop[i2]=Pop[i1];
		else
			Pop[i1]=Pop[i2];
		
		
		double u1=fRand(0,1);
		double u2=fRand(0,1);
	
		double zx,zy;
		zx=sqrt(-2*log(u1))*cos(2*PI*u2);
		zy=sqrt(-2*log(u1))*sin(2*PI*u2);

		complex<double> M ( sqrt(delta)*zx , sqrt(delta)*zy);
		Pop[i1]+=M;
		
		delta*=0.99;
		cerr << delta << endl;
		}
	
	for (int i=0; i<Pop.size(); i++)
		cout << Pop[i].real() << " " << Pop[i].imag() << " " << fitness(Pop[i]) << endl;

	cerr << Pop[0] << "=>" << fitness(Pop[0]) << endl;
}
