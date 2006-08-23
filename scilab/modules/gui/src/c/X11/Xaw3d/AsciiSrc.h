/* $XConsortium: AsciiSrc.h,v 1.15 94/04/17 20:11:46 kaleb Exp $ */

/*

Copyright (c) 1989, 1994  X Consortium

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
 * AsciiSrc.h - Public Header file for Ascii Text Source.
 *
 * This is the public header file for the Ascii Text Source.
 * It is intended to be used with the Text widget, the simplest way to use
 * this text source is to use the AsciiText Object.
 *
 * Date:    June 29, 1989
 *
 * By:      Chris D. Peterson
 *          MIT X Consortium 
 *          kit@expo.lcs.mit.edu
 */


#ifndef _XawAsciiSrc_h
#define _XawAsciiSrc_h

#include <X11/Xaw3d/TextSrc.h>

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

extern WidgetClass asciiSrcObjectClass;

typedef struct _AsciiSrcClassRec *AsciiSrcObjectClass;
typedef struct _AsciiSrcRec      *AsciiSrcObject;

/*
 * Just to make people's lives a bit easier.
 */

#define AsciiSourceObjectClass AsciiSrcObjectClass
#define AsciiSourceObject      AsciiSrcObject

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

#define XtRAsciiType "AsciiType"

#define XtEstring "string"
#define XtEfile "file"

typedef enum {XawAsciiFile, XawAsciiString} XawAsciiType;

/************************************************************
 *
 * Public routines 
 *
 ************************************************************/

_XFUNCPROTOBEGIN

/*	Function Name: XawAsciiSourceFreeString
 *	Description: Frees the string returned by a get values call
 *                   on the string when the source is of type string.
 *	Arguments: w - the AsciiSrc object.
 *	Returns: none.
 */

extern void XawAsciiSourceFreeString(
#if NeedFunctionPrototypes
    Widget		/* w */
#endif
);

/*	Function Name: XawAsciiSave
 *	Description: Saves all the pieces into a file or string as required.
 *	Arguments: w - the asciiSrc Object.
 *	Returns: TRUE if the save was successful.
 */

extern Boolean XawAsciiSave(
#if NeedFunctionPrototypes
    Widget		/* w */
#endif
);

/*	Function Name: XawAsciiSaveAsFile
 *	Description: Save the current buffer as a file.
 *	Arguments: w - the asciiSrc object.
 *                 name - name of the file to save this file into.
 *	Returns: True if the save was successful.
 */

extern Boolean XawAsciiSaveAsFile(
#if NeedFunctionPrototypes
    Widget		/* w */,
    _Xconst char*	/* name */
#endif 
);

/*	Function Name: XawAsciiSourceChanged
 *	Description: Returns true if the source has changed since last saved.
 *	Arguments: w - the asciiSource object.
 *	Returns: a Boolean (see description).
 */

extern Boolean XawAsciiSourceChanged(
#if NeedFunctionPrototypes
    Widget		/* w */
#endif
);

_XFUNCPROTOEND

#endif /* _XawAsciiSrc_h */

