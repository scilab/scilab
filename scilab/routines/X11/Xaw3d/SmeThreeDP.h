/*
* $KK: SmeThreeDP.h,v 0.3 92/11/04 xx:xx:xx keithley Exp $
*/

/***********************************************************
Copyright 1987, 1988 by Digital Equipment Corporation, Maynard, Massachusetts,
and the Massachusetts Institute of Technology, Cambridge, Massachusetts.
Copyright 1992 by Kaleb Keithley

                        All Rights Reserved

Permission to use, copy, modify, and distribute this software and its 
documentation for any purpose and without fee is hereby granted, 
provided that the above copyright notice appear in all copies and that
both that copyright notice and this permission notice appear in 
supporting documentation, and that the names of Digital, MIT, or Kaleb
Keithley not be used in advertising or publicity pertaining to distribution 
of the software without specific, written prior permission.  

DIGITAL DISCLAIMS ALL WARRANTIES WITH REGARD TO THIS SOFTWARE, INCLUDING
ALL IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS, IN NO EVENT SHALL
DIGITAL BE LIABLE FOR ANY SPECIAL, INDIRECT OR CONSEQUENTIAL DAMAGES OR
ANY DAMAGES WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS,
WHETHER IN AN ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION,
ARISING OUT OF OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS
SOFTWARE.

******************************************************************/

#ifndef _XawSmeThreeDP_h
#define _XawSmeThreeDP_h

#include <X11/Xaw3d/SmeThreeD.h>
#include <X11/Xaw3d/SmeP.h>

typedef struct {
    Dimension	    shadow_width;
    Pixel	    top_shadow_pixel;
    Pixel	    bot_shadow_pixel;
    Pixmap	    top_shadow_pxmap;
    Pixmap	    bot_shadow_pxmap;
    int		    top_shadow_contrast;
    int		    bot_shadow_contrast;
    GC		    top_shadow_GC;
    GC		    bot_shadow_GC;
    GC		    erase_GC;
    XtPointer	    user_data;
    Boolean	    be_nice_to_cmap;
    Boolean	    shadowed;
  } SmeThreeDPart;

/* Full instance record declaration */
typedef struct _SmeThreeDRec {
    ObjectPart	    object;
    RectObjPart	    rectangle;
    SmePart	    sme;
    SmeThreeDPart   sme_threeD;
  } SmeThreeDRec;

typedef void (*XawSme3dShadowDrawProc)(
#if NeedFunctionPrototypes
    Widget
#endif
);

typedef struct {
    XawSme3dShadowDrawProc	shadowdraw;
  } SmeThreeDClassPart;

/* Full class record declaration. */
typedef struct _SmeThreeDClassRec {
    RectObjClassPart	rect_class;
    SmeClassPart	sme_class;
    SmeThreeDClassPart	sme_threeD_class;
  } SmeThreeDClassRec;

/* Class pointer. */
extern SmeThreeDClassRec smeThreeDClassRec;

#define XtInheritXawSme3dShadowDraw ((XawSme3dShadowDrawProc) _XtInherit)

#endif /* _XawSmeThreeDP_h */
