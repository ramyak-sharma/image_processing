//clang++ main.cpp image.cpp -o run
#include "image.h"
#include "kernels.h"

int main(int argc, char** argv)
{
	Image test1("test/f1.png");
	Image test2("test/f2.png");

	Image res = test1;
	int s;
	float *ker = getKernel(EDGE_DETECTION_2, s);

	res.convolveClampBorder(0,s,s,ker, s/2, s/2);
	res.convolveClampBorder(1,s,s,ker, s/2, s/2);
	res.convolveClampBorder(2,s,s,ker, s/2, s/2);
	res.write("test/res.png");

	return 0;
}
