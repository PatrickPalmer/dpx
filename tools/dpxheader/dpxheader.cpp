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


using namespace std;
using namespace dpx;




void Usage()
{
	cout << "usage: dpxheader [-xX] file.dpx" << endl;
	cout << "       -x xml format" << endl;
	cout << "       -X xml format without declaration line" << endl;
}


string XmlTag(const string str)
{
	string r;
	int len = str.length();
	for (int i = 0; i < len; i++)
	{
		char ch = tolower(str[i]);
		if (ch == ' ')
			ch = '_';
		r += ch;
	}
	return r;
}


bool sub = false;

void StartSub(const string t, const int num, const bool xml)
{
	sub = true;
	if (xml)
	{
		cout << "  <" << XmlTag(t);
		if (num >= 0)
			cout << " id=\"" << num << "\"";
		cout << ">" << endl;
	}
	else
	{
		cout << t;
		if (num >= 0)
			cout << "# " << num;
		cout << endl;
	}
}


void EndSub(const string t, const bool xml)
{
	sub = false;
	if (xml)
	{
		cout << "  </" << XmlTag(t) << ">" << endl;
	}
}


void Output(const string t, const string v, const bool xml)
{
	if (sub)
		cout << "  ";
			
	if (xml)
	{
		cout << "  <" << XmlTag(t) << ">" << v << "</" << XmlTag(t) << ">" << endl;
	}
	else
	{
		cout << t << ": " << v << endl;
	}
}



void Output(const string t, const R32 v, const bool xml)
{
	ostringstream o;
	o << v;
	Output(t, o.str(), xml);
}


void Output(const string t, const U16 v, const bool xml)
{
	ostringstream o;
	o << v;
	Output(t, o.str(), xml);
}


void StrOutput(const string t, const string v, const int len, const bool xml)
{
	char buf[256];
	::strncpy(buf, v.c_str(), len);
	buf[len] = '\0';
	
	if (sub)
		cout << "  ";
			
	if (xml)
	{
		cout << "  <" << XmlTag(t) << ">" << buf << "</" << XmlTag(t) << ">" << endl;
	}
	else
	{
		cout << t << ": " << buf << endl;
	}
}





string Display(U8 d)
{
	if (d == 0xff)
		return "0xff";
	else
	{
		ostringstream o;
		o << U16(d);
		return o.str();
	}
}


string Display(U16 d)
{
	if (d == 0xffff)
		return "0xffff";
	else
	{
		ostringstream o;
		o << d;
		return o.str();
	}
}


string Display(U32 d)
{
	if (d == 0xffffffff)
		return "0xffffffff";
	else
	{
		ostringstream o;
		o << d;
		return o.str();
	}
}


string Display(R32 d)
{
	if (d == 0xffffffff)
		return "0xffffffff";
	else
	{
		ostringstream o;
		o << d;
		return o.str();
	}	
}

string DisplayOrientation(U32 o)
{
	string s;
	
	switch (o)
	{
	case kLeftToRightTopToBottom:
		s = "Left to Right Top To Bottom";
		break;
	case kRightToLeftTopToBottom:
		s = "Right to Left Top To Bottom";
		break;		
	case kLeftToRightBottomToTop:
		s = "Left to Right Bottom To Top";
		break;
	case kRightToLeftBottomToTop:
		s = "Right To Left Bottom to Top";
		break;
	case kTopToBottomLeftToRight:
		s = "Top To Bottom Left to Right";
		break;
	case kTopToBottomRightToLeft:
		s = "Top To Bottom Right to Left";
		break;
	case kBottomToTopLeftToRight:
		s = "Bottom to Top Left to Right";
		break;
	case kBottomToTopRightToLeft:
		s = "Bottom to Top Right to Left";
		break;
	case kUndefinedOrientation:
		s = "Undefined";
		break;
	default:
		s = "Unknown " + Display(o);
	}

	return s;
}


string DisplayPacking(U8 p)
{
	string s;
	
	switch (p)
	{
	case kPacked:
		s = "Packed";
		break;
	case kFilledMethodA:
		s = "Filled Method A";
		break;
	case kFilledMethodB:
		s = "Filled Method B";
		break;
	default:
		s = "Unknown " + Display(p);
	}
	
	return s;
}
	
	
	
	
string DisplayDescriptor(U8 d)
{
	string s;
	
	switch (d)
	{
	case kUserDefinedDescriptor:
		s = "User Defined";
		break;
	case kRed:
		s = "Red";
		break;
	case kGreen:
		s = "Green";
		break;
	case kBlue:
		s = "Blue";
		break;
	case kAlpha:
		s = "Alpha";
		break;
	case kLuma:
		s = "Luma";
		break;
	case kColorDifference:
		s = "Color Difference";
		break;
	case kDepth:
		s = "Depth";
		break;
	case kCompositeVideo:
		s = "Composite Video";
		break;
	case kRGB:
		s = "RGB";
		break;
	case kRGBA:
		s = "RGBA";
		break;
	case kABGR:
		s = "ABGR";
		break;
	case kCbYCrY:
		s = "CbYCrY";
		break;
	case kCbYACrYA:
		s = "CbYaCrYa";
		break;
	case kCbYCr:
		s = "CbYCr";
		break;
	case kCbYCrA:
		s = "CbYCra";
		break;
	case kUserDefined2Comp:
		s = "User Defined 2";
		break;
	case kUserDefined3Comp:
		s = "User Defined 3";
		break;
	case kUserDefined4Comp:
		s = "User Defined 4";
		break;
	case kUserDefined5Comp:
		s = "User Defined 5";
		break;
	case kUserDefined6Comp:
		s = "User Defined 6";
		break;
	case kUserDefined7Comp:
		s = "User Defined 7";
		break;
	case kUserDefined8Comp:
		s = "User Defined 8";
		break;
	case kUndefinedDescriptor:
		s = "Undefined";
		break;
	default:
		s = "Unknown " + Display(d);
		break;
	}
	
	return s;
}


string DisplayCharacteristic(U8 c) 
{
	string s;
	
	switch (c)
	{
	case kUserDefined:
		s = "User Defined";
		break;
	case kPrintingDensity:
		s = "Printing Density";
		break;
	case kLinear:
		s = "Linear";
		break;
	case kLogarithmic:
		s = "Logarithmic";
		break;
	case kUnspecifiedVideo:
		s = "Unspecified Video";
		break;
	case kSMPTE274M:
		s = "SMPTE 274M";
		break;
	case kITUR709:
		s = "ITU-R 709";
		break;
	case kITUR601:
		s = "ITU-R 601";
		break;
	case kITUR602:
		s = "ITU-R 702";
		break;
	case kNTSCCompositeVideo:
		s = "NTSC Composite Video";
		break;
	case kPALCompositeVideo:
		s = "PAL Composite Video";
		break;
	case kZLinear:
		s = "Linear";
		break;
	case kZHomogeneous:
		s = "Homogeneous";
		break;
	case kUndefinedCharacteristic:
		s = "Undefined";
		break;
	default:
		s = "Unknown " + Display(c);
		break;
	}
	
	return s;
}


string DisplayVideoSignal(U8 v)
{
	string s;
	
	switch (v)
	{
	case kUndefined:
		s = "Undefined";
		break;
	case kNTSC:
		s = "NTSC";
		break;
	case kPAL:
		s = "PAL";
		break;
	case kPAL_M:
		s = "PAL M";
		break;
	case kSECAM:
		s = "SECAM";
		break;
	case k525LineInterlace43AR:
		s = "525 Line Interlace 4:3";
		break;
	case k625LineInterlace43AR:
		s = "625 Line Interlace 4:3";
		break;
	case k525LineInterlace169AR:
		s = "525 Line Interlace 16:9";
		break;
	case k625LineInterlace169AR:
		s = "625 Line Interlace 16:9";
		break;
	case k1050LineInterlace169AR:
		s = "1050 Line Interlace 16:9";
		break;
	case k1125LineInterlace169AR_274:
		s = "1125 Line Interlace 16:9";
		break;
	case k1250LineInterlace169AR:
		s = "1250 Line Interlace 16:9";
		break;
	case k1125LineInterlace169AR_240:
		s = "1125 Line Interlace 16:9";
		break;		
	case k525LineProgressive169AR:
		s = "525 Line Progressive 16:9";
		break;
	case k625LineProgressive169AR:
		s = "625 Line Progressive 16:9";
		break;
	case k750LineProgressive169AR:
		s = "750 Line Progressive 16:9";
		break;
	case k1125LineProgressive169AR:
		s = "1125 Line Progressive 16:9";
		break;
	default:
		s = "Unknown " + Display(v);
		break;
	}
	
	return s;
}



int main(int argc, char **argv)
{
	bool xml = false;
	bool xmlDeclaration = false;
	int findx = 1;
	char buf[24];
		
	// check argument count
	if (argc == 3)
	{
		if (strcmp("-x", argv[1]) == 0)
		{
			findx = 2;
			xml = true;
			xmlDeclaration = true;
		}
		else if (strcmp("-X", argv[1]) == 0)
		{
			findx = 2;
			xml = true;
		}
		else
		{
			Usage();
			return 1;
		}
	}
	else if (argc != 2)
	{
		Usage();
		return 1;
	}

	// open the image
	InStream img;
	if (!img.Open(argv[findx]))
	{
		cout << "Unable to open file " << argv[findx] << endl;
		return 1;
	}

	dpx::Header header;
	
	if (!header.Read(&img))
	{
		cout << "Unable to read header" << endl;
		return 1;
	}

		
	// display header info
	if (xml)
	{
		if (xmlDeclaration)
			cout << "<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"yes\"?>" << endl;
		cout << "<dpx>" << endl;
	}
	
	char str[201];
	header.Version(str);
	StrOutput("Version",str, 8, xml);
	
	::sprintf(buf, "%x", header.MagicNumber());
	StrOutput("Magic Number", buf, 8, xml);
	StrOutput("Endian Swap", (header.RequiresByteSwap() ? "true" : "false"), 6, xml);
	Output("Image Offset", Display(header.ImageOffset()), xml);
	Output("File Size", Display(header.FileSize()), xml);
 	Output("Ditto Key", Display(header.DittoKey()), xml);
	Output("Generic Size", Display(header.GenericSize()), xml);
	Output("Industry Size", Display(header.IndustrySize()), xml);
	Output("User Size", Display(header.UserSize()), xml);

	header.FileName(str);
	StrOutput("Filename", str, 100, xml);
	header.CreationTimeDate(str);
	StrOutput("Creation Time and Date", str, 32, xml);
	header.Creator(str);
	StrOutput("Creator", str, 100, xml);
	header.Project(str);
	StrOutput("Project", str, 200, xml);
	header.Copyright(str);
	StrOutput("Copyright", str, 200, xml);
	Output("Encrypt Key", Display(header.EncryptKey()), xml);

	Output("Image Orientation", DisplayOrientation(header.ImageOrientation()), xml);
	Output("Number of Elements", header.NumberOfElements(), xml);
	Output("Width", Display(header.PixelsPerLine()), xml);
	Output("Height", Display(header.LinesPerElement()), xml);
			
	for (int i = 0; i < header.numberOfElements; i++)
	{
		StartSub("Image Element", (i+1), xml);
		Output("Data Sign", Display(header.DataSign(i)), xml);
		Output("Low Data", Display(header.LowData(i)), xml);
		Output("Low Quantity", Display(header.LowQuantity(i)), xml);
		Output("High Data", Display(header.HighData(i)), xml);
		Output("High Quantity", Display(header.chan[i].highQuantity), xml);
		Output("Descriptor", DisplayDescriptor(header.ImageDescriptor(i)), xml);
		Output("Transfer", DisplayCharacteristic(header.Transfer(i)), xml);
		Output("Colorimetric", DisplayCharacteristic(header.Colorimetric(i)), xml);		
		Output("Bit Size", Display(header.BitDepth(i)), xml);
		Output("Packing", DisplayPacking(header.ImagePacking(i)), xml);
		Output("Encoding", (header.ImageEncoding(i) ? "true" : "false"), xml);
		Output("Data Offset", Display(header.DataOffset(i)), xml);
		Output("End of Line Padding", Display(header.EndOfLinePadding(i)), xml);
		Output("End of Image Padding", Display(header.EndOfImagePadding(i)), xml);
		header.Description(i, str);
		StrOutput("Description", str, 32, xml);
		EndSub("Image Element", xml);
	}

	Output("X Offset", Display(header.XOffset()), xml);
	Output("Y Offset", Display(header.YOffset()) , xml);
	Output("X Center", Display(header.XCenter()), xml);
	Output("Y Center", Display(header.YCenter()), xml);
	Output("X Original Size", Display(header.XOriginalSize()), xml);
	Output("Y Original Size", Display(header.YOriginalSize()), xml);
	header.SourceImageFileName(str);
	StrOutput("Source Image Filename", str, 100, xml);
	header.SourceTimeDate(str);
	StrOutput("Source Time and Date", str, 24, xml);
	header.InputDevice(str);
	StrOutput("Input Device", str, 32, xml);
	header.InputDeviceSerialNumber(str);
	StrOutput("Input Device Serial Number", str, 32, xml);
	StartSub("Border", -1, xml);
	Output("XL", Display(header.Border(0)), xml);
	Output("XR", Display(header.Border(1)), xml);
	Output("YT", Display(header.Border(2)), xml);
	Output("YB", Display(header.Border(3)), xml);	
	EndSub("Border", xml);
	StartSub("Aspect Ratio", -1, xml);
	Output("Horizontal", Display(header.AspectRatio(0)), xml);
	Output("Vertical", Display(header.AspectRatio(1)), xml);	
	EndSub("Aspect Ratio", xml);
	Output("X Scanned Size", Display(header.XScannedSize()), xml);
	Output("Y Scanned Size", Display(header.YScannedSize()), xml);

	header.FilmEdgeCode(buf);
	StrOutput("Film Code", buf, 16, xml);

	header.Format(str);
	StrOutput("Format", str, 32, xml);
	Output("Frame Position", Display(header.FramePosition()), xml);
	Output("Sequence Length", Display(header.SequenceLength()), xml);
	Output("Held Count", Display(header.HeldCount()), xml);
	Output("Frame Rate", Display(header.FrameRate()), xml);
	Output("Shutter Angle", Display(header.ShutterAngle()), xml);
	header.FrameId(str);
	StrOutput("Frame Id", str, 32, xml);
	header.SlateInfo(str);
	StrOutput("Slate Info", str, 100, xml);

	header.TimeCode(buf);
	Output("Timecode", buf, xml);
	header.UserBits(buf);
	Output("User Bits", buf, xml);
	Output("Interlace", Display(header.Interlace()), xml);
	Output("Field Number", Display(header.FieldNumber()), xml);
	Output("Video Signal", DisplayVideoSignal(header.Signal()), xml);

	Output("Horizontal Sample Rate", Display(header.HorizontalSampleRate()), xml);
	Output("Vertical Sample Rate", Display(header.VerticalSampleRate()), xml);
	Output("Temporal Frame Rate", Display(header.TemporalFrameRate()), xml);
	Output("Time Offset", Display(header.TimeOffset()), xml);
	Output("Gamma", Display(header.Gamma()), xml);
	Output("Black Level", Display(header.BlackLevel()), xml);
	Output("Black Gain", Display(header.BlackGain()), xml);
	Output("Break Point", Display(header.BreakPoint()), xml);
	Output("White Point", Display(header.WhiteLevel()), xml);
	Output("Integration Times", Display(header.IntegrationTimes()), xml);
	
	if (xml)
		cout << "</dpx>" << endl;
		
	return 0;
}

