///////////////////////////////////////////////////////////////////////////////////////////////
//	PCX.h
//	PCX Image Format Loading/Saving Class Definition
//	
////////////////////////////////////////////////////////////////////////////////////////////////

#include "Utils/mem/memory_allocator.h"
#include "Utils/IOStream/IDataStream.h"
#include "Image/Image.h"
#include "PCX.h" 

#include <memory.h>

#define MAX_WIDTH       32768
#define MAX_HEIGHT      32768

#pragma pack(push,1)
// PCX File Header Struct
struct PCXHeader
{
    ubyte   Manufacturer;           //Should always be 0Ah
    ubyte   Version;                //0=PC Paintbrush v2.5
                                    //2=PC Paintbrush v2.8 w palette information
                                    //3=PC Paintbrush v2.8 w/o palette information
                                    //4=PC Paintbrush/Windows
                                    //5=PC Paintbrush v3.0+
    ubyte   Encoding;               //Should always be 01h
    ubyte   Bpp;                    //Bits per pixel
    ushort  xMin;                   //Left margin of image
    ushort  yMin;                   //Upper margin of image
    ushort  xMax;                   //Right margin of image
    ushort  yMax;                   //Lower margin of image
    ushort  horDPI;                 //Horizontal DPI resolution
    ushort  vertDPI;                //Vertical DPI resolution
    ubyte   palette[48];            //Color palette setting for 16-color images 16 RGB triplets
    ubyte   reserved;               //Reserved
    ubyte   colorPlanes;            //Number of color planes: 4 -- 16 colors, 3 -- 24 bit color (16.7 million colors) 
    ushort  bytesPerLines;          //Number of bytes per line (the width of the image in bytes). For 320x200, 256-color images, this is 320 bytes per line. 
    ushort  PaletteType;            //Palette information 1=color/bw palette, 2=grayscale image
    ushort  hScrSize;               //Horizontal screen size   
    ushort  vScrSize;               //Vertical screen size   
    ubyte   filler[54];             //reserved, set to 0     
};

#pragma pack(pop)

//  Planes  Bit Depth   Display Type 
//      1       1           Monochrome 
//      1       2   `       CGA (4 colour palleted) 
//      3       1           EGA (8 color palletted) 
//      4       1           EGA or high-res. (S)VGA (16 color palletted) 
//      1       8           XGA or low-res. VGA (256 color palletted/greyscale)
 
//  3 or 4      8           SVGA/XGA and above (24/32-bit "true color", 
//                          3x greyscale planes with optional 8-bit alpha channel)
 


////////////////////////////////////////////////////////////////////////////////////////////////
const char CPCXImageIO::m_strExt[] = {"pcx"};
////////////////////////////////////////////////////////////////////////////////////////////////
const char CPCXImageIO::m_strInfo[] = {"zSoft Paintbrush"};


////////////////////////////////////////////////////////////////////////////////////////////////
CPCXImageIO::CPCXImageIO()
{
}

////////////////////////////////////////////////////////////////////////////////////////////////
CPCXImageIO::~CPCXImageIO()
{
}

////////////////////////////////////////////////////////////////////////////////////////////////
const char* CPCXImageIO::GetInfoString(uint32t idx) const
{
    return &m_strInfo[0];
}

////////////////////////////////////////////////////////////////////////////////////////////////
const char* CPCXImageIO::GetExtString(uint32t idx) const
{
    return &m_strExt[0];
}


////////////////////////////////////////////////////////////////////////////////////////////////
CBMImage* CPCXImageIO::LoadBMImage(IDataStream *fs, uint32t loadflags)
{
    if(!fs && !fs->IsOpened())
        return NULL;

    PCXHeader header = {0};
    uint32t ireaded = fs->Read(&header,sizeof(PCXHeader));
    if(ireaded != sizeof(PCXHeader))
        return NULL;

    if(!CheckHeader(header))
        return NULL;

    CBMImage *image = NULL;
    if(header.colorPlanes==1)
        image = Load8BitImage(header,fs,loadflags);
    else
        image = LoadRGBImage(header,fs,loadflags);


    if(loadflags & IMGIO_VERT_FLIP)
        this->VertFlipImage(image->GetPixels(),image->GetWidth(),image->GetHeight(), CBMImgFmtInfo::GetChannelsCount(image->GetFormat()));

    return image;
}


////////////////////////////////////////////////////////////////////////////////////////////////
bool CPCXImageIO::SaveBMImage(IDataStream *fs, const CBMImage *image, ImgIOParam *params)
{
    if(!image && !image->GetPixels())
        return false;
    if(!fs && !fs->IsOpened())
        return false;

    BMIFMT fmt = image->GetFormat();
    uint32t width = image->GetWidth();
    uint32t height = image->GetHeight();
    ubyte *pixels = image->GetPixels();
    uint32t clrplanes = CBMImgFmtInfo::GetChannelsCount(fmt);

    if(width < 2 || width > MAX_WIDTH || height < 2 || height > MAX_HEIGHT)
        return false;

    //Check Supported Formats
    if((fmt != IMGFMT_L8) && (fmt != IMGFMT_RGB8) && (fmt != IMGFMT_RGBA8))
        return false;

    PCXHeader header = {0};
    SetFileHeader(header,width,height,clrplanes);

    for(uint32t h = 0; h < height; h++)
    {
        ubyte *data = pixels + (width*(height-h-1)*clrplanes); //--((loadflags & IMGIO_VERT_FLIP)?(width*(height-h-1)*clrplanes):(width*h*clrplanes));
        if(!WriteLine(fs,width,clrplanes,data))
            return false;
    }

    //Write Palette
    if(clrplanes==1)
    {
        if(!WritePalette(fs,image))
            return false;
    }

    return true;
}

////////////////////////////////////////////////////////////////////////////////////////////////
bool CPCXImageIO::CheckHeader(const PCXHeader &header)
{
    if(header.Manufacturer != 10)//Must Be 10
        return false;

    if(header.Version != 5)//Checking Version
        return false;

    if(header.Encoding != 1)
        return false;

    if(header.Bpp != 8)//Bpp Must Be 8
        return false;

    //Number Of Color Planes
    if(header.colorPlanes != 1 && header.colorPlanes != 3 && header.colorPlanes != 4)
        return false;

    if(header.PaletteType != 1 && header.PaletteType != 2)
        return false;

    return true;
}

////////////////////////////////////////////////////////////////////////////////////////////////
CBMImage* CPCXImageIO::LoadRGBImage(const PCXHeader &header, IDataStream *fs, uint32t loadflags)
{
    boolt res = fs->Seek(0,seek_end);
    uint32t fpos = fs->GetPos();

    uint32t datasize = fpos - sizeof(PCXHeader);
    uint32t width = (uint32t)header.bytesPerLines;
    uint32t height = (uint32t)(header.yMax - header.yMin + 1);

    ubyte *compdata = new ubyte[datasize];

    //Setting File Pointer To Image Data
    fs->Seek(sizeof(PCXHeader),seek_set);
    uint32t ireaded = fs->Read(compdata,datasize);

    if(ireaded != datasize)
    {
        delete [] compdata;
        return NULL;
    }

    BMIFMT fmt = (header.colorPlanes == 3 || (loadflags & IMGIO_LOAD_ALPHA)) ? (IMGFMT_RGB8) : (IMGFMT_RGBA8);
    CBMImage *image = new CBMImage(fmt,width,height,1); //CBMImage(fmt,width,height,1,IMGFMT_NONE,0,0,0,0,1);//CBMImage(fmt,width,height,1);
    if(!image)
    {
        delete [] compdata;
        return NULL;
    }
    ubyte *data = image->GetPixels();
    if(!data)
    {
        delete image;
        delete [] compdata;
        return NULL;
    }


    //Decoding Image Pixels
    uint32t pos = 0;
    uint32t clrplanes = (loadflags & IMGIO_LOAD_ALPHA) ? (3) : ((uint32t)header.colorPlanes);
    for(uint32t h = 0; h < height; h++)
    {
        for(uint32t planes = 0; planes < clrplanes; planes++)
        {
            uint32t x = 0;
            ubyte *ptr = data + h*width*clrplanes + planes;
            while(x < width)
            {
                ubyte c = compdata[pos++];
                if(c > 0xbf)
                {
                    uint32t numRepeat = c & 0x3f;
                    c = compdata[pos++];
                    x += numRepeat;
                    for(uint32t i = 0; i < numRepeat; i++)
                    {
                        *ptr = c;
                        ptr += clrplanes;                      
                    }
                }
                else
                {
                    *ptr = c;
                    ptr += clrplanes;
                    x++;
                }
            }
        }
    }

    delete [] compdata;

    return image;
}
////////////////////////////////////////////////////////////////////////////////////////////////
CBMImage* CPCXImageIO::Load8BitImage(const PCXHeader &header, IDataStream *fs, uint32t loadflags)
{
    //Reading Palette Data
    const uint32t palsize = 769;
    boolt res = fs->Seek(-int32t(palsize),seek_end);
    ubyte palette[palsize] = {0};
    fs->Read(palette,palsize);

    //Checking Palette
    if(palette[0] != 12)
        return NULL;

    //Getting File End Pos
    uint32t fpos = fs->GetPos();

    uint32t width = (uint32t)header.bytesPerLines;
    uint32t height = (uint32t)(header.yMax - header.yMin + 1);

    //Reserving Memory For Image Data
    uint32t datasize = fpos - palsize - sizeof(PCXHeader);
    memory_allocator *mem_alloc = memory_allocator_selector::get_allocator(img_alloc,datasize);
    ubyte *compdata = (ubyte*)mem_alloc->alloc(datasize);
    //--ubyte *compdata = new ubyte[datasize];

    //Setting File Pointer To Image Data
    fs->Seek(sizeof(PCXHeader),seek_set);
    uint32t ireaded = fs->Read(compdata,datasize);

    if(ireaded != datasize)
    {
        //--delete [] compdata;
        mem_alloc->dealloc(compdata);
        return NULL;
    }

    BMIFMT fmt = (loadflags & IMGIO_EXPAND_PALETTE) ? (IMGFMT_RGB8) : (IMGFMT_L8);
    BMIFMT palfmt = (loadflags & IMGIO_EXPAND_PALETTE) ? (IMGFMT_NONE) : (IMGFMT_RGB8);
    uint32t palclrnum = (loadflags & IMGIO_EXPAND_PALETTE) ? (0) : (256);
    CBMImage *image = new CBMImage(fmt,width,height,1,palfmt,palclrnum,0,0,0,1); //CBMImage(fmt,width,height,1);
    if(!image)
    {
        //--delete [] compdata;
        mem_alloc->dealloc(compdata);
        delete (image);
        return NULL;
    }
    ubyte *data = image->GetPixels();
    if(!data)
    {
        //--delete image;
        //--delete [] compdata;
        mem_alloc->dealloc(compdata);
        delete (image);
        return NULL;
    }

    //Decoding Image Pixels
    uint32t idx = 0;
    uint32t pos = 0;
    uint32t size = width*height*CBMImgFmtInfo::GetChannelsCount(fmt);
    while(idx < size)
    {
        ubyte c = compdata[pos++];
        if(c > 0xbf)
        {
            uint32t numRepeat = c & 0x3f;
            c = compdata[pos++];

            if(loadflags & IMGIO_EXPAND_PALETTE)
            {
                for(uint32t i = 0; i < numRepeat; i++)
                {
                    data[idx++] = palette[c*3+1];
                    data[idx++] = palette[c*3+2];
                    data[idx++] = palette[c*3+3];
                }
            }
            else
            {
                for(uint32t i = 0; i < numRepeat; i++)
                    data[idx++] = c;
            }
        }
        else
        {
            if(loadflags & IMGIO_EXPAND_PALETTE)
            {
                data[idx++] = palette[c*3+1];
                data[idx++] = palette[c*3+2];
                data[idx++] = palette[c*3+3];
            }
            else
            {
                data[idx++] = c;
            }
        }
    }

    if(!(loadflags & IMGIO_EXPAND_PALETTE))
    {
        //--ubyte *pal = image->CreatePaletteData(IMGFMT_RGB8,256);
        ubyte *pal = image->GetPalette();
        memcpy(pal,&palette[1],768);
    }

    //--delete [] compdata;
    mem_alloc->dealloc(compdata);

    return image;
}

////////////////////////////////////////////////////////////////////////////////////////////////
void CPCXImageIO::VertFlipImage(ubyte *pixels, uint32t width, uint32t height, uint32t channels)
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
void CPCXImageIO::SetFileHeader(PCXHeader &header, uint32t width, uint32t height, uint32t clrplanes)
{
    header.Manufacturer = 10;
    header.Version = 5;
    header.Encoding = 1;
    header.Bpp = 8;
    header.xMax = (ushort)width - 1;
    header.yMax = (ushort)height - 1;
    header.bytesPerLines = (ushort)width;
    header.horDPI = 72;
    header.vertDPI = 72;
    header.PaletteType = 1;
    header.colorPlanes = (ubyte)clrplanes;

}

////////////////////////////////////////////////////////////////////////////////////////////////
bool CPCXImageIO::WriteLine(IDataStream *fs, uint32t width, uint32t offset, const ubyte *pixels)
{
    const uint32t linesize = 1024;
    ubyte linedata[linesize] = {0};
    uint32t linepos = 0;
    const ubyte *pdata = pixels;

    uint32t x = 0;
    while(x < width)
    {
        ubyte val = pdata[0];
        uint32t num = 1;
        while((num < 63) || ((x + num) < width))
        {            
            ubyte nextval = pixels[num*offset];
            if(nextval != val)
                break;

            num++;
        }

        x += num;
        pdata += num*offset;

        /////////////
        if(num > 1)
        {
            ubyte rlecode = 0xc0 + (ubyte)num;
            linedata[linepos++] = rlecode;
            linedata[linepos++] = val;
        }
        else
        {
            if(val > 0xbf)
                linedata[linepos++] = 0xc1;
            
            linedata[linepos++] = val;
        }   

        //Flush Data
        if(linepos > (linesize-3))
        {
            uint32t ires = fs->Write(linedata,linepos+1);
            if(ires != linepos+1)
                return false;

            linepos = 0;
            memset(linedata,0,linesize);
        }

    }

    return !(fs->Write(linedata,linepos+1) != (linepos+1));
}

////////////////////////////////////////////////////////////////////////////////////////////////
bool CPCXImageIO::WritePalette(IDataStream *fs, const CBMImage *image)
{
    ubyte palette[769] = {0};
    palette[0] = 12;

    if((image->GetPalColorsNum() == 256) && (image->GetPaletteFormat() == IMGFMT_RGB8))
    {
        memcpy(&palette[1],image->GetPalette(),768);
    }
    else
    {
        for(uint32t i = 0; i < 256; i++)
        {
            palette[i*3+1] = i;
            palette[i*3+2] = i;
            palette[i*3+3] = i;
        }
    }

    return (fs->Write(palette,769) == 769);
}