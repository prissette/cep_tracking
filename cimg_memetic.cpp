#include <iostream>
#include <complex>
#include <vector>
#include <algorithm>

#include "CImg.h"
using namespace cimg_library;
using namespace std;

double fitness(complex<int> A)
	{
	return abs(A-complex<int>(400,300));
	}

void move(complex<int> &A)
	{
	complex<int> M(rand()%3-1, rand()%3-1);
	
	A+=M;
	}

int main(void)
{
CImg<unsigned char> image(1024,512);
CImgDisplay displ(image);

vector<complex<int> > individu;

for (int i=0; i<1000; i++)
	individu.push_back( complex<int>(rand()%image.width(), rand()%image.height() ));

for (int i=0; i<individu.size(); i++)
	image( individu[i].real(), individu[i].imag())= 255;

while (!displ.is_keyESC())
	{
		for (int i=0; i<image.width(); i++)
		for (int j=0; j<image.height(); j++)
			if (image(i,j)!=0 )
				image(i,j)=image(i,j)-1;
	
		
	random_shuffle(individu.begin(), individu.end() );
	
	for (int i=0; i<individu.size(); i+=2)
		{
		if (fitness(individu[i])<fitness(individu[i+1]))
			individu[i+1]=individu[i];
		else
			individu[i]=individu[i+1];
		move(individu[i]);
		}

	
	for (int i=0; i<individu.size(); i++)
		image( individu[i].real(), individu[i].imag())= 255;



	displ.wait(10);
	displ.display(image);
	}


}
