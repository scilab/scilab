/* $XConsortium: MultiSrc.c,v 1.8 95/01/24 22:29:08 kaleb Exp $ */

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
 *      Authors: Chris Peterson MIT X Consortium
 *               Li Yuhong      OMRON Corporation
 *               Frank Sheeran  OMRON Corporation
 *
 * Much code taken from X11R3 String and Disk Sources.
 */

/*

Copyright (c) 1991, 1994  X Consortium

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
 * MultiSrc.c - MultiSrc object. (For use with the text widget).
 *
 */

#include <X11/IntrinsicP.h>
#include <X11/StringDefs.h>
#include <X11/Xfuncs.h>
#include <X11/Xaw3d/XawInit.h>
#include <X11/Xaw3d/MultiSrcP.h>
#include <X11/Xaw3d/XawImP.h>
#include <X11/Xmu/Misc.h>
#include <X11/Xmu/CharSet.h>
#include "XawI18n.h"
#include <X11/Xos.h>
#include <stdio.h>
#include <ctype.h>
#include <errno.h>

/****************************************************************
 *
 * Full class record constant
 *
 ****************************************************************/

/* Private Data */

static int magic_value = MAGIC_VALUE;

#define offset(field) XtOffsetOf(MultiSrcRec, multi_src.field)

static XtResource resources[] = {
    {XtNstring, XtCString, XtRString, sizeof (XtPointer),
       offset(string), XtRPointer, NULL},
    {XtNtype, XtCType, XtRMultiType, sizeof (XawAsciiType),
       offset(type), XtRImmediate, (XtPointer)XawAsciiString},
    /* not used. */
    {XtNdataCompression, XtCDataCompression, XtRBoolean, sizeof (Boolean),
       offset(data_compression), XtRImmediate, (XtPointer) FALSE},
    {XtNpieceSize, XtCPieceSize, XtRInt, sizeof (XawTextPosition),
       offset(piece_size), XtRImmediate, (XtPointer) BUFSIZ},
    {XtNcallback, XtCCallback, XtRCallback, sizeof(XtPointer), 
       offset(callback), XtRCallback, (XtPointer)NULL},
    {XtNuseStringInPlace, XtCUseStringInPlace, XtRBoolean, sizeof (Boolean),
       offset(use_string_in_place), XtRImmediate, (XtPointer) FALSE},
    {XtNlength, XtCLength, XtRInt, sizeof (int),
       offset(multi_length), XtRInt, (XtPointer) &magic_value},

};
#undef offset

static XawTextPosition Scan(), Search(), ReadText();
static int ReplaceText();
static MultiPiece * FindPiece(), * AllocNewPiece();
static FILE * InitStringOrFile();
static void FreeAllPieces(), RemovePiece(), BreakPiece(), LoadPieces();
static void RemoveOldStringOrFile(),  CvtStringToMultiType();
static void ClassInitialize(), Initialize(), Destroy(), GetValuesHook();
static String StorePiecesInString();
static Boolean SetValues(), WriteToFile();

#define MyWStrncpy( t,s,wcnt ) (void) memmove( (t), (s), (wcnt)*sizeof(wchar_t))

#ifndef MyWStrncpy
static void (MyWStrncpy)();
#endif

extern char *tmpnam();
#ifdef X_NOT_STDC_ENV
extern int errno; 
#endif

#ifdef X_NOT_POSIX
#define Off_t long
#define Size_t unsigned int
#else
#define Off_t off_t
#define Size_t size_t
#endif

extern wchar_t* _XawTextMBToWC();
extern char *_XawTextWCToMB();

#define superclass		(&textSrcClassRec)
MultiSrcClassRec multiSrcClassRec = {
  { /* object_class fields */	
    /* superclass	  	*/	(WidgetClass) superclass,
    /* class_name	  	*/	"MultiSrc",
    /* widget_size	  	*/	sizeof(MultiSrcRec),
    /* class_initialize   	*/	ClassInitialize,
    /* class_part_initialize	*/	NULL,
    /* class_inited       	*/	FALSE,
    /* initialize	  	*/	Initialize,
    /* initialize_hook		*/	NULL,
    /* pad		  	*/	NULL,
    /* pad		  	*/	NULL,
    /* pad		  	*/	0,
    /* resources	  	*/	resources,
    /* num_resources	  	*/	XtNumber(resources),
    /* xrm_class	  	*/	NULLQUARK,
    /* pad		  	*/	FALSE,
    /* pad		  	*/	FALSE,
    /* pad			*/	FALSE,
    /* pad		  	*/	FALSE,
    /* destroy		  	*/	Destroy,
    /* pad		  	*/	NULL,
    /* pad		  	*/	NULL,
    /* set_values	  	*/	SetValues,
    /* set_values_hook		*/	NULL,
    /* pad			*/	NULL,
    /* get_values_hook		*/	GetValuesHook,
    /* pad		 	*/	NULL,
    /* version			*/	XtVersion,
    /* callback_private   	*/	NULL,
    /* pad		   	*/	NULL,
    /* pad			*/	NULL,
    /* pad			*/	NULL,
    /* extension		*/	NULL
  },
  { /* textSrc_class fields */
    /* Read                     */      ReadText,
    /* Replace                  */      ReplaceText,
    /* Scan                     */      Scan,
    /* Search                   */      Search,
    /* SetSelection             */      XtInheritSetSelection,
    /* ConvertSelection         */      XtInheritConvertSelection
  },
  { /* multiSrc_class fields */
    /* Keep the compiler happy */       '\0'
  }
};

WidgetClass multiSrcObjectClass = (WidgetClass)&multiSrcClassRec;

/************************************************************
 *
 * Semi-Public Interfaces.
 *
 ************************************************************/

/*      Function Name: ClassInitialize
 *      Description: Class Initialize routine, called only once.
 *      Arguments: none.
 *      Returns: none.
 */

static void
ClassInitialize()
{
  XawInitializeWidgetSet();
  XtAddConverter( XtRString, XtRMultiType, CvtStringToMultiType,
		 NULL, (Cardinal) 0);
}

/*      Function Name: Initialize
 *      Description: Initializes the simple menu widget
 *      Arguments: request - the widget requested by the argument list.
 *                 new     - the new widget with both resource and non
 *                           resource values.
 *      Returns: none.
 */

/* ARGSUSED */
static void
Initialize(request, new, args, num_args)
    Widget request, new;
    ArgList args;
    Cardinal* num_args;
{
  MultiSrcObject src = (MultiSrcObject) new;
  FILE * file;

/*
 * Set correct flags (override resources) depending upon widget class.
 */

  src->multi_src.changes = FALSE;
  src->multi_src.allocated_string = FALSE;

  file = InitStringOrFile(src, src->multi_src.type == XawAsciiFile);
  LoadPieces(src, file, NULL);

  if (file != NULL) fclose(file);
  src->text_src.text_format = XawFmtWide;

}

/*	Function Name: ReadText
 *	Description: This function reads the source.
 *	Arguments: w - the MultiSource widget.
 *                 pos - position of the text to retreive.
 * RETURNED        text - text block that will contain returned text.
 *                length - maximum number of characters to read.
 *	Returns: The number of characters read into the buffer.
 */

static XawTextPosition
ReadText(w, pos, text, length)
    Widget w;
    XawTextPosition pos;
    XawTextBlock* text;	
    int length;		
{
  MultiSrcObject src = (MultiSrcObject) w;
  XawTextPosition count, start;
  MultiPiece * piece = FindPiece(src, pos, &start);
    
  text->format = XawFmtWide;
  text->firstPos = pos;
  text->ptr = (char *)(piece->text + (pos - start));
  count = piece->used - (pos - start);
  text->length = (length > count) ? count : length;
  return(pos + text->length);
}

/*	Function Name: ReplaceText.
 *	Description: Replaces a block of text with new text.
 *	Arguments: w - the MultiSource widget.
 *                 startPos, endPos - ends of text that will be removed.
 *                 text - new text to be inserted into buffer at startPos.
 *	Returns: XawEditError or XawEditDone.
 */

/*ARGSUSED*/
static int 
ReplaceText( w, startPos, endPos, u_text_p)
    Widget w;
    XawTextPosition startPos, endPos;
    XawTextBlock* u_text_p;
{
  MultiSrcObject src = (MultiSrcObject) w;
  MultiPiece *start_piece, *end_piece, *temp_piece;
  XawTextPosition start_first, end_first;
  int length, firstPos;
  wchar_t *wptr;
  Boolean local_artificial_block = False;
  XawTextBlock text;

  /* STEP 1: The user handed me a text block called `u_text' that may be 
   * in either FMTWIDE or FMT8BIT (ie MB.)  Later code needs the block 
   * `text' to hold FMTWIDE.  So, this copies `u_text' to `text', and if 
   * `u_text' was MB, I knock it up to WIDE. */

  if ( u_text_p->length == 0 )	/* if so, the block contents never ref'd. */
      text.length = 0;

  else if ( u_text_p->format == XawFmtWide) {
      local_artificial_block = False;		/* ie, don't have to free it ourselves*/
      text.firstPos = u_text_p->firstPos;
      text.length =   u_text_p->length;
      text.ptr =      u_text_p->ptr;
      /* text.format is unneeded */

  } else {
      /* WARNING! u_text->firstPos and length are in units of CHAR, not CHARACTERS! */

      local_artificial_block = True;	/* ie, have to free it ourselves */
      text.firstPos = 0;
      text.length = u_text_p->length; /* _XawTextMBToWC converts this to wchar len. */

      text.ptr = (char*)_XawTextMBToWC( XtDisplay(XtParent(w)),
			 &(u_text_p->ptr[u_text_p->firstPos]), &(text.length) );

      /* I assert the following assignment is not needed - since Step 4
      depends on length, it has no need of a terminating NULL.  I think
      the ASCII-version has the same needless NULL. */
      /*((wchar_t*)text.ptr)[ text.length ] = NULL;*/
  }


  /* STEP 2: some initialization... */

  if (src->text_src.edit_mode == XawtextRead) 
    return(XawEditError);

  start_piece = FindPiece(src, startPos, &start_first);
  end_piece = FindPiece(src, endPos, &end_first);


  /* STEP 3: remove the empty pieces... */

  if (start_piece != end_piece) {
    temp_piece = start_piece->next;

  /* If empty and not the only piece then remove it. */

    if ( ((start_piece->used = startPos - start_first) == 0) &&
	 !((start_piece->next == NULL) && (start_piece->prev == NULL)) )
      RemovePiece(src, start_piece);

    while (temp_piece != end_piece) {
      temp_piece = temp_piece->next;
      RemovePiece(src, temp_piece->prev);
    }
    end_piece->used -= endPos - end_first;
    if (end_piece->used != 0)
      MyWStrncpy(end_piece->text, (end_piece->text + endPos - end_first),
		(int) end_piece->used);
  }
  else {			/* We are fully in one piece. */
    if ( (start_piece->used -= endPos - startPos) == 0) {
      if ( !((start_piece->next == NULL) && (start_piece->prev == NULL)) )
	RemovePiece(src, start_piece);
    }
    else {
      MyWStrncpy(start_piece->text + (startPos - start_first),
		start_piece->text + (endPos - start_first),
		(int) (start_piece->used - (startPos - start_first)) );
      if ( src->multi_src.use_string_in_place && 
	   ((src->multi_src.length - (endPos - startPos)) < 
	    (src->multi_src.piece_size - 1)) ) 
	start_piece->text[src->multi_src.length - (endPos - startPos)] = (wchar_t)0;
    }
  }

  src->multi_src.length += text.length -(endPos - startPos);
  /*((TextWidget)src->object.parent)->text.lastPos = src->multi_src.length;*/



  /* STEP 4: insert the new stuff */

  if ( text.length != 0) {

    start_piece = FindPiece(src, startPos, &start_first);
    
    length = text.length;
    firstPos = text.firstPos;
    
    while (length > 0) {
      wchar_t* ptr;
      int fill;
      
      if (src->multi_src.use_string_in_place) {
	if (start_piece->used == (src->multi_src.piece_size - 1)) {
          /*
           * The string is used in place, then the string
           * is not allowed to grow.
           */
          start_piece->used = src->multi_src.length =
                                                 src->multi_src.piece_size - 1;
          /*((TextWidget)src->object.parent)->text.lastPos = src->multi_src.length;*/


          start_piece->text[src->multi_src.length] = (wchar_t)0;
	  return(XawEditError);
	}
      }


      if (start_piece->used == src->multi_src.piece_size) {
	BreakPiece(src, start_piece);
	start_piece = FindPiece(src, startPos, &start_first);
      }

      fill = Min((int)(src->multi_src.piece_size - start_piece->used), length);
      
      ptr = start_piece->text + (startPos - start_first);
      MyWStrncpy(ptr + fill, ptr, 
		(int) start_piece->used - (startPos - start_first));
      wptr =(wchar_t *)text.ptr;
      (void)wcsncpy(ptr, wptr + firstPos, fill);
      
      startPos += fill;
      firstPos += fill;
      start_piece->used += fill;
      length -= fill;
    }
  }

  if ( local_artificial_block == True )

      /* In other words, text is not the u_text that the user handed me but
      one I made myself.  I only care, because I need to free the string. */

      XFree( text.ptr );

  if (src->multi_src.use_string_in_place)
    start_piece->text[start_piece->used] = (wchar_t)0;

  src->multi_src.changes = TRUE;

  XtCallCallbacks(w, XtNcallback, NULL);

  return(XawEditDone);
}

/*	Function Name: Scan
 *	Description: Scans the text source for the number and type
 *                   of item specified.
 *	Arguments: w - the MultiSource widget.
 *                 position - the position to start scanning.
 *                 type - type of thing to scan for.
 *                 dir - direction to scan.
 *                 count - which occurance if this thing to search for.
 *                 include - whether or not to include the character found in
 *                           the position that is returned. 
 *	Returns: the position of the item found.
 *
 * Note: While there are only 'n' characters in the file there are n+1 
 *       possible cursor positions (one before the first character and
 *       one after the last character.
 */

static 
XawTextPosition 
Scan( w, position, type, dir, count, include )
    Widget w;
    XawTextPosition position;
    XawTextScanType type;
    XawTextScanDirection dir;
    int count;
    Boolean include;
{
  MultiSrcObject src = (MultiSrcObject) w;
  int inc;
  MultiPiece * piece;
  XawTextPosition first, first_eol_position = 0;
  wchar_t * ptr;

  if (type == XawstAll) {	/* Optimize this common case. */
    if (dir == XawsdRight)
      return(src->multi_src.length);
    return(0);			/* else. */
  }


  /* STEP 1: basic sanity checks */

  if (position > src->multi_src.length)
    position = src->multi_src.length;


  if ( dir == XawsdRight ) {
    if (position == src->multi_src.length)
      return(src->multi_src.length);
    inc = 1;
  }
  else {
    if (position == 0)
      return(0);
    inc = -1;
    position--;
  }

  piece = FindPiece(src, position, &first);

  if ( piece->used == 0 ) return(0); /* i.e., buffer is empty. */

  ptr = (position - first) + piece->text;

  switch (type) {
  case XawstEOL: 
  case XawstParagraph: 
  case XawstWhiteSpace: 
    for ( ; count > 0 ; count-- ) {
      Boolean non_space = FALSE, first_eol = TRUE;
      /* CONSTCOND */
      while (TRUE) {
        wchar_t c = *ptr;

	ptr += inc;
	position += inc;
	
	if (type == XawstWhiteSpace) {
	  if (iswspace(c)) {
	    if (non_space) 
	      break;
	  }
	  else
	    non_space = TRUE;
	}
	else if (type == XawstEOL) {
          if (c == _Xaw_atowc(XawLF)) break;
	}
	else { /* XawstParagraph */
	  if (first_eol) {
            if (c == _Xaw_atowc(XawLF)) {
	      first_eol_position = position;
	      first_eol = FALSE;
	    }
	  }
	  else
            if ( c == _Xaw_atowc(XawLF))
              break;
            else if ( !iswspace(c) )
	      first_eol = TRUE;
	}
	      

	if ( ptr < piece->text ) {
	  piece = piece->prev;
	  if (piece == NULL)	/* Begining of text. */
	    return(0);
	  ptr = piece->text + piece->used - 1;
	}
	else if ( ptr >= (piece->text + piece->used) ) {
	  piece = piece->next;
	  if (piece == NULL)	/* End of text. */
	    return(src->multi_src.length);
	  ptr = piece->text;
	}
      }
    }
    if (!include) {
      if ( type == XawstParagraph)
	position = first_eol_position;
      position -= inc;
    }
    break;
  case XawstPositions: 
    position += count * inc;
    break;
  case XawstAll:		/* handled in special code above */
  default:
    break;
  }

  if ( dir == XawsdLeft )
    position++;

  if (position >= src->multi_src.length)
    return(src->multi_src.length);
  if (position < 0)
    return(0);

  return(position);
}

/*	Function Name: Search
 *	Description: Searchs the text source for the text block passed
 *	Arguments: w - the MultiSource Widget.
 *                 position - the position to start scanning.
 *                 dir - direction to scan.
 *                 text - the text block to search for.
 *	Returns: the position of the item found.
 */

static XawTextPosition 
Search(w, position, dir, text )
    Widget w;
    XawTextPosition position;
    XawTextScanDirection dir;
    XawTextBlock* text;
{
  MultiSrcObject src = (MultiSrcObject) w;
  int inc, count = 0;
  wchar_t * ptr;
  wchar_t* wtarget;
  int wtarget_len;
  Display * d = XtDisplay(XtParent(w));
  MultiPiece * piece;
  wchar_t* buf;
  XawTextPosition first;


  /* STEP 1: First, a brief sanity check. */

  if ( dir == XawsdRight )
    inc = 1;
  else {
    inc = -1;
    if (position == 0)
      return(XawTextSearchError);	/* scanning left from 0??? */
    position--;
  }


  /* STEP 2: Ensure I have a local wide string.. */

  /* Since this widget stores 32bit chars, I check here to see if
  I'm being passed a string claiming to be 8bit chars (ie, MB text.)
  If that is the case, naturally I convert to 32bit format. */

  /*if the block was FMT8BIT, length will convert to REAL wchar count below */
  wtarget_len = text->length;

  if ( text->format == XawFmtWide )
      wtarget = &( ((wchar_t*)text->ptr) [text->firstPos] );
  else
  {
      /* The following converts wtarget_len from byte len to wchar count */
      wtarget = _XawTextMBToWC( d, &text->ptr[ text->firstPos ], &wtarget_len );
  }

  /* OK, I can now assert that wtarget holds wide characters, wtarget_len
  holds an accurate count of those characters, and that firstPos has been
  effectively factored out of the following computations. */


  /* STEP 3: SEARCH! */

  buf = (wchar_t *)XtMalloc((unsigned)sizeof(wchar_t) * wtarget_len );
  (void)wcsncpy(buf, wtarget, wtarget_len );
  piece = FindPiece(src, position, &first);
  ptr = (position - first) + piece->text;

  /* CONSTCOND */
  while (TRUE) {
    if (*ptr == ((dir == XawsdRight) ? *(buf + count) 
		                     : *(buf + wtarget_len - count - 1)) ) {
      if (count == (text->length - 1))
	break;
      else
	count++;
    }
    else {
      if (count != 0) {
	position -=inc * count;
	ptr -= inc * count;
      }
      count = 0;
    }

    ptr += inc;
    position += inc;
    
    while ( ptr < piece->text ) {
      piece = piece->prev;
      if (piece == NULL) {	/* Begining of text. */
	XtFree((char *)buf);
	return(XawTextSearchError);
      }
      ptr = piece->text + piece->used - 1;
    }
   
    while ( ptr >= (piece->text + piece->used) ) {
      piece = piece->next;
      if (piece == NULL) {	/* End of text. */
	XtFree((char *)buf);
	return(XawTextSearchError);
      }
      ptr = piece->text;
    }
  }

  XtFree( (char *) buf );
  if (dir == XawsdLeft)
    return( position );
  return( position - ( wtarget_len - 1 ) );
}

/*	Function Name: SetValues
 *	Description: Sets the values for the MultiSource.
 *	Arguments: current - current state of the widget.
 *                 request - what was requested.
 *                 new - what the widget will become.
 *	Returns: True if redisplay is needed.
 */

/* ARGSUSED */
static Boolean
SetValues(current, request, new, args, num_args)
    Widget current, request, new;
    ArgList args;
    Cardinal* num_args;
{
  MultiSrcObject src =      (MultiSrcObject) new;
  MultiSrcObject old_src = (MultiSrcObject) current;
  XtAppContext app_con = XtWidgetToApplicationContext(new);
  Boolean total_reset = FALSE, string_set = FALSE;
  FILE * file;
  int i;

  if ( old_src->multi_src.use_string_in_place != 
       src->multi_src.use_string_in_place ) {
      XtAppWarning( app_con,
	   "MultiSrc: The XtNuseStringInPlace resources may not be changed.");
       src->multi_src.use_string_in_place = 
	   old_src->multi_src.use_string_in_place;
  }

  for (i = 0; i < *num_args ; i++ ) 
      if (streq(args[i].name, XtNstring)) {
	  string_set = TRUE;
	  break;
      }
  
  if ( string_set || (old_src->multi_src.type != src->multi_src.type) ) {
    RemoveOldStringOrFile(old_src, string_set);
    file = InitStringOrFile(src, string_set);

    /* Load pieces does this logic for us, but it shouldn't.  Its messy.*/
    /*if (old_src->multi_src.type == XawAsciiString)
        LoadPieces(src, NULL, src->multi_src.string);
    else*/
        LoadPieces(src, file, NULL);
    if (file != NULL) fclose(file);
    XawTextSetSource( XtParent(new), new, 0);   /* Tell text widget
						   what happened. */
    total_reset = TRUE;
  }

  if ( old_src->multi_src.multi_length != src->multi_src.multi_length ) 
      src->multi_src.piece_size = src->multi_src.multi_length;

  if ( !total_reset && (old_src->multi_src.piece_size
      					!= src->multi_src.piece_size) ) {
      String mb_string = StorePiecesInString( old_src );

      if ( mb_string != 0 ) {
          FreeAllPieces( old_src );
          LoadPieces( src, NULL, mb_string );
          XtFree( mb_string );
      } else {
          /* If the buffer holds bad chars, don't touch it... */
          XtAppWarningMsg( app_con,
		"convertError", "multiSource", "XawError",
                 XtName( XtParent( (Widget) old_src ) ), NULL, NULL );
          XtAppWarningMsg( app_con,
		"convertError", "multiSource", "XawError",
                 "Non-character code(s) in buffer.", NULL, NULL );
      }
  }

  return(FALSE);
}

/*	Function Name: GetValuesHook
 *	Description: This is a get values hook routine that sets the
 *                   values specific to the multi source.
 *	Arguments: w - the MultiSource Widget.
 *                 args - the argument list.
 *                 num_args - the number of args.
 *	Returns: none.
 */

static void
GetValuesHook(w, args, num_args)
    Widget w;
    ArgList args;
    Cardinal* num_args;
{
  MultiSrcObject src = (MultiSrcObject) w;
  int i;

  if (src->multi_src.type == XawAsciiString) {
    for (i = 0; i < *num_args ; i++ ) 
      if (streq(args[i].name, XtNstring)) {
	  if (src->multi_src.use_string_in_place) {
              *((char **) args[i].value) = (char *)
					src->multi_src.first_piece->text;
	  }
	  else {
	      if (_XawMultiSave(w))	/* If save sucessful. */
		  *((char **) args[i].value) = src->multi_src.string;
	  }
	break;
      }
  }
}    

/*	Function Name: Destroy
 *	Description: Destroys an multi source (frees all data)
 *	Arguments: src - the Multi source Widget to free.
 *	Returns: none.
 */

static void 
Destroy (w)
    Widget w;
{
    RemoveOldStringOrFile((MultiSrcObject) w, True);
}

/************************************************************
 *
 * Public routines 
 *
 ************************************************************/

/*	Function Name: XawMultiSourceFreeString
 *	Description: Frees the string returned by a get values call
 *                   on the string when the source is of type string.
 *	Arguments: w - the MultiSrc widget.
 *	Returns: none.
 *
 * The public interface is XawAsciiSourceFreeString!
 */

void
#if NeedFunctionPrototypes
_XawMultiSourceFreeString(
    Widget w)
#else
_XawMultiSourceFreeString(w)
    Widget w;
#endif
{
  MultiSrcObject src = (MultiSrcObject) w;

/*if (src->multi_src.allocated_string&& src->multi_src.type != XawAsciiFile) {*/
  /* ASSERT: src->multi_src.allocated_string -> we MUST free .string! */
  if ( src->multi_src.allocated_string ) {
    XtFree(src->multi_src.string);
    src->multi_src.allocated_string = FALSE;
    src->multi_src.string = NULL;
  }
}

/*	Function Name: _XawMultiSave
 *	Description: Saves all the pieces into a file or string as required.
 *	Arguments: w - the multiSrc Widget.
 *	Returns: TRUE if the save was successful.
 *
 * The public interface is XawAsciiSave(w)!
 */

Boolean
#if NeedFunctionPrototypes
_XawMultiSave(
    Widget w)
#else
_XawMultiSave(w)
    Widget w;
#endif
{
  MultiSrcObject src = (MultiSrcObject) w;
  XtAppContext app_con = XtWidgetToApplicationContext(w);
  char * mb_string;

/*
 * If using the string in place then there is no need to play games
 * to get the internal info into a readable string.
 */

  if (src->multi_src.use_string_in_place) 
    return(TRUE);

  if (src->multi_src.type == XawAsciiFile) {

      if (!src->multi_src.changes) 		/* No changes to save. */
          return(TRUE);

      mb_string = StorePiecesInString( src );

      if ( mb_string != 0 ) {
          if ( WriteToFile( mb_string, src->multi_src.string ) == FALSE ) {
              XtFree( mb_string );
              return( FALSE );
          }
          XtFree( mb_string );
          src->multi_src.changes = FALSE;
          return( TRUE );
      } else {
          /* If the buffer holds bad chars, don't touch it... */
          XtAppWarningMsg( app_con,
		"convertError", "multiSource", "XawError",
                "Due to illegal characters, file not saved.", NULL, NULL);
          return( FALSE );
      }
  }
  else {

  /* THIS FUNCTIONALITY IS UNDOCUMENTED, probably UNNEEDED?  The manual
  says this routine's only function is to save files to disk.  -Sheeran */

      mb_string = StorePiecesInString( src );

      if ( mb_string == 0 ) {
          /* If the buffer holds bad chars, don't touch it... */
          XtAppWarningMsg( app_con,
		"convertError", "multiSource", "XawError",
                 XtName( XtParent( (Widget) src ) ), NULL, NULL);
          return( FALSE );
      }

      /* assert: mb_string holds good characters so the buffer is fine */
      if (src->multi_src.allocated_string == TRUE) 
          XtFree(src->multi_src.string);
      else
          src->multi_src.allocated_string = TRUE;
    
      src->multi_src.string = mb_string;
  }
  src->multi_src.changes = FALSE;
  return(TRUE);
}

/*	Function Name: XawMultiSaveAsFile
 *	Description: Save the current buffer as a file.
 *	Arguments: w - the MultiSrc widget.
 *                 name - name of the file to save this file into.
 *	Returns: True if the save was sucessful.
 *
 * The public interface is XawAsciiSaveAsFile!
 */

Boolean
#if NeedFunctionPrototypes
_XawMultiSaveAsFile(
    Widget w, 
    _Xconst char* name)
#else
_XawMultiSaveAsFile(w, name)
    Widget w;
    String name;
#endif
{
  MultiSrcObject src = (MultiSrcObject) w;
  String mb_string;
  Boolean ret;

  mb_string = StorePiecesInString( src ); 

  if ( mb_string != 0 ) {
      ret = WriteToFile( mb_string, name );
      XtFree( mb_string );
      return( ret );
  }

  /* otherwise there was a conversion error.  So print widget name too. */
  XtAppWarningMsg( XtWidgetToApplicationContext(w),
		"convertError", "multiSource", "XawError",
		XtName( XtParent( (Widget) src ) ), NULL, NULL);
  return( False );
}

/************************************************************
 *
 * Private Functions.
 *
 ************************************************************/

static void
RemoveOldStringOrFile(src, checkString) 
    MultiSrcObject src;
    Boolean checkString;
{
  FreeAllPieces(src);

  if (checkString && src->multi_src.allocated_string) {
    XtFree(src->multi_src.string);
    src->multi_src.allocated_string = False;
    src->multi_src.string = NULL;
  }
}

/*	Function Name: WriteToFile
 *	Description: Write the string specified to the begining of the file
 *                   specified.
 *	Arguments: string - string to write.
 *                 name - the name of the file
 *	Returns: returns TRUE if sucessful, FALSE otherwise.
 */

static Boolean
WriteToFile(string, name)
    String string, name;
{
  int fd;
  
  if ( ((fd = creat(name, 0666)) == -1 ) ||
       (write(fd, string, sizeof(unsigned char) * strlen(string)) == -1) )
    return(FALSE);

  if ( close(fd) == -1 ) 
    return(FALSE);

  return(TRUE);
}


/*	Function Name: StorePiecesInString
 *	Description:   store the pieces in memory into a char string.
 *	Arguments:     src - the multiSrc to gather data from
 *	Returns:       char *mb_string.     Caller must free.
 *                  or 0: conversion error. Caller must panic!
 */

static String
StorePiecesInString(src)
    MultiSrcObject src;
{
  wchar_t* wc_string;
  char *mb_string;
  int char_count = src->multi_src.length;
  XawTextPosition first;
  MultiPiece * piece;

  /* I believe the char_count + 1 and the NULL termination are unneeded! FS*/

  wc_string = (wchar_t*) XtMalloc((unsigned)(char_count + 1) * sizeof(wchar_t));

  for (first = 0, piece = src->multi_src.first_piece ; piece != NULL;
      				first += piece->used, piece = piece->next)
      (void) wcsncpy( wc_string + first, piece->text, piece->used );

  wc_string[ char_count ] = (wchar_t)0; /* NULL terminate this sucker. */


  /* This will refill all pieces to capacity. */

  if ( src->multi_src.data_compression ) {
    FreeAllPieces( src );
    LoadPieces( src, NULL, (char *)wc_string );
  }

  /* Lastly, convert it to a MB format and send it back. */

  mb_string = _XawTextWCToMB( XtDisplayOfObject( (Widget)src ),
						wc_string, &char_count );

  /* NOTE THAT mb_string MAY BE ZERO IF THE CONVERSION FAILED. */
  XtFree( (char*) wc_string );
  return( mb_string );
}


/*	Function Name: InitStringOrFile.
 *	Description: Initializes the string or file.
 *	Arguments: src - the MultiSource.
 *	Returns: none - May exit though.
 */

static FILE *
InitStringOrFile(src, newString)
    MultiSrcObject src;
    Boolean newString;
{
    char * open_mode = NULL;
    FILE * file;
    char fileName[TMPSIZ];
    Display *d = XtDisplayOfObject((Widget)src);

    if (src->multi_src.type == XawAsciiString) {

	if (src->multi_src.string == NULL)
	    src->multi_src.length = 0;

	else if (! src->multi_src.use_string_in_place) {
	    int length;
            String temp = XtNewString(src->multi_src.string);
            if ( src->multi_src.allocated_string )
                XtFree( src->multi_src.string );
            src->multi_src.allocated_string = True;
	    src->multi_src.string = temp;

	    length = strlen(src->multi_src.string);

	    /* Wasteful, throwing away the WC string, but need side effect! */
            (void) _XawTextMBToWC(d, src->multi_src.string, &length);
	    src->multi_src.length = (XawTextPosition) length;
	} else {
	    src->multi_src.length = strlen(src->multi_src.string);
	    /* In case the length resource is incorrectly set */
	    if (src->multi_src.length > src->multi_src.multi_length)
		src->multi_src.multi_length = src->multi_src.length;

	    if (src->multi_src.multi_length == MAGIC_VALUE) 
		src->multi_src.piece_size = src->multi_src.length;
	    else
		src->multi_src.piece_size = src->multi_src.multi_length + 1;
	}
		
       /*((TextWidget)src->object.parent)->text.lastPos = src->multi_src.length;*/
	return(NULL);
    }

/*
 * type is XawAsciiFile.
 */
    
    src->multi_src.is_tempfile = FALSE;

    switch (src->text_src.edit_mode) {
    case XawtextRead:
	if (src->multi_src.string == NULL)
	    XtErrorMsg("NoFile", "multiSourceCreate", "XawError",
		     "Creating a read only disk widget and no file specified.",
		       NULL, 0);
	open_mode = "r";
	break;
    case XawtextAppend:
    case XawtextEdit:
	if (src->multi_src.string == NULL) {

            if ( src->multi_src.allocated_string )
                XtFree( src->multi_src.string );
            src->multi_src.allocated_string = False;
	    src->multi_src.string = fileName;

	    (void) tmpnam(src->multi_src.string);
	    src->multi_src.is_tempfile = TRUE;
	    open_mode = "w";
	} else
	    open_mode = "r+";
	break;
    default:
	XtErrorMsg("badMode", "multiSourceCreate", "XawError",
		"Bad editMode for multi source; must be Read, Append or Edit.",
		   NULL, NULL);
    }

    /* Allocate new memory for the temp filename, because it is held in
     * a stack memory buffer.  We must verify that all routines that set
     * .string first check .allocated_string and free it - plumbing Sheeran.
     */
    if (newString || src->multi_src.is_tempfile) {
	if ( src->multi_src.allocated_string )
	    src->multi_src.string = XtNewString(src->multi_src.string);
	src->multi_src.allocated_string = TRUE;
    }
    
    if (!src->multi_src.is_tempfile) {
	if ((file = fopen(src->multi_src.string, open_mode)) != 0) {
	    (void) fseek(file, (Off_t)0, 2);
            src->multi_src.length = ftell (file);
	    return file;
	} else {
	    String params[2];
	    Cardinal num_params = 2;
	    
	    params[0] = src->multi_src.string;
	    params[1] = strerror(errno);
	    XtAppWarningMsg(XtWidgetToApplicationContext((Widget)src),
			    "openError", "multiSourceCreate", "XawWarning",
			    "Cannot open file %s; %s", params, &num_params);
	}
    } 
    src->multi_src.length = 0;
    return((FILE *)NULL);
#undef StrLen
}

/* LoadPieces:  This routine takes either the MB contents of open file `file' or the
MB contents of string or the MB contents of src->multi_src.string and places
them in Pieces in WC format.
 
CAUTION: You must have src->multi_src.length set to file length bytes 
when src->multi_src.type == XawAsciiFile.  src->multi_src.length must be
the length of the parameter string if string is non-NULL.		*/

static void
LoadPieces(src, file, string)
    MultiSrcObject src;
    FILE* file;
    char* string;
{
  Display *d = XtDisplayOfObject((Widget)src);
  wchar_t* local_str, *ptr;
  MultiPiece* piece = NULL;
  XawTextPosition left;
  int bytes = sizeof(wchar_t);
  char* temp_mb_holder = NULL;

  /* 
   * This is tricky - the _XawTextMBtoWC converter uses its 3rd arg 
   * in as MB length, out as WC length.  We want local_length to be 
   * WC count. 
   */
  int local_length = src->multi_src.length;

  if (string != NULL) {
    /* 
     * ASSERT: IF our caller passed a non-null string, THEN 
     * src->multi_src.length is currently string's * byte count, 
     * AND string is in a MB format.
     */
    local_str = _XawTextMBToWC(d, (char *)string, &local_length);
    src->multi_src.length = (XawTextPosition) local_length;
  } else if (src->multi_src.type != XawAsciiFile) {
    /* 
     * here, we are not changing the contents, just reloading, 
     * so don't change len...
     */
    local_length = src->multi_src.string ? 
		   strlen( src->multi_src.string ) : 0;
    local_str = _XawTextMBToWC( d, (char*)src->multi_src.string, &local_length );
  } else {
    if (src->multi_src.length != 0) {
      temp_mb_holder = 
	XtMalloc((unsigned)(src->multi_src.length + 1) * sizeof(unsigned char));
      fseek(file, (Off_t)0, 0);
      src->multi_src.length = fread (temp_mb_holder, 
				     (Size_t)sizeof(unsigned char), 
				     (Size_t)src->multi_src.length, file);
      if (src->multi_src.length <= 0) 
	XtAppErrorMsg( XtWidgetToApplicationContext ((Widget) src),
		       "readError", "multiSource", "XawError",
		       "fread returned error.", NULL, NULL);
      local_length = src->multi_src.length;
      local_str = _XawTextMBToWC(d, temp_mb_holder, &local_length);
      src->multi_src.length = local_length;

      if ( local_str == 0 ) {
	String params[2];
	Cardinal num_params;
	static char err_text[] = 
		"<<< FILE CONTENTS NOT REPRESENTABLE IN THIS LOCALE >>>";

	params[0] = XtName(XtParent((Widget)src));
	params[1] = src->multi_src.string;
	num_params = 2;

	XtAppWarningMsg( XtWidgetToApplicationContext((Widget)src),
			 "readLocaleError", "multiSource", "XawError",
    "%s: The file `%s' contains characters not representable in this locale.",
			 params, &num_params);
	src->multi_src.length = sizeof err_text;
	local_length = src->multi_src.length;
        local_str = _XawTextMBToWC(d, err_text, &local_length);
	src->multi_src.length = local_length;
      }
    } else { /*ASSERT that since following while loop looks at local_length
        this isn't needed.  Sheeran, Omron KK, 1993/07/15
        temp_mb_holder[src->multi_src.length] = '\0';*/
        local_str = (wchar_t*)temp_mb_holder;
    }
  }

  if (src->multi_src.use_string_in_place) {
      piece = AllocNewPiece(src, piece);
      piece->used = Min(src->multi_src.length, src->multi_src.piece_size);
      piece->text = (wchar_t*)src->multi_src.string;
      return;
  }

  ptr = local_str;
  left = local_length;

  do {
      piece = AllocNewPiece(src, piece);

      piece->text = (wchar_t*)XtMalloc(src->multi_src.piece_size * bytes);
      piece->used = Min(left, src->multi_src.piece_size);
      if (piece->used != 0)
          (void) wcsncpy(piece->text, ptr, piece->used);

      left -= piece->used;
      ptr += piece->used;
  } while (left > 0);

  if ( temp_mb_holder )
      XtFree( (char*) temp_mb_holder );
}


/*	Function Name: AllocNewPiece
 *	Description: Allocates a new piece of memory.
 *	Arguments: src - The MultiSrc Widget.
 *                 prev - the piece just before this one, or NULL.
 *	Returns: the allocated piece.
 */

static MultiPiece *
AllocNewPiece(src, prev)
    MultiSrcObject src;
    MultiPiece * prev;
{
  MultiPiece * piece = XtNew(MultiPiece);

  if (prev == NULL) {
    src->multi_src.first_piece = piece;
    piece->next = NULL;
  }
  else {
    if (prev->next != NULL)
      (prev->next)->prev = piece;
    piece->next = prev->next;
    prev->next = piece;
  }
  
  piece->prev = prev;

  return(piece);
}

/*	Function Name: FreeAllPieces
 *	Description: Frees all the pieces
 *	Arguments: src - The MultiSrc Widget.
 *	Returns: none.
 */

static void 
FreeAllPieces(src)
    MultiSrcObject src;
{
  MultiPiece * next, * first = src->multi_src.first_piece;

  if (first->prev != NULL)
    printf("Xaw MultiSrc Object: possible memory leak in FreeAllPieces().\n");

  for ( ; first != NULL ; first = next ) {
    next = first->next;
    RemovePiece(src, first);
  }
}
  
/*	Function Name: RemovePiece
 *	Description: Removes a piece from the list.
 *	Arguments: 
 *                 piece - the piece to remove.
 *	Returns: none.
 */

static void
RemovePiece(src, piece)
    MultiSrcObject src;
    MultiPiece* piece;
{
  if (piece->prev == NULL)
    src->multi_src.first_piece = piece->next;
  else
    (piece->prev)->next = piece->next;

  if (piece->next != NULL)
    (piece->next)->prev = piece->prev;

  if (!src->multi_src.use_string_in_place)
    XtFree((char *)piece->text);

  XtFree((char *)piece);
}

/*	Function Name: FindPiece
 *	Description: Finds the piece containing the position indicated.
 *	Arguments: src - The MultiSrc Widget.
 *                 position - the position that we are searching for.
 * RETURNED        first - the position of the first character in this piece.
 *	Returns: piece - the piece that contains this position.
 */

static MultiPiece *
FindPiece(src, position, first)
    MultiSrcObject src;
    XawTextPosition position, *first;
{
  MultiPiece * old_piece = NULL, * piece = src->multi_src.first_piece;
  XawTextPosition temp;

  for ( temp = 0 ; piece != NULL ; temp += piece->used, piece = piece->next ) {
    *first = temp;
    old_piece = piece;

    if ((temp + piece->used) > position) 
      return(piece);
  }
  return(old_piece);	  /* if we run off the end the return the last piece */
}
    
/*	Function Name: BreakPiece
 *	Description: Breaks a full piece into two new pieces.
 *	Arguments: src - The MultiSrc Widget.
 *                 piece - the piece to break.
 *	Returns: none.
 */

#define HALF_PIECE (src->multi_src.piece_size/2)

static void
BreakPiece(src, piece)
    MultiSrcObject src;
    MultiPiece* piece;
{
  MultiPiece * new = AllocNewPiece(src, piece);
  
  new->text = (wchar_t*)XtMalloc(src->multi_src.piece_size * sizeof(wchar_t));
  (void) wcsncpy(new->text, piece->text + HALF_PIECE,
          src->multi_src.piece_size - HALF_PIECE);
  piece->used = HALF_PIECE;
  new->used = src->multi_src.piece_size - HALF_PIECE; 
}

/* Convert string "XawAsciiString" and "XawAsciiFile" to quarks. */

/* ARGSUSED */
static void
CvtStringToMultiType(args, num_args, fromVal, toVal)
    XrmValuePtr args;		/* unused */
    Cardinal*	num_args;	/* unused */
    XrmValuePtr	fromVal;
    XrmValuePtr	toVal;
{
  static XawAsciiType type;
  static XrmQuark  XtQEstring = NULLQUARK;
  static XrmQuark  XtQEfile;
  XrmQuark q;
  char lowerName[40];

  if (XtQEstring == NULLQUARK) {
    XtQEstring = XrmPermStringToQuark(XtEstring);
    XtQEfile   = XrmPermStringToQuark(XtEfile);
  }

  if (strlen ((char*) fromVal->addr) < sizeof lowerName) {
    XmuCopyISOLatin1Lowered(lowerName, (char *) fromVal->addr);
    q = XrmStringToQuark(lowerName);

    if (q == XtQEstring)     type = XawAsciiString;
    else if (q == XtQEfile)  type = XawAsciiFile;
    else {
      toVal->size = 0;
      toVal->addr = NULL;
      return;
    }
    toVal->size = sizeof type;
    toVal->addr = (XPointer) &type;
    return;
  }
  toVal->size = 0;
  toVal->addr = NULL;
}
