#include <iostream>
#include <complex>
#include <cstdlib>
#include <cmath>
#include <vector>
#include <algorithm>
using namespace std;


#ifndef PI
#define PI (3.141592653589793)
#endif

double maxi(double a, double b)
{
	return (a>b)?a:b;
}

double fRand(double fMin, double fMax)
{
    double f = (double)rand() / RAND_MAX;
    return fMin + f * (fMax - fMin);
}

double fitness(complex<double> C)
{
	complex<double> C1(-2.,-2.), C2(2.,2.); // centres des gaussienne
	double E1=1., E2=2.;	// etalement 2=>plus grosse
	double H1=1., H2=0.9; // hauteur
	
	return maxi(H1*exp(-norm(C1-C)/E1) , H2*exp(-norm(C2-C)/E2))  ;
}

int main(void)
{
	vector< complex<double> > Pop;
	for (int i=0; i<100; i++)
		{
		complex<double> C(fRand(-5.,5.),fRand(-5.,5.)); 
		//cout << C.real() << " " << C.imag() << " " << fitness(C) << endl;
		Pop.push_back(C);
		}
		
	double delta=1;	
	for (int i=0; i<1000; i++)	
		{
		int i1, i2;
		i1 = rand()% Pop.size();
		i2 = rand()% Pop.size();
		
		if (fitness(Pop[i1])>fitness(Pop[i2]))
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
}
