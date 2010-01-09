// -*- mode: C++; tab-width: 4 -*-
// vi: ts=4

/*
 * Copyright (c) 2009, Patrick Palmer.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without 
 * modification, are permitted provided that the following conditions are met:
 *
 *   - Redistributions of source code must retain the above copyright notice,
 *     this list of conditions and the following disclaimer.
 *
 *   - Redistributions in binary form must reproduce the above copyright
 *     notice, this list of conditions and the following disclaimer in the
 *     documentation and/or other materials provided with the distribution.
 *
 *   - Neither the name of Patrick Palmer nor the names of its
 *     contributors may be used to endorse or promote products derived from
 *     this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" 
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE 
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE 
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE 
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR 
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF 
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS 
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN 
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) 
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE 
 * POSSIBILITY OF SUCH DAMAGE.
 */


#include <iostream>
#include <sstream>
#include <string>


#include "DPX.h"
#include "tiffio.h"


using namespace std;
using namespace dpx;



void Usage()
{
	cout << "usage: dpx2tiff [-8] file.dpx file.tif" << endl;
}

   

int main(int argc, char **argv)
{
	int offset = 1;
	bool write8bit = false;
	//bool write8bit = true;
	
	if (argc == 4 && strcmp(argv[1], "-8") == 0)
	{
		write8bit = true;
		offset++;
	}
	else if (argc != 3)
	{
		Usage();
		return 1;
	}

	// open the image
	InStream img;
	if (!img.Open(argv[offset]))
	{
		cout << "Unable to open file " << argv[1] << endl;
		return 1;
	}
	
	dpx::Reader dpx;
	dpx.SetInStream(&img);
	if (!dpx.ReadHeader())
	{
		cout << "Unable to read header" << endl;
		return 2;
	}
	
	TIFF *out;
	out = TIFFOpen(argv[offset+1], "w");
	if (out == NULL)
	{
		cout << "Unable to open output file" << endl;
		return 3;
	}

	// data size, override if user specifies
	dpx::DataSize size = dpx.header.ComponentDataSize(0);
	int nob = dpx.header.ComponentByteCount(0);
	if (write8bit)
	{
		size = dpx::kByte;
		nob = 1;
	}

	cout << "Image Width " << dpx.header.Width() << " Height " << 
			dpx.header.Height() << " component byte count " << 
			dpx.header.ComponentByteCount(0) << endl;
	
	// conversion
	int format = PHOTOMETRIC_RGB;
	int elementCount = 3;
	if (dpx.header.ImageElementComponentCount(0) == 1) {
		format = PHOTOMETRIC_MINISBLACK;
		elementCount = 1;
	}

	TIFFSetField(out, TIFFTAG_IMAGEWIDTH, (uint32) dpx.header.Width());
	TIFFSetField(out, TIFFTAG_IMAGELENGTH, (uint32) dpx.header.Height());
	TIFFSetField(out, TIFFTAG_BITSPERSAMPLE, (uint32) nob * 8);
	TIFFSetField(out, TIFFTAG_SAMPLESPERPIXEL, elementCount);
	TIFFSetField(out, TIFFTAG_COMPRESSION, COMPRESSION_NONE);
	TIFFSetField(out, TIFFTAG_ORIENTATION, ORIENTATION_TOPLEFT);
	TIFFSetField(out, TIFFTAG_PLANARCONFIG, PLANARCONFIG_CONTIG);
	TIFFSetField(out, TIFFTAG_PHOTOMETRIC, format);
	TIFFSetField(out, TIFFTAG_MINSAMPLEVALUE, 0);
	int max = 0xff;
	if (nob == 2)
		max = 0xffff;
	else
		max = 0xffffffff;
	TIFFSetField(out, TIFFTAG_MAXSAMPLEVALUE, max);
	
	tdata_t buf = _TIFFmalloc(TIFFScanlineSize(out));
	if (buf == NULL) 
	{
		cout << "memory allocation error" << endl;
		return 4;
	}

	Block block(0, 0, dpx.header.Width()-1, 0);
	int y;
	for (y = 0; y < dpx.header.Height(); y++)
	{
		block.y1 = y;
		block.y2 = y;
			
		if (dpx.ReadBlock(buf, size, block, dpx.header.ImageDescriptor(0)) == false)
		{
			cout << "unable to read line " << y << " with component data size " << size << endl;
			return 5;
		}

		if (TIFFWriteScanline(out, buf, y, 0) < 0)
		{
			cout << "unable to write tiff scanline " << y << endl;
			return 6;
		}
	}


	_TIFFfree(buf);
	
	img.Close();
	TIFFClose(out);
	
	return 0;
}

