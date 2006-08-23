/* $XConsortium: SmeLine.c,v 1.15 94/04/17 20:12:51 eswu Exp $ */

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
 *
 * Author:  Chris D. Peterson, MIT X Consortium
 */

/*
 * Sme.c - Source code for the generic menu entry
 *
 * Date:    September 26, 1989
 *
 * By:      Chris D. Peterson
 *          MIT X Consortium 
 *          kit@expo.lcs.mit.edu
 */

#include <stdio.h>
#include <X11/IntrinsicP.h>
#include <X11/StringDefs.h>

#include <X11/Xaw3d/XawInit.h>
#include <X11/Xaw3d/SmeLineP.h>
#include <X11/Xaw3d/Cardinals.h>

#define offset(field) XtOffsetOf(SmeLineRec, sme_line.field)
static XtResource resources[] = {
  {XtNlineWidth, XtCLineWidth, XtRDimension, sizeof(Dimension),
     offset(line_width), XtRImmediate, (XtPointer) 1},
  {XtNstipple, XtCStipple, XtRBitmap, sizeof(Pixmap),
     offset(stipple), XtRImmediate, (XtPointer) XtUnspecifiedPixmap},
  {XtNforeground, XtCForeground, XtRPixel, sizeof(Pixel),
     offset(foreground), XtRString, XtDefaultForeground},
};   
#undef offset

/*
 * Function definitions. 
 */

static void Redisplay(), Initialize();
static void DestroyGC(), CreateGC();
static Boolean SetValues();
static XtGeometryResult QueryGeometry();


#define SUPERCLASS (&smeClassRec)

SmeLineClassRec smeLineClassRec = {
  {
    /* superclass         */    (WidgetClass) SUPERCLASS,
    /* class_name         */    "SmeLine",
    /* size               */    sizeof(SmeLineRec),
    /* class_initialize   */	XawInitializeWidgetSet,
    /* class_part_initialize*/	NULL,
    /* Class init'ed      */	FALSE,
    /* initialize         */    Initialize,
    /* initialize_hook    */	NULL,
    /* realize            */    NULL,
    /* actions            */    NULL,
    /* num_actions        */    ZERO,
    /* resources          */    resources,
    /* resource_count     */	XtNumber(resources),
    /* xrm_class          */    NULLQUARK,
    /* compress_motion    */    FALSE, 
    /* compress_exposure  */    FALSE,
    /* compress_enterleave*/ 	FALSE,
    /* visible_interest   */    FALSE,
    /* destroy            */    DestroyGC,
    /* resize             */    NULL,
    /* expose             */    Redisplay,
    /* set_values         */    SetValues,
    /* set_values_hook    */	NULL,
    /* set_values_almost  */	XtInheritSetValuesAlmost,  
    /* get_values_hook    */	NULL,			
    /* accept_focus       */    NULL,
    /* intrinsics version */	XtVersion,
    /* callback offsets   */    NULL,
    /* tm_table		  */    NULL,
    /* query_geometry	  */    QueryGeometry,
    /* display_accelerator*/    NULL,
    /* extension	  */    NULL
  },{
    /* Menu Entry Fields */
      
    /* highlight */             XtInheritHighlight,
    /* unhighlight */           XtInheritUnhighlight,
    /* notify */		XtInheritNotify,		
    /* extension */             NULL				
  },{
    /* Line Menu Entry Fields */
    /* extension */             NULL				
  }
};

WidgetClass smeLineObjectClass = (WidgetClass) &smeLineClassRec;

/************************************************************
 *
 * Semi-Public Functions.
 *
 ************************************************************/

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
Cardinal *num_args;
{
    SmeLineObject entry = (SmeLineObject) new;

    if (entry->rectangle.height == 0)
	entry->rectangle.height = entry->sme_line.line_width;

    CreateGC(new);
}

/*	Function Name: CreateGC
 *	Description: Creates the GC for the line entry widget.
 *	Arguments: w - the Line entry widget.
 *	Returns: none
 *
 *      We can only share the GC if there is no stipple, because
 *      we need to change the stipple origin when drawing.
 */

static void
CreateGC(w)
Widget w;
{
    SmeLineObject entry = (SmeLineObject) w;
    XGCValues values;
    XtGCMask mask = GCForeground | GCGraphicsExposures | GCLineWidth ;
    
    values.foreground = entry->sme_line.foreground;
    values.graphics_exposures = FALSE;
    values.line_width = entry->sme_line.line_width;
    
    if (entry->sme_line.stipple != XtUnspecifiedPixmap) {
	values.stipple = entry->sme_line.stipple;
	values.fill_style = FillStippled; 
	mask |= GCStipple | GCFillStyle;
	
	entry->sme_line.gc = XCreateGC(XtDisplayOfObject(w), 
				      RootWindowOfScreen(XtScreenOfObject(w)),
				      mask, &values);
    }
    else
	entry->sme_line.gc = XtGetGC(w, mask, &values);
}

/*	Function Name: DestroyGC
 *	Description: Destroys the GC when we are done with it.
 *	Arguments: w - the Line entry widget.
 *	Returns: none
 */

static void
DestroyGC(w)
Widget w;
{
    SmeLineObject entry = (SmeLineObject) w;

    if (entry->sme_line.stipple != XtUnspecifiedPixmap) 
	XFreeGC(XtDisplayOfObject(w), entry->sme_line.gc);
    else
	XtReleaseGC(w, entry->sme_line.gc);
}

/*	Function Name: Redisplay
 *	Description: Paints the Line
 *	Arguments: w - the menu entry.
 *                 event, region - NOT USED.
 *	Returns: none
 */

/*ARGSUSED*/
static void
Redisplay(w, event, region)
Widget w;
XEvent * event;
Region region;
{
    SmeLineObject entry = (SmeLineObject) w;
    int y = entry->rectangle.y + 
	    (int)(entry->rectangle.height - entry->sme_line.line_width) / 2;

    if (entry->sme_line.stipple != XtUnspecifiedPixmap) 
	XSetTSOrigin(XtDisplayOfObject(w), entry->sme_line.gc, 0, y);

    XFillRectangle(XtDisplayOfObject(w), XtWindowOfObject(w),
		   entry->sme_line.gc, 
		   0, y, (unsigned int) entry->rectangle.width, 
		   (unsigned int) entry->sme_line.line_width );
}

/*      Function Name: SetValues
 *      Description: Relayout the menu when one of the resources is changed.
 *      Arguments: current - current state of the widget.
 *                 request - what was requested.
 *                 new - what the widget will become.
 *      Returns: none
 */

/* ARGSUSED */
static Boolean
SetValues(current, request, new, args, num_args)
Widget current, request, new;
ArgList args;
Cardinal *num_args;
{
    SmeLineObject entry = (SmeLineObject) new;
    SmeLineObject old_entry = (SmeLineObject) current;
  
    if ( (entry->sme_line.line_width != old_entry->sme_line.line_width) &&
	 (entry->sme_line.stipple != old_entry->sme_line.stipple) ) {
	DestroyGC(current);
	CreateGC(new);
	return(TRUE);
    }
    return(FALSE);
}

/*	Function Name: QueryGeometry.
 *	Description: Returns the preferred geometry for this widget.
 *	Arguments: w - the menu entry object.
 *                 itended, return - the intended and return geometry info.
 *	Returns: A Geometry Result.
 *
 * See the Intrinsics manual for details on what this function is for.
 * 
 * I just return the height and a width of 1.
 */

static XtGeometryResult
QueryGeometry(w, intended, return_val) 
Widget w;
XtWidgetGeometry *intended, *return_val;
{
    SmeObject entry = (SmeObject) w;
    Dimension width;
    XtGeometryResult ret_val = XtGeometryYes;
    XtGeometryMask mode = intended->request_mode;

    width = 1;			/* we can be really small. */

    if ( ((mode & CWWidth) && (intended->width != width)) ||
	 !(mode & CWWidth) ) {
	return_val->request_mode |= CWWidth;
	return_val->width = width;
	mode = return_val->request_mode;
	
	if ( (mode & CWWidth) && (width == entry->rectangle.width) )
	    return(XtGeometryNo);
	return(XtGeometryAlmost);
    }
    return(ret_val);
}
