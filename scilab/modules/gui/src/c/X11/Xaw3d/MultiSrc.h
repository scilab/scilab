/* $XConsortium: MultiSrc.h,v 1.2 94/04/17 20:12:26 kaleb Exp $ */

/*
 * Copyright 1991 by OMRON Corporation
 *
 * Permission to use, copy, modify, distribute, and sell this software and its
 * documentation for any purpose is hereby granted without fee, provided that
 * the above copyright notice appear in all copies and that both that
 * copyright notice and this permission notice appear in supporting
 * documentation, and that the name OMRON not be used in
 * advertising or publicity pertaining to distribution of the software without
 * specific, written prior permission.  OMRON makes no representations
 * about the suitability of this software for any purpose.  It is provided
 * "as is" without express or implied warranty.
 *
 * OMRON DISCLAIMS ALL WARRANTIES WITH REGARD TO THIS SOFTWARE,
 * INCLUDING ALL IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS, IN NO
 * EVENT SHALL OMRON BE LIABLE FOR ANY SPECIAL, INDIRECT OR
 * CONSEQUENTIAL DAMAGES OR ANY DAMAGES WHATSOEVER RESULTING FROM LOSS OF USE,
 * DATA OR PROFITS, WHETHER IN AN ACTION OF CONTRACT, NEGLIGENCE OR OTHER
 * TORTUOUS ACTION, ARISING OUT OF OR IN CONNECTION WITH THE USE OR
 * PERFORMANCE OF THIS SOFTWARE.
 *
 *      Author: Li Yuhong	 OMRON Corporation
 */

/*

Copyright (c) 1989, 1991, 1994  X Consortium

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

/*
 * This file was modified from AsciiSrc.h.
 *
 * By Li Yuhong, Sept. 18, 1990
 */

#ifndef _XawMultiSrc_h
#define _XawMultiSrc_h

#include <X11/Xaw3d/TextSrc.h>
/*Xfuncproto.h included by Intrinsic.h*/

/* Resources:

 Name		     Class		RepType		Default Value
 ----		     -----		-------		-------------
 callback	     Callback		Callback	(none)
 dataCompression     DataCompression	Boolean		True
 length		     Length		int		(internal)
 pieceSize	     PieceSize		int		BUFSIZ
 string		     String		String		NULL
 type		     Type		XawAsciiType	XawAsciiString
 useStringInPlace    UseStringInPlace	Boolean		False

*/
 
/* Class record constants */

extern WidgetClass multiSrcObjectClass;

typedef struct _MultiSrcClassRec *MultiSrcObjectClass;
typedef struct _MultiSrcRec      *MultiSrcObject;

/*
 * Just to make people's lives a bit easier.
 */

#define MultiSourceObjectClass MultiSrcObjectClass
#define MultiSourceObject      MultiSrcObject

/*
 * Resource Definitions.
 */

#define XtCDataCompression "DataCompression"
#define XtCPieceSize "PieceSize"
#define XtCType "Type"
#define XtCUseStringInPlace "UseStringInPlace"

#define XtNdataCompression "dataCompression"
#define XtNpieceSize "pieceSize"
#define XtNtype "type"
#define XtNuseStringInPlace "useStringInPlace"

#define XtRMultiType "MultiType"

#define XtEstring "string"
#define XtEfile "file"

/************************************************************
 *
 * THESE ROUTINES ARE NOT PUBLIC: Source should call
 *
 * the AsciiSrc API which currently forwards requests here.
 *
 * future versions (like theres going to be an R7 Xaw!) may
 *
 * eliminate this file or at least these functions entirely.
 *
 ************************************************************/

_XFUNCPROTOBEGIN


extern void XawMultiSourceFreeString(
#if NeedFunctionPrototypes
    Widget		/* w */
#endif
);

extern Boolean _XawMultiSave(
#if NeedFunctionPrototypes
    Widget		/* w */
#endif
);

extern Boolean _XawMultiSaveAsFile(
#if NeedFunctionPrototypes
    Widget		/* w */,
    _Xconst char*	/* name */
#endif 
);


_XFUNCPROTOEND

#endif /* _XawMultiSrc_h  - Don't add anything after this line. */

