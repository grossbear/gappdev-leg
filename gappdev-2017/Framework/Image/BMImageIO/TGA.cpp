///////////////////////////////////////////////////////////////////////////////////////////////
//	TGA.cpp
//	Targa Image Format Loading/Saving Class Definition
//	
////////////////////////////////////////////////////////////////////////////////////////////////

#include "Utils/mem/memory_allocator.h"
#include "Utils/IOStream/IDataStream.h"
#include "Image/Image.h"
#include "TGA.h"

#include <memory.h>

#define MAX_WIDTH       32768
#define MAX_HEIGHT      32768

#define ATTRIB_VERT     0x20
#define ATTRIB_HORI     0x10

#pragma pack (push,1)

// Targa Image File Header
struct TGAHeader 
{
	ubyte  descLen;     // identsize - size of ID field that follows 18 byte header (0 usually)
	ubyte  cmapType;    // Color Map Type - type of colour map 0=none, 1=has palette
	ubyte  imageType;   // type of image 0=none,1=indexed,2=rgb,3=grey,+8=rle packed
	ushort cmapStart;   // Color Map Start - first colour map entry in palette
	ushort cmapEntries; // Color Map Length - number of colours in palette
	ubyte  cmapBits;    // Color Map Bits - number of bits per palette entry 15,16,24,32
	ushort xOffset;     // image x origin
	ushort yOffset;     // image y origin
	ushort width;       // image width in pixel
	ushort height;      // image height in pixel
	ubyte  bpp;         // image bits per pixel 8,16,24,32
	ubyte  attrib;      // Descriptor - image descriptor bits (vh flip bits)
};

#pragma pack (pop)


////////////////////////////////////////////////////////////////////////////////////////////////
char CTGAImageIO::m_strExt[] = {"tga"};
////////////////////////////////////////////////////////////////////////////////////////////////
char CTGAImageIO::m_strInfo[] = {"TrueVision Image File"};


////////////////////////////////////////////////////////////////////////////////////////////////
CTGAImageIO::CTGAImageIO()
{
}

////////////////////////////////////////////////////////////////////////////////////////////////
CTGAImageIO::~CTGAImageIO()
{
}

////////////////////////////////////////////////////////////////////////////////////////////////
const char* CTGAImageIO::GetInfoString(uint32t idx) const
{
    return &m_strInfo[0];
}

////////////////////////////////////////////////////////////////////////////////////////////////
const char* CTGAImageIO::GetExtString(uint32t idx) const
{
    return &m_strExt[0];
}

////////////////////////////////////////////////////////////////////////////////////////////////
CBMImage* CTGAImageIO::LoadBMImage(IDataStream *datastream, uint32t loadflags)
{
    //Check If IDataStream Is Not NULL Pointer
    if(datastream==NULL) return NULL;

    //Chekc If IDataStream Is Opened
    if(!datastream->IsOpened()) return NULL;

    //Reading File Header
    TGAHeader tgaheader = {0};
    uint32t ireaded = datastream->Read(&tgaheader,sizeof(TGAHeader));
    if(ireaded < sizeof(TGAHeader))
        return NULL;

    //Check If File Header Is Correct
    if(!CheckFileHeader(tgaheader))
        return NULL;

    //Reading ImageID Field
    if(tgaheader.descLen > 0)
    {
        tbyte imgdesc[256]={0};
        uint32t ireaded = datastream->Read(imgdesc,(uint32t)tgaheader.descLen);
        if(ireaded < (uint32t)tgaheader.descLen)
            return NULL;
    }

    CBMImage *image = NULL;
    ubyte imgtype = tgaheader.imageType;
    if(imgtype==1)//Uncompressed Color Mapped Image
        image = this->LoadUncompressedColorMappedImage(datastream,tgaheader,loadflags);
    else if(imgtype == 2 || imgtype == 3)//Uncompressed Image
        image = this->LoadUncompressedImage(datastream,tgaheader,loadflags);
    else if(imgtype == 10 || imgtype == 11)//Compressed Image
        image = this->LoadCompressedImage(datastream,tgaheader,loadflags);
    else if(imgtype == 9)//Compressed Color Mapped Image 
        image = this->LoadCompressedColorMappedImage(datastream,tgaheader,loadflags);

    //Flipping Image If File Attrib == 32
    if((tgaheader.attrib & ATTRIB_VERT) || (tgaheader.attrib & ATTRIB_HORI) && (image!=NULL) && (image->GetWidth() > 1))
    {
        if(tgaheader.attrib & ATTRIB_VERT)
            VertFlipImage(image->GetPixels(),image->GetWidth(),image->GetHeight(),CBMImgFmtInfo::GetChannelsCount(image->GetFormat()));
        if(tgaheader.attrib & ATTRIB_HORI)
            HoriFlipImage(image->GetPixels(),image->GetWidth(),image->GetHeight(),CBMImgFmtInfo::GetChannelsCount(image->GetFormat()));
    }

    return image;
}

////////////////////////////////////////////////////////////////////////////////////////////////
bool CTGAImageIO::SaveBMImage(IDataStream *datastream, const CBMImage *image, ImgIOParam *params)
{
    //If No Image Or Image Is Corrupt Exit Function
    if(image==NULL && image->GetPixels()==NULL && image->GetWidth()==0 && datastream==NULL)
        return false;

    //If Iostream Is Not Opened
    if(!datastream->IsOpened())
        return false;

    BMIFMT fmt = image->GetFormat();
    if(fmt != IMGFMT_L8 || fmt != IMGFMT_RGB8 || fmt != IMGFMT_RGBA8 || fmt != IMGFMT_BGR8 || fmt != IMGFMT_BGRA8)
        return false;

    ubyte *imgpal = image->GetPalette();
    BMIFMT palfmt = image->GetPaletteFormat();
    uint32t palclrcnt = image->GetPalColorsNum();
    //Checking Palette Data
    if(imgpal)
    {
        if(palfmt != IMGFMT_RGB8 || palfmt != IMGFMT_RGBA8 || palfmt != IMGFMT_BGR8 || palfmt != IMGFMT_BGRA8)
            return false;

        if(palclrcnt < 2 || palclrcnt > 256)
            return false;
    }

    //Setting File Header
    TGAHeader tgaheader = {0};
    SetFileHeader(tgaheader,*image);

    //Writing File Header Into File
    uint32t wsize = datastream->Write(&tgaheader,sizeof(tgaheader));
    //if(wsize != sizeof(tgaheader))
    //    return false;

    //Saving Palette
    if(imgpal)
    {
        ubyte palette[1024]={0};
        uint32t palchannels = CBMImgFmtInfo::GetChannelsCount(palfmt);
        memcpy(palette,imgpal,palclrcnt*palchannels);
        if(palfmt!=IMGFMT_BGR8 || palfmt!=IMGFMT_BGRA8)
            RevertChannels(palette,palclrcnt,1,palchannels);

        uint32t isize = datastream->Write(palette,palclrcnt*palchannels);
        //if(isize != palclrcnt*palchannels)
        //    return false;
    }

    //Saving Pixel Data
    ubyte   *pixdata = image->GetPixels();
    uint32t width = image->GetWidth();
    uint32t imgwidth = CBMImgInfo::CalcWidthSize(fmt,width); 
    uint32t imgheight = image->GetHeight();
    uint32t imgstride = image->GetStride();
    if(fmt == IMGFMT_BGR8 || fmt == IMGFMT_BGRA8 || fmt == IMGFMT_L8)
    {
        for(uint32t h = 0; h < imgheight; h++)
        {
            datastream->Write(pixdata,imgwidth);
            pixdata += imgwidth+imgstride;
        }
    }
    else
    {
        uint32t off = (fmt==IMGFMT_RGBA8)?(4):(3);
        //--ubyte *line = new ubyte[imgwidth];
        memory_allocator *mem_alloc = memory_allocator_selector::get_allocator(img_alloc,imgwidth);
        ubyte *linedata = (ubyte*)mem_alloc->alloc(imgwidth);

        for(uint32t h = 0; h < imgheight; h++)
        {
            memcpy(linedata,pixdata,imgwidth);
            RevertChannels(linedata,width,1,off);
            datastream->Write(linedata,imgwidth);
            pixdata += imgwidth+imgstride;
        }

        mem_alloc->dealloc(linedata);
    }

    return true;
}

////////////////////////////////////////////////////////////////////////////////////////////////
// Protected Methods
////////////////////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////////////////////
// Check TGA Image File Header
bool CTGAImageIO::CheckFileHeader(const TGAHeader& header)
{
    if(header.width == 0 || header.height == 0 || header.width > MAX_WIDTH || header.height > MAX_HEIGHT)
        return false;

    //cmapType - Color Map Type 0=none, 1=has palette
    if(header.cmapType != 0 && header.cmapType != 1)
        return false;

    //imageType
    ubyte imgtype = header.imageType;
    if(imgtype != 1 && imgtype != 2 && imgtype != 3 && imgtype != 9 && imgtype != 10 && imgtype != 11)
        return false;

    ubyte bpp = header.bpp;
    if(bpp != 32 && bpp != 24 && bpp != 8)
        return false;

    //Checking Color Map Entries
    if(imgtype == 1 || imgtype == 9)
    {
        if(header.cmapBits != 24 && header.cmapBits != 32)
            return false;
        if(header.cmapEntries < 2 || header.cmapEntries > 256)
            return false;
    }

    return true;
}

////////////////////////////////////////////////////////////////////////////////////////////////
// Setting TGA File Heade For Writing
void CTGAImageIO::SetFileHeader(TGAHeader& header, const CBMImage &image)
{
    BMIFMT fmt = image.GetFormat();
    ubyte *palette = image.GetPalette();
    
    header.bpp = CBMImgFmtInfo::GetChannelsCount(fmt)*8;
    header.imageType = (fmt==IMGFMT_L8) ? ((palette)?(1):(3)) : (2);
    header.width = image.GetWidth();
    header.height = image.GetHeight();
    
    if(palette)
    {
        header.cmapBits = CBMImgFmtInfo::GetChannelsCount(image.GetPaletteFormat())*8;
        header.cmapEntries = image.GetPalColorsNum();
        header.cmapType = 1;
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////
CBMImage* CTGAImageIO::LoadUncompressedImage(IDataStream *file, const TGAHeader &header, uint32t loadflags)
{
    uint32t width = (uint32t)header.width;
    uint32t height = (uint32t)header.height;
    uint32t channels = (uint32t)header.bpp/8;

    //&& (header.attrib != 0x20)
    BMIFMT chantab1[] = {IMGFMT_NONE,IMGFMT_L8,IMGFMT_NONE,IMGFMT_RGB8,IMGFMT_RGBA8};
    BMIFMT chantab2[] = {IMGFMT_NONE,IMGFMT_L8,IMGFMT_NONE,IMGFMT_BGR8,IMGFMT_BGRA8};
    BMIFMT fmt = (loadflags & IMGIO_BGR_TO_RGB) ? (chantab1[channels]) : (chantab2[channels]);

    CBMImage *image = new CBMImage(fmt,width,height,1); //CBMImage(fmt,width,height,1,IMGFMT_NONE,0,0,0,0,1);//CBMImage(fmt,width,height,1);
    ubyte *pixels = image->GetPixels();
    if(pixels==NULL)
    {
        delete (image);
        return NULL;
    }

    uint32t fullwidth = width*CBMImgFmtInfo::GetBytesPerPixel(fmt);

    for(uint32t h = 0; h < height; h++)
    {
        uint32t ireaded = file->Read(pixels,fullwidth);
        if(ireaded < fullwidth)
        {
            delete (image);
            return NULL;
        }
        //if(stride!=0)
        //    file->Read(&pitch,stride);
        pixels += fullwidth;
    }

    if((loadflags & IMGIO_BGR_TO_RGB) && (channels == 3 || channels == 4))
        RevertChannels(image->GetPixels(),width,height,channels);

    return image;
}

////////////////////////////////////////////////////////////////////////////////////////////////
// Loading Compressed TGA Image
CBMImage* CTGAImageIO::LoadCompressedImage(IDataStream *file, const TGAHeader &header, uint32t loadflags)
{
    uint32t width = (uint32t)header.width;
    uint32t height = (uint32t)header.height;
    uint32t channels = (uint32t)header.bpp/8;

    BMIFMT chantab1[] = {IMGFMT_NONE,IMGFMT_L8,IMGFMT_NONE,IMGFMT_RGB8,IMGFMT_RGBA8};
    BMIFMT chantab2[] = {IMGFMT_NONE,IMGFMT_L8,IMGFMT_NONE,IMGFMT_BGR8,IMGFMT_BGRA8};
    BMIFMT fmt = (loadflags & IMGIO_BGR_TO_RGB) ? (chantab1[channels]) : (chantab2[channels]);

    CBMImage *image = new CBMImage(fmt,width,height,1);//CBMImage(fmt,width,height,1,IMGFMT_NONE,0,0,0,0,1);//CBMImage(fmt,width,height,1);
    ubyte *pixels = image->GetPixels();
    if(pixels==NULL)
    {
        delete (image);
        return NULL;
    }

    // Nuber of pixels in the image
    uint32t pixelcount = width*height;
    // Current pixel being read
    uint32t currentpixel = 0;
    // Current byte 
    uint32t currentbyte = 0;
    // Storage for 1 pixel
    ubyte *pix = pixels;
    
    do
    {
        // Storage for "chunk" header
        ubyte chunkheader = 0;
        //--if(fread(&chunkheader, sizeof(ubyte),1, file) == 0)
        if(file->Read(&chunkheader,sizeof(ubyte)) == 0)
        {
            delete image;// Delete image data
            return NULL;// Return NULL
        }

        // If the ehader is < 128, it means the that is the number of RAW color packets minus 1
        // that follow the header
        if(chunkheader < 128)
        {
            // add 1 to get number of following color values
            chunkheader++;
            // Read RAW color values
            for(short counter = 0; counter < chunkheader; counter++)
            {
                //Try To Read 1 Pixel
                //--if(fread(&pix[currentbyte], 1, nChannels, file) != nChannels)
                if(file->Read(&pix[currentbyte],channels) != channels)
                {
                    delete image;
                    return NULL;
                }

                // Increase thecurrent byte by the number of bytes per pixel
                currentbyte += channels;
                // Increase current pixel by 1
				currentpixel++;	

                // Make sure we havent read too many pixels
                if(currentpixel > pixelcount)											
				{
                    // if there is too many... Display an error! "Too many pixels read" "ERROR"
				    delete image;
                    return NULL;
                }
            }
        }
        // chunkheader > 128 RLE data, next color reapeated chunkheader - 127 times
        else
        {
            ubyte colorbuf[4]={0};
            // Subteact 127 to get rid of the ID bit
			chunkheader -= 127;			
            // Attempt to read following color values
			//--if(fread(colorbuf, 1, nChannels, file) != nChannels)		
            if(file->Read(colorbuf,channels) != channels)
			{	
                // If attempt fails... Display error (again) "Could not read from file"  "ERROR"
				delete image;
                return NULL;
			}
            uint32t tell = file->GetPos();

            for(short counter = 0; counter < chunkheader; counter++)
            {
                // Copy Data Into Image Pixels
                memcpy(&pix[currentbyte],colorbuf,sizeof(ubyte)*channels);
                // Increase current byte by the number of bytes per pixel
                currentbyte += channels;
                // Increase pixel count by 1
                currentpixel++;

                // Make sure we havent written too many pixels
                if(currentpixel > pixelcount)
                {
                    delete image;
                    return NULL;
                }
            }

        }
    }
    while(currentpixel < pixelcount);

    //Converting BGR Format To RGB
    if((loadflags & IMGIO_BGR_TO_RGB) && (channels == 3 || channels == 4))
        RevertChannels(image->GetPixels(),width,height,channels);

    return image;
}

////////////////////////////////////////////////////////////////////////////////////////////////
CBMImage* CTGAImageIO::LoadUncompressedColorMappedImage(IDataStream *file, const TGAHeader &header, uint32t loadflags)
{
    uint32t palbpp = (ubyte)header.cmapBits;
    uint32t palchannels = palbpp/8;
    uint32t width = (uint32t)header.width;
    uint32t height = (uint32t)header.height;
    uint32t palclrcnt = (uint32t)header.cmapEntries;

    //Reading Palette Data
    ubyte palette[1024] = {0};
    uint32t palsize = palclrcnt*palchannels;
    uint32t ireaded = file->Read(palette,palsize);
    if(ireaded < palsize)
        return NULL;

    //Reverting Palette Channels
    RevertChannels(palette,palclrcnt,1,palchannels);

    //--uint32t flags = m_iLoadFlags;
    BMIFMT fmt = (loadflags & IMGIO_EXPAND_PALETTE) ? ((palbpp==24)?(IMGFMT_RGB8):((loadflags & IMGIO_LOAD_PALETTE_ALPHA)?(IMGFMT_RGB8):(IMGFMT_RGBA8))) : (IMGFMT_L8);
    BMIFMT palfmt = (loadflags & IMGIO_EXPAND_PALETTE) ? (IMGFMT_NONE) : ((palbpp==24)?(IMGFMT_RGB8):(IMGFMT_RGBA8));

    //Creating Image
    CBMImage *image = new CBMImage(fmt,width,height,1,palfmt,palclrcnt,0,0,0,1);//CBMImage(fmt,width,height,1);
    ubyte *pixels = image->GetPixels();
    if(pixels==NULL)
    {
        delete (image);
        return NULL;
    }
   
    //Reading Data And Save Palette Separately
    if(fmt == IMGFMT_L8)
    {
        uint32t ireaded = file->Read(pixels,width*height);
        //--ubyte *pal = image->CreatePaletteData((palchannels==4)?(IMGFMT_RGBA8):(IMGFMT_RGB8),palclrcnt);
        if((ireaded < width*height))
        {
            delete (image);
            return NULL;
        }
        ubyte *pal = image->GetPalette();
        memcpy(pal,palette,palclrcnt*palsize);

        return image;
    }

    
    memory_allocator *mem_alloc = memory_allocator_selector::get_allocator(img_alloc,width);
    ubyte *linedata = (ubyte*)mem_alloc->alloc(width);
    //Extracting Palette
    //ubyte *line = new ubyte[width];
    uint32t channels = (fmt==IMGFMT_RGB8)?(3):(4);
    for(uint32t h = 0; h < height; h++)
    {
        uint32t ireaded = file->Read(linedata,width);
        if(ireaded < width)
        {
            //--delete [] line;
            mem_alloc->dealloc(linedata);
            delete (image);
            return NULL;
        }
        for(uint32t w = 0; w < width; w++)
        {
            uint32t clridx = linedata[w];
            memcpy(pixels,&palette[clridx*palchannels],channels);
            pixels += channels;
        }
    }
    mem_alloc->dealloc(linedata);
    //delete [] line;

    return image;
}

////////////////////////////////////////////////////////////////////////////////////////////////
CBMImage* CTGAImageIO::LoadCompressedColorMappedImage(IDataStream *file, const TGAHeader &header, uint32t loadflags)
{
    uint32t palbpp = (uint32t)header.cmapBits;
    uint32t palchannels = palbpp/8;
    uint32t width = (uint32t)header.width;
    uint32t height = (uint32t)header.height;
    uint32t palclrcnt = (uint32t)header.cmapEntries;

    //Reading Palette Data
    ubyte palette[1024]={0};
    uint32t palsize = palclrcnt*palchannels;
    uint32t ireaded = file->Read(palette,palsize);
    if(ireaded < palsize)
        return NULL;

    //Reverting Palette Channels
    RevertChannels(palette,palclrcnt,1,palchannels);

    //--uint32t flags = m_iLoadFlags;
    BMIFMT fmt = (loadflags & IMGIO_EXPAND_PALETTE) ? ((palbpp==24)?(IMGFMT_RGB8):((loadflags & IMGIO_LOAD_PALETTE_ALPHA)?(IMGFMT_RGB8):(IMGFMT_RGBA8))) : (IMGFMT_L8);
    BMIFMT palfmt = (loadflags & IMGIO_EXPAND_PALETTE) ? (IMGFMT_NONE) : ((palbpp==24)?(IMGFMT_RGB8):(IMGFMT_RGBA8));

    //Creating Image
    CBMImage *image = new CBMImage(fmt,width,height,1,palfmt,palclrcnt,0,0,0,1);//CBMImage(fmt,width,height,1);
    ubyte *pixels = image->GetPixels();
    if(pixels==NULL)
    {
        delete (image);
        return NULL;
    }

    //Decompressing Image Data
    uint32t channels = CBMImgFmtInfo::GetChannelsCount(fmt);
    if(!DecompressData(pixels,channels,file,header,(loadflags & IMGIO_EXPAND_PALETTE)?(palette):(NULL)))
    {
        delete (image);
        return NULL;
    }

    //Saving Palette
    if(!(loadflags & IMGIO_EXPAND_PALETTE))
    {
        //--ubyte *pal = image->CreatePaletteData((palchannels==4)?(IMGFMT_RGBA8):(IMGFMT_RGB8),palclrcnt);
        ubyte *pal = image->GetPalette();
        memcpy(pal,palette,palclrcnt*palchannels);
    }
    
    return image;
}

////////////////////////////////////////////////////////////////////////////////////////////////
bool CTGAImageIO::DecompressData(ubyte *pixels, uint32t imgchannels, IDataStream *file, const TGAHeader &header, ubyte *palette)
{
    uint32t width = (uint32t)header.width;
    uint32t height = (uint32t)header.height;
    //uint32t imgchannels = (uint32t)header.bpp/8;

    uint32t palchannels = header.cmapBits/8;
    uint32t palclrcnt = header.cmapEntries;

    uint32t channels = (palette!=NULL)?(1):(imgchannels);

    // Nuber of pixels in the image
    uint32t pixelcount = width*height;
    // Current pixel being read
    uint32t currentpixel = 0;
    // Current byte 
    uint32t currentbyte = 0;
    // Storage for 1 pixel
    ubyte *pix = pixels;

    do
    {
        // Storage for "chunk" header
        ubyte chunkheader = 0;
        //--if(fread(&chunkheader, sizeof(ubyte),1, file) == 0)
        if(file->Read(&chunkheader,sizeof(ubyte)) == 0)
            return false;

        // If the ehader is < 128, it means the that is the number of RAW color packets minus 1
        // that follow the header
        if(chunkheader < 128)
        {
            // add 1 to get number of following color values
            chunkheader++;

            ubyte chunkdata[512]={0};
            ubyte *pdata = (palette)?(&chunkdata[0]):(&pix[currentpixel*imgchannels]);

            // Read RAW color values
            uint32t ireaded = file->Read(pdata,chunkheader*channels);
            if(ireaded != chunkheader*channels)
                return false;

            // Make sure we havent read too many pixels
            if((currentpixel+chunkheader) > pixelcount)											
			    return false;// if there is too many... Display an error! "Too many pixels read" "ERROR"

            if(palette)
            {
                for(ubyte counter = 0; counter < chunkheader; counter++)
                {
                    uint32t clridx = chunkdata[counter];
                    memcpy(&pix[currentpixel*imgchannels+counter*imgchannels],&palette[clridx*palchannels],imgchannels);
                }
            }
            
            // Increase thecurrent byte by the number of bytes per pixel
            currentbyte += chunkheader*channels;
            // Increase current pixel by 1
			currentpixel += chunkheader;
            
        }
        // chunkheader > 128 RLE data, next color reapeated chunkheader - 127 times
        else
        {
            ubyte buffer[4]={0};
            // Subteact 127 to get rid of the ID bit
			chunkheader -= 127;			
            // Attempt to read following color values		
            if(file->Read(buffer,channels) != channels)
			    return false; // If attempt fails... Display error (again) "Could not read from file"  "ERROR"
            
            if(palette)
            {
                ubyte *palptr = &palette[buffer[0]*palchannels];
                memcpy(buffer,palptr,palclrcnt);
            }
            
            for(ubyte counter = 0; counter < chunkheader; counter++)
            {
                // Copy Data Into Image Pixels
                memcpy(&pix[currentpixel*imgchannels],buffer,imgchannels);

                // Increase current byte by the number of bytes per pixel
                currentbyte += channels;
                // Increase pixel count by 1
                currentpixel ++;

                // Make sure we havent written too many pixels
                if(currentpixel > pixelcount)
                    return false;
                
            }

        }
    }
    while(currentpixel < pixelcount);

    return true;
}

////////////////////////////////////////////////////////////////////////////////////////////////
void CTGAImageIO::RevertChannels(ubyte *pixels, uint32t width, uint32t height, uint32t channels)
{
    ubyte *ptr = pixels;
    uint32t size = width*height;

    for(uint32t i = 0; i < size; i++)
    {
        ptr[0] ^= ptr[2] ^= 
        ptr[0] ^= ptr[2];
        ptr += channels;
    }   
}

////////////////////////////////////////////////////////////////////////////////////////////////
void CTGAImageIO::VertFlipImage(ubyte *pixels, uint32t width, uint32t height, uint32t channels)
{
    uint32t fullwidth = width*channels;
    
    ubyte *ptr_start = pixels;
    ubyte *ptr_end = pixels + fullwidth*(height-1);

    uint32t w2 = width>>1;
    uint32t h2 = height>>1;

    for(uint32t y = 0; y < h2; y++)
    {
        for(uint32t x = 0; x < fullwidth; x++)
        {
            ptr_start[x] ^= ptr_end[x] ^=
            ptr_start[x] ^= ptr_end[x];
        }
        ptr_start += fullwidth;
        ptr_end -= fullwidth;
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////
void CTGAImageIO::HoriFlipImage(ubyte *pixels, uint32t width, uint32t height, uint32t channels)
{
    uint32t fullwidth = width*channels;

    uint32t w2 = (width>>1)*channels;
    for(uint32t y = 0; y < height; y++)
    {
        ubyte *start = &pixels[y*fullwidth];
        ubyte *end = &pixels[y*fullwidth + (width)*channels];

        for(uint32t x = 0; x < w2; x++)
        {
            for(uint32t c = 0; c < channels; c++)
            {
                start[c] ^= end[c] ^= 
                start[c] ^= end[c];
            }
            start += channels;
            end -= channels;
        }
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////////////////////