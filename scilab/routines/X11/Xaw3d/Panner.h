/*
 * $XConsortium: Panner.h,v 1.22 94/04/17 20:12:32 rws Exp $
 *
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
 * Author:  Jim Fulton, MIT X Consortium
 */

#ifndef _XawPanner_h
#define _XawPanner_h

#include <X11/Xaw3d/Reports.h>

/*****************************************************************************
 * 
 * Panner Widget (subclass of Simple)
 * 
 * This widget is used to represent navigation in a 2d coordinate system.
 * 
 * Parameters:
 * 
 *  Name		Class		Type		Default
 *  ----		-----		----		-------
 * 
 *  allowOff		AllowOff	Boolean		FALSE
 *  background		Background	Pixel		XtDefaultBackground
 *  backgroundStipple	BackgroundStipple	String	NULL
 *  canvasWidth		CanvasWidth	Dimension	0
 *  canvasHeight	CanvasHeight	Dimension	0
 *  defaultScale	DefaultScale	Dimension	8 percent
 *  foreground		Foreground	Pixel		XtDefaultBackground
 *  internalSpace	InternalSpace	Dimension	4
 *  lineWidth		LineWidth	Dimension	0
 *  reportCallback	ReportCallback	XtCallbackList	NULL
 *  resize		Resize		Boolean		TRUE
 *  rubberBand		RubberBand	Boolean		FALSE
 *  shadowColor		ShadowColor	Pixel		XtDefaultForeground
 *  shadowThickness	ShadowThickness	Dimension	2
 *  sliderX		SliderX		Position	0
 *  sliderY		SliderY		Position	0
 *  sliderWidth		SliderWidth	Dimension	0
 *  sliderHeight	SliderHeight	Dimension	0
 * 
 *****************************************************************************/

					/* new instance and class names */
#ifndef _XtStringDefs_h_
#define XtNresize "resize"
#define XtCResize "Resize"
#endif

#define XtNallowOff "allowOff"
#define XtCAllowOff "AllowOff"
#define XtNbackgroundStipple "backgroundStipple"
#define XtCBackgroundStipple "BackgroundStipple"
#define XtNdefaultScale "defaultScale"
#define XtCDefaultScale "DefaultScale"
#define XtNcanvasWidth "canvasWidth"
#define XtCCanvasWidth "CanvasWidth"
#define XtNcanvasHeight "canvasHeight"
#define XtCCanvasHeight "CanvasHeight"
#define XtNinternalSpace "internalSpace"
#define XtCInternalSpace "InternalSpace"
#define XtNlineWidth "lineWidth"
#define XtCLineWidth "LineWidth"
#define XtNrubberBand "rubberBand"
#define XtCRubberBand "RubberBand"
#define XtNshadowThickness "shadowThickness"
#define XtCShadowThickness "ShadowThickness"
#define XtNshadowColor "shadowColor"
#define XtCShadowColor "ShadowColor"
#define XtNsliderX "sliderX"
#define XtCSliderX "SliderX"
#define XtNsliderY "sliderY"
#define XtCSliderY "SliderY"
#define XtNsliderWidth "sliderWidth"
#define XtCSliderWidth "SliderWidth"
#define XtNsliderHeight "sliderHeight"
#define XtCSliderHeight "SliderHeight"

					/* external declarations */
extern WidgetClass pannerWidgetClass;

typedef struct _PannerClassRec *PannerWidgetClass;
typedef struct _PannerRec      *PannerWidget;

#endif /* _XawPanner_h */
