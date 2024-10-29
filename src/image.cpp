#define STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "image.h"
#include "stb_image.h"
#include "stb_image_write.h"


Image::Image(const char* filename)
{
	if(read(filename))
	{
		std::cout<<"file read! "<<filename<<"\n";
		size = w*h*channels;
	}else
	{
		std::cout<<"file reading error "<<filename<<"\n";
	}
}

Image::Image(int w, int h, int channels)
	:w(w), h(h), channels(channels)
{
	size = w*h*channels;
	data = new uint8_t[size];
	
	for (int i = 0; i < size; i++)
	{
		data[i]=255;
	}
	

}

Image::Image(const Image& img)
	: Image(img.w, img.h, img.channels)
{
	memcpy(data, img.data, size);
}

Image::~Image()
{
	stbi_image_free(data);
}

bool Image::read(const char* filename)
{
	data = stbi_load(filename, &w, &h, &channels, 0);
	return data!=NULL;
}

bool Image::write(const char* filename)
{
	ImageType type = getFileType(filename);
	int success;
	switch(type)
	{
		case PNG:
			success = stbi_write_png(filename, w, h, channels, data, w*channels);
			break;
		case JPG:
			success = stbi_write_jpg(filename, w, h, channels, data, 100);
			break;
		case BMP:
			success = stbi_write_bmp(filename, w, h, channels, data);
			break;
		case TGA:
			success = stbi_write_tga(filename, w, h, channels, data);
			break;
			
	}
	return success;
}

ImageType Image::getFileType(const char* filename)
{
	const char* ext = strrchr(filename, '.');
	if(ext!=nullptr)
	{
		if(strcmp(ext, ".png")==0)
		{
			return PNG;
		}
		else if(strcmp(ext, ".jpg")==0)
		{
			return JPG;
		}
		else if(strcmp(ext, ".bmp")==0)
		{
			return BMP;
		}
		else if(strcmp(ext, ".tga")==0)
		{
			return TGA;
		}
	}
	return PNG;
}


Image& Image::grayscale_avg()
{
	if(channels >= 3)
	{
		for (int i = 0; i < size; i+=3)
		{
			int gray = (data[i]+data[i+1]+data[i+2])/3;	
			memset(data+i, gray, 3);
		}
	}
	else
	{
		printf("Image has less than 3 channels, so its assumed to be grayscale");	
	}
	return *this;
}

Image& Image::grayscale_lum()
{
	if(channels >= 3)
	{
		for (int i = 0; i < size; i+=channels)
		{
			int gray = 0.2126*data[i]+0.7152*data[i+1]+0.0722*data[i+2];	
			memset(data+i, gray, 3);
		}
	}
	else
	{
		printf("Image has less than 3 channels, so its assumed to be grayscale");	
	}
	return *this;
}

Image& Image::colorMask(float r, float g, float b)
{
	if(channels<3)
	{
		printf("ERROR: color mask requires at least 3 channels; channels less than 3\n");
	}
	else if(r<0||g<0||b<0||r>1||g>1||b>1)
	{
		printf("r, g, and b values must be >=0 and <= 1");
	}
	else
	{
		for	(int i = 0; i<size; i+=channels)
		{
			data[i]*=r;
			data[i+1]*=g;
			data[i+2]*=b;
		}
	}
	return *this;
}

Image& Image::convolveClamp0(int channel, int kerH, int kerW, float ker[], int kRc, int kCc)
{
	uint8_t new_data[w*h];
	int center = kRc*kerW+kCc;
	for(int k = channel; k<size ; k+=channels)
	{
		int sum = 0;
		for(int i=-((long)kRc); i<(long)kerH-kRc; i++)
		{
			long row = ((long)k/channels)/w-i;
			if(row<0)
			{
				continue;
			}
			if(row>h-1)
			{
				continue;
			}

			for(int j=-((long)kCc); j<(long)kerW-kCc; j++)
			{
				long col = ((long)k/channels)%w-j;
				if(col<0)
				{
					continue;
				}
				if(col>w-1)
				{
					continue;
				}
			sum += ker[center+i*(long)kerW+j]*data[(row*w+col)*channels+channel];
			}
		}	
		new_data[k/channels]=(uint8_t)byteBound(round(sum));
	}
	for(int k = channel; k<size ; k+=channels)
	{
		data[k] = new_data[k/channels];
	}
	return *this;
}

Image& Image::convolveClampBorder(int channel, int kerH, int kerW, float ker[], int kRc, int kCc)
{
	uint8_t new_data[w*h];
	int center = kRc*kerW+kCc;
	for(int k = channel; k<size ; k+=channels)
	{
		int sum = 0;
		for(int i=-((long)kRc); i<(long)kerH-kRc; i++)
		{
			long row = ((long)k/channels)/w-i;
			if(row<0)
			{
				row = 0;
			}
			if(row>h-1)
			{
				row = h-1;
			}

			for(int j=-((long)kCc); j<(long)kerW-kCc; j++)
			{
				long col = ((long)k/channels)%w-j;
				if(col<0)
				{
					col=0;
				}
				if(col>w-1)
				{
					col=w-1;
				}
			sum += ker[center+i*(long)kerW+j]*data[(row*w+col)*channels+channel];
			}
		}	
		new_data[k/channels]=(uint8_t)byteBound(round(sum));
	}
	for(int k = channel; k<size ; k+=channels)
	{
		data[k] = new_data[k/channels];
	}
	return *this;
}

Image& Image::convolveCyclic(int channel, int kerH, int kerW, float ker[], int kRc, int kCc)
{
	uint8_t new_data[w*h];
	int center = kRc*kerW+kCc;
	for(int k = channel; k<size ; k+=channels)
	{
		int sum = 0;
		for(int i=-((long)kRc); i<(long)kerH-kRc; i++)
		{
			long row = ((long)k/channels)/w-i;
			if(row<0)
			{
				row = row%h + h;
			}
			if(row>h-1)
			{
				row = row%h;
			}

			for(int j=-((long)kCc); j<(long)kerW-kCc; j++)
			{
				long col = ((long)k/channels)%w-j;
				if(col<0)
				{
					col=col%w+w;
				}
				if(col>w-1)
				{
					col=col%w;
				}
			sum += ker[center+i*(long)kerW+j]*data[(row*w+col)*channels+channel];
			}
		}	
		new_data[k/channels]=(uint8_t)byteBound(round(sum));
	}
	for(int k = channel; k<size ; k+=channels)
	{
		data[k] = new_data[k/channels];
	}
	return *this;
}




int byteBound(int a)
{
	if (a<0)return 0;
	if (a>255) return 255;
	return a;
}

Image diffMap(Image& img1, Image& img2)
{
	int w = fmin(img1.w, img2.w);
	int h = fmin(img1.h, img2.h);
	int c = fmin(img1.channels, img2.channels);
	Image res(w,h,c);

	for (int i = 0; i<h; i++)
	{
		for(int j = 0; j<w; j++)
		{
			for(int k = 0; k<c; k++)
			{
				res.data[(i*w+j)*c+k] = byteBound(abs(img1.data[(i*w+j)*c+k]-img2.data[(i*w+j)*c+k]));
			}
		}
	}

	return res;
}

Image diffMap_scale(Image& img1, Image& img2, int scale)
{
	//this is basically the same function but scales up the
	//largest difference to 255
	//this basically means that small differences will be 
	//easier to notice

	int w = fmin(img1.w, img2.w);
	int h = fmin(img1.h, img2.h);
	int c = fmin(img1.channels, img2.channels);
	Image res(w,h,c);
	int largest = 0;

	for (int i = 0; i<h; i++)
	{
		for(int j = 0; j<w; j++)
		{
			for(int k = 0; k<c; k++)
			{
				res.data[(i*w+j)*c+k] = byteBound(abs(img1.data[(i*w+j)*c+k]-img2.data[(i*w+j)*c+k]));
				largest = fmax(largest, res.data[(i*w+j)*c+k]);
			}
		}
	}

	scale = 255/fmax(1, fmax(scale, largest));
	for(int i = 0; i<res.size; i++)
	{
		res.data[i]*=scale;
	}

	return res;
}
