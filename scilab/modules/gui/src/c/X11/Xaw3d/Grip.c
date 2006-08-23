/* $XConsortium: Grip.c,v 1.33 94/04/17 20:12:10 kaleb Exp $ */

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
 * Grip.c - Grip Widget (Used by Paned Widget)
 *
 */
#include <X11/IntrinsicP.h>
#include <X11/StringDefs.h>
#include <X11/Xaw3d/XawInit.h>
#include <X11/Xaw3d/GripP.h>

static XtResource resources[] = {
   {XtNwidth, XtCWidth, XtRDimension, sizeof(Dimension),
      XtOffsetOf(GripRec, core.width), XtRImmediate,
      (XtPointer) DEFAULT_GRIP_SIZE},
   {XtNheight, XtCHeight, XtRDimension, sizeof(Dimension),
      XtOffsetOf(GripRec, core.height), XtRImmediate,
      (XtPointer) DEFAULT_GRIP_SIZE},
   {XtNforeground, XtCForeground, XtRPixel, sizeof(Pixel),
      XtOffsetOf(GripRec, core.background_pixel), XtRString,
      XtDefaultForeground},
   {XtNborderWidth, XtCBorderWidth, XtRDimension, sizeof(Dimension),
      XtOffsetOf(GripRec, core.border_width), XtRImmediate, (XtPointer)0},
   {XtNcallback, XtCCallback, XtRCallback, sizeof(XtPointer), 
      XtOffsetOf(GripRec, grip.grip_action), XtRCallback, NULL},
};

static void GripAction( /* Widget, XEvent*, String*, Cardinal */ );

static XtActionsRec actionsList[] =
{
  {"GripAction",      GripAction},
};

#define SuperClass (&threeDClassRec)

GripClassRec gripClassRec = {
   {
/* core class fields */
    /* superclass         */   (WidgetClass) SuperClass,
    /* class name         */   "Grip",
    /* size               */   sizeof(GripRec),
    /* class initialize   */   XawInitializeWidgetSet,
    /* class_part_init    */   NULL,
    /* class_inited       */   FALSE,
    /* initialize         */   NULL,
    /* initialize_hook    */   NULL,
    /* realize            */   XtInheritRealize,
    /* actions            */   actionsList,
    /* num_actions        */   XtNumber(actionsList),
    /* resources          */   resources,
    /* resource_count     */   XtNumber(resources),
    /* xrm_class          */   NULLQUARK,
    /* compress_motion    */   TRUE,
    /* compress_exposure  */   TRUE,
    /* compress_enterleave*/   TRUE,
    /* visible_interest   */   FALSE,
    /* destroy            */   NULL,
    /* resize             */   NULL,
    /* expose             */   XtInheritExpose,
    /* set_values         */   NULL,
    /* set_values_hook    */   NULL,
    /* set_values_almost  */   XtInheritSetValuesAlmost,
    /* get_values_hook    */   NULL,
    /* accept_focus       */   NULL,
    /* version            */   XtVersion,
    /* callback_private   */   NULL,
    /* tm_table           */   NULL,
    /* query_geometry     */   XtInheritQueryGeometry,
    /* display_accelerator*/   XtInheritDisplayAccelerator,
    /* extension          */   NULL
   },
/* Simple class fields initialization */
   {
    /* change_sensitive   */   XtInheritChangeSensitive
   },
/* ThreeD class fields initialization */
   {
    /* shadowdraw	  */   0    /* inherited expose method knows how */
   },
/* Grip class fields initialization */
   {
    /* not used		  */   0
   }
};

WidgetClass gripWidgetClass = (WidgetClass) &gripClassRec;

static void GripAction( widget, event, params, num_params )
    Widget widget;
    XEvent *event;
    String *params;
    Cardinal *num_params;
{
    XawGripCallDataRec call_data;

    call_data.event = event;
    call_data.params = params;
    call_data.num_params = *num_params;

    XtCallCallbacks( widget, XtNcallback, (XtPointer)&call_data );
}
