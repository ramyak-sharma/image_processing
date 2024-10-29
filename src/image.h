#include <stdint.h>
#include <cstdio>
#include <iostream>


//TODO: 
// Edge Detection -- DONE
// Saliency Mapping
// Texture Analysis
// Fourier Transform for Frequency Analysis
// Image Pyramids with scanning window

enum ImageType{
	PNG=0, JPG, BMP, TGA
};

struct Image
{
	uint8_t* data = NULL;
	size_t size = 0;
	int w;
	int h;
	int channels;
	
	Image(const char* filename);
	Image(int w, int h, int channels);
	Image(const Image& img);
	~Image();
	
	bool read(const char* filename);
	bool write(const char* filename);
	
	ImageType getFileType(const char* filename);
	bool makeGrayscale();
	Image& grayscale_avg();
	Image& grayscale_lum();

	//colormask can be used to hightlight certain colors
	//or put more focus on them
	Image& colorMask(float r, float g, float b);
	//r,g,b is [0,1]
	
	Image& convolveClamp0(int channel, int kerH, int kerW, float ker[], int kRc, int kCc);
	Image& convolveClampBorder(int channel, int kerH, int kerW, float ker[], int kRc, int kCc);

};

//Utility functions

int byteBound(int a);

//image diffmap between 2 consecutive frames can be use to detect motion
Image diffMap(Image& img1, Image& img2);
Image diffMap_scale(Image& img1, Image& img2, int scale=0);


