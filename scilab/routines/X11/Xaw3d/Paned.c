/* $XConsortium: Paned.c,v 1.27 94/04/17 20:12:28 kaleb Exp $ */

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
 * Paned.c - Paned Composite Widget.
 *
 * Updated and significantly modified from the Athena VPaned Widget.
 *
 * Date:    March 1, 1989
 *
 * By:      Chris D. Peterson
 *          MIT X Consortium
 *          kit@expo.lcs.mit.edu
 */

#include <X11/IntrinsicP.h>
#include <X11/cursorfont.h>
#include <X11/StringDefs.h>

#include <X11/Xmu/Misc.h>
#include <X11/Xmu/Converters.h>

#include <X11/Xaw3d/XawInit.h>
#include <X11/Xaw3d/Grip.h>
#include <X11/Xaw3d/PanedP.h>

/* I don't know why Paned.c calls _XawImCallVendorShellExtResize, but... */
#include <X11/Xaw3d/XawImP.h> 

#include <ctype.h>

typedef enum {UpLeftPane = 'U', LowRightPane = 'L', 
	      ThisBorderOnly = 'T', AnyPane = 'A' } Direction;

#define NO_INDEX -100
#define IS_GRIP  NULL

#define PaneInfo(w)	((Pane)(w)->core.constraints)
#define HasGrip(w)	(PaneInfo(w)->grip != NULL)
#define IsPane(w)	((w)->core.widget_class != gripWidgetClass)
#define PaneIndex(w)	(PaneInfo(w)->position)
#define IsVert(w)       ( (w)->paned.orientation == XtorientVertical )

#define ForAllPanes(pw, childP) \
  for ( (childP) = (pw)->composite.children ; \
        (childP) < (pw)->composite.children + (pw)->paned.num_panes ; \
        (childP)++ )

#define ForAllChildren(pw, childP) \
  for ( (childP) = (pw)->composite.children ; \
        (childP) < (pw)->composite.children + (pw)->composite.num_children ; \
        (childP)++ )

/*****************************************************************************
 *
 * Full instance record declaration
 *
 ****************************************************************************/

static char defGripTranslations[] =
    "<Btn1Down>:		GripAction(Start, UpLeftPane) \n\
     <Btn2Down>:		GripAction(Start, ThisBorderOnly) \n\
     <Btn3Down>:		GripAction(Start, LowRightPane) \n\
     <Btn1Motion>:		GripAction(Move, UpLeft) \n\
     <Btn2Motion>:		GripAction(Move, ThisBorder) \n\
     <Btn3Motion>:		GripAction(Move, LowRight) \n\
     Any<BtnUp>:		GripAction(Commit)";

#define offset(field) XtOffsetOf(PanedRec, paned.field)

static XtResource resources[] = {
    {XtNinternalBorderColor, XtCBorderColor, XtRPixel, sizeof(Pixel),
	 offset(internal_bp), XtRString, 
         (XtPointer) XtDefaultForeground},
    {XtNinternalBorderWidth, XtCBorderWidth, XtRDimension, sizeof(Dimension),
	 offset(internal_bw), XtRImmediate, (XtPointer) 1},
    {XtNgripIndent, XtCGripIndent, XtRPosition, sizeof(Position),
	 offset(grip_indent), XtRImmediate, (XtPointer) 10},
    {XtNrefigureMode, XtCBoolean, XtRBoolean, sizeof(Boolean),
         offset(refiguremode), XtRImmediate, (XtPointer) TRUE},
    {XtNgripTranslations, XtCTranslations, XtRTranslationTable,
         sizeof(XtTranslations),
         offset(grip_translations), XtRString, (XtPointer)defGripTranslations},
    {XtNorientation,  XtCOrientation, XtROrientation, sizeof(XtOrientation),
         offset(orientation), XtRImmediate, (XtPointer) XtorientVertical},

    /* Cursors - both horiz and vertical have to work. */

    {XtNcursor, XtCCursor, XtRCursor, sizeof(Cursor),
         offset(cursor), XtRImmediate, None},
    {XtNgripCursor, XtCCursor, XtRCursor, sizeof(Cursor),
         offset(grip_cursor), XtRImmediate, None},
    {XtNverticalGripCursor, XtCCursor, XtRCursor, sizeof(Cursor),
         offset(v_grip_cursor), XtRString, "sb_v_double_arrow"},
    {XtNhorizontalGripCursor, XtCCursor, XtRCursor, sizeof(Cursor),
         offset(h_grip_cursor), XtRString, "sb_h_double_arrow"},

    {XtNbetweenCursor, XtCCursor, XtRCursor, sizeof(Cursor),
         offset(adjust_this_cursor), XtRString, None},
    {XtNverticalBetweenCursor, XtCCursor, XtRCursor, sizeof(Cursor),
         offset(v_adjust_this_cursor), XtRString, "sb_left_arrow"},
    {XtNhorizontalBetweenCursor, XtCCursor, XtRCursor, sizeof(Cursor),
         offset(h_adjust_this_cursor), XtRString, "sb_up_arrow"},

    {XtNupperCursor, XtCCursor, XtRCursor, sizeof(Cursor),
         offset(adjust_upper_cursor), XtRString, "sb_up_arrow"},
    {XtNlowerCursor, XtCCursor, XtRCursor, sizeof(Cursor),
         offset(adjust_lower_cursor), XtRString, "sb_down_arrow"},
    {XtNleftCursor, XtCCursor, XtRCursor, sizeof(Cursor),
         offset(adjust_left_cursor), XtRString, "sb_left_arrow"},
    {XtNrightCursor, XtCCursor, XtRCursor, sizeof(Cursor),
         offset(adjust_right_cursor), XtRString, "sb_right_arrow"},
};

#undef offset

#define offset(field) XtOffsetOf(PanedConstraintsRec, paned.field)

static XtResource subresources[] = {
    {XtNallowResize, XtCBoolean, XtRBoolean, sizeof(Boolean),
	 offset(allow_resize), XtRImmediate, (XtPointer) FALSE},
    {XtNposition, XtCPosition, XtRInt, sizeof(int),
         offset(position), XtRImmediate, (XtPointer) 0},
    {XtNmin, XtCMin, XtRDimension, sizeof(Dimension),
         offset(min), XtRImmediate, (XtPointer) PANED_GRIP_SIZE},
    {XtNmax, XtCMax, XtRDimension, sizeof(Dimension),
         offset(max), XtRImmediate, (XtPointer) ~0},
    {XtNpreferredPaneSize, XtCPreferredPaneSize, XtRDimension,
	 sizeof(Dimension), offset(preferred_size), 
         XtRImmediate, (XtPointer) PANED_ASK_CHILD},
    {XtNresizeToPreferred, XtCBoolean, XtRBoolean, sizeof(Boolean),
         offset(resize_to_pref), XtRImmediate, (XtPointer) FALSE},
    {XtNskipAdjust, XtCBoolean, XtRBoolean, sizeof(Boolean),
         offset(skip_adjust), XtRImmediate, (XtPointer) FALSE},
    {XtNshowGrip, XtCShowGrip, XtRBoolean, sizeof(Boolean),
	 offset(show_grip), XtRImmediate, (XtPointer) TRUE},
};

#undef offset

static void ClassInitialize(), Initialize();
static void Realize(), Resize();
static void Redisplay();
static void GetGCs(), ReleaseGCs();
static void RefigureLocationsAndCommit();
static Boolean SetValues();
static XtGeometryResult GeometryManager();
static void ChangeManaged();
static void InsertChild();
static void DeleteChild();
static Boolean PaneSetValues();
static Dimension PaneSize(), GetRequestInfo();
static Boolean SatisfiesRule1(), SatisfiesRule2(), SatisfiesRule3();

static void PushPaneStack();
static void GetPaneStack();
static Boolean PopPaneStack();
static void ClearPaneStack();

#define SuperClass ((ConstraintWidgetClass)&constraintClassRec)

PanedClassRec panedClassRec = {
   {
/* core class fields */
    /* superclass         */   (WidgetClass) SuperClass,
    /* class name         */   "Paned",
    /* size               */   sizeof(PanedRec),
    /* class_initialize   */   ClassInitialize,
    /* class_part init    */   NULL,
    /* class_inited       */   FALSE,
    /* initialize         */   Initialize,
    /* initialize_hook    */   NULL,
    /* realize            */   Realize,
    /* actions            */   NULL,
    /* num_actions        */   0,
    /* resources          */   resources,
    /* resource_count     */   XtNumber(resources),
    /* xrm_class          */   NULLQUARK,
    /* compress_motion    */   TRUE,
    /* compress_exposure  */   TRUE,
    /* compress_enterleave*/   TRUE,
    /* visible_interest   */   FALSE,
    /* destroy            */   ReleaseGCs,
    /* resize             */   Resize,
    /* expose             */   Redisplay,
    /* set_values         */   SetValues,
    /* set_values_hook    */   NULL,
    /* set_values_almost  */   XtInheritSetValuesAlmost,
    /* get_values_hook    */   NULL,
    /* accept_focus       */   NULL,
    /* version            */   XtVersion,
    /* callback_private   */   NULL,
    /* tm_table           */   NULL,
    /* query_geometry	  */   XtInheritQueryGeometry,
    /* display_accelerator*/   XtInheritDisplayAccelerator,
    /* extension          */   NULL
   }, {
/* composite class fields */
    /* geometry_manager   */   GeometryManager,
    /* change_managed     */   ChangeManaged,
    /* insert_child       */   InsertChild,
    /* delete_child       */   DeleteChild,
    /* extension          */   NULL
   }, {
/* constraint class fields */
    /* subresources       */   subresources,
    /* subresource_count  */   XtNumber(subresources),
    /* constraint_size    */   sizeof(PanedConstraintsRec),
    /* initialize         */   NULL,
    /* destroy            */   NULL,
    /* set_values         */   PaneSetValues,
    /* extension          */   NULL
   }
};

WidgetClass panedWidgetClass = (WidgetClass) &panedClassRec;

/* For compatibility. */
WidgetClass vPanedWidgetClass = (WidgetClass) &panedClassRec;

/***********************************************************
 *
 * Private Functions.
 *
 ************************************************************/

/*	Function Name: AdjustPanedSize
 *	Description: Adjusts the size of the pane.
 *	Arguments: pw - the paned widget to adjust.
 *                 off_size - the new off_size to use.
 *                 result_ret - result of query ** RETURNED **
 *                 on_size_ret - the new on_size ** RETURNED **
 *                 off_size_ret - the new off_size ** RETURNED **
 *	Returns: the amount of change in size.
 */

static void
AdjustPanedSize(pw, off_size, result_ret, on_size_ret, off_size_ret)
PanedWidget pw;
Dimension off_size;
XtGeometryResult * result_ret;
Dimension * on_size_ret, * off_size_ret;
{
    Dimension old_size = PaneSize( (Widget) pw, IsVert(pw));
    Dimension newsize = 0;
    Widget * childP;
    XtWidgetGeometry request, reply;
    request.request_mode = CWWidth | CWHeight;

    ForAllPanes(pw, childP) {
        int size = Max(PaneInfo(*childP)->size, (int)PaneInfo(*childP)->min);
	AssignMin(size, (int) PaneInfo(*childP)->max);
        newsize += size + pw->paned.internal_bw;
    }
    newsize -= pw->paned.internal_bw;

    if (newsize < 1) newsize = 1;

    if ( IsVert(pw) ) {
        request.width = off_size;
	request.height = newsize;
    }
    else {
        request.width = newsize;
	request.height = off_size;
    }

    if (result_ret != NULL) {
      request.request_mode |= XtCWQueryOnly;

      *result_ret = XtMakeGeometryRequest( (Widget) pw, &request, &reply );
      _XawImCallVendorShellExtResize( (Widget) pw );

      if ( (newsize == old_size) || (*result_ret == XtGeometryNo) ) {
	  *on_size_ret = old_size;
	  *off_size_ret = off_size;
	  return;
      }
      if (*result_ret != XtGeometryAlmost) {
	  *on_size_ret = GetRequestInfo( &request, IsVert(pw) );
      	  *off_size_ret = GetRequestInfo( &request, !IsVert(pw) );
	  return;
      }
      *on_size_ret = GetRequestInfo( &reply, IsVert(pw) );
      *off_size_ret = GetRequestInfo( &reply, !IsVert(pw) );
      return;
    }

    if (newsize == old_size) return;

    if (XtMakeGeometryRequest( (Widget) pw,
			      &request, &reply) == XtGeometryAlmost)
        XtMakeGeometryRequest( (Widget) pw, &reply, &request);
}

/*	Function Name: PaneSize
 *	Description: returns the width or height of the pane depending
 *                   upon the orientation we are using.
 *	Arguments: w - and widget.
 *                 vertical - TRUE if this is vertically oriented pane.
 *	Returns: the size requested
 *
 *      vertical  - return height
 *      !vertical - return width
 */

static Dimension
PaneSize(w, vertical)
Widget w;
Boolean vertical;
{
    if (vertical) return (w->core.height);
    return (w->core.width);
}

/*	Function Name: GetRequestInfo
 *	Description: returns request information.
 *	Arguments:  geo_struct - a geometry struct to get information out of.
 *                  vert - TRUE if this is a vertical paned widget.
 *	Returns: the request information.
 */

static Dimension
GetRequestInfo(geo_struct, vert)
XtWidgetGeometry * geo_struct;
Boolean vert;
{
    if ( vert ) return ( (Dimension) geo_struct->height);
    return ( (Dimension) geo_struct->width);
}

/*	Function Name: ChoosePaneToResize.
 *	Description: This function chooses a pane to resize.
 *                   They are chosen using the following rules:
 *
 *                   1) size < max && size > min
 *                   2) skip adjust == FALSE
 *                   3) widget not its prefered height &&
 *                      this change will bring it closer &&
 *                      The user has not resized this pane.
 *           
 *                   If no widgets are found that fits all the rules then
 *                      rule #3 is broken.
 *                   If there are still no widgets found than
 *                      rule #2 is broken.
 *                   Rule #1 is never broken.
 *                   If no widgets are found then NULL is returned.
 * 
 *	Arguments: pw - the paned widget.
 *                 paneindex - the index of the current pane.
 *                 dir - direction to search first.
 *                 shrink - TRUE if we need to shrink a pane, FALSE otherwise.
 *	Returns: pane to resize or NULL.
 */

static Pane
ChoosePaneToResize(pw, paneindex, dir, shrink)
PanedWidget pw;
int paneindex;
Direction dir;
Boolean shrink;
{
    Widget *childP;
    int rules = 3;
    Direction _dir = dir;
    int _index = paneindex;

    if ( (paneindex == NO_INDEX) || (dir == AnyPane) ) {  /* Use defaults. */
      _dir = LowRightPane;		/* Go up. - really. */
      _index = pw->paned.num_panes - 1;	/* Start the last pane, and work
					   backwards. */
    }
    childP = pw->composite.children + _index;

    /* CONSTCOND */
    while(TRUE) {
        Pane pane = PaneInfo(*childP);
        
        if ( (rules < 3 || SatisfiesRule3(pane, shrink)) &&
	     (rules < 2 || SatisfiesRule2(pane))         &&
	     (SatisfiesRule1(pane, shrink))              &&
	     ((paneindex != PaneIndex(*childP)) || (dir == AnyPane)) )
	    return(pane);

/*
 * This is counter-intuitive, but if we are resizing the pane
 * above the grip we want to choose a pane below the grip to lose,
 * and visa-versa.
 */

	if (_dir == LowRightPane) --childP; else ++childP;

/*
 * If we have come to and edge then reduce the rule set, and try again.
 * If we are reduced the rules to none, then return NULL.
 */
	
	if ( (childP - pw->composite.children < 0) ||
	     (childP - pw->composite.children >= pw->paned.num_panes) ) {
	    if (--rules < 1)  /* less strict rules. */
	      return(NULL);
	    childP = pw->composite.children + _index;
	}
    }
}

/*	Function Name: StatisfiesRule1
 *	Description: check for to see if this pane satisfies rule 1.
 *	Arguments: pane - the pane to check.
 *                 shrink -TRUE if we want to shrink this pane, FALSE otherwise
 *	Returns: TRUE if the rule is satisfied.
 */

static Boolean
SatisfiesRule1(pane, shrink)
Pane pane;
Boolean shrink;
{
  return( (shrink && (pane->size != pane->min)) ||
	  (!shrink && (pane->size != pane->max)) );
}
 
/*	Function Name: StatisfiesRule2
 *	Description: check for to see if this pane satisfies rule 2.
 *	Arguments: pane - the pane to check.
 *	Returns: TRUE if the rule is satisfied.
 */

static Boolean
SatisfiesRule2(pane)
Pane pane;
{
  return(!pane->skip_adjust || pane->paned_adjusted_me);
}
 
/*	Function Name: StatisfiesRule3
 *	Description: check for to see if this pane satisfies rule 3.
 *	Arguments: pane - the pane to check.
 *                 shrink -TRUE if we want to shrink this pane, FALSE otherwise
 *	Returns: TRUE if the rule is satisfied.
 */

static Boolean
SatisfiesRule3(pane, shrink)
Pane pane;
Boolean shrink;
{
  return ( pane->paned_adjusted_me &&
	   ( (shrink && ((int)pane->wp_size <= pane->size)) ||
	     (!shrink && ((int)pane->wp_size >= pane->size))) );
}

/*	Function Name: LoopAndRefigureChildren.
 *	Description: if we are resizing either the UpleftPane or LowRight Pane
 *                   loop through all the children to see if any will allow us
 *                   to resize them.
 *	Arguments: pw - the paned widget.
 *                 paneindex - the number of the pane border we are moving.
 *                 dir - the pane to move (either UpLeftPane or LowRightPane).
 *                 sizeused - current amount of space used. 
 *                            THIS VALUE IS USED AND RETURNED.
 *	Returns: none.
 */

static void
LoopAndRefigureChildren(pw, paneindex, dir, sizeused)
PanedWidget pw;
int paneindex, *sizeused;
Direction dir;
{
    int pane_size = (int) PaneSize( (Widget) pw, IsVert(pw));
    Boolean shrink = (*sizeused > pane_size);

    if (dir == LowRightPane) paneindex++;

    while (*sizeused != pane_size) { /* While all panes do not fit properly. */
/*
 * Choose a pane to resize.
 * First look on the Pane Stack, and then go hunting for another one.
 * If we fail to find a pane to resize then give up.
 */
        Pane pane;
	int start_size;
	Dimension old;
	Boolean rule3_ok = FALSE, from_stack = TRUE;

	GetPaneStack(pw, shrink, &pane, &start_size);
	if (pane == NULL) {
	    pane = ChoosePaneToResize(pw, paneindex, dir, shrink);
	    if (pane == NULL) 
	        return; /* no one to resize, give up. */

	    rule3_ok = SatisfiesRule3(pane, shrink);
	    from_stack = FALSE;
	    PushPaneStack(pw, pane);
	}


/*
 * Try to resize this pane so that all panes will fit, take min and max
 * into account.
 */
	old = pane->size;
	pane->size += pane_size - *sizeused;

	if (from_stack) {
	    if (shrink) {
	        AssignMax(pane->size, start_size);
	    }			/* don't remove these braces. */
	    else
	        AssignMin(pane->size, start_size);

	    if (pane->size == start_size) (void) PopPaneStack(pw);
	}
	else if (rule3_ok) {
	    if (shrink) {
	        AssignMax(pane->size, (int) pane->wp_size);
	    }			/* don't remove these braces. */
	    else
	        AssignMin(pane->size, (int) pane->wp_size);
	}

	pane->paned_adjusted_me = (pane->size != pane->wp_size);
	AssignMax(pane->size, (int) pane->min);
	AssignMin(pane->size, (int) pane->max);
	*sizeused += (pane->size - old);
    }
}

/*	Function Name: RefigureLocations
 *	Description: refigures all locations of children.
 *	Arguments: pw - the paned widget.
 *                 paneindex - child to start refiguring at.
 *                 dir - direction to move from child.
 *	Returns: none.
 *
 *      There are special arguments to paneindex and dir, they are:
 *      paneindex - NO_INDEX.
 *      dir   - AnyPane.
 *
 *      If either of these is true then all panes may be resized and
 *      the choosing of panes procedes in reverse order starting with the
 *      last child.
 */

static void 
RefigureLocations(pw, paneindex, dir)
PanedWidget pw;
int paneindex;
Direction dir;
{
    Widget *childP;
    int pane_size = (int) PaneSize( (Widget) pw, IsVert(pw) );
    int sizeused = 0;
    Position loc = 0;

    if (pw->paned.num_panes == 0 || !pw->paned.refiguremode) return;

/*
 * Get an initial estimate of the size we will use.
 */

    ForAllPanes(pw, childP) {
        Pane pane = PaneInfo(*childP);
	AssignMax(pane->size, (int) pane->min);
	AssignMin(pane->size, (int) pane->max);
	sizeused += (int) pane->size + (int) pw->paned.internal_bw;
    }
    sizeused -= (int) pw->paned.internal_bw;

    if ( (dir != ThisBorderOnly) && (sizeused != pane_size) ) 
      LoopAndRefigureChildren(pw, paneindex, dir, &sizeused);

/* 
 * If we still are not the right size, then tell the pane that
 * wanted to resize that it can't.
 */


    if ( (paneindex != NO_INDEX) && (dir != AnyPane) ) {
	Pane pane = PaneInfo(*(pw->composite.children + paneindex));
        Dimension old = pane->size;

	pane->size += pane_size - sizeused;
	AssignMax(pane->size, (int) pane->min);
	AssignMin(pane->size, (int) pane->max);
	sizeused += pane->size - old;
    }
    
/*
 * It is possible that the panes will not fit inside the vpaned widget, but
 * we have tried out best.
 *
 * Assign each pane a location.
 */

    ForAllPanes(pw, childP) {
        PaneInfo(*childP)->delta = loc;
        loc += PaneInfo(*childP)->size + pw->paned.internal_bw;
    }
}

/*	Function Name: CommitNewLocations
 *	Description: Commits all of the previously figured locations.
 *	Arguments: pw - the paned widget.
 *	Returns: none.
 */

static void 
CommitNewLocations(pw)
PanedWidget pw;
{
    Widget *childP;
    XWindowChanges changes;

    changes.stack_mode = Above;

    ForAllPanes(pw, childP) {
	Pane pane = PaneInfo(*childP);
	Widget grip = pane->grip; /* may be NULL. */

	if (IsVert(pw)) {
	    XtMoveWidget(*childP, (Position) 0, pane->delta);
	    XtResizeWidget(*childP, pw->core.width, (Dimension) pane->size,
			   (Dimension) 0);

	    if (HasGrip(*childP)) {	    /* Move and Display the Grip */
	        changes.x = pw->core.width - pw->paned.grip_indent -
		            grip->core.width - grip->core.border_width*2;
		changes.y = (*childP)->core.y + (*childP)->core.height -
		            grip->core.height/2 - grip->core.border_width + 
			    pw->paned.internal_bw/2;
	    }
	}
	else {
	    XtMoveWidget(*childP, pane->delta, (Position) 0);
	    XtResizeWidget(*childP, (Dimension) pane->size, pw->core.height,
			   (Dimension) 0);


	    if (HasGrip(*childP)) {	    /* Move and Display the Grip */
	        changes.x = (*childP)->core.x + (*childP)->core.width -
	                    grip->core.width/2 - grip->core.border_width + 
			    pw->paned.internal_bw/2;
		changes.y = pw->core.height - pw->paned.grip_indent -
	                    grip->core.height - grip->core.border_width*2;
	    }
	}

/*
 * This should match XtMoveWidget, except that we're also insuring the 
 * grip is Raised in the same request.
 */

	if (HasGrip(*childP)) {
	    grip->core.x = changes.x;
	    grip->core.y = changes.y;

	    if (XtIsRealized(pane->grip))
	        XConfigureWindow( XtDisplay(pane->grip), XtWindow(pane->grip),
				  CWX | CWY | CWStackMode, &changes );
	}
    }
    ClearPaneStack(pw);
}

/*	Function Name: RefigureLocationsAndCommit
 *	Description: Refigures all locations in a paned widget and
 *                   commits them immediately.
 *	Arguments: pw - the paned widget.
 *	Returns: none
 *
 *      This function does nothing if any of the following are true.
 *      o refiguremode is false.
 *      o The widget is unrealized.
 *      o There are no panes is the paned widget.
 *
 *      NOTE: This is the resize Procedure for the Paned widget.
 */

static void 
RefigureLocationsAndCommit(w)
Widget w;
{
    PanedWidget pw = (PanedWidget) w;
    if (pw->paned.refiguremode && XtIsRealized( (Widget) pw) &&
	pw->paned.num_panes > 0 ) {
	RefigureLocations(pw, NO_INDEX, AnyPane);
	CommitNewLocations(pw);
    }
}

/*	Function Name: _DrawRect
 *	Description: Draws a rectangle in the proper orientation.
 *	Arguments: pw - the paned widget.
 *                 gc - gc to used for the draw.
 *                 on_olc, off_loc - location of upper left corner of rect.
 *                 on_size, off_size - size of rectangle.
 *	Returns: none
 */

static void
_DrawRect(pw, gc, on_loc, off_loc, on_size, off_size)
PanedWidget pw;
GC gc;
int on_loc, off_loc;
unsigned int on_size, off_size;
{
  if (IsVert(pw)) 
    XFillRectangle(XtDisplay(pw), XtWindow(pw), gc, 
		   off_loc, on_loc, off_size, on_size);
  else
    XFillRectangle(XtDisplay(pw), XtWindow(pw), gc,
		   on_loc, off_loc, on_size, off_size);
}

/*	Function Name: _DrawInternalBorders
 *	Description: Draws the internal borders into the paned widget.
 *	Arguments: pw - the paned widget.
 *                 gc - the GC to use to draw the borders.
 *	Returns: none.
 */

static void
_DrawInternalBorders(pw, gc)
PanedWidget pw;
GC gc;
{
    Widget *childP;
    int on_loc, off_loc;
    unsigned int on_size, off_size;

/*
 * This is an optimization.  Do not paint the internal borders if
 * they are the same color as the background.
 */

    if (pw->core.background_pixel == pw->paned.internal_bp)
	return;

    off_loc = 0; 
    off_size = (unsigned int) PaneSize( (Widget) pw, !IsVert(pw) );
    on_size = (unsigned int) pw->paned.internal_bw;

    ForAllPanes(pw, childP) {
        on_loc = IsVert(pw) ? (*childP)->core.y : (*childP)->core.x;
	on_loc -= (int) on_size;

	_DrawRect( pw, gc, on_loc, off_loc, on_size, off_size);
    }
}

/* 
 * This allows good reuse of code, as well as descriptive function names.
 */

#define DrawInternalBorders(pw) _DrawInternalBorders((pw), (pw)->paned.normgc);
#define EraseInternalBorders(pw) _DrawInternalBorders((pw), (pw)->paned.invgc);

/*	Function Name: _DrawTrackLines
 *	Description: Draws the lines that animate the pane borders when the
 *                   grips are moved.
 *	Arguments: pw - the Paned widget.
 *                 erase - if True then just erase track lines, else
 *                         draw them in.
 *	Returns: none.
 */

static void
_DrawTrackLines(pw, erase)
PanedWidget pw;
Boolean erase;
{
    Widget *childP;
    Pane pane;
    int on_loc, off_loc;
    unsigned int on_size, off_size;

    off_loc = 0;
    off_size = PaneSize( (Widget) pw, !IsVert(pw));

    ForAllPanes(pw, childP) {
        pane = PaneInfo(*childP);
	if ( erase || (pane->olddelta != pane->delta) ) {
	    on_size = pw->paned.internal_bw; 
	    if (!erase) {
	        on_loc = PaneInfo(*childP)->olddelta - (int) on_size;

		_DrawRect( pw, pw->paned.flipgc,
			  on_loc, off_loc, on_size, off_size);
	    }

	    on_loc = PaneInfo(*childP)->delta - (int) on_size;

	    _DrawRect(pw, pw->paned.flipgc,
		      on_loc, off_loc, on_size, off_size);

	    pane->olddelta = pane->delta;
	}
    }
}

/* 
 * This allows good reuse of code, as well as descriptive function names.
 */

#define DrawTrackLines(pw) _DrawTrackLines((pw), FALSE);
#define EraseTrackLines(pw) _DrawTrackLines((pw), TRUE);

/*	Function Name: GetEventLocation
 *	Description: Converts and event to an x and y location.
 *	Arguments: pw - the paned widget.
 *                 event - a pointer to an event.
 *	Returns: if this is a vertical pane then (y) else (x).
 */

static int
GetEventLocation(pw, event)
PanedWidget pw;
XEvent *event;
{
    int x, y;

    switch (event->xany.type) {
        case ButtonPress:
	case ButtonRelease: 
            x = event->xbutton.x_root;
	    y = event->xbutton.y_root;
	    break;
	case KeyPress:
	case KeyRelease:    
	    x = event->xkey.x_root;
	    y = event->xkey.y_root;
	    break;
        case MotionNotify:  
	    x = event->xmotion.x_root;
	    y = event->xmotion.y_root;
	    break;
	default:	    
	    x = pw->paned.start_loc;
	    y = pw->paned.start_loc;
    }
    if (IsVert(pw)) 
        return(y);
    return(x);
}

/*	Function Name: StartGripAdjustment
 *	Description: Starts the grip adjustment procedure.
 *	Arguments: pw - the paned widget.
 *                 grip - the grip widget selected.
 *                 dir - the direction that we are to be moving.
 *	Returns: none.
 */

static void
StartGripAdjustment(pw, grip, dir)
PanedWidget pw;
Widget grip;
Direction dir;
{
    Widget *childP;
    Cursor cursor;

    pw->paned.whichadd = pw->paned.whichsub = (Widget) NULL;

    if (dir == ThisBorderOnly || dir == UpLeftPane)
      pw->paned.whichadd = pw->composite.children[PaneIndex(grip)];
    if (dir == ThisBorderOnly || dir == LowRightPane)
      pw->paned.whichsub = pw->composite.children[PaneIndex(grip) + 1];

/*
 * Change the cursor.
 */

    if (XtIsRealized(grip)) {
        if ( IsVert(pw) ) {
	    if (dir == UpLeftPane) 
	        cursor = pw->paned.adjust_upper_cursor;
	    else if (dir == LowRightPane) 
  	        cursor = pw->paned.adjust_lower_cursor;
	    else {
	        if ( pw->paned.adjust_this_cursor == None)
		    cursor = pw->paned.v_adjust_this_cursor;
		else
		    cursor = pw->paned.adjust_this_cursor;
	    }
	}
	else {
	    if (dir == UpLeftPane) 
	        cursor = pw->paned.adjust_left_cursor;
	    else if (dir == LowRightPane) 
  	        cursor = pw->paned.adjust_right_cursor;
	    else {
	        if (pw->paned.adjust_this_cursor == None)
		    cursor = pw->paned.h_adjust_this_cursor;
		else
		    cursor = pw->paned.adjust_this_cursor;
	    }
	}
    
	XDefineCursor(XtDisplay(grip), XtWindow(grip), cursor);
    }

    EraseInternalBorders(pw);
    ForAllPanes(pw, childP) 
        PaneInfo(*childP)->olddelta = -99;
}

/*	Function Name: MoveGripAdjustment
 *	Description: This routine moves all panes around when a grip is moved. 
 *	Arguments: pw - the paned widget.
 *                 grip - the grip that we are moving.
 *                 dir - the direction the pane we are interested is w.r.t the
 *                       grip.
 *                 loc - location of pointer in proper direction.
 *	Returns: none.
 */

static void
MoveGripAdjustment(pw, grip, dir, loc)
PanedWidget pw;
Widget grip;
Direction dir;
int loc;
{
    int diff, add_size = 0, sub_size = 0;

    diff = loc - pw->paned.start_loc;

    if (pw->paned.whichadd) 
        add_size = PaneSize(pw->paned.whichadd, IsVert(pw) ) + diff;

    if (pw->paned.whichsub) 
        sub_size = PaneSize(pw->paned.whichsub, IsVert(pw) ) - diff;

/*
 * If moving this border only then do not allow either of the borders
 * to go beyond the min or max size allowed.
 */

    if ( (dir == ThisBorderOnly) ) {
      int old_add_size = add_size, old_sub_size;

      AssignMax(add_size, (int) PaneInfo(pw->paned.whichadd)->min);
      AssignMin(add_size, (int) PaneInfo(pw->paned.whichadd)->max);
      if (add_size != old_add_size) 
	  sub_size += old_add_size - add_size;

      old_sub_size = sub_size;
      AssignMax(sub_size, (int) PaneInfo(pw->paned.whichsub)->min);
      AssignMin(sub_size, (int) PaneInfo(pw->paned.whichsub)->max);
      if (sub_size != old_sub_size) return; /* Abort to current sizes. */
    }

    if (add_size != 0)
        PaneInfo(pw->paned.whichadd)->size = add_size;
    if (sub_size != 0)
        PaneInfo(pw->paned.whichsub)->size = sub_size;
    RefigureLocations(pw, PaneIndex(grip), dir);
    DrawTrackLines(pw);
}

/*	Function Name: CommitGripAdjustment
 *	Description: Commits the grip adjustment.
 *	Arguments: pw - the paned widget.
 *	Returns: none
 */

static void
CommitGripAdjustment(pw)
PanedWidget pw;
{
    EraseTrackLines(pw);
    CommitNewLocations(pw);
    DrawInternalBorders(pw);
	   
/*
 * Since the user selected this size then use it as the preferred size. 
 */

    if (pw->paned.whichadd) {
        Pane pane = PaneInfo(pw->paned.whichadd);
	pane->wp_size = pane->size;
    }
    if (pw->paned.whichsub) {
        Pane pane = PaneInfo(pw->paned.whichsub);
	pane->wp_size = pane->size;
    }
}

/*	Function Name: HandleGrip
 *	Description: Handles the grip manipulations.
 *	Arguments: grip - the grip widget that has been moved.
 *                 junk - ** NOT USED **
 *                 call_data - data passed to us from the grip widget.
 *	Returns: none.
 */

/* ARGSUSED */
static void
HandleGrip(grip, junk, callData)
Widget grip;
XtPointer junk, callData;
{
    XawGripCallData call_data = (XawGripCallData)callData;
    PanedWidget pw = (PanedWidget) XtParent(grip);
    int loc;
    char action_type;
    Cursor cursor;
    Direction direction = 0;
    Arg arglist[1];

    action_type = *call_data->params[0];

    if (call_data->num_params == 0                             ||
	(action_type == 'C' && call_data->num_params != 1)      ||
	(action_type != 'C' && call_data->num_params != 2))
      	XtError( "Paned GripAction has been passed incorrect parameters." );

    if (islower(action_type)) action_type = toupper(action_type);

    loc = GetEventLocation(pw, (XEvent *) (call_data->event));

    if (action_type != 'C') {
	if ( isupper(*call_data->params[1]) )
	  direction = (Direction) *call_data->params[1];
	else
	  direction = (Direction) toupper(*call_data->params[1]);
    }

    switch (action_type) {
	case 'S':		/* Start adjustment */
            pw->paned.resize_children_to_pref = FALSE;
            StartGripAdjustment(pw, grip, direction);
	    pw->paned.start_loc = loc;	
	    break;

	case 'M': 
	    MoveGripAdjustment(pw, grip, direction, loc);
	    break;

	case 'C':
	    XtSetArg(arglist[0], XtNcursor, &cursor);
	    XtGetValues(grip, arglist, (Cardinal) 1);
	    XDefineCursor(XtDisplay(grip), XtWindow(grip), cursor);
	    CommitGripAdjustment(pw);
	    break;

	default:
	    XtError( "Paned GripAction(); 1st parameter invalid" );
     }
}

/*	Function Name: ResortChildren
 *	Description: Resorts the children so that all managed children
 *                   are first.
 *	Arguments: pw - the paned widget.
 *	Returns: none.
 */

static void
ResortChildren(pw)
PanedWidget pw;
{
    Widget * unmanagedP, * childP;

    unmanagedP = NULL;
    ForAllChildren(pw, childP) {
       if (!IsPane(*childP) || !XtIsManaged(*childP)) {
	   /*
	    * We only keep track of the first unmanaged pane.
	    */
	   if (unmanagedP == NULL)    
	       unmanagedP = childP;
       }
       else {			     /* must be a managed pane */
	   /*
	    * If an earlier widget was not a managed pane, then swap 
	    */
	   if (unmanagedP != NULL) {	   
	       Widget child = *unmanagedP;
	       *unmanagedP = *childP;
	       *childP = child;
	       childP = unmanagedP;  /* easiest to just back-track */
	       unmanagedP = NULL;    /* in case there is another managed */
	   }
       }
   }
}

/*	Function Name: ManageAndUnmanageGrips
 *	Description: This function manages and unmanages the grips so that
 *                   the managed state of each grip matches that of its pane.
 *	Arguments: pw - the paned widget.
 *	Returns: none.
 */

static void   
ManageAndUnmanageGrips(pw)
PanedWidget pw;
{
   WidgetList managed_grips, unmanaged_grips;
   Widget *managedP, *unmanagedP, *childP;
   Cardinal alloc_size;

   alloc_size = (Cardinal) sizeof(Widget) * pw->composite.num_children / 2;
   managedP = managed_grips = (WidgetList) XtMalloc(alloc_size);
   unmanagedP = unmanaged_grips = (WidgetList) XtMalloc(alloc_size);

   ForAllChildren(pw, childP) 
       if (IsPane(*childP) && HasGrip(*childP))
	   if ( XtIsManaged(*childP) ) 
	       *managedP++ = PaneInfo(*childP)->grip;
	   else
	       *unmanagedP++ = PaneInfo(*childP)->grip;
   
   if (managedP != managed_grips) {
       *unmanagedP++ = *--managedP;   /* Last grip is never managed */
       XtManageChildren( managed_grips, (Cardinal)(managedP - managed_grips) );
   }

   if (unmanagedP != unmanaged_grips)
       XtUnmanageChildren( unmanaged_grips,
			   (Cardinal)(unmanagedP - unmanaged_grips) );

   XtFree((char *)managed_grips);
   XtFree((char *)unmanaged_grips);
}

/*	Function Name: CreateGrip
 *	Description: Creates a grip widget.
 *	Arguments: child - the child that wants a grip to be created for it.
 *	Returns: none.
 */

static void
CreateGrip(child)
Widget child;
{
    PanedWidget pw = (PanedWidget) XtParent(child);
    Arg arglist[2];
    Cardinal num_args = 0;
    Cursor cursor;
     
    XtSetArg(arglist[num_args], XtNtranslations, pw->paned.grip_translations);
    num_args++;
    if ( (cursor = pw->paned.grip_cursor) == None )
        if (IsVert(pw))
	    cursor = pw->paned.v_grip_cursor;
	else
	    cursor = pw->paned.h_grip_cursor;

    XtSetArg(arglist[num_args], XtNcursor, cursor);
    num_args++;
    PaneInfo(child)->grip = XtCreateWidget("grip", gripWidgetClass, (Widget)pw,
					   arglist, num_args);
    
    XtAddCallback(PaneInfo(child)->grip, XtNcallback, 
		  HandleGrip, (XtPointer) child);
}

/*	Function Name: GetGCs
 *	Description: Gets new GC's.
 *	Arguments: w - the paned widget.
 *	Returns: none.
 */

static void
GetGCs(w)
Widget w;
{
    PanedWidget pw = (PanedWidget) w;
    XtGCMask valuemask;
    XGCValues values;

/*
 * Draw pane borders in internal border color. 
 */

    values.foreground = pw->paned.internal_bp;	
    valuemask = GCForeground;
    pw->paned.normgc = XtGetGC(w, valuemask, &values);

/*
 * Erase pane borders with background color. 
 */

    values.foreground = pw->core.background_pixel;	
    valuemask = GCForeground;
    pw->paned.invgc = XtGetGC(w, valuemask, &values);

/*
 * Draw Track lines (animate pane borders) in internal border color ^ bg color.
 */

    values.function = GXinvert;
    values.plane_mask = pw->paned.internal_bp ^ pw->core.background_pixel;
    values.subwindow_mode = IncludeInferiors; 
    valuemask = GCPlaneMask | GCFunction | GCSubwindowMode;
    pw->paned.flipgc = XtGetGC(w, valuemask, &values);
}

/*	Function Name: SetChildrenPrefSizes.
 *	Description: Sets the preferred sizes of the children.
 *	Arguments: pw - the paned widget.
 *	Returns: none.
 */

static void
SetChildrenPrefSizes(pw, off_size)
PanedWidget pw;
Dimension off_size;
{
    Widget * childP;
    Boolean vert = IsVert(pw);
    XtWidgetGeometry request, reply;

    ForAllPanes(pw, childP)
        if ( pw->paned.resize_children_to_pref          ||
	     (PaneInfo(*childP)->size == 0)             ||
	     (PaneInfo(*childP)->resize_to_pref) ) {

	    if (PaneInfo(*childP)->preferred_size != PANED_ASK_CHILD) 
	        PaneInfo(*childP)->wp_size=PaneInfo(*childP)->preferred_size;
	    else {
	        if( vert ) {
		    request.request_mode = CWWidth;
		    request.width = off_size;
		}
		else {
		    request.request_mode = CWHeight;
		    request.height = off_size;
		}

		if ((XtQueryGeometry( *childP, &request, &reply ) 
	                                         == XtGeometryAlmost) &&
		    (reply.request_mode = (vert ? CWHeight : CWWidth)))
		    PaneInfo(*childP)->wp_size = GetRequestInfo(&reply, vert);
		else
		    PaneInfo(*childP)->wp_size = PaneSize(*childP, vert);
	    } 

	    PaneInfo(*childP)->size = PaneInfo(*childP)->wp_size;
	  }
}

/*	Function Name: ChangeAllGripCursors
 *	Description: Changes all the grip cursors.
 *	Arguments: pw - the paned widget.
 *	Returns: none
 */

static void
ChangeAllGripCursors(pw)
PanedWidget pw;
{
    Widget * childP;

    ForAllPanes(pw, childP) {
	Arg arglist[1];
	Cursor cursor;
      
	if ( (cursor = pw->paned.grip_cursor) == None )
	    if ( IsVert(pw) )
	        cursor = pw->paned.v_grip_cursor;
	    else
	        cursor = pw->paned.h_grip_cursor;

	if (HasGrip (*childP)) {
	    XtSetArg(arglist[0], XtNcursor, cursor);
	    XtSetValues(PaneInfo(*childP)->grip, arglist, (Cardinal) 1);
	}
    }
}
      
/************************************************************
 *
 * Stack Manipulation routines (Private).
 *
 ************************************************************/

/*	Function Name: PushPaneStack
 *	Description: Pushes a value onto the pane stack.
 *	Arguments: pw - the paned widget.
 *                 pane - the pane that we are pushing.
 *	Returns: none.
 */

static void
PushPaneStack(pw, pane)
PanedWidget pw;
Pane pane;
{
  PaneStack * stack = (PaneStack *) XtMalloc(sizeof(PaneStack));

  stack->next = pw->paned.stack;
  stack->pane = pane;
  stack->start_size = pane->size;

  pw->paned.stack = stack;
}

/*	Function Name: GetPaneStack
 *	Description: Gets the top value from the pane stack.
 *	Arguments: pw - the paned widget.
 *                 shrink - TRUE if we want to shrink this pane,
 *                          FALSE otherwise.
 * ** RETURNED **  pane - the pane that we are popping.
 * ** RETURNED **  start_size - the size that this pane started at. 
 *	Returns: none.
 */

static void
GetPaneStack(pw, shrink, pane, start_size)
PanedWidget pw;
Boolean shrink;
Pane * pane;
int * start_size;
{
  if (pw->paned.stack == NULL) { 
    *pane = NULL; 
    return;
  }

  *pane = pw->paned.stack->pane;
  *start_size = pw->paned.stack->start_size;

  if (shrink != ((*pane)->size > *start_size)) *pane = NULL;
}

/*	Function Name: PopPaneStack
 *	Description: Pops the top item off the pane stack.
 *	Arguments: pw - the paned widget.
 *	Returns: TRUE if this is not the last element on the stack.
 */

static Boolean
PopPaneStack(pw)
PanedWidget pw;
{
  PaneStack * stack = pw->paned.stack;

  if (stack == NULL) return(FALSE);

  pw->paned.stack = stack->next;
  XtFree((char*)stack);

  if (pw->paned.stack == NULL) return(FALSE);
  return(TRUE);
}

/*	Function Name: ClearPaneStack
 *	Description: removes all entries from the pane stack.
 *	Arguments: pw - the paned widget.
 *	Returns: none
 */

static void
ClearPaneStack(pw)
PanedWidget pw;
{
  while(PopPaneStack(pw));
}

/************************************************************
 *
 * Semi-public routines. 
 *
 ************************************************************/

/*	Function Name: ClassInitialize
 *	Description: The Paned widgets class initialization proc.
 *	Arguments: none.
 *	Returns: none.
 */

static void 
ClassInitialize()
{
    XawInitializeWidgetSet();
    XtAddConverter( XtRString, XtROrientation, XmuCvtStringToOrientation,
		    (XtConvertArgList)NULL, (Cardinal)0 );
}

/* The Geometry Manager only allows changes after Realize if
 * allow_resize is True in the constraints record.  
 * 
 * For vertically paned widgets:
 *
 * It only allows height changes, but offers the requested height
 * as a compromise if both width and height changes were requested.
 *
 * For horizontal widgets the converse is true.
 * As all good Geometry Managers should, we will return No if the
 * request will have no effect; i.e. when the requestor is already
 * of the desired geometry.
 */

static XtGeometryResult GeometryManager(w, request, reply)
Widget w;
XtWidgetGeometry *request, *reply;
{
    PanedWidget pw = (PanedWidget) XtParent(w);
    XtGeometryMask mask = request->request_mode;
    Dimension old_size, old_wpsize, old_paned_size;
    Pane pane = PaneInfo(w);
    Boolean vert = IsVert(pw);
    Dimension on_size, off_size;
    XtGeometryResult result;
    Boolean almost = FALSE;

/*
 * If any of the following is true, disallow the geometry change.
 *
 * o The paned widget is realized and allow_resize is false for the pane.
 * o The child did not ask to change the on_size.
 * o The request is not a width or height request.
 * o The requested size is the same as the current size.
 */

    if ( (XtIsRealized((Widget)pw) && !pane->allow_resize)        ||
	 !(mask & ((vert) ? CWHeight : CWWidth))                  ||
         (mask & ~(CWWidth | CWHeight))                           ||
         (GetRequestInfo(request, vert) ==  PaneSize(w, vert)) ) {
        return XtGeometryNo;
    }

    old_paned_size = PaneSize( (Widget) pw, vert);
    old_wpsize = pane->wp_size;
    old_size = pane->size;

    pane->wp_size = pane->size = GetRequestInfo(request, vert);

    AdjustPanedSize(pw, PaneSize((Widget) pw, !vert), &result, &on_size,
		    &off_size);

/*
 * Fool the Refigure Locations proc to thinking that we are
 * a different on_size;
 */

    if (result != XtGeometryNo) 
	if (vert) 
	    pw->core.height = on_size;
	else 
	    pw->core.width = on_size;
    
    RefigureLocations(pw, PaneIndex(w), AnyPane);

/* 
 * Set up reply struct and reset core on_size.
 */
    
    if (vert) {
        pw->core.height = old_paned_size;
        reply->height = pane->size;
	reply->width = off_size;
    }
    else {
        pw->core.width = old_paned_size;
        reply->height = off_size;
	reply->width = pane->size;
    }    

/*
 * IF either of the following is true.
 *
 * o There was a "off_size" request and the new "off_size" is different
 *   from that requested.
 * o There was no "off_size" request and the new "off_size" is different
 * 
 * o The "on_size" we will allow is different from that requested.
 * 
 * THEN: set almost
 */

    if ( !((vert ? CWWidth : CWHeight) & mask))
        if (vert) 
	    request->width = w->core.width;
	else
	    request->height = w->core.height;

    almost = GetRequestInfo(request, !vert) != GetRequestInfo(reply, !vert);
    almost |= (GetRequestInfo(request, vert) != GetRequestInfo(reply, vert));

    if ( (mask & XtCWQueryOnly) || almost ) {
	pane->wp_size = old_wpsize;
	pane->size = old_size;
	RefigureLocations(pw, PaneIndex(w), AnyPane);
	reply->request_mode = CWWidth | CWHeight;
	if (almost) return XtGeometryAlmost;
    }
    else {
        AdjustPanedSize(pw, PaneSize((Widget) pw, !vert), 
			(XtGeometryResult *)NULL, 
			(Dimension *)NULL, (Dimension *)NULL);
	CommitNewLocations( pw );	/* layout already refigured. */
    }
    return XtGeometryDone;
}

/* ARGSUSED */
static void Initialize(request, new, args, num_args)
Widget request, new;
ArgList args;
Cardinal *num_args;
{
    PanedWidget pw = (PanedWidget)new;

    GetGCs( (Widget) pw);

    pw->paned.recursively_called = False;
    pw->paned.stack = NULL;
    pw->paned.resize_children_to_pref = TRUE;
    pw->paned.num_panes = 0;
}

static void 
Realize(w, valueMask, attributes)
Widget w;
Mask *valueMask;
XSetWindowAttributes *attributes;
{
    PanedWidget pw = (PanedWidget) w;
    Widget * childP;

    if ((attributes->cursor = (pw)->paned.cursor) != None)
	*valueMask |= CWCursor;

    (*SuperClass->core_class.realize) (w, valueMask, attributes);

/*
 * Before we commit the new locations we need to realize all the panes and
 * their grips.
 */

    ForAllPanes(pw, childP) {
        XtRealizeWidget( *childP );
	if (HasGrip (*childP))
	    XtRealizeWidget( PaneInfo(*childP)->grip );
    }

    RefigureLocationsAndCommit(w);
    pw->paned.resize_children_to_pref = FALSE;
} /* Realize */

static void 
ReleaseGCs(w)
Widget w;
{
    PanedWidget pw = (PanedWidget)w;

    XtReleaseGC( w, pw->paned.normgc );
    XtReleaseGC( w, pw->paned.invgc );
    XtReleaseGC( w, pw->paned.flipgc );
} 

static void InsertChild(w)
Widget w;
{
   Pane pane = PaneInfo(w);

   /* insert the child widget in the composite children list with the */
   /* superclass insert_child routine.                                */
   (*SuperClass->composite_class.insert_child)(w);

   if (!IsPane(w)) return;

   /* ||| Panes will be added in the order they are created, temporarily */

   if ( pane->show_grip == TRUE ) {
       CreateGrip(w);
       if (pane->min == PANED_GRIP_SIZE) 
	   pane->min = PaneSize(pane->grip, IsVert((PanedWidget) XtParent(w)));
   }
   else {
       if (pane->min == PANED_GRIP_SIZE)
	   pane->min = 1;
       pane->grip = NULL;
   }

   pane->size = 0;
   pane->paned_adjusted_me = FALSE;

} /* InsertChild */

static void DeleteChild(w)
Widget w;
{
    /* remove the subwidget info and destroy the grip */
   
    if ( IsPane(w) && HasGrip(w) ) XtDestroyWidget(PaneInfo(w)->grip);

    /* delete the child widget in the composite children list with the */
    /* superclass delete_child routine.                                */
    (*SuperClass->composite_class.delete_child) (w);

} /* DeleteChild */

static void ChangeManaged(w)
   Widget w;
{
   PanedWidget pw = (PanedWidget)w;
   Boolean vert = IsVert(pw);
   Dimension size;
   Widget *childP;

   if (pw->paned.recursively_called++) return;

/*
 * If the size is zero then set it to the size of the widest or tallest pane.
 */

   if ( (size = PaneSize( (Widget) pw, !vert )) == 0) {
       size = 1;
       ForAllChildren(pw, childP)
	   if ( XtIsManaged(*childP) && (PaneSize( *childP, !vert ) > size) )
	       size = PaneSize( *childP, !vert );
   }

   ManageAndUnmanageGrips(pw);
   pw->paned.recursively_called = False;
   ResortChildren(pw);		

   pw->paned.num_panes = 0;
   ForAllChildren(pw, childP) 
       if ( IsPane(*childP) )
	   if ( XtIsManaged(*childP) ) {
	       Pane pane = PaneInfo(*childP);
	       if (HasGrip(*childP))
		   PaneInfo(pane->grip)->position = pw->paned.num_panes;
	       pane->position = pw->paned.num_panes; /*TEMPORY -CDP 3/89 */
	       pw->paned.num_panes++;
	   }
	   else
	       break;		/* This list is already sorted. */

   SetChildrenPrefSizes( (PanedWidget) w, size);

/*
 * ForAllPanes can now be used. 
 */

   if ( PaneSize((Widget) pw, vert) == 0 ) 
       AdjustPanedSize(pw, size, (XtGeometryResult *)NULL, 
			(Dimension *)NULL, (Dimension *)NULL);

   if (XtIsRealized( (Widget) pw)) 
       RefigureLocationsAndCommit( (Widget) pw); 

} /* ChangeManaged */

/*	Function Name: Resize
 *	Description: The paned widget's resize proc.
 *	Arguments: w - the paned widget.
 *	Returns: none.
 */

static void
Resize(w)
Widget w;
{
    SetChildrenPrefSizes( (PanedWidget) w,
			  PaneSize(w, !IsVert((PanedWidget) w)) );
    RefigureLocationsAndCommit(w);
}

/* ARGSUSED */
static void
Redisplay(w, event, region)
Widget w;
XEvent * event;			/* unused. */
Region region;			/* unused. */
{
    DrawInternalBorders( (PanedWidget) w);
}

/* ARGSUSED */
static Boolean 
SetValues(old, request, new, args, num_args)
Widget old, request, new;
ArgList args;
Cardinal *num_args;
{
    PanedWidget old_pw = (PanedWidget) old;
    PanedWidget new_pw = (PanedWidget) new;
    Boolean redisplay = FALSE;

    if ( (old_pw->paned.cursor != new_pw->paned.cursor) && XtIsRealized(new))
        XDefineCursor(XtDisplay(new), XtWindow(new), new_pw->paned.cursor);

    if ( (old_pw->paned.internal_bp != new_pw->paned.internal_bp) ||
	 (old_pw->core.background_pixel != new_pw->core.background_pixel) ) {
        ReleaseGCs(old);
	GetGCs(new);
	redisplay = TRUE;
    }

    if ( (old_pw->paned.grip_cursor != new_pw->paned.grip_cursor)     ||
 	 (old_pw->paned.v_grip_cursor != new_pw->paned.v_grip_cursor) ||
 	 (old_pw->paned.h_grip_cursor != new_pw->paned.h_grip_cursor) ) {
        ChangeAllGripCursors(new_pw);
    }
	
    if ( IsVert(old_pw) != IsVert(new_pw)) {
/*
 * We are fooling the paned widget into thinking that is needs to
 * fully refigure everything, which is what we want.
 */
        if (IsVert(new_pw))
	    new_pw->core.width = 0;
	else
	    new_pw->core.height = 0;

	new_pw->paned.resize_children_to_pref = TRUE;
        ChangeManaged(new);	/* Seems weird, but does the right thing. */
	new_pw->paned.resize_children_to_pref = FALSE;
	if (new_pw->paned.grip_cursor == None)
	    ChangeAllGripCursors(new_pw);
	return(TRUE);
    }

    if (old_pw->paned.internal_bw != new_pw->paned.internal_bw) {
        AdjustPanedSize( new_pw, PaneSize(new, !IsVert(old_pw)),
			 (XtGeometryResult *)NULL, 
			 (Dimension *)NULL, (Dimension *)NULL);
        RefigureLocationsAndCommit(new);
	return(TRUE);		/* We have done a full configuration, return.*/
    }
    
    if ( (old_pw->paned.grip_indent != new_pw->paned.grip_indent) &&
	 (XtIsRealized(new)) ) {
        CommitNewLocations(new_pw);
	redisplay = TRUE;
    }

    return (redisplay);
} /* SetValues */


/* ARGSUSED */
static Boolean 
PaneSetValues(old, request, new, args, num_args)
Widget old, request, new;
ArgList args;
Cardinal *num_args;
{
    Pane old_pane = PaneInfo(old);
    Pane new_pane = PaneInfo(new);
    Boolean redisplay = FALSE;

    /* Check for new min and max. */

    if (old_pane->min != new_pane->min || old_pane->max != new_pane->max)
	XawPanedSetMinMax(new, (int)new_pane->min, (int)new_pane->max);

    /* Check for change in XtNshowGrip. */

    if (old_pane->show_grip != new_pane->show_grip)
        if (new_pane->show_grip == TRUE) {
	    CreateGrip(new);
	    if (XtIsRealized(XtParent(new))) {
	        if (XtIsManaged(new)) /* if paned is unrealized this will
				       happen automatically at realize time.*/
		    XtManageChild(PaneInfo(new)->grip); /* manage the grip. */
		XtRealizeWidget(PaneInfo(new)->grip); /* realize the grip. */
	        CommitNewLocations( (PanedWidget) XtParent(new) );
	    }
	}
	else if ( HasGrip(old) ) {
	    XtDestroyWidget( old_pane->grip );
	    new_pane->grip = NULL;
	    redisplay = TRUE;
	}

  /* ||| need to look at position changes */

    return(redisplay);
}

/************************************************************
 *
 * Public routines. 
 *
 ************************************************************/

/*	Function Name: XawPanedSetMinMax
 *	Description: Sets the min and max size for a pane.
 *	Arguments: widget - the widget that is a child of the Paned widget.
 *                 min, max - the new min and max size for the pane.
 *	Returns: none.
 */

void 
#if NeedFunctionPrototypes
XawPanedSetMinMax(Widget widget, int min, int max)
#else
XawPanedSetMinMax(widget, min, max)
Widget widget;
int min, max;
#endif
{
    Pane pane = PaneInfo(widget);

    pane->min = min;
    pane->max = max;
    RefigureLocationsAndCommit( widget->core.parent );
}

/*	Function Name: XawPanedGetMinMax
 *	Description: Gets the min and max size for a pane.
 *	Arguments: widget - the widget that is a child of the Paned widget.
 ** RETURNED **    min, max - the current min and max size for the pane.
 *	Returns: none.
 */

void 
#if NeedFunctionPrototypes
XawPanedGetMinMax(Widget widget, int *min, int *max)
#else
XawPanedGetMinMax(widget, min, max)
Widget widget;
int    *min, *max;
#endif
{
    Pane pane = PaneInfo(widget);

    *min = pane->min;
    *max = pane->max;
}

/*	Function Name: XawPanedSetRefigureMode
 *	Description: Allows a flag to be set the will inhibit 
 *                   the paned widgets relayout routine.
 *	Arguments: w - the paned widget.
 *                 mode - if FALSE then inhibit refigure.
 *	Returns: none.
 */

void 
#if NeedFunctionPrototypes
XawPanedSetRefigureMode(Widget w,
#if NeedWidePrototypes
			int mode)
#else
			Boolean mode)
#endif
#else
XawPanedSetRefigureMode(w, mode)
Widget w;
Boolean mode;
#endif
{
    ((PanedWidget) w)->paned.refiguremode = mode;
    RefigureLocationsAndCommit( w );
}

/*	Function Name: XawPanedGetNumSub
 *	Description: Returns the number of panes in the paned widget.
 *	Arguments: w - the paned widget.
 *	Returns: the number of panes in the paned widget.
 */

int 
#if NeedFunctionPrototypes
XawPanedGetNumSub(Widget w)
#else
XawPanedGetNumSub(w)
Widget w;
#endif
{
    return ((PanedWidget)w)->paned.num_panes;
}

/*	Function Name: XawPanedAllowResize
 *	Description: Allows a flag to be set that determines if the paned
 *                   widget will allow geometry requests from this child
 *	Arguments: widget - a child of the paned widget.
 *	Returns: none.
 */

void 
#if NeedFunctionPrototypes
XawPanedAllowResize(Widget widget,
#if NeedWidePrototypes
		    int allow_resize)
#else
		    Boolean allow_resize)
#endif
#else
XawPanedAllowResize(widget, allow_resize)
Widget widget;
Boolean allow_resize;
#endif
{
    PaneInfo(widget)->allow_resize = allow_resize;
}
