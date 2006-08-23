/* $XConsortium: XawImP.h,v 1.4 95/06/06 20:50:30 kaleb Exp $ */

/*
 * Copyright 1991 by OMRON Corporation
 *
 * Permission to use, copy, modify, distribute, and sell this software and its
 * documentation for any purpose is hereby granted without fee, provided that
 * the above copyright notice appear in all copies and that both that
 * copyright notice and this permission notice appear in supporting
 * documentation, and that the name of OMRON not be used in advertising or
 * publicity pertaining to distribution of the software without specific,
 * written prior permission.  OMRON makes no representations about the
 * suitability of this software for any purpose.  It is provided "as is"
 * without express or implied warranty.
 *
 * OMRON DISCLAIMS ALL WARRANTIES WITH REGARD TO THIS SOFTWARE, INCLUDING
 * ALL IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS, IN NO EVENT SHALL
 * OMRON BE LIABLE FOR ANY SPECIAL, INDIRECT OR CONSEQUENTIAL DAMAGES OR
 * ANY DAMAGES WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS,
 * WHETHER IN AN ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTUOUS ACTION,
 * ARISING OUT OF OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS
 * SOFTWARE. 
 *
 *	Author:	Seiji Kuwari	OMRON Corporation
 *				kuwa@omron.co.jp
 *				kuwa%omron.co.jp@uunet.uu.net
 */				

/*

Copyright (c) 1994  X Consortium

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.  IN NO EVENT SHALL THE
X CONSORTIUM BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN
AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

Except as contained in this notice, the name of the X Consortium shall not be
used in advertising or otherwise to promote the sale, use or other dealings
in this Software without prior written authorization from the X Consortium.

*/

#ifndef _XawImP_h
#define _XawImP_h

#define XtNinputMethod		"inputMethod"
#define XtCInputMethod		"InputMethod"
#define XtNpreeditType		"preeditType"
#define XtCPreeditType		"PreeditType"
#define XtNopenIm		"openIm"
#define XtCOpenIm		"OpenIm"
#define XtNsharedIc		"sharedIc"
#define XtCSharedIc		"SharedIc"

#include <X11/Xaw3d/Text.h>

#define	CIICFocus	(1 << 0)
#define	CIFontSet	(1 << 1)
#define	CIFg		(1 << 2)
#define	CIBg		(1 << 3)
#define	CIBgPixmap	(1 << 4)
#define	CICursorP	(1 << 5)
#define	CILineS		(1 << 6)

typedef	struct _XawImPart
{
    XIM			xim;
    XrmResourceList	resources;
    Cardinal		num_resources;
    Boolean		open_im;
    Boolean		initialized;
    Dimension		area_height;
    String		input_method;
    String		preedit_type;
} XawImPart;

typedef struct _XawIcTablePart
{
    Widget		widget;
    XIC			xic;
    XIMStyle		input_style;
    unsigned long	flg;
    unsigned long	prev_flg;
    Boolean		ic_focused;
    XFontSet		font_set;
    Pixel		foreground;
    Pixel		background;
    Pixmap		bg_pixmap;
    XawTextPosition	cursor_position;
    unsigned long	line_spacing;
    Boolean		openic_error;
    struct _XawIcTablePart *next;
} XawIcTablePart, *XawIcTableList;

typedef	struct _XawIcPart
{
    XIMStyle		input_style;
    Boolean		shared_ic;
    XawIcTableList	shared_ic_table;
    XawIcTableList	current_ic_table;
    XawIcTableList	ic_table;
} XawIcPart;

typedef	struct _contextDataRec
{
    Widget		parent;
    Widget		ve;
} contextDataRec;

typedef	struct _contextErrDataRec
{
    Widget		widget;
    XIM			xim;
} contextErrDataRec;

void _XawImResizeVendorShell( 
#if NeedFunctionPrototypes
    Widget /* w */
#endif
);

Dimension _XawImGetShellHeight( 
#if NeedFunctionPrototypes
    Widget /* w */
#endif
);

void _XawImRealize( 
#if NeedFunctionPrototypes
    Widget /* w */
#endif
);

void _XawImInitialize( 
#if NeedFunctionPrototypes
    Widget, /* w */
    Widget  /* ext */
#endif
);

void _XawImReconnect( 
#if NeedFunctionPrototypes
    Widget  /* w */
#endif
);

void _XawImRegister( 
#if NeedFunctionPrototypes
    Widget  /* w */
#endif
);

void _XawImUnregister( 
#if NeedFunctionPrototypes
    Widget  /* w */
#endif
);

void _XawImSetValues( 
#if NeedFunctionPrototypes
    Widget,  /* w */
    ArgList, /* args */
    Cardinal /* num_args */
#endif
);

/* DON'T USE THIS FUNCTION -- it's going away in the next release */
void _XawImVASetValues( 
#if NeedVarargsPrototypes
    Widget,  /* w */
    ... 
#endif
);

void _XawImSetFocusValues( 
#if NeedFunctionPrototypes
    Widget,  /* w */
    ArgList, /* args */
    Cardinal /* num_args */
#endif
);

/* DON'T USE THIS FUNCTION -- it's going away in the next release */
void _XawImVASetFocusValues( 
#if NeedVarargsPrototypes
    Widget,  /* w */
    ... 
#endif
);

void _XawImUnsetFocus( 
#if NeedFunctionPrototypes
    Widget  /* w */
#endif
);

int  _XawImWcLookupString( 
#if NeedFunctionPrototypes
    Widget,   /* w */
    XKeyPressedEvent*, /* event */
    wchar_t*, /* buffer_return */
    int,      /* bytes_buffer */
    KeySym*,  /* keysym_return */
    Status*   /* status return */
#endif
);

int  _XawImGetImAreaHeight( 
#if NeedFunctionPrototypes
    Widget  /* w */
#endif
);

void _XawImCallVendorShellExtResize( 
#if NeedFunctionPrototypes
    Widget  /* w */
#endif
);

void _XawImDestroy( 
#if NeedFunctionPrototypes
    Widget,  /* w */
    Widget   /* ext */
#endif
);

#endif	/* _XawImP_h */
