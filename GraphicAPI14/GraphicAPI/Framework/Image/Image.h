#ifndef _IMAGE_H_
#define _IMAGE_H_


//Texture format enums
enum FORMAT {
	FORMAT_NONE		= 0,
	FORMAT_I1		= 1,
	FORMAT_I2		= 2,
	FORMAT_I4		= 3,
	FORMAT_I8		= 4,
	FORMAT_IA8		= 5,
	FORMAT_GREY8	= 6,
	FORMAT_RGB8		= 7,
	FORMAT_RGBA8	= 8,
	FORMAT_I16		= 9,
	FORMAT_IA16		= 10,
	FORMAT_GREY16	= 11,
	FORMAT_RGB16	= 12,
	FORMAT_RGBA16	= 13,
	FORMAT_I16f		= 14,
	FORMAT_IA16f	= 15,
	FORMAT_GREY16f	= 16,
	FORMAT_RGB16f	= 17,
	FORMAT_RGBA16f	= 18,
	FORMAT_I32f		= 19,
	FORMAT_IA32f	= 20,
	FORMAT_GREY32f	= 21,
	FORMAT_RGB32f	= 22,
	FORMAT_RGBA32f	= 23,
	FORMAT_RGB332	= 24,
	FORMAT_DXT1		= 25,
	FORMAT_DXT3		= 26,
	FORMAT_DXT5		= 27,
	FORMAT_3DC		= 28,
};

inline int getChannelCount(const FORMAT format)
{
	static const int channels[]={0,1,1,1,1,2,1,3,4,1,2,1,3,4,1,2,1,3,4,1,2,1,3,4,3,3,4,4,2};
	return channels[format];
}

inline int getBytesPerPixel(const FORMAT format)
{
	static const int bytes[]={0,1,1,1,1,2,1,3,4,2,4,2,6,8,2,4,2,6,8,4,8,4,12,16};
	return bytes[format];
}

inline int getBytesPerChannel(const FORMAT format)
{
	return (format<=FORMAT_RGBA8) ? 1 : (format<=FORMAT_RGBA16f) ? 2 : 4;
}

inline int getPaletteSize(const FORMAT format)
{
	static const int size[]={0,8,16,64,1024,1024,0,0,0,65536*4,65536*4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
	return size[format];
}

inline bool isPalettedFormat(const FORMAT format)
{
	return ((format>=FORMAT_I1 && format<FORMAT_GREY8) ||
			(format>=FORMAT_I16 && format<FORMAT_GREY16) ||
			(format>=FORMAT_I16f && format<FORMAT_GREY16f) ||
			(format>=FORMAT_I32f && format<FORMAT_GREY32f));
}

//Image class
class CImage
{
public:
	CImage();
	CImage(const CImage* copy);
	~CImage();

	void* Create(FORMAT fmt,int w,int h,int d,int imgCount=1,int mipMapCount=1);
	bool CopyImage(CImage* copy);
	void Release();

	int getWidth()		const { return width; }
	int getHeight()		const { return height; }
	int getDepth()		const { return depth; }
	int getPitch()		const { return pitch; }
	bool is1D()			const { return (height==1 && depth==1); }
	bool is2D()			const { return (height>1 && depth==1); }
	bool is3D()			const { return (depth>1); }
	FORMAT getFormat()	const { return format; }

	void* getPixels()	const { return pixels; }
	void* getPixels(int imgNum);
	void* getPalette()	const { return palette; }
	
	int getImageCount() const { return nImages; }

	void ExpandPalette(bool ExpandAlphaChannel=false);
	void setPalette(ubyte* pal, uint size) ;

	void LoadTestPattern();
	void CreateGreyPalette();
	CImage* getImage(int num);

protected:

	int width,height,depth,pitch;
	int nMipMaps;
	int nImages;
	FORMAT format;
	ubyte* pixels;
	ubyte* palette;

};

#endif //_IMAGE_H_