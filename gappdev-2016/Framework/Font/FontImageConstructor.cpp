/////////////////////////////////////////////////////////////////////////////////////////////
// FontImageConstructor.cpp
//
/////////////////////////////////////////////////////////////////////////////////////////////


#include "FontImageConstructor.h"

/////////////////////////////////////////////////////////////////////////////////////////////
CBMImage* CFontImageConstructor::CreateFontImage(uint firstchar, uint lastchar, uint fontsize, uint spacewidth,
									const uint sizestab[][3], const ubyte *psymbdata, bool balpha)
{
	//calculating init font image size
	uint imgwidth = 32;
	uint imgheight = 16;

	std::vector<CharDesc> symbsdesc;
    bool bresult = false;
	while(!bresult)
	{
        //Init Symbol Desc Vector
        InitSymbDesc(symbsdesc,spacewidth,fontsize);

		if(imgwidth > imgheight)
			imgheight <<= 1;
		else
			imgwidth <<= 1;

		bresult = SetSymbolsDesc(symbsdesc,firstchar,lastchar,imgwidth,imgheight,sizestab);
	}

    CBMImage *image = SetImage(symbsdesc,imgwidth,imgheight,psymbdata,balpha);

	return image;
}

/////////////////////////////////////////////////////////////////////////////////////////////
void CFontImageConstructor::InitSymbDesc(std::vector<CharDesc> &symbsdesc, uint spacewidth, uint fontsize)
{
    symbsdesc.clear();
	CharDesc spacedesc = {0};
	spacedesc.charsymb = 32;
	spacedesc.sizex = spacewidth;
    spacedesc.sizey = fontsize;
	symbsdesc.push_back(spacedesc);
}

/////////////////////////////////////////////////////////////////////////////////////////////
bool CFontImageConstructor::SetSymbolsDesc(std::vector<CharDesc> &symbsdesc, uint firstchar, uint lastchar,
                                            uint imgwidth, uint imgheight,
                                            const uint sizestab[][3])
{
    uint posx = 1;
    uint posy = imgheight-2;
    uint maxoff = 0;
    uint symbcnt = lastchar - firstchar + 1;
    for(uint idx = 0; idx < symbcnt; idx++)
    {
        uint symbwidth = sizestab[idx][0];
		uint symbheight = sizestab[idx][1];
        uint symbdescent = sizestab[idx][2];

        if(posx + symbwidth >= imgwidth) {
            if(symbwidth+2>imgwidth) 
                return false;
            if(maxoff+2>posy)
                return false;

            posx = 1;
            posy -= (maxoff+2);
            maxoff = 0;
        }
        if(symbheight > posy) return false;

        maxoff = (maxoff < symbheight) ? (symbheight) : (maxoff);
        CharDesc symboldesc={0};
        symboldesc.charsymb = firstchar + idx;
		symboldesc.descent = symbdescent;
		symboldesc.posx = posx;
		symboldesc.posy = posy - (symbheight-1);
		symboldesc.sizex = symbwidth;
		symboldesc.sizey = symbheight;
		symbsdesc.push_back(symboldesc);

        posx += symbwidth + 2;
    }

    return true;
}

/////////////////////////////////////////////////////////////////////////////////////////////
CBMImage* CFontImageConstructor::SetImage(std::vector<CharDesc> &symbsdesc, uint imgwidth, uint imgheight, 
                                          const ubyte *psymbdata, bool balpha)
{
    BMIFMT fmt = (balpha) ? (IMGFMT_LA8) : (IMGFMT_L8);
    uint channels = (balpha) ? (2) : (1);
    CBMImage *image = new CBMImage(fmt,imgwidth,imgheight,1,IMGFMT_NONE,0,symbsdesc.size()*sizeof(CharDesc));
    
    ubyte *infodata = image->GetInfoData();
    memcpy(infodata,&symbsdesc[0],symbsdesc.size()*sizeof(CharDesc));
    
    ubyte *pixdata = image->GetPixels();
    memset(pixdata,0,imgwidth*imgheight*sizeof(ubyte));
    const ubyte *data = psymbdata;
    for(uint i = 0; i < symbsdesc.size()-1; i++)
	{
		uint posx = symbsdesc[i+1].posx;
		uint posy = symbsdesc[i+1].posy;
		ubyte *ptr = pixdata + imgwidth*channels*posy + posx*channels;
		
		uint width = symbsdesc[i+1].sizex;
		uint height = symbsdesc[i+1].sizey;
        //const ubyte *input = data+(line-1)*width;
        if(balpha) {
            for(uint line = height; line > 0; line--) {
                const ubyte *input = data+(line-1)*width;
                for(uint x = 0; x < width; x++) {
                    ptr[x*2] = *input;
                    ptr[x*2+1] = *input;
                    input++;
                }
            }
        }
        else {
		    for(uint line = height; line > 0; line--) 
            {
			    memcpy(ptr,data+(line-1)*width,width*sizeof(ubyte));
			    ptr += imgwidth;	
		    }
        }
        data += width*height;
	}

    return image;
}

/////////////////////////////////////////////////////////////////////////////////////////////