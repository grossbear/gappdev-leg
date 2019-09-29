///////////////////////////////////////////////////////////////////////////////////////////////
//	PNG.h
//	PNG Image Format Loading/Saving Class Definition
//	
////////////////////////////////////////////////////////////////////////////////////////////////

#include "Utils/mem/memory_allocator.h"
#include "Image/Image.h"
#include "Utils/IOStream/IDataStream.h"
#include "PNG.h" 

#include <setjmp.h>
#include <memory.h>

////////////////////////////////////////////////////////////////////////////////////////////////
const char CPNGImageIO::m_strExt[] = {"png"};
////////////////////////////////////////////////////////////////////////////////////////////////
const char CPNGImageIO::m_strInfo[] = {"Portable Network Graphics Image File"};


////////////////////////////////////////////////////////////////////////////////////////////////
CPNGImageIO::CPNGImageIO()
{
}

////////////////////////////////////////////////////////////////////////////////////////////////
CPNGImageIO::~CPNGImageIO()
{
}

////////////////////////////////////////////////////////////////////////////////////////////////
const char* CPNGImageIO::GetInfoString(uint32t idx) const
{
    return &m_strInfo[0];
}

////////////////////////////////////////////////////////////////////////////////////////////////
const char* CPNGImageIO::GetExtString(uint32t idx) const
{
    return &m_strExt[0];
}


////////////////////////////////////////////////////////////////////////////////////////////////
CBMImage* CPNGImageIO::LoadBMImage(IDataStream *datastream, uint32t loadflags)
{
    if(!datastream && !datastream->IsOpened())
        return NULL;

    png_structp png_s;
	png_infop png_i;
	char header[8];

    //Reading And Check Header
    uint32t readed = datastream->Read(header,8);
    if(readed != 8)
        return NULL;

    bool is_png = !::png_sig_cmp((png_bytep)header, 0, 8);
    if(!is_png)
        return NULL;

    //Create Read Struct
    png_s = png_create_read_struct(PNG_LIBPNG_VER_STRING, (png_voidp)&png_s,PngErrorFunc, PngWarnFunc);
    if(!png_s)
        return NULL;

    // ...And Info Struct
    png_i = png_create_info_struct(png_s);
    if (!png_i)
    {
        png_destroy_read_struct(&png_s, (png_infopp)NULL, (png_infopp)NULL);
        return NULL;
    }
    if(setjmp(png_s->jmpbuf)) 
    {
		// TODO: delete allocated memory?
		// Free all of the memory associated with the png_ptr and info_ptr
		png_destroy_read_struct(&png_s,&png_i,(png_infopp)NULL);
		return NULL;
	}
	png_set_read_fn(png_s,(void *)datastream, PngReadFunc);

    //Rewind File
    datastream->Seek(0, seek_set);

    uint32t img_w, img_h;
    int img_bpp, color_type, interlance_type;
	png_read_info(png_s, png_i);
	png_get_IHDR(png_s, png_i, &img_w, &img_h, &img_bpp, &color_type, &interlance_type, NULL, NULL);
	// tell libpng to strip 16 bit/color files down to 8 bits/color
	//--png_set_strip_16(png_s);

	// Expand Paletted Colors Into True RGB Triplets
    if(color_type == PNG_COLOR_TYPE_PALETTE)
        png_set_palette_to_rgb(png_s);
	//--if(color_type == PNG_COLOR_TYPE_PALETTE) 
    //--    png_set_expand(png_s);

	// Expand Grayscale Images To The Full 8 Bits From 1, 2, Or 4 Bits/Pixel
	if(color_type == PNG_COLOR_TYPE_GRAY && img_bpp < 8) 
        //--png_set_expand(png_s);
        png_set_expand_gray_1_2_4_to_8(png_s);

	// Expand paletted or RGB images with transparency to full alpha channels
	// so the data will be available as RGBA quartets.
	if(png_get_valid(png_s,png_i,PNG_INFO_tRNS)) 
        png_set_expand(png_s);

	// Grey images are upgraded to RGB
	//--if(color_type == PNG_COLOR_TYPE_GRAY || color_type == PNG_COLOR_TYPE_GRAY_ALPHA) 
    //--    png_set_gray_to_rgb(png_s);

	// If we don't have true alpha, pad with an alpha channel representing opaque
	//--png_set_filler(png_s,0xff,PNG_FILLER_AFTER);
	// Turn on interlace handling
	png_set_interlace_handling(png_s);
	// Update image info based on transformations
	png_read_update_info(png_s, png_i);

    BMIFMT fmt = SelImgFmt(img_bpp,color_type);
    uint32t fmtsize = CBMImgFmtInfo::GetBytesPerPixel(fmt);
    if(fmt == IMGFMT_NONE)
    {
        png_destroy_read_struct(&png_s,&png_i,(png_infopp)NULL);
		return NULL;
    }

    // Creating Image Data
    CBMImage *image = new CBMImage(fmt,(uint32t)img_w,(uint32t)img_h,1); //CBMImage(fmt,(uint32t)img_w,(uint32t)img_h,1,IMGFMT_NONE,0,0,0,0,1);
    if(!image || !image->GetPixels())
    {
        png_destroy_read_struct(&png_s, &png_i, (png_infopp)NULL);
        return NULL;
    }
    ubyte *bits = image->GetPixels();

	
	// Create Rowpointers
	png_bytepp rows = new png_bytep[img_h];

	if (!rows) 
    {
		delete image;
        png_destroy_read_struct(&png_s, &png_i, (png_infopp)NULL);
        return NULL;
	}
	for (int i = 0; i < (int)img_h ; i++) 
		rows[i] = (png_bytep)&bits[(img_h - i - 1) * img_w * fmtsize];

	// Read Image
	png_read_image(png_s, rows);
	// Read Past Image
	png_read_end(png_s, png_i);
	// Cleanup
	png_destroy_read_struct(&png_s, &png_i, (png_infopp)NULL);
	
	delete [] rows;
    
    return image;
}


////////////////////////////////////////////////////////////////////////////////////////////////
// Saving Png Image
bool CPNGImageIO::SaveBMImage(IDataStream *datastream, const CBMImage *image, ImgIOParam *params)
{
    if(!image && !image->GetPixels() && !datastream && !datastream->IsOpened())
        return false;

    BMIFMT fmt = image->GetFormat();
    if(fmt < IMGFMT_L8 || fmt > IMGFMT_RGBA16)
        return false;

    uint32t width = image->GetWidth();
    uint32t height = image->GetHeight();
    uint32t pixsize = CBMImgFmtInfo::GetBytesPerPixel(fmt);

    png_structp png_ptr;
	png_infop info_ptr;
	
	png_ptr = png_create_write_struct(PNG_LIBPNG_VER_STRING, (png_voidp)&png_ptr, PngErrorFunc, PngWarnFunc);
	
	if (!png_ptr)
		return false;

    info_ptr = png_create_info_struct(png_ptr);
	if (!info_ptr)
    {
        png_destroy_write_struct(&png_ptr, (png_infopp)NULL);
		return false;//--WRITE_ERROR;
    }
    
    if (setjmp(png_jmpbuf(png_ptr)))
    {
        png_destroy_write_struct(&png_ptr, &info_ptr);
		return false;//--WRITE_ERROR;
    }

    png_set_write_fn(png_ptr, (png_voidp)datastream,  PngWriteFunc, NULL);

    // Write Header 
	if (setjmp(png_jmpbuf(png_ptr)))
    {
        png_destroy_write_struct(&png_ptr, &info_ptr);
		return false;
    }

    //////////
    int bit_depth = CBMImgFmtInfo::GetChannelSize(fmt,1);
    uint32t channum = CBMImgFmtInfo::GetChannelsCount(fmt);
    int color_type = 0;
    if(fmt == 1)
        color_type = PNG_COLOR_TYPE_GRAY;
    else if(fmt == 2)
        color_type = PNG_COLOR_TYPE_GRAY_ALPHA;
    else if(fmt == 3)
        color_type = PNG_COLOR_TYPE_RGB;
    else
        color_type = PNG_COLOR_TYPE_RGB_ALPHA;

    // Setting Png File Header
	png_set_IHDR(png_ptr, info_ptr, (png_uint_32)width, (png_uint_32)height,
		     bit_depth, color_type, PNG_INTERLACE_NONE,
		     PNG_COMPRESSION_TYPE_BASE, PNG_FILTER_TYPE_BASE);

    if(fmt == IMGFMT_BGR8 || fmt == IMGFMT_BGRA8)
        png_set_bgr(png_ptr);

    png_text text_ptr[3];
	// Optionally write comments into the image 
	text_ptr[0].key = "Title";
	text_ptr[0].text = "png image";
	text_ptr[0].compression = PNG_TEXT_COMPRESSION_NONE;
	/*text_ptr[0].text_length = 14;
	text_ptr[1].key = "Author";
	text_ptr[1].text = "name";
	text_ptr[1].compression = PNG_TEXT_COMPRESSION_NONE;
	text_ptr[1].text_length = 6;
	text_ptr[2].key = "Description";
	text_ptr[2].text = NULL;//(char *)s.c_str();
	text_ptr[2].compression = PNG_TEXT_COMPRESSION_zTXt;
	text_ptr[1].text_length = 0;//s.length();*/
	png_set_text(png_ptr, info_ptr, text_ptr, 1);

	png_write_info(png_ptr, info_ptr);

	// write bytes 
	if (setjmp(png_jmpbuf(png_ptr)))
    {
        png_destroy_write_struct(&png_ptr, &info_ptr);
		return false;
    }

    ubyte *pixdata = image->GetPixels();

    // Create RowPointers
    png_bytepp rows = new png_bytep[height];
    if(!rows)
    {
        png_destroy_write_struct(&png_ptr, &info_ptr);
        return false;
    }
    for (int i = 0; i < (int)height ; i++) 
		rows[i] = (png_bytep)&pixdata[(height-i-1) * width * pixsize];
    

    //png_bytep row;
    //--for(uint32t i = 0; i < height; i++)
    //--    png_write_row(png_ptr,(png_bytep)&pixdata[width * pixsize * i]);
	
    png_write_image(png_ptr, rows);

	// end write 
	if (setjmp(png_jmpbuf(png_ptr)))
    {
        png_destroy_write_struct(&png_ptr, &info_ptr);
		return false;
    }

    //////////////////////////////
	png_write_end(png_ptr, info_ptr);
    png_destroy_write_struct(&png_ptr, &info_ptr);

    delete [] rows;

    return true;
}

///////////////////////////////////////////////////////////////////////////////////
// Selecting Image Format
BMIFMT CPNGImageIO::SelImgFmt(int img_bpp, int color_type)
{
    // Choosing Image Format
    BMIFMT fmt = IMGFMT_NONE;
    switch(color_type)
    {
    case PNG_COLOR_TYPE_GRAY:
        if(img_bpp <= 8)
            fmt = IMGFMT_L8;
        else if(img_bpp == 16)
            fmt = IMGFMT_L16;
        break;

    case PNG_COLOR_TYPE_GRAY_ALPHA:
        if(img_bpp <= 8)
            fmt = IMGFMT_LA8;
        else if(img_bpp == 16)
            fmt = IMGFMT_LA16;
        break;

    case PNG_COLOR_TYPE_PALETTE:
    case PNG_COLOR_TYPE_RGB:
        if(img_bpp <= 8)
            fmt = IMGFMT_RGB8;
        else if(img_bpp == 16)
            fmt = IMGFMT_RGB16;
        break;

    case PNG_COLOR_TYPE_RGB_ALPHA:
        if(img_bpp == 8)
            fmt = IMGFMT_RGBA8;
        else if(img_bpp == 16)
            fmt = IMGFMT_RGBA16;
        break;
    }

    return fmt;
}

///////////////////////////////////////////////////////////////////////////////////
// png related
void CPNGImageIO::PngErrorFunc(png_structp png_s, const char *png_msg) 
{
	longjmp(png_s->jmpbuf,1);
}
///////////////////////////////////////////////////////////////////////////////////
void CPNGImageIO::PngWarnFunc(png_structp png_s, const char *png_msg) 
{
}
///////////////////////////////////////////////////////////////////////////////////
void CPNGImageIO::PngReadFunc(png_structp png_s, png_bytep buffer, png_size_t buffsize) 
{
    IDataStream *s = (IDataStream *)png_get_io_ptr(png_s);
    uint32t readed = s->Read(buffer,(uint32t)buffsize);
}
///////////////////////////////////////////////////////////////////////////////////
void CPNGImageIO::PngWriteFunc(png_structp png_s, png_bytep buff, png_size_t buffsize)
{
	IDataStream *s = (IDataStream *)png_get_io_ptr(png_s);
	uint32t len = s->Write(buff, (int)buffsize);
}
