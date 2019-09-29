/////////////////////////////////////////////////////////////////////////////////////////////
// ImageIO.cpp
/////////////////////////////////////////////////////////////////////////////////////////////

#include "Image.h"
#include "ImageIO.h"
#include "Utils/IOStream/IDataStream.h"
#include "Utils/IOStream/FileStream.h"

#include <stdio.h>
#include <memory.h>
#include <string.h>



/////////////////////////////////////////////////////////////////////////////////////////////
/*
void CBMImageIO::VertFlipImage(uint32t width, uint32t height, uint32t depth, uint32t mipmaps, uint32t align, BMIFMT fmt, ubyte *data)
{
    if(CBMImgFmtInfo::IsCompressedFormat(fmt)) return ;

    uint32t mipnum = CBMImgInfo::CalcMipMapsNum(width,height,depth);
    mipnum = (mipmaps < mipnum) ? (mipmaps) : (mipnum);

    depth = (depth==0)?(1):(depth);
    height = (height==0)?(1):(height);

    ubyte *base = data;
    ubyte *ptr1 = data;
    ubyte *ptr2 = data;

    uint32t maxwidth = CBMImgInfo::CalcFullWidthSize(fmt,width,align);
    ubyte *tmpdat = new ubyte[maxwidth];

    for(uint32t d = 0; d < depth; d++)
    {
        uint32t mipwidth = width;
        uint32t mipheight = height;
        for(uint32t level = 0; level <= mipnum; level++)
        {
            uint32t fullwidth = CBMImgInfo::CalcFullWidthSize(fmt,mipwidth,align);
            ptr1 = base;
            ptr2 = base + fullwidth*(mipheight-1);
            for(uint32t h = 0; h < mipheight>>1; h++)
            {
                memcpy(tmpdat,ptr1,fullwidth);
                memcpy(ptr1,ptr2,fullwidth);
                memcpy(ptr2,tmpdat,fullwidth);
                ptr1 += fullwidth;
                ptr2 -= fullwidth;
            }
            base += fullwidth*mipheight;
            mipwidth = ((mipwidth>>1)>0)?(mipwidth>>1):(1);
            mipheight = ((mipheight>>1)>0)?(mipheight>>1):(1);
        }
    }

    delete [] tmpdat;
}
*/
/////////////////////////////////////////////////////////////////////////////////////////////