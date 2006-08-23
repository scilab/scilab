/* $XConsortium: ToggleP.h,v 1.9 94/04/17 20:13:19 converse Exp $ */

/*

Copyright (c) 1989  X Consortium

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
 * ToggleP.h - Private definitions for Toggle widget
 * 
 * Author: Chris D. Peterson
 *         MIT X Consortium
 *         kit@expo.lcs.mit.edu
 *  
 * Date:   January 12, 1989
 *
 */

#ifndef _XawToggleP_h
#define _XawToggleP_h

#include <X11/Xaw3d/Toggle.h>
#include <X11/Xaw3d/CommandP.h>

/***********************************************************************
 *
 * Toggle Widget Private Data
 *
 ***********************************************************************/

#define streq(a, b) ( strcmp((a), (b)) == 0 )

typedef struct _RadioGroup {
  struct _RadioGroup *prev, *next; /* Pointers to other elements in group. */
  Widget widget;		  /* Widget corrosponding to this element. */
} RadioGroup;

/************************************
 *
 *  Class structure
 *
 ***********************************/

   /* New fields for the Toggle widget class record */
typedef struct _ToggleClass  {
    XtActionProc Set;
    XtActionProc Unset;
    XtPointer extension;
} ToggleClassPart;

   /* Full class record declaration */
typedef struct _ToggleClassRec {
    CoreClassPart	core_class;
    SimpleClassPart	simple_class;
    ThreeDClassPart	threeD_class;
    LabelClassPart	label_class;
    CommandClassPart	command_class;
    ToggleClassPart     toggle_class;
} ToggleClassRec;

extern ToggleClassRec toggleClassRec;

/***************************************
 *
 *  Instance (widget) structure 
 *
 **************************************/

    /* New fields for the Toggle widget record */
typedef struct {
    /* resources */
    Widget      widget;
    XtPointer   radio_data;

    /* private data */
    RadioGroup * radio_group;
} TogglePart;

   /* Full widget declaration */
typedef struct _ToggleRec {
    CorePart         core;
    SimplePart	     simple;
    ThreeDPart       threeD;
    LabelPart	     label;
    CommandPart	     command;
    TogglePart       toggle;
} ToggleRec;

#endif /* _XawToggleP_h */


