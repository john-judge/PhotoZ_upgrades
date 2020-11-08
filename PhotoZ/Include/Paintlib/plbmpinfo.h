/*
/--------------------------------------------------------------------
|
|      $Id: plbmpinfo.h,v 1.1 2002/08/04 20:17:57 uzadow Exp $
|
|      Copyright (c) 1996-2002 Ulrich von Zadow
|
\--------------------------------------------------------------------
*/

#ifndef INCL_PLBMPINFO
#define INCL_PLBMPINFO

#include "plobject.h"
#include "plpoint.h"

//! This is a simple base class that holds bitmap metainformation. It is
//! used as a base class by PLPicDecoder and PLBmp.
class PLBmpInfo : public PLObject
{

public:
  //! 
  PLBmpInfo (const PLPoint& Size, int bpp, const PLPoint& Resolution, 
             bool bAlphaChannel, bool bIsGreyscale);

  //! 
  PLBmpInfo () {}

  //!
  PLBmpInfo (const char * pszInfo);

  // This class doesn't have any virtual functions and there's nothing to 
  // destroy, so we don't define a destructor  - and the default assignment
  // and equality operators work well too.

  //!
  const PLPoint& GetSize () const
  { return m_Size; }

  //!
  int GetWidth () const
  { return m_Size.x; }

  //!
  int GetHeight () const
  { return m_Size.y; }

  //!
  int GetBitsPerPixel () const
  { return m_bpp; }

  //!
  bool HasAlpha () const
  { return m_bAlphaChannel; }

  //!
  bool IsGreyscale () const
  { return m_bIsGreyscale; }


  //! Gets the bitmap resolution in pixels per inch. Returns 0 if the
  //! resolution is unknown.
  const PLPoint& GetResolution () const
  { return m_Resolution; }

  //!
  void AsString (char * psz, int len) const;
  
  //!
  bool operator == (const PLBmpInfo & Other) const;

protected:
  void SetBmpInfo (const PLPoint& Size, int bpp, const PLPoint& Resolution, 
                   bool bAlphaChannel, bool bIsGreyscale);
  void SetBmpInfo (const PLBmpInfo& SrcInfo);

  PLPoint m_Size;
  int     m_bpp;
  PLPoint m_Resolution;

  bool    m_bAlphaChannel;
  bool    m_bIsGreyscale;
};


#endif
/*
/--------------------------------------------------------------------
|
|      $Log: plbmpinfo.h,v $
|      Revision 1.1  2002/08/04 20:17:57  uzadow
|      Added PLBmpInfo class, ability to extract metainformation from images without loading the whole image and proper greyscale support.
|      Major improvements in tests.
|      Update to VS .NET
|
|
\--------------------------------------------------------------------
*/
