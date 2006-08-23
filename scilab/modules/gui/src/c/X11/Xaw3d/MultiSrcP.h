/* $XConsortium: MultiSrcP.h,v 1.2 94/04/17 20:12:27 kaleb Exp $ */

/*
 * Copyright 1991 by OMRON Corporation
 *
 * Permission to use, copy, modify, distribute, and sell this software and its
 * documentation for any purpose is hereby granted without fee, provided that
 * the above copyright notice appear in all copies and that both that
 * copyright notice and this permission notice appear in supporting
 * documentation, and that the name OMRON not be used in
 * advertising or publicity pertaining to distribution of the software without
 * specific, written prior permission.  OMRON make no representations
 * about the suitability of this software for any purpose.  It is provided
 * "as is" without express or implied warranty.
 *
 * OMRON DISCLAIM ALL WARRANTIES WITH REGARD TO THIS SOFTWARE,
 * INCLUDING ALL IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS, IN NO
 * EVENT SHALL OMRON BE LIABLE FOR ANY SPECIAL, INDIRECT OR
 * CONSEQUENTIAL DAMAGES OR ANY DAMAGES WHATSOEVER RESULTING FROM LOSS OF USE,
 * DATA OR PROFITS, WHETHER IN AN ACTION OF CONTRACT, NEGLIGENCE OR OTHER
 * TORTUOUS ACTION, ARISING OUT OF OR IN CONNECTION WITH THE USE OR
 * PERFORMANCE OF THIS SOFTWARE.
 *
 *      Author: Li Yuhong	 OMRON Corporation
 */


/***********************************************************

Copyright (c) 1987, 1988, 1991, 1994  X Consortium

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
 * MultiSrcP.h - Private Header for Multi Text Source.
 *
 * This is the private header file for the Multi Text Source.
 * It is intended to be used with the Text widget, the simplest way to use
 * this text source is to use the MultiText Object.
 *
 * Date:    June 29, 1989
 *
 * By:      Chris D. Peterson
 *          MIT X Consortium 
 *          kit@expo.lcs.mit.edu
 */

/* 
 * TextSrcP.h - Private definitions for MultiSrc object
 * 
 */

/*
 * This file was changed from AsciiSrcP.h.
 *
 * By Li Yuhong, Sept. 18, 1990
 */

#ifndef _XawMultiSrcP_h
#define _XawMultiSrcP_h

#include <X11/Xaw3d/TextSrcP.h>
#include <X11/Xaw3d/MultiSrc.h>

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

typedef struct _MultiPiece {	/* Piece of the text file of BUFSIZ allocated 
				   characters. */
  wchar_t* text;		/* The text in this buffer. */
  XawTextPosition used;		/* The number of characters of this buffer 
				   that have been used. */
  struct _MultiPiece *prev, *next;	/* linked list pointers. */
} MultiPiece;

/************************************************************
 *
 * New fields for the MultiSrc object class record.
 *
 ************************************************************/

typedef struct _MultiSrcClassPart { char foo; } MultiSrcClassPart;

/* Full class record declaration */
typedef struct _MultiSrcClassRec {
    ObjectClassPart     object_class;
    TextSrcClassPart	text_src_class;
    MultiSrcClassPart	multi_src_class;
} MultiSrcClassRec;

extern MultiSrcClassRec multiSrcClassRec;

/* New fields for the MultiSrc object record */

typedef struct _MultiSrcPart {

  /* Resources. */

  XIC ic;			/* for X Input Method. */
  XtPointer string;		/* either the string, or the file name, depend-
                                 ing upon the `type'.  ALWAYS IN MB FORMAT. */
  XawAsciiType type;		/* either string or disk. */
  XawTextPosition piece_size;	/* Size of text buffer for each piece. */
  Boolean data_compression;	/* compress to minimum memory automatically
				   on save? */
  XtCallbackList callback;	/* A callback list to call when the source is
				   changed. */
  Boolean use_string_in_place;	/* Use the string passed in place. */
  int     multi_length;		/* length field for multi string emulation. */

/* Private data. */

  Boolean	is_tempfile;	  /* Is this a temporary file? */
  Boolean       changes;	  /* Has this file been edited? */
  Boolean       allocated_string; /* Have I allocated the
				     string in multi_src->string? */
  XawTextPosition length; 	/* length of file - IN CHARACTERS, NOT BYTES. */
  MultiPiece * first_piece;	/* first piece of the text. */
} MultiSrcPart;

/****************************************************************
 *
 * Full instance record declaration
 *
 ****************************************************************/

typedef struct _MultiSrcRec {
  ObjectPart    object;
  TextSrcPart	text_src;
  MultiSrcPart	multi_src;
} MultiSrcRec;

#if NeedFunctionPrototypes
extern void _XawMultiSourceFreeString( Widget );
#else
extern void _XawMultiSourceFreeString();
#endif

#endif /* _XawMultiSrcP_h  --- Don't add anything after this line. */
