/*
* $XConsortium: AsciiSrcP.h,v 1.10 94/04/17 20:11:49 kaleb Exp $
*/


/***********************************************************

Copyright (c) 1987, 1988, 1994  X Consortium

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


Copyright 1987, 1988 by Digital Equipment Corporation, Maynard, Massachusetts.

                        All Rights Reserved

Permission to use, copy, modify, and distribute this software and its 
documentation for any purpose and without fee is hereby granted, 
provided that the above copyright notice appear in all copies and that
both that copyright notice and this permission notice appear in 
supporting documentation, and that the name of Digital not be
used in advertising or publicity pertaining to distribution of the
software without specific, written prior permission.  

DIGITAL DISCLAIMS ALL WARRANTIES WITH REGARD TO THIS SOFTWARE, INCLUDING
ALL IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS, IN NO EVENT SHALL
DIGITAL BE LIABLE FOR ANY SPECIAL, INDIRECT OR CONSEQUENTIAL DAMAGES OR
ANY DAMAGES WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS,
WHETHER IN AN ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION,
ARISING OUT OF OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS
SOFTWARE.

******************************************************************/

/*
 * AsciiSrcP.h - Private Header for Ascii Text Source.
 *
 * This is the private header file for the Ascii Text Source.
 * It is intended to be used with the Text widget, the simplest way to use
 * this text source is to use the AsciiText Object.
 *
 * Date:    June 29, 1989
 *
 * By:      Chris D. Peterson
 *          MIT X Consortium 
 *          kit@expo.lcs.mit.edu
 */

/* 
 * TextSrcP.h - Private definitions for AsciiSrc object
 * 
 */

#ifndef _XawAsciiSrcP_h
#define _XawAsciiSrcP_h

#include <X11/Xaw3d/TextSrcP.h>
#include <X11/Xaw3d/AsciiSrc.h>

/************************************************************
 *
 * Private declarations.
 *
 ************************************************************/

#ifdef L_tmpnam
#define TMPSIZ L_tmpnam
#else
#define TMPSIZ 32		/* bytes to allocate for tmpnam */
#endif

#define MAGIC_VALUE ((XawTextPosition) -1) /* Magic value. */

#define streq(a, b)        ( strcmp((a), (b)) == 0 )

typedef struct _Piece {		/* Piece of the text file of BUFSIZ allocated 
				   characters. */
  char * text;			/* The text in this buffer. */
  XawTextPosition used;		/* The number of characters of this buffer 
				   that have been used. */
  struct _Piece *prev, *next;	/* linked list pointers. */
} Piece;

/************************************************************
 *
 * New fields for the AsciiSrc object class record.
 *
 ************************************************************/

typedef struct _AsciiSrcClassPart { char foo; } AsciiSrcClassPart;

/* Full class record declaration */
typedef struct _AsciiSrcClassRec {
    ObjectClassPart     object_class;
    TextSrcClassPart	text_src_class;
    AsciiSrcClassPart	ascii_src_class;
} AsciiSrcClassRec;

extern AsciiSrcClassRec asciiSrcClassRec;

/* New fields for the AsciiSrc object record */

typedef struct _AsciiSrcPart {

  /* Resources. */

  char       *string;		/* either the string, or the
				   file name, depending upon the type. */
  XawAsciiType type;		/* either string or disk. */
  XawTextPosition piece_size;	/* Size of text buffer for each piece. */
  Boolean data_compression;	/* compress to minimum memory automatically
				   on save? */
  XtCallbackList callback;	/* A callback list to call when the source is
				   changed. */
  Boolean use_string_in_place;	/* Use the string passed in place. */
  int     ascii_length;		/* length field for ascii string emulation. */

#ifdef ASCII_DISK
  String filename;		/* name of file for Compatability. */
#endif /* ASCII_DISK */

/* Private data. */

  Boolean	is_tempfile;	  /* Is this a temporary file? */
  Boolean       changes;	  /* Has this file been edited? */
  Boolean       allocated_string; /* Have I allocated the
				     string in ascii_src->string? */
  XawTextPosition length; 	/* length of file */
  Piece * first_piece;		/* first piece of the text. */
} AsciiSrcPart;

/****************************************************************
 *
 * Full instance record declaration
 *
 ****************************************************************/

typedef struct _AsciiSrcRec {
  ObjectPart    object;
  TextSrcPart	text_src;
  AsciiSrcPart	ascii_src;
} AsciiSrcRec;

#endif /* _XawAsciiSrcP_h */ 
