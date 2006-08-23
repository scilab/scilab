/* $XConsortium: List.c,v 1.39 94/04/17 20:12:15 kaleb Exp $ */

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
 * List.c - List widget
 *
 * This is a List widget.  It allows the user to select an item in a list and
 * notifies the application through a callback function.
 *
 *	Created: 	8/13/88
 *	By:		Chris D. Peterson
 *                      MIT X Consortium
 */

#include <stdio.h>
#include <ctype.h>

#include <X11/IntrinsicP.h>
#include <X11/StringDefs.h>

#include <X11/Xmu/Drawing.h>

#include <X11/Xaw3d/XawInit.h>
#include <X11/Xaw3d/ListP.h>

/* These added so widget knows whether its height, width are user selected.
I also added the freedoms member of the list widget part. */

#define HeightLock  1
#define WidthLock   2
#define LongestLock 4

#define HeightFree( w )  !(((ListWidget)(w))->list.freedoms & HeightLock )
#define WidthFree( w )   !(((ListWidget)(w))->list.freedoms & WidthLock )
#define LongestFree( w ) !(((ListWidget)(w))->list.freedoms & LongestLock )

/* 
 * Default Translation table.
 */

static char defaultTranslations[] =  
  "<Btn1Down>:   Set()\n\
   <Btn1Up>:     Notify()";

/****************************************************************
 *
 * Full class record constant
 *
 ****************************************************************/

/* Private Data */

#define offset(field) XtOffset(ListWidget, field)

static XtResource resources[] = {
    {XtNforeground, XtCForeground, XtRPixel, sizeof(Pixel),
	offset(list.foreground), XtRString, XtDefaultForeground},
    {XtNcursor, XtCCursor, XtRCursor, sizeof(Cursor),
       offset(simple.cursor), XtRString, "left_ptr"},
    {XtNfont,  XtCFont, XtRFontStruct, sizeof(XFontStruct *),
	offset(list.font),XtRString, XtDefaultFont},
    {XtNfontSet,  XtCFontSet, XtRFontSet, sizeof(XFontSet ),
	offset(list.fontset),XtRString, XtDefaultFontSet},
    {XtNlist, XtCList, XtRPointer, sizeof(char **),
       offset(list.list), XtRString, NULL},
    {XtNdefaultColumns, XtCColumns, XtRInt,  sizeof(int),
	offset(list.default_cols), XtRImmediate, (XtPointer)2},
    {XtNlongest, XtCLongest, XtRInt,  sizeof(int),
	offset(list.longest), XtRImmediate, (XtPointer)0},
    {XtNnumberStrings, XtCNumberStrings, XtRInt,  sizeof(int),
	offset(list.nitems), XtRImmediate, (XtPointer)0},
    {XtNpasteBuffer, XtCBoolean, XtRBoolean,  sizeof(Boolean),
	offset(list.paste), XtRImmediate, (XtPointer) False},
    {XtNforceColumns, XtCColumns, XtRBoolean,  sizeof(Boolean),
	offset(list.force_cols), XtRImmediate, (XtPointer) False},
    {XtNverticalList, XtCBoolean, XtRBoolean,  sizeof(Boolean),
	offset(list.vertical_cols), XtRImmediate, (XtPointer) False},
    {XtNinternalWidth, XtCWidth, XtRDimension,  sizeof(Dimension),
	offset(list.internal_width), XtRImmediate, (XtPointer)2},
    {XtNinternalHeight, XtCHeight, XtRDimension, sizeof(Dimension),
	offset(list.internal_height), XtRImmediate, (XtPointer)2},
    {XtNcolumnSpacing, XtCSpacing, XtRDimension,  sizeof(Dimension),
	offset(list.column_space), XtRImmediate, (XtPointer)6},
    {XtNrowSpacing, XtCSpacing, XtRDimension,  sizeof(Dimension),
	offset(list.row_space), XtRImmediate, (XtPointer)2},
    {XtNcallback, XtCCallback, XtRCallback, sizeof(XtPointer),
        offset(list.callback), XtRCallback, NULL},
};

static void Initialize();
static void ChangeSize();
static void Resize();
static void Redisplay();
static void Destroy();
static Boolean Layout();
static XtGeometryResult PreferredGeom();
static Boolean SetValues();
static void Notify(), Set(), Unset();

static XtActionsRec actions[] = {
      {"Notify",         Notify},
      {"Set",            Set},
      {"Unset",          Unset},
};

ListClassRec listClassRec = {
  {
/* core_class fields */	
    /* superclass	  	*/	(WidgetClass) &simpleClassRec,
    /* class_name	  	*/	"List",
    /* widget_size	  	*/	sizeof(ListRec),
    /* class_initialize   	*/	XawInitializeWidgetSet,
    /* class_part_initialize	*/	NULL,
    /* class_inited       	*/	FALSE,
    /* initialize	  	*/	Initialize,
    /* initialize_hook		*/	NULL,
    /* realize		  	*/	XtInheritRealize,
    /* actions		  	*/	actions,
    /* num_actions	  	*/	XtNumber(actions),
    /* resources	  	*/	resources,
    /* num_resources	  	*/	XtNumber(resources),
    /* xrm_class	  	*/	NULLQUARK,
    /* compress_motion	  	*/	TRUE,
    /* compress_exposure  	*/	FALSE,
    /* compress_enterleave	*/	TRUE,
    /* visible_interest	  	*/	FALSE,
    /* destroy		  	*/	Destroy,
    /* resize		  	*/	Resize,
    /* expose		  	*/	Redisplay,
    /* set_values	  	*/	SetValues,
    /* set_values_hook		*/	NULL,
    /* set_values_almost	*/	XtInheritSetValuesAlmost,
    /* get_values_hook		*/	NULL,
    /* accept_focus	 	*/	NULL,
    /* version			*/	XtVersion,
    /* callback_private   	*/	NULL,
    /* tm_table		   	*/	defaultTranslations,
   /* query_geometry		*/      PreferredGeom,
  },
/* Simple class fields initialization */
  {
    /* change_sensitive		*/	XtInheritChangeSensitive
  },
/* List class fields initialization */
  {
    /* not used			*/	0
  },
};

WidgetClass listWidgetClass = (WidgetClass)&listClassRec;

/****************************************************************
 *
 * Private Procedures
 *
 ****************************************************************/

static void GetGCs(w)
Widget w;
{
    XGCValues	values;
    ListWidget lw = (ListWidget) w;    

    values.foreground	= lw->list.foreground;
    values.font		= lw->list.font->fid;

    if ( lw->simple.international == True )
        lw->list.normgc = XtAllocateGC( w, 0, (unsigned) GCForeground,
				 &values, GCFont, 0 );
    else
        lw->list.normgc = XtGetGC( w, (unsigned) GCForeground | GCFont,
				 &values);

    values.foreground	= lw->core.background_pixel;

    if ( lw->simple.international == True )
        lw->list.revgc = XtAllocateGC( w, 0, (unsigned) GCForeground,
				 &values, GCFont, 0 );
    else
        lw->list.revgc = XtGetGC( w, (unsigned) GCForeground | GCFont,
				 &values);

    values.tile       = XmuCreateStippledPixmap(XtScreen(w), 
						lw->list.foreground,
						lw->core.background_pixel,
						lw->core.depth);
    values.fill_style = FillTiled;

    if ( lw->simple.international == True )
        lw->list.graygc = XtAllocateGC( w, 0, (unsigned) GCTile | GCFillStyle,
			      &values, GCFont, 0 );
    else
        lw->list.graygc = XtGetGC( w, (unsigned) GCFont | GCTile | GCFillStyle,
			      &values);
}


/* CalculatedValues()
 *
 * does routine checks/computations that must be done after data changes
 * but won't hurt if accidently called
 *
 * These calculations were needed in SetValues.  They were in ResetList.
 * ResetList called ChangeSize, which made an XtGeometryRequest.  You
 * MAY NOT change your geometry from within a SetValues. (Xt man,
 * sect. 9.7.2)  So, I factored these changes out. */

static void CalculatedValues( w )
Widget w;
{
    int i, len;

    ListWidget lw = (ListWidget) w;

    /* If list is NULL then the list will just be the name of the widget. */

    if (lw->list.list == NULL) {
      lw->list.list = &(lw->core.name);
      lw->list.nitems = 1;
    }

    /* Get number of items. */

    if (lw->list.nitems == 0)
        for ( ; lw->list.list[lw->list.nitems] != NULL ; lw->list.nitems++);

    /* Get column width. */

    if ( LongestFree( lw ) )  {

        lw->list.longest = 0; /* so it will accumulate real longest below */

        for ( i = 0 ; i < lw->list.nitems; i++)  {
            if ( lw->simple.international == True )
	        len = XmbTextEscapement(lw->list.fontset, lw->list.list[i],
			 			    strlen(lw->list.list[i]));
            else
                len = XTextWidth(lw->list.font, lw->list.list[i],
			 			    strlen(lw->list.list[i]));
            if (len > lw->list.longest)
                lw->list.longest = len;
        }
    }

    lw->list.col_width = lw->list.longest + lw->list.column_space;
}

/*	Function Name: ResetList
 *	Description: Resets the new list when important things change.
 *	Arguments: w - the widget.
 *                 changex, changey - allow the height or width to change?
 *
 *	Returns: TRUE if width or height have been changed
 */

static void
ResetList( w, changex, changey )
Widget w;
Boolean changex, changey;
{
    Dimension width = w->core.width;
    Dimension height = w->core.height;

    CalculatedValues( w );

    if( Layout( w, changex, changey, &width, &height ) )
      ChangeSize( w, width, height );
}

/*	Function Name: ChangeSize.
 *	Description: Laysout the widget.
 *	Arguments: w - the widget to try change the size of.
 *	Returns: none.
 */

static void
ChangeSize(w, width, height)
Widget w;
Dimension width, height;
{
    XtWidgetGeometry request, reply;

    request.request_mode = CWWidth | CWHeight;
    request.width = width;
    request.height = height;

    switch ( XtMakeGeometryRequest(w, &request, &reply) ) {
    case XtGeometryYes:
        break;
    case XtGeometryNo:
        break;
    case XtGeometryAlmost:
	Layout(w, (request.height != reply.height),
	          (request.width != reply.width),
	       &(reply.width), &(reply.height));
	request = reply;
	switch (XtMakeGeometryRequest(w, &request, &reply) ) {
	case XtGeometryYes:
	case XtGeometryNo:
	    break;
	case XtGeometryAlmost:
	    request = reply;
	    Layout(w, FALSE, FALSE, &(request.width), &(request.height));
	    request.request_mode = CWWidth | CWHeight;
	    XtMakeGeometryRequest(w, &request, &reply);
	    break;
	default:
	  XtAppWarning(XtWidgetToApplicationContext(w),
		       "List Widget: Unknown geometry return.");
	  break;
	}
	break;
    default:
	XtAppWarning(XtWidgetToApplicationContext(w),
		     "List Widget: Unknown geometry return.");
	break;
    }
}

/*	Function Name: Initialize
 *	Description: Function that initilizes the widget instance.
 *	Arguments: junk - NOT USED.
 *                 new  - the new widget.
 *	Returns: none
 */

/* ARGSUSED */
static void 
Initialize(junk, new, args, num_args)
Widget junk, new;
ArgList args;
Cardinal *num_args;
{
    ListWidget lw = (ListWidget) new;

/* 
 * Initialize all private resources.
 */

    /* record for posterity if we are free */
    lw->list.freedoms = (lw->core.width != 0) * WidthLock +
                        (lw->core.height != 0) * HeightLock +
                        (lw->list.longest != 0) * LongestLock;

    GetGCs(new);

    /* Set row height. based on font or fontset */

    if (lw->simple.international == True )
        lw->list.row_height =
                     XExtentsOfFontSet(lw->list.fontset)->max_ink_extent.height
                        + lw->list.row_space;
    else
        lw->list.row_height = lw->list.font->max_bounds.ascent
			+ lw->list.font->max_bounds.descent
			+ lw->list.row_space;

    ResetList( new, WidthFree( lw ), HeightFree( lw ) );

    lw->list.highlight = lw->list.is_highlighted = NO_HIGHLIGHT;

} /* Initialize */

/*	Function Name: CvtToItem
 *	Description: Converts Xcoord to item number of item containing that
 *                   point.
 *	Arguments: w - the list widget.
 *                 xloc, yloc - x location, and y location.
 *	Returns: the item number.
 */

static int
CvtToItem(w, xloc, yloc, item)
Widget w;
int xloc, yloc;
int *item;
{
    int one, another;
    ListWidget lw = (ListWidget) w;
    int ret_val = OKAY;

    if (lw->list.vertical_cols) {
        one = lw->list.nrows * ((xloc - (int) lw->list.internal_width)
	    / lw->list.col_width);
        another = (yloc - (int) lw->list.internal_height) 
	        / lw->list.row_height;
	 /* If out of range, return minimum possible value. */
	if (another >= lw->list.nrows) {
	    another = lw->list.nrows - 1;
	    ret_val = OUT_OF_RANGE;
	}
    }
    else {
        one = (lw->list.ncols * ((yloc - (int) lw->list.internal_height) 
              / lw->list.row_height)) ;
	/* If in right margin handle things right. */
        another = (xloc - (int) lw->list.internal_width) / lw->list.col_width;
	if (another >= lw->list.ncols) {
	    another = lw->list.ncols - 1; 
	    ret_val = OUT_OF_RANGE;
	}
    }  
    if ((xloc < 0) || (yloc < 0))
        ret_val = OUT_OF_RANGE;
    if (one < 0) one = 0;
    if (another < 0) another = 0;
    *item = one + another;
    if (*item >= lw->list.nitems) return(OUT_OF_RANGE);
    return(ret_val);
}

/*	Function Name: FindCornerItems.
 *	Description: Find the corners of the rectangle in item space.
 *	Arguments: w - the list widget.
 *                 event - the event structure that has the rectangle it it.
 *                 ul_ret, lr_ret - the corners ** RETURNED **.
 *	Returns: none.
 */

static void
FindCornerItems(w, event, ul_ret, lr_ret)
Widget w;
XEvent * event;
int *ul_ret, *lr_ret;
{
    int xloc, yloc;

    xloc = event->xexpose.x;
    yloc = event->xexpose.y;
    CvtToItem(w, xloc, yloc, ul_ret);
    xloc += event->xexpose.width;
    yloc += event->xexpose.height;
    CvtToItem(w, xloc, yloc, lr_ret);
}

/*	Function Name: ItemInRectangle
 *	Description: returns TRUE if the item passed is in the given rectangle.
 *	Arguments: w - the list widget.
 *                 ul, lr - corners of the rectangle in item space.
 *                 item - item to check.
 *	Returns: TRUE if the item passed is in the given rectangle.
 */

static Boolean
ItemInRectangle(w, ul, lr, item)
Widget w;
int ul, lr, item;
{
    ListWidget lw = (ListWidget) w;
    int mod_item;
    int things;
    
    if (item < ul || item > lr) 
        return(FALSE);
    if (lw->list.vertical_cols)
        things = lw->list.nrows;
    else
        things = lw->list.ncols;

    mod_item = item % things;
    if ( (mod_item >= ul % things) && (mod_item <= lr % things ) )
        return(TRUE);
    return(FALSE);
}


/* HighlightBackground()
 *
 * Paints the color of the background for the given item.  It performs
 * clipping to the interior of internal_width/height by hand, as its a
 * simple calculation and probably much faster than using Xlib and a clip mask.
 *
 *  x, y - ul corner of the area item occupies.
 *  gc - the gc to use to paint this rectangle */

static void
HighlightBackground( w, x, y, gc )
Widget w;
int x, y;
GC gc;
{
    ListWidget lw = (ListWidget) w;

    /* easy to clip the rectangle by hand and probably alot faster than Xlib */

    Dimension width               = lw->list.col_width;
    Dimension height              = lw->list.row_height;
    Dimension frame_limited_width = w->core.width - lw->list.internal_width - x;
    Dimension frame_limited_height= w->core.height- lw->list.internal_height- y;

    /* Clip the rectangle width and height to the edge of the drawable area */

    if  ( width > frame_limited_width )
        width = frame_limited_width;
    if  ( height> frame_limited_height)
        height = frame_limited_height;

    /* Clip the rectangle x and y to the edge of the drawable area */

    if ( x < lw->list.internal_width ) {
        width = width - ( lw->list.internal_width - x );
        x = lw->list.internal_width;
    }
    if ( y < lw->list.internal_height) {
        height = height - ( lw->list.internal_height - x );
        y = lw->list.internal_height;
    }
    XFillRectangle( XtDisplay( w ), XtWindow( w ), gc, x, y,
		    width, height );
}


/* ClipToShadowInteriorAndLongest()
 *
 * Converts the passed gc so that any drawing done with that GC will not
 * write in the empty margin (specified by internal_width/height) (which also
 * prevents erasing the shadow.  It also clips against the value longest.
 * If the user doesn't set longest, this has no effect (as longest is the
 * maximum of all item lengths).  If the user does specify, say, 80 pixel
 * columns, though, this prevents items from overwriting other items. */

static void ClipToShadowInteriorAndLongest(lw, gc_p, x)
    ListWidget lw; 
    GC* gc_p;
    Dimension x;
{
    XRectangle rect;

    rect.x = x;
    rect.y = lw->list.internal_height;
    rect.height = lw->core.height - lw->list.internal_height * 2;
    rect.width = lw->core.width - lw->list.internal_width - x;
    if ( rect.width > lw->list.longest )
        rect.width = lw->list.longest;

    XSetClipRectangles( XtDisplay((Widget)lw),*gc_p,0,0,&rect,1,YXBanded );
}


/*  PaintItemName()
 *
 *  paints the name of the item in the appropriate location.
 *  w - the list widget.
 *  item - the item to draw.
 *
 *  NOTE: no action taken on an unrealized widget. */

static void
PaintItemName(w, item)
Widget w;
int item;
{
    char * str;
    GC gc;
    int x, y, str_y;
    ListWidget lw = (ListWidget) w;
    XFontSetExtents *ext  = XExtentsOfFontSet(lw->list.fontset);

    if (!XtIsRealized(w)) return; /* Just in case... */

    if (lw->list.vertical_cols) {
	x = lw->list.col_width * (item / lw->list.nrows)
	  + lw->list.internal_width;
        y = lw->list.row_height * (item % lw->list.nrows)
	  + lw->list.internal_height;
    }
    else {
        x = lw->list.col_width * (item % lw->list.ncols)
	  + lw->list.internal_width;
        y = lw->list.row_height * (item / lw->list.ncols)
	  + lw->list.internal_height;
    }

    if ( lw->simple.international == True )
        str_y = y + abs(ext->max_ink_extent.y); 
    else
        str_y = y + lw->list.font->max_bounds.ascent;

    if (item == lw->list.is_highlighted) {
        if (item == lw->list.highlight) {
            gc = lw->list.revgc;
	    HighlightBackground(w, x, y, lw->list.normgc);
	}
        else {
	    if (XtIsSensitive(w)) 
	        gc = lw->list.normgc;
	    else
	        gc = lw->list.graygc;
	    HighlightBackground(w, x, y, lw->list.revgc);
	    lw->list.is_highlighted = NO_HIGHLIGHT;
        }
    }
    else {
        if (item == lw->list.highlight) {
            gc = lw->list.revgc;
	    HighlightBackground(w, x, y, lw->list.normgc);
	    lw->list.is_highlighted = item;
	}
	else {
	    if (XtIsSensitive(w)) 
	        gc = lw->list.normgc;
	    else
	        gc = lw->list.graygc;
	}
    }

    /* List's overall width contains the same number of inter-column
    column_space's as columns.  There should thus be a half
    column_width margin on each side of each column.
    The row case is symmetric. */

    x     += lw->list.column_space / 2;
    str_y += lw->list.row_space    / 2;

    str =  lw->list.list[item];	/* draw it */

    ClipToShadowInteriorAndLongest( lw, &gc, x );

    if ( lw->simple.international == True )
        XmbDrawString( XtDisplay( w ), XtWindow( w ), lw->list.fontset,
		  gc, x, str_y, str, strlen( str ) );
    else
        XDrawString( XtDisplay( w ), XtWindow( w ),
		  gc, x, str_y, str, strlen( str ) );

    XSetClipMask( XtDisplay( w ), gc, None );
}

    
/* Redisplay()
 *
 * Repaints the widget window on expose events.
 * w - the list widget.
 * event - the expose event for this repaint.
 * junk - not used, unless three-d patch enabled. */

/* ARGSUSED */
static void 
Redisplay(w, event, junk)
Widget w;
XEvent *event;
Region junk;
{
    int item;			/* an item to work with. */
    int ul_item, lr_item;       /* corners of items we need to paint. */
    ListWidget lw = (ListWidget) w;

    if (event == NULL) {	/* repaint all. */
        ul_item = 0;
	lr_item = lw->list.nrows * lw->list.ncols - 1;
	XClearWindow(XtDisplay(w), XtWindow(w));
    }
    else
        FindCornerItems(w, event, &ul_item, &lr_item);
    
    for (item = ul_item; (item <= lr_item && item < lw->list.nitems) ; item++)
      if (ItemInRectangle(w, ul_item, lr_item, item))
	PaintItemName(w, item);
}


/* PreferredGeom()
 *
 * This tells the parent what size we would like to be
 * given certain constraints.
 * w - the widget.
 * intended - what the parent intends to do with us.
 * requested - what we want to happen. */

static XtGeometryResult 
PreferredGeom(w, intended, requested)
Widget w;
XtWidgetGeometry *intended, *requested;
{
    Dimension new_width, new_height;
    Boolean change, width_req, height_req;
    
    width_req = intended->request_mode & CWWidth;
    height_req = intended->request_mode & CWHeight;

    if (width_req)
      new_width = intended->width;
    else
      new_width = w->core.width;

    if (height_req)
      new_height = intended->height;
    else
      new_height = w->core.height;

    requested->request_mode = 0;
    
/*
 * We only care about our height and width.
 */

    if ( !width_req && !height_req)
      return(XtGeometryYes);
    
    change = Layout(w, !width_req, !height_req, &new_width, &new_height);

    requested->request_mode |= CWWidth;
    requested->width = new_width;
    requested->request_mode |= CWHeight;
    requested->height = new_height;

    if (change)
        return(XtGeometryAlmost);
    return(XtGeometryYes);
}


/* Resize()
 *
 * resizes the widget, by changing the number of rows and columns. */

static void
Resize(w)
    Widget w;
{
    Dimension width, height;

    width = w->core.width;
    height = w->core.height;

    if (Layout(w, FALSE, FALSE, &width, &height))
	XtAppWarning(XtWidgetToApplicationContext(w),
	   "List Widget: Size changed when it shouldn't have when resising.");
}


/* Layout()
 *
 * lays out the item in the list.
 * w - the widget.
 * xfree, yfree - TRUE if we are free to resize the widget in
 *                this direction.
 * width, height- the is the current width and height that we are going
 *                we are going to layout the list widget to,
 *                depending on xfree and yfree of course.
 *                               
 * RETURNS: TRUE if width or height have been changed. */

static Boolean
Layout(w, xfree, yfree, width, height)
Widget w;
Boolean xfree, yfree;
Dimension *width, *height;
{
    ListWidget lw = (ListWidget) w;
    Boolean change = FALSE;
    
/* 
 * If force columns is set then always use number of columns specified
 * by default_cols.
 */

    if (lw->list.force_cols) {
        lw->list.ncols = lw->list.default_cols;
	if (lw->list.ncols <= 0) lw->list.ncols = 1;
	/* 12/3 = 4 and 10/3 = 4, but 9/3 = 3 */
	lw->list.nrows = ( ( lw->list.nitems - 1) / lw->list.ncols) + 1 ;
	if (xfree) {		/* If allowed resize width. */

            /* this counts the same number
            of inter-column column_space 's as columns.  There should thus be a
            half column_space margin on each side of each column...*/

	    *width = lw->list.ncols * lw->list.col_width
	           + 2 * lw->list.internal_width;
	    change = TRUE;
	}
	if (yfree) {		/* If allowed resize height. */
	    *height = (lw->list.nrows * lw->list.row_height)
                    + 2 * lw->list.internal_height;
	    change = TRUE;
	}
	return(change);
    }

/*
 * If both width and height are free to change the use default_cols
 * to determine the number columns and set new width and height to
 * just fit the window.
 */

    if (xfree && yfree) {
        lw->list.ncols = lw->list.default_cols;
	if (lw->list.ncols <= 0) lw->list.ncols = 1;
	lw->list.nrows = ( ( lw->list.nitems - 1) / lw->list.ncols) + 1 ;
        *width = lw->list.ncols * lw->list.col_width
	       + 2 * lw->list.internal_width;
	*height = (lw->list.nrows * lw->list.row_height)
                + 2 * lw->list.internal_height;
	change = TRUE;
    }
/* 
 * If the width is fixed then use it to determine the number of columns.
 * If the height is free to move (width still fixed) then resize the height
 * of the widget to fit the current list exactly.
 */
    else if (!xfree) {
        lw->list.ncols = ( (int)(*width - 2 * lw->list.internal_width)
	                    / (int)lw->list.col_width);
	if (lw->list.ncols <= 0) lw->list.ncols = 1;
	lw->list.nrows = ( ( lw->list.nitems - 1) / lw->list.ncols) + 1 ;
	if ( yfree ) {
  	    *height = (lw->list.nrows * lw->list.row_height)
		    + 2 * lw->list.internal_height;
	    change = TRUE;
	}
    }
/* 
 * The last case is xfree and !yfree we use the height to determine
 * the number of rows and then set the width to just fit the resulting
 * number of columns.
 */
    else if (!yfree) {		/* xfree must be TRUE. */
        lw->list.nrows = (int)(*height - 2 * lw->list.internal_height) 
	                 / (int)lw->list.row_height;
	if (lw->list.nrows <= 0) lw->list.nrows = 1;
	lw->list.ncols = (( lw->list.nitems - 1 ) / lw->list.nrows) + 1;
	*width = lw->list.ncols * lw->list.col_width 
	       + 2 * lw->list.internal_width;
	change = TRUE;
    }      
    return(change);
}


/* Notify() - ACTION
 *
 * Notifies the user that a button has been pressed, and
 * calls the callback; if the XtNpasteBuffer resource is true
 * then the name of the item is also put in CUT_BUFFER0.	*/

/* ARGSUSED */
static void
Notify(w, event, params, num_params)
Widget w;
XEvent * event;
String * params;
Cardinal *num_params;
{
    ListWidget lw = ( ListWidget ) w;
    int item, item_len;
    XawListReturnStruct ret_value;

/* 
 * Find item and if out of range then unhighlight and return. 
 * 
 * If the current item is unhighlighted then the user has aborted the
 * notify, so unhighlight and return.
 */

    if ( ((CvtToItem(w, event->xbutton.x, event->xbutton.y, &item))
	  == OUT_OF_RANGE) || (lw->list.highlight != item) ) {
        XawListUnhighlight(w);
        return;
    }

    item_len = strlen(lw->list.list[item]);

    if ( lw->list.paste )	/* if XtNpasteBuffer set then paste it. */
        XStoreBytes(XtDisplay(w), lw->list.list[item], item_len);

/* 
 * Call Callback function.
 */

    ret_value.string = lw->list.list[item];
    ret_value.list_index = item;
    
    XtCallCallbacks( w, XtNcallback, (XtPointer) &ret_value);
}


/* Unset() - ACTION
 *
 * unhighlights the current element. */

/* ARGSUSED */
static void
Unset(w, event, params, num_params)
Widget w;
XEvent * event;
String * params;
Cardinal *num_params;
{
  XawListUnhighlight(w);
}


/* Set() - ACTION
 *
 * Highlights the current element. */

/* ARGSUSED */
static void
Set(w, event, params, num_params)
Widget w;
XEvent * event;
String * params;
Cardinal *num_params;
{
  int item;
  ListWidget lw = (ListWidget) w;

  if ( (CvtToItem(w, event->xbutton.x, event->xbutton.y, &item))
      == OUT_OF_RANGE)
    XawListUnhighlight(w);		        /* Unhighlight current item. */
  else if ( lw->list.is_highlighted != item )   /* If this item is not */
    XawListHighlight(w, item);	                /* highlighted then do it. */
}

/*
 * Set specified arguments into widget
 */

static Boolean 
SetValues(current, request, new, args, num_args)
Widget current, request, new;
ArgList args;
Cardinal *num_args;
{
    ListWidget cl = (ListWidget) current;
    ListWidget rl = (ListWidget) request;
    ListWidget nl = (ListWidget) new;
    Boolean redraw = FALSE;
    XFontSetExtents *ext = XExtentsOfFontSet(nl->list.fontset);

    /* If the request height/width is different, lock it.  Unless its 0. If */
    /* neither new nor 0, leave it as it was.  Not in R5. */
    if ( nl->core.width != cl->core.width )
        nl->list.freedoms |= WidthLock;
    if ( nl->core.width == 0 )
        nl->list.freedoms &= ~WidthLock;

    if ( nl->core.height != cl->core.height )
        nl->list.freedoms |= HeightLock;
    if ( nl->core.height == 0 )
        nl->list.freedoms &= ~HeightLock;

    if ( nl->list.longest != cl->list.longest )
        nl->list.freedoms |= LongestLock;
    if ( nl->list.longest == 0 )
        nl->list.freedoms &= ~LongestLock;

    /* _DONT_ check for fontset here - it's not in GC.*/

    if (  (cl->list.foreground       != nl->list.foreground)       ||
	  (cl->core.background_pixel != nl->core.background_pixel) ||
	  (cl->list.font             != nl->list.font)                ) {
	XGCValues values;
	XGetGCValues(XtDisplay(current), cl->list.graygc, GCTile, &values);
	XmuReleaseStippledPixmap(XtScreen(current), values.tile);
	XtReleaseGC(current, cl->list.graygc);
	XtReleaseGC(current, cl->list.revgc);
	XtReleaseGC(current, cl->list.normgc);
        GetGCs(new);
        redraw = TRUE;
    }

    if ( ( cl->list.font != nl->list.font ) &&
				( cl->simple.international == False ) )
        nl->list.row_height = nl->list.font->max_bounds.ascent
	                    + nl->list.font->max_bounds.descent
			    + nl->list.row_space;

    else if ( ( cl->list.fontset != nl->list.fontset ) &&
				( cl->simple.international == True ) )
        nl->list.row_height = ext->max_ink_extent.height + nl->list.row_space;

    /* ...If the above two font(set) change checkers above both failed, check
    if row_space was altered.  If one of the above passed, row_height will
    already have been re-calculated. */

    else if ( cl->list.row_space != nl->list.row_space ) {

        if (cl->simple.international == True )
            nl->list.row_height = ext->max_ink_extent.height + nl->list.row_space;
        else
            nl->list.row_height = nl->list.font->max_bounds.ascent
	                        + nl->list.font->max_bounds.descent
			        + nl->list.row_space;
    }

    if ((cl->core.width           != nl->core.width)           ||
	(cl->core.height          != nl->core.height)          ||
	(cl->list.internal_width  != nl->list.internal_width)  ||
	(cl->list.internal_height != nl->list.internal_height) ||
	(cl->list.column_space    != nl->list.column_space)    ||
	(cl->list.row_space       != nl->list.row_space)       ||
	(cl->list.default_cols    != nl->list.default_cols)    ||
	(  (cl->list.force_cols   != nl->list.force_cols) &&
	   (rl->list.force_cols   != nl->list.ncols) )         ||
	(cl->list.vertical_cols   != nl->list.vertical_cols)   ||
	(cl->list.longest         != nl->list.longest)         ||
	(cl->list.nitems          != nl->list.nitems)          ||
	(cl->list.font            != nl->list.font)            ||
   /* Equiv. fontsets might have different values, but the same fonts, so the
   next comparison is sloppy but not dangerous.  */
	(cl->list.fontset         != nl->list.fontset)         ||
	(cl->list.list            != nl->list.list)          )   {

        CalculatedValues( new );
        Layout( new, WidthFree( nl ), HeightFree( nl ),
			 &nl->core.width, &nl->core.height );
        redraw = TRUE;
    }

    if (cl->list.list != nl->list.list)
	nl->list.is_highlighted = nl->list.highlight = NO_HIGHLIGHT;

    if ((cl->core.sensitive != nl->core.sensitive) ||
	(cl->core.ancestor_sensitive != nl->core.ancestor_sensitive)) {
        nl->list.highlight = NO_HIGHLIGHT;
	redraw = TRUE;
    }
    
    if (!XtIsRealized(current))
      return(FALSE);
      
    return(redraw);
}

static void Destroy(w)
    Widget w;
{
    ListWidget lw = (ListWidget) w;
    XGCValues values;
    
    XGetGCValues(XtDisplay(w), lw->list.graygc, GCTile, &values);
    XmuReleaseStippledPixmap(XtScreen(w), values.tile);
    XtReleaseGC(w, lw->list.graygc);
    XtReleaseGC(w, lw->list.revgc);
    XtReleaseGC(w, lw->list.normgc);
}

/* Exported Functions */

/*	Function Name: XawListChange.
 *	Description: Changes the list being used and shown.
 *	Arguments: w - the list widget.
 *                 list - the new list.
 *                 nitems - the number of items in the list.
 *                 longest - the length (in Pixels) of the longest element
 *                           in the list.
 *                 resize - if TRUE the the list widget will
 *                          try to resize itself.
 *	Returns: none.
 *      NOTE:      If nitems of longest are <= 0 then they will be calculated.
 *                 If nitems is <= 0 then the list needs to be NULL terminated.
 */

void
#if NeedFunctionPrototypes
XawListChange(Widget w, char ** list, int nitems, int longest,
#if NeedWidePrototypes
	      int resize_it)
#else
	      Boolean resize_it)
#endif
#else
XawListChange(w, list, nitems, longest, resize_it)
Widget w;
char ** list;
int nitems, longest;
Boolean resize_it;
#endif
{
    ListWidget lw = (ListWidget) w;
    Dimension new_width = w->core.width;
    Dimension new_height = w->core.height;

    lw->list.list = list;

    if ( nitems <= 0 ) nitems = 0;
    lw->list.nitems = nitems;
    if ( longest <= 0 ) longest = 0;

    /* If the user passes 0 meaning "calculate it", it must be free */
    if ( longest != 0 )
        lw->list.freedoms |= LongestLock;
    else /* the user's word is god. */
        lw->list.freedoms &= ~LongestLock;

    if ( resize_it )
        lw->list.freedoms &= ~WidthLock & ~HeightLock;
    /* else - still resize if its not locked */

    lw->list.longest = longest;

    CalculatedValues( w );

    if( Layout( w, WidthFree( w ), HeightFree( w ),
		&new_width, &new_height ) )
        ChangeSize( w, new_width, new_height );

    lw->list.is_highlighted = lw->list.highlight = NO_HIGHLIGHT;
    if ( XtIsRealized( w ) )
      Redisplay( w, (XEvent *)NULL, (Region)NULL );
}

/*	Function Name: XawListUnhighlight
 *	Description: unlights the current highlighted element.
 *	Arguments: w - the widget.
 *	Returns: none.
 */

void
#if NeedFunctionPrototypes
XawListUnhighlight(Widget w)
#else
XawListUnhighlight(w)
Widget w;
#endif
{
    ListWidget lw = ( ListWidget ) w;

    lw->list.highlight = NO_HIGHLIGHT;
    if (lw->list.is_highlighted != NO_HIGHLIGHT)
        PaintItemName(w, lw->list.is_highlighted); /* unhighlight this one. */
}

/*	Function Name: XawListHighlight
 *	Description: Highlights the given item.
 *	Arguments: w - the list widget.
 *                 item - the item to hightlight.
 *	Returns: none.
 */

void
#if NeedFunctionPrototypes
XawListHighlight(Widget w, int item)
#else
XawListHighlight(w, item)
Widget w;
int item;
#endif
{
    ListWidget lw = ( ListWidget ) w;
    
    if (XtIsSensitive(w)) {
        lw->list.highlight = item;
        if (lw->list.is_highlighted != NO_HIGHLIGHT)
            PaintItemName(w, lw->list.is_highlighted);  /* Unhighlight. */
	PaintItemName(w, item); /* HIGHLIGHT this one. */ 
    }
}

/*	Function Name: XawListShowCurrent
 *	Description: returns the currently highlighted object.
 *	Arguments: w - the list widget.
 *	Returns: the info about the currently highlighted object.
 */

XawListReturnStruct *
#if NeedFunctionPrototypes
XawListShowCurrent(Widget w)
#else
XawListShowCurrent(w)
Widget w;
#endif
{
    ListWidget lw = ( ListWidget ) w;
    XawListReturnStruct * ret_val;

    ret_val = (XawListReturnStruct *) 
	          XtMalloc (sizeof (XawListReturnStruct));/* SPARE MALLOC OK */
    
    ret_val->list_index = lw->list.highlight;
    if (ret_val->list_index == XAW_LIST_NONE)
      ret_val->string = "";
    else
      ret_val->string = lw->list.list[ ret_val->list_index ];

    return(ret_val);
}

