# SMPTE DPX v2 Image Format reader/writer #
Version 0.5, January 9, 2009
http://dpx.googlecode.com

DPX image reader / writer library written in portable C++.



## DPX Standard ##

The SMPTE DPX v2 268M-2003 standards document is copyrighted and can
not be freely distributed.  It is available for purchase from the
Society of Motion Picture and Television Engineers (http://www.smpte.org).
There are a number of other SMPTE standards referenced within the
DPX standard and would need to be purchased as well.



## Future ##

Areas for future development.

  * run length encoding
  * sampling conversions (i.e., 422 to 444)
  * chroma conversions (i.e., YPbPr to RGB)
  * high speed pathways (RGB, 10bit unpacking to 16bit)
  * high speed conversions written in SSE2
  * writing user data