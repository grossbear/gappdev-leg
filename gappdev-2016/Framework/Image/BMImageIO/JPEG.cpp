///////////////////////////////////////////////////////////////////////////////////////////////
//	JPEG.h
//	JPEG Image Format Loading/Saving Class Definition
//	
////////////////////////////////////////////////////////////////////////////////////////////////

#include "Utils/IOStream/IDataStream.h"
#include "Image/Image.h"
#include "JPEG.h"
//#include "libjpeg/cdjpeg.h"

#include <setjmp.h>
#include <memory.h>


////////////////////////////////////////////////////////////////////////////////////////////////
struct my_error_mgr {
  struct jpeg_error_mgr pub;	// "public" fields

  jmp_buf setjmp_buffer;	// for return to caller 
};

typedef struct my_error_mgr * my_error_ptr;

//
// Here's the routine that will replace the standard error_exit method:
//

METHODDEF(void)
my_error_exit (j_common_ptr cinfo)
{
	// cinfo->err really points to a my_error_mgr struct, so coerce pointer 
	my_error_ptr myerr = (my_error_ptr) cinfo->err;

	// Always display the message. 
	// We could postpone this until after returning, if we chose. 
	(*cinfo->err->output_message) (cinfo);

	// Return control to the setjmp point 
	longjmp(myerr->setjmp_buffer, 1);
}


////////////////////////////////////////////////////////////////////////////////////////////////
char CJPEGImageIO::m_strInfo[] = {"JPEG Image File"};
////////////////////////////////////////////////////////////////////////////////////////////////
char CJPEGImageIO::m_strExt[] = {"jpg"};

////////////////////////////////////////////////////////////////////////////////////////////////
CJPEGImageIO::CJPEGImageIO()
{
}

////////////////////////////////////////////////////////////////////////////////////////////////
CJPEGImageIO::~CJPEGImageIO()
{
}

////////////////////////////////////////////////////////////////////////////////////////////////
const char* CJPEGImageIO::GetInfoString(uint32t idx) const
{
    return &m_strInfo[0];
}

////////////////////////////////////////////////////////////////////////////////////////////////
const char* CJPEGImageIO::GetExtString(uint32t idx) const
{
    return &m_strExt[0];
}

////////////////////////////////////////////////////////////////////////////////////////////////
CBMImage* CJPEGImageIO::LoadBMImage(IDataStream *datastream, uint32t loadflags)
{
    //If File Stream Is Not NULL
    if(datastream == NULL)
        return NULL;

    // If File Stream Is Not Opened
    if(!datastream->IsOpened())
        return NULL;

    // This struct contains the JPEG decompression parameters and pointers to
	// working space (which is allocated as needed by the JPEG library).
	struct jpeg_decompress_struct cinfo;

	// We use our private extension JPEG error handler.
	// Note that this struct must live as long as the main JPEG parameter
	// struct, to avoid dangling-pointer problems.
	struct my_error_mgr jerr;

	// More stuff //
	JSAMPARRAY buffer;		// Output row buffer 
	int row_stride;			// physical row width in output buffer 
	
	// Step 1: allocate and initialize JPEG decompression object 

	// We set up the normal JPEG error routines, then override error_exit. 
	cinfo.err = jpeg_std_error(&jerr.pub);
	jerr.pub.error_exit = my_error_exit;
	// Establish the setjmp return context for my_error_exit to use. 
	
	if (setjmp(jerr.setjmp_buffer)) 
	{
		// If we get here, the JPEG code has signaled an error.
		// We need to clean up the JPEG object, close the input file, and return.
		jpeg_destroy_decompress(&cinfo);
		return NULL;
	}
	//Now we can initialize the JPEG decompression object. 
	jpeg_create_decompress(&cinfo);

	// Step 2: specify data source (eg, a file) //
	// Set source to NULL, cause We Will Use Our Own Stream Object And Own Jpeg Read Function
	jpeg_stdio_src(&cinfo, NULL); 
	cinfo.client_data = datastream;//Set File Stream Object To Client Data Pointer
	cinfo.src->fill_input_buffer = CJPEGImageIO::JpgReadFunc;//Set Own File Read Function

    //cjpeg_source_ptr src_mgr = jinit_read_bmp(cinfo);

	// Step 3: read file parameters with jpeg_read_header() //
	jpeg_read_header(&cinfo, TRUE);
    

    if(cinfo.num_components != 3 && cinfo.num_components != 1)
	{
		jpeg_destroy_decompress(&cinfo);
		return NULL;
	}
	
	// We can ignore the return value from jpeg_read_header since
	//   (a) suspension is not possible with the stdio data source, and
	//   (b) we passed TRUE to reject a tables-only JPEG file as an error.
	// See libjpeg.txt for more info.

	// Step 4: set parameters for decompression //

	// In this example, we don't need to change any of the defaults set by
	// jpeg_read_header(), so we do nothing here.

	// Step 5: Start decompressor //
	jpeg_start_decompress(&cinfo);

	// We can ignore the return value since suspension is not possible
	// with the stdio data source.

	// We may need to do some setup of our own at this point before reading
	// the data.  After jpeg_start_decompress() we have the correct scaled
	// output image dimensions available, as well as the output colormap
	// if we asked for color quantization.
	// In this example, we need to make an output work buffer of the right size.
	//
	// JSAMPLEs per row in output buffer //
	row_stride = cinfo.output_width * cinfo.output_components;
	// Make a one-row-high sample array that will go away when done with image //
	buffer = (*cinfo.mem->alloc_sarray)
			((j_common_ptr) &cinfo, JPOOL_IMAGE, row_stride, 1);


    BMIFMT fmt = (cinfo.num_components==3)?(IMGFMT_RGB8):(IMGFMT_L8);
    // Creating image
    CBMImage *image = new CBMImage(fmt,(uint32t)cinfo.image_width,(uint32t)cinfo.image_height);//CBMImage(fmt,(uint32t)cinfo.image_width,(uint32t)cinfo.image_height);
    // Getting Pixel Data
    ubyte *pixels = image->GetPixels();

	// Step 6: while (scan lines remain to be read) 
	//           jpeg_read_scanlines(...); 

    uint32t pixsize = CBMImgFmtInfo::GetBytesPerPixel(fmt);
    //--uint32t fullsize = uint32t(cinfo.output_width)*pixsize;
    int32t fullsize = (int32t)(uint32t(cinfo.output_width)*pixsize);
    // Set Last Line
    pixels = pixels+fullsize*uint32t(cinfo.image_height-1);
    // If LoadFlags Are Set To Load Flip Image
    if(loadflags & IMGIO_VERT_FLIP)
    {
        fullsize = -fullsize;
        pixels = 0;
    }
	// Here we use the library's state variable cinfo.output_scanline as the
	// loop counter, so that we don't have to keep track ourselves.
	while (cinfo.output_scanline < cinfo.output_height) 
	{
		// jpeg_read_scanlines expects an array of pointers to scanlines.
		// Here the array is only one element long, but you could ask for
		// more than one scanline at a time if that's more convenient.

		(void) jpeg_read_scanlines(&cinfo, buffer, 1);

        // Saving colors in image pixels
        memcpy(pixels,buffer[0],fullsize);
        pixels -= fullsize;
	}

	
	// Step 7: Finish decompression //

	(void) jpeg_finish_decompress(&cinfo);
	// We can ignore the return value since suspension is not possible
	// with the stdio data source.

	// Step 8: Release JPEG decompression object //

	// This is an important step since it will release a good deal of memory. //
	jpeg_destroy_decompress(&cinfo);

	// After finish_decompress, we can close the input file.
	// Here we postpone it until after no more JPEG errors are possible,
	// so as to simplify the setjmp error logic above.  (Actually, I don't
	// think that jpeg_destroy can do an error exit, but why assume anything...)

	// At this point you may want to check to see whether any corrupt-data
	// warnings occurred (test whether jerr.pub.num_warnings is nonzero).

	// And we're done! //
	
	return image;
}

////////////////////////////////////////////////////////////////////////////////////////////////
bool CJPEGImageIO::SaveBMImage(IDataStream *datastream, const CBMImage *image, ImgIOParam *params)
{
    if(image==NULL || datastream==NULL) return false;
    if(!datastream->IsOpened()) return false;

    BMIFMT fmt = image->GetFormat();
    if(fmt != IMGFMT_RGB8 && fmt != IMGFMT_L8) return false;
        
    // This struct contains the JPEG compression parameters and pointers to
	// working space (which is allocated as needed by the JPEG library).
	// It is possible to have several such structures, representing multiple
	// compression/decompression processes, in existence at once.  We refer
	// to any one struct (and its associated working data) as a "JPEG object".
	//
	struct jpeg_compress_struct cinfo;

	// This struct represents a JPEG error handler.  It is declared separately
	// because applications often want to supply a specialized error handler
	// (see the second half of this file for an example).  But here we just
	// take the easy way out and use the standard error handler, which will
	// print a message on stderr and call exit() if compression fails.
	// Note that this struct must live as long as the main JPEG parameter
	// struct, to avoid dangling-pointer problems.
	///
	struct jpeg_error_mgr jerr;

	// Step 1: allocate and initialize JPEG compression object //

	// We have to set up the error handler first, in case the initialization
	// step fails.  (Unlikely, but it could happen if you are out of memory.)
	// This routine fills in the contents of struct jerr, and returns jerr's
	// address which we place into the link field in cinfo.
	//

	cinfo.err = jpeg_std_error(&jerr);
	// Now we can initialize the JPEG compression object. //
	jpeg_create_compress(&cinfo);

	 // Step 2: specify data destination (eg, a file) //
	// Note: steps 2 and 3 can be done in either order. //

	// Here we use the library-supplied code to send compressed data to a
	// stdio stream.  You can also write your own code to do something else.
	// VERY IMPORTANT: use "b" option to fopen() if you are on a machine that
	// requires it in order to write binary files.
	//
    //jpeg_stdio_src(&cinfo, NULL); 
    jpeg_stdio_dest(&cinfo,NULL);
    cinfo.client_data = datastream;//Set File Stream Object To Client Data Pointer
    cinfo.dest->empty_output_buffer = CJPEGImageIO::JpgWriteFunc; //Set Own File Write Function
    cinfo.dest->term_destination = CJPEGImageIO::JpgTermDestFunc; //Setting Own End File Write Function
	
    //Step 3: set parameters for compression //

	// First we supply a description of the input image.
	// Four fields of the cinfo struct must be filled in:
	//
    uint32t width = image->GetWidth();
    uint32t height = image->GetHeight();
    ubyte *pixels = image->GetPixels();
    uint32t fullwidth = width * ((fmt==IMGFMT_RGB8)?(3):(1));

	cinfo.image_width = (JDIMENSION)width; 	 //image width and height, in pixels 
	cinfo.image_height = (JDIMENSION)height;
    cinfo.input_components = (fmt==IMGFMT_RGB8)?(3):(1);	// # of color components per pixel 
    cinfo.in_color_space = (fmt==IMGFMT_RGB8) ? (JCS_RGB) : (JCS_GRAYSCALE); 	// colorspace of input image 
	// Now use the library's routine to set default compression parameters.
	// (You must set at least cinfo.in_color_space before calling this,
	//since the defaults depend on the source color space.)
	//
	jpeg_set_defaults(&cinfo);

    // Getting Quality Value If Available
    int quality = 95;
    if(params)
    {
        ImgIOParam *ptr = params;
        while(ptr->type != IMGIOPARAM_NONE)
        {
            if(ptr->type == IMGIOPARAM_JPEG_QUALITY)
                quality = ptr->value;
        }
    }

	// Now you can set any non-default parameters you wish to.
	// Here we just illustrate the use of quality (quantization table) scaling:
	///
    ;
	jpeg_set_quality(&cinfo, quality, TRUE); // limit to baseline-JPEG values ///

	// Step 4: Start compressor ///

	// TRUE ensures that we will write a complete interchange-JPEG file.
	// Pass TRUE unless you are very sure of what you're doing.
	///
	jpeg_start_compress(&cinfo, TRUE);

	// Step 5: while (scan lines remain to be written) ///
	//           jpeg_write_scanlines(...); ///

	// Here we use the library's state variable cinfo.next_scanline as the
	// loop counter, so that we don't have to keep track ourselves.
	// To keep things simple, we pass one scanline per call; you can pass
	// more if you wish, though.
	///
	
    // More stuff //
	JSAMPARRAY buffer;		// Output row buffer 
    buffer = (*cinfo.mem->alloc_sarray)
        ((j_common_ptr) &cinfo, JPOOL_IMAGE, (JDIMENSION)fullwidth, 1);

    pixels = pixels + fullwidth*(height-1);
	while (cinfo.next_scanline < cinfo.image_height) 
	{
		// jpeg_write_scanlines expects an array of pointers to scanlines.
		 // Here the array is only one element long, but you could pass
		 // more than one scanline at a time if that's more convenient.
		 ///
        
        memcpy(buffer[0],pixels,fullwidth);
        (void) jpeg_write_scanlines(&cinfo, buffer, 1);
        pixels -= fullwidth;
	}

	// Step 6: Finish compression ///

	jpeg_finish_compress(&cinfo);
	//// After finish_compress, we can close the output file. ///
	//fclose(outfile);

	// Step 7: release JPEG compression object ///

	// This is an important step since it will release a good deal of memory. ///
	jpeg_destroy_compress(&cinfo);

	// And we're done! ///

	return true;
}


////////////////////////////////////////////////////////////////////////////////////////////////
// Jpeg Read Special Function
boolean CJPEGImageIO::JpgReadFunc(j_decompress_ptr cinfo)
{
    IDataStream *stream = (IDataStream *)cinfo->client_data;
	size_t nbytes = (size_t)stream->Read(jpeg_get_src_buffer(cinfo),(uint32t)jpeg_get_buf_read_size());
	return update_fill_input_buffer(cinfo, nbytes);

}
	
////////////////////////////////////////////////////////////////////////////////////////////////
// Jpeg Write Special Function
boolean CJPEGImageIO::JpgWriteFunc(j_compress_ptr cinfo)
{
    IDataStream *stream = (IDataStream*)cinfo->client_data;
    size_t nbytes = (size_t)stream->Write(jpeg_get_dest_buffer(cinfo),(uint32t)jpeg_get_buf_write_size());
    return update_empty_output_buffer(cinfo);
}

////////////////////////////////////////////////////////////////////////////////////////////////
// Jpeg Terminate Destination - Usually Flush Buffer
void CJPEGImageIO::JpgTermDestFunc(j_compress_ptr cinfo)
{
    IDataStream *stream = (IDataStream*)cinfo->client_data;
    size_t datacount = jpeg_get_term_datacount(cinfo);
    stream->Write(jpeg_get_dest_buffer(cinfo),datacount);
}
////////////////////////////////////////////////////////////////////////////////////////////////

