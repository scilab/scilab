/*
 * $XConsortium: Toggle.h,v 1.16 94/04/17 20:13:19 kaleb Exp $
 *
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
 * ToggleP.h - Private definitions for Toggle widget
 *
 * Author: Chris D. Peterson
 *         MIT X Consortium
 *         kit@expo.lcs.mit.edu
 *  
 * Date:   January 12, 1989
 */

#ifndef _XawToggle_h
#define _XawToggle_h

/***********************************************************************
 *
 * Toggle Widget
 *
 ***********************************************************************/

#include <X11/Xaw3d/Command.h>

/* Resources:

 Name		     Class		RepType		Default Value
 ----		     -----		-------		-------------
 radioGroup          RadioGroup         Widget          NULL              +
 radioData           RadioData          Pointer         (XPointer) Widget  ++
 state               State              Boolean         Off

 background	     Background		Pixel		XtDefaultBackground
 bitmap		     Pixmap		Pixmap		None
 border		     BorderColor	Pixel		XtDefaultForeground
 borderWidth	     BorderWidth	Dimension	1
 callback	     Callback		Pointer		NULL
 cursor		     Cursor		Cursor		None
 destroyCallback     Callback		Pointer		NULL
 font		     Font		XFontStructx*	XtDefaultFont
 foreground	     Foreground		Pixel		XtDefaultForeground
 height		     Height		Dimension	text height
 highlightThickness  Thickness		Dimension	2
 insensitiveBorder   Insensitive	Pixmap		Gray
 internalHeight	     Height		Dimension	2
 internalWidth	     Width		Dimension	4
 justify	     Justify		XtJustify	XtJustifyCenter
 label		     Label		String		NULL
 mappedWhenManaged   MappedWhenManaged	Boolean		True
 resize		     Resize		Boolean		True
 sensitive	     Sensitive		Boolean		True
 width		     Width		Dimension	text width
 x		     Position		Position	0
 y		     Position		Position	0

+ To use the toggle as a radio toggle button, set this resource to point to
  any other widget in the radio group.

++ This is the data returned from a call to XtToggleGetCurrent, by default
   this is set to the name of toggle widget.

*/

/*
 * These should be in StringDefs.h but aren't so we will define
 * them here if they are needed.
 */


#define XtCWidget "Widget"
#define XtCState "State"
#define XtCRadioGroup "RadioGroup"
#define XtCRadioData "RadioData"

#ifndef _XtStringDefs_h_
#define XtRWidget "Widget"
#endif

#define XtNstate "state"
#define XtNradioGroup "radioGroup"
#define XtNradioData "radioData"

extern WidgetClass               toggleWidgetClass;

typedef struct _ToggleClassRec   *ToggleWidgetClass;
typedef struct _ToggleRec        *ToggleWidget;


/************************************************************
 * 
 * Public Functions
 *
 ************************************************************/

_XFUNCPROTOBEGIN
   
/*	Function Name: XawToggleChangeRadioGroup
 *	Description: Allows a toggle widget to change radio lists.
 *	Arguments: w - The toggle widget to change lists.
 *                 radio_group - any widget in the new list.
 *	Returns: none.
 */

extern void XawToggleChangeRadioGroup(
#if NeedFunctionPrototypes
    Widget		/* w */,
    Widget		/* radio_group */
#endif
);

/*	Function Name: XawToggleGetCurrent
 *	Description: Returns the RadioData associated with the toggle
 *                   widget that is currently active in a toggle list.
 *	Arguments: radio_group - any toggle widget in the toggle list.
 *	Returns: The XtNradioData associated with the toggle widget.
 */

extern XtPointer XawToggleGetCurrent(
#if NeedFunctionPrototypes
    Widget		/* radio_group */
#endif
);

/*	Function Name: XawToggleSetCurrent
 *	Description: Sets the Toggle widget associated with the
 *                   radio_data specified.
 *	Arguments: radio_group - any toggle widget in the toggle list.
 *                 radio_data - radio data of the toggle widget to set.
 *	Returns: none.
 */

extern void XawToggleSetCurrent(
#if NeedFunctionPrototypes
    Widget		/* radio_group */,
    XtPointer		/* radio_data */
#endif
);
 
/*	Function Name: XawToggleUnsetCurrent
 *	Description: Unsets all Toggles in the radio_group specified.
 *	Arguments: radio_group - any toggle widget in the toggle list.
 *	Returns: none.
 */

extern void XawToggleUnsetCurrent(
#if NeedFunctionPrototypes
    Widget		/* radio_group */
#endif
);

_XFUNCPROTOEND

#endif /* _XawToggle_h */
