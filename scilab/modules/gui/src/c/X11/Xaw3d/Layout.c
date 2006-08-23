/*
 * $XConsortium: Layout.c,v 1.1 91/09/13 18:51:44 keith Exp $
 *
 * Copyright 1991 Massachusetts Institute of Technology
 *
 * Permission to use, copy, modify, distribute, and sell this software and its
 * documentation for any purpose is hereby granted without fee, provided that
 * the above copyright notice appear in all copies and that both that
 * copyright notice and this permission notice appear in supporting
 * documentation, and that the name of M.I.T. not be used in advertising or
 * publicity pertaining to distribution of the software without specific,
 * written prior permission.  M.I.T. makes no representations about the
 * suitability of this software for any purpose.  It is provided "as is"
 * without express or implied warranty.
 *
 * M.I.T. DISCLAIMS ALL WARRANTIES WITH REGARD TO THIS SOFTWARE, INCLUDING ALL
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS, IN NO EVENT SHALL M.I.T.
 * BE LIABLE FOR ANY SPECIAL, INDIRECT OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
 * WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN ACTION
 * OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF OR IN 
 * CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
 *
 * Author:  Keith Packard, MIT X Consortium
 */

#include <X11/IntrinsicP.h>
#include <X11/StringDefs.h>

#include <X11/Xmu/Misc.h>
#include <X11/Xmu/Converters.h>

#ifdef MOTIF
# include <Xm/XmP.h>
#endif

#if defined(LAYOUT)
# include "LayoutP.h"
#else
# include <X11/Xaw3d/LayoutP.h>
#endif

#include <ctype.h>
#include <stdio.h>

#undef DEBUG
#ifdef DEBUG
static char *DBUG_currentproc, *DBUG_lastproc;
static int DBUG_level = 0;
# define DBUG_ENTER(s)        \
    {DBUG_lastproc=DBUG_currentproc;DBUG_currentproc=s;\
    fprintf(stderr,"begin: (%d) %s\n",++DBUG_level,s);}
# define DBUG_VOID_RETURN \
    {fprintf(stderr,"end:   (%d) %s\n",DBUG_level--,DBUG_currentproc); \
    DBUG_currentproc=DBUG_lastproc;return;}
# define DBUG_RETURN(f,v) \
    {fprintf(stderr,"return (%d) %s (",DBUG_level--,DBUG_currentproc); \
    fprintf(stderr,(f),(v)); \
    fprintf(stderr,")\n"); DBUG_currentproc=DBUG_lastproc;return (v);}
# define DBUG_PRINT(f,s)        \
    {fprintf(stderr,"       (%d) %s:",DBUG_level, DBUG_currentproc); \
    fprintf(stderr,f,s);fprintf(stderr,"\n");}
#else
# define DBUG_ENTER(s) 
# define DBUG_VOID_RETURN return
# define DBUG_PRINT(f,s)
# define DBUG_RETURN(f,v) return(v)
#endif

/*****************************************************************************
 *
 * Full instance record declaration
 *
 ****************************************************************************/

#define offset(field) XtOffsetOf(LayoutRec, layout.field)

static XtResource resources[] = {
    {XtNlayout, XtCLayout, XtRLayout, sizeof (BoxPtr),
	offset(layout), XtRLayout, NULL },
    {XtNdebug, XtCBoolean, XtRBoolean, sizeof(Boolean),
	 offset(debug), XtRImmediate, (XtPointer) FALSE},
};

#undef offset

static void ClassInitialize(), Initialize();
static void Resize();
static Boolean SetValues();
static XtGeometryResult GeometryManager();
static void ChangeManaged();
static void InsertChild();
static XtGeometryResult QueryGeometry ();
static void GetDesiredSize ();
#ifdef MOTIF
static void Redisplay ();
#endif

static void LayoutLayout ();
static void LayoutGetNaturalSize ();
static void LayoutFreeLayout ();

extern void LayYYsetsource(), LayYYsetdest();
extern int LayYYparse();

#ifdef MOTIF
#define SuperClass ((ConstraintWidgetClass)&xmManagerClassRec)
#else
#define SuperClass ((ConstraintWidgetClass)&constraintClassRec)
#endif

LayoutClassRec layoutClassRec = {
   {
/* core class fields */
    /* superclass         */   (WidgetClass) SuperClass,
    /* class name         */   "Layout",
    /* size               */   sizeof(LayoutRec),
    /* class_initialize   */   ClassInitialize,
    /* class_part init    */   NULL,
    /* class_inited       */   FALSE,
    /* initialize         */   Initialize,
    /* initialize_hook    */   NULL,
    /* realize            */   XtInheritRealize,
    /* actions            */   NULL,
    /* num_actions        */   0,
    /* resources          */   resources,
    /* resource_count     */   XtNumber(resources),
    /* xrm_class          */   NULLQUARK,
    /* compress_motion    */   FALSE,
    /* compress_exposure  */   0,
    /* compress_enterleave*/   FALSE,
    /* visible_interest   */   FALSE,
    /* destroy            */   NULL,
    /* resize             */   Resize,
#ifdef MOTIF
    /* expose             */   Redisplay,
#else
    /* expose             */   NULL,
#endif
    /* set_values         */   SetValues,
    /* set_values_hook    */   NULL,
    /* set_values_almost  */   XtInheritSetValuesAlmost,
    /* get_values_hook    */   NULL,
    /* accept_focus       */   NULL,
    /* version            */   XtVersion,
    /* callback_private   */   NULL,
#ifdef MOTIF
    /* tm_table           */   XtInheritTranslations,
#else
    /* tm_table           */   NULL,
#endif
    /* query_geometry	  */   QueryGeometry,
    /* display_accelerator*/   XtInheritDisplayAccelerator,
    /* extension          */   NULL
   }, {
/* composite class fields */
    /* geometry_manager   */   GeometryManager,
    /* change_managed     */   ChangeManaged,
    /* insert_child       */   InsertChild,
    /* delete_child       */   XtInheritDeleteChild,
    /* extension          */   NULL
   }, {
/* constraint class fields */
    /* subresources       */   NULL,
    /* subresource_count  */   0,
    /* constraint_size    */   sizeof(LayoutConstraintsRec),
    /* initialize         */   NULL,
    /* destroy            */   NULL,
    /* set_values         */   NULL,
    /* extension          */   NULL
   },
#ifdef MOTIF
   {
    /* manager class */
    XtInheritTranslations,                /* translations           */
    NULL,                                 /* syn resources          */
    0,                                    /* num syn_resources      */
    NULL,                                 /* get_cont_resources     */
    0,                                    /* num_get_cont_resources */
    XmInheritParentProcess,               /* parent_process       */
    NULL,                                 /* extension              */
   },
#endif
   { /* layout_class fields */     
    0
   }
};

WidgetClass layoutWidgetClass = (WidgetClass) &layoutClassRec;

#define ForAllChildren(pw, childP) \
  for ( (childP) = (pw)->composite.children ; \
        (childP) < (pw)->composite.children + (pw)->composite.num_children ; \
        (childP)++ ) if (!XtIsManaged(*childP)) ; else

/************************************************************
 *
 * Semi-public routines. 
 *
 ************************************************************/

/*	Function Name: ClassInitialize
 *	Description: The Layout widgets class initialization proc.
 *	Arguments: none.
 *	Returns: none.
 */

/*ARGSUSED*/
static Boolean
CvtStringToLayout (dpy, args, num_args, from, to, converter_data)
    Display	*dpy;
    XrmValue	*args;
    Cardinal	*num_args;
    XrmValue	*from, *to;
    XtPointer	*converter_data;
{
    static BoxPtr tmp;
    
    LayYYsetsource ((char *) from->addr);
    if (!to->addr) to->addr = (XtPointer)&tmp;
    LayYYsetdest ((BoxPtr *) to->addr);
    to->size = sizeof (BoxPtr *);
    return  LayYYparse() ? FALSE : TRUE;
}

/*ARGSUSED*/
static void
DisposeLayout (app, to, data, args, num_args)
    XtAppContext    app;
    XrmValue	    *to;
    XtPointer	    data;
    XrmValuePtr	    args;
    Cardinal	    *num_args;
{
    LayoutFreeLayout (* (LayoutPtr *) to->addr);
}

static void 
ClassInitialize()
{
    XtSetTypeConverter ( XtRString, XtRLayout, CvtStringToLayout,
		    (XtConvertArgList)NULL, (Cardinal)0, XtCacheNone, 
 		    DisposeLayout );
}

#ifdef MOTIF
static void Redisplay ( gw, event, region ) 
Widget gw;
XEvent *event;
Region region;
{
   /*
    * If the Layout widget is visible, redraw gadgets.
    */
 
    if ( XtIsRealized ( gw ) && gw->core.visible ) 
    {
        _XmRedisplayGadgets ( gw, event, region );
    }
    /* ChangeManaged(gw);*/
}
#endif

/*ARGSUSED*/
static XtGeometryResult GeometryManager(child, request, reply)
    Widget		child;
    XtWidgetGeometry	*request, *reply;
{
    LayoutWidget    w = (LayoutWidget) XtParent(child);
    SubInfoPtr	    p = SubInfo(child);
    int		    bw;
    Bool	    changed, bwChanged;

    bw = p->naturalBw;
    changed = FALSE;
    bwChanged = FALSE;
    if (request->request_mode & CWBorderWidth &&
	request->border_width != child->core.border_width) 
    {
	p->naturalBw = bw;
	bw = request->border_width;
	changed = TRUE;
	bwChanged = TRUE;
    }
    if (bwChanged || ((request->request_mode & CWWidth) &&
	request->width != child->core.width))
    {
	p->naturalSize[LayoutHorizontal] = request->width + bw * 2;
	changed = TRUE;
    }
    if (bwChanged || ((request->request_mode & CWHeight) &&
	request->height != child->core.height))
    {
	p->naturalSize[LayoutVertical] = request->height + bw * 2;
	changed = TRUE;
    }
    if (changed)
	LayoutLayout (w, TRUE);
    return XtGeometryDone;
}

/* ARGSUSED */
static void Initialize(request, new, args, num_args)
Widget request, new;
ArgList args;
Cardinal *num_args;
{
/*    LayoutWidget w = (LayoutWidget)new; */
}

static void ChangeManaged(gw)
   Widget gw;
{
    LayoutWidget	w = (LayoutWidget) gw;
    Widget		*children;

    DBUG_ENTER("changeManaged");

    ForAllChildren (w, children)
	GetDesiredSize (*children);
    LayoutLayout ((LayoutWidget) w, TRUE);
#ifdef MOTIF
    _XmNavigChangeManaged ( gw );        
#endif
    DBUG_VOID_RETURN;
}

static void
GetDesiredSize (child)
    Widget  child;
{
    XtWidgetGeometry	desired;
    SubInfoPtr		p;
    
    XtQueryGeometry (child, (XtWidgetGeometry *) NULL, &desired);
    p = SubInfo (child);
    p->naturalBw = desired.border_width;
    p->naturalSize[LayoutHorizontal] = desired.width + desired.border_width * 2;
    p->naturalSize[LayoutVertical] = desired.height + desired.border_width * 2;
}

static void InsertChild (child)
    Widget  child;
{
    (*SuperClass->composite_class.insert_child) (child);
    GetDesiredSize (child);
}

static void
Resize(gw)
    Widget gw;
{
    LayoutLayout ((LayoutWidget) gw, FALSE);
}

/* ARGSUSED */
static Boolean 
SetValues(gold, greq, gnew, args, num_args)
    Widget gold, greq, gnew;
    ArgList args;
    Cardinal *num_args;
{
    LayoutWidget    old = (LayoutWidget) gold,
		    new = (LayoutWidget) gnew;

    if (old->layout.layout != new->layout.layout)
	LayoutLayout (new, TRUE);
    return FALSE;
} /* SetValues */

static XtGeometryResult
QueryGeometry (gw, request, prefered_return)
    Widget		gw;
    XtWidgetGeometry	*request, *prefered_return;
{
    LayoutWidget	w = (LayoutWidget) gw;
    XtGeometryResult	result;
    XtWidgetGeometry	prefered_size;

    if (request && !(request->request_mode & (CWWidth|CWHeight)))
	return XtGeometryYes;
    LayoutGetNaturalSize (w, &prefered_size.width, &prefered_size.height);
    prefered_return->request_mode = 0;
    result = XtGeometryYes;
    if (!request) {
	prefered_return->width = prefered_size.width;
	prefered_return->height= prefered_size.height;
	if (prefered_size.width != w->core.width) {
	    prefered_return->request_mode |= CWWidth;
	    result = XtGeometryAlmost;
	}
	if (prefered_size.height != w->core.height) {
	    prefered_return->request_mode |= CWHeight;
	    result = XtGeometryAlmost;
	}
    } else {
    	if (request->request_mode & CWWidth) {
	    if (prefered_size.width > request->width)
	    {
	    	if (prefered_size.width == w->core.width)
		    result = XtGeometryNo;
	    	else if (result != XtGeometryNo) {
		    result = XtGeometryAlmost;
		    prefered_return->request_mode |= CWWidth;
		    prefered_return->width = prefered_size.width;
	    	}
	    }
    	}
    	if (request->request_mode & CWHeight) {
	    if (prefered_size.height > request->height)
	    {
	    	if (prefered_size.height == w->core.height)
		    result = XtGeometryNo;
	    	else if (result != XtGeometryNo) {
		    result = XtGeometryAlmost;
		    prefered_return->request_mode |= CWHeight;
		    prefered_return->height = prefered_size.height;
	    	}
	    }
    	}
    }
    return result;
}

/*
 * Layout section.  Exports LayoutGetNaturalSize and
 * LayoutLayout to above section
 */

static void
PrintGlue (g)
    GlueRec g;
{
    if (g.order == 0 || g.value != 1.0)
	(void) printf ("%g", g.value);
    if (g.order > 0)
    {
	(void) printf ("%s", " inf");
	if (g.order > 1)
	    (void) printf (" %d", g.order);
    }
}

static void
PrintDirection (dir)
    LayoutDirection dir;
{
    switch (dir) {
    case LayoutHorizontal:
	(void) printf ("%s", "horizontal");
	break;
    case LayoutVertical:
	(void) printf ("%s", "vertical");
	break;
    default:
	(void) printf ("Unknown layout direction %d\n", dir);
	break;
  
    }
}

static void
TabTo(level)
    int	level;
{
    while (level--)
	(void) printf ("%s", "  ");
}

static void
PrintBox (box, level)
    BoxPtr	    box;
    int		    level;
{
    BoxPtr	child;
    
    TabTo (level);
    (void) printf ("%s", "< ");
    (void) printf ("%s", " + "); 
    PrintGlue (box->params.stretch[LayoutHorizontal]);
    (void) printf ("%s", " - "); 
    PrintGlue (box->params.shrink[LayoutHorizontal]);
    (void) printf ("%s", " * ");
    (void) printf ("%s", " + "); 
    PrintGlue (box->params.stretch[LayoutVertical]);
    (void) printf ("%s", " - "); 
    PrintGlue (box->params.shrink[LayoutVertical]);
    (void) printf ("%s", " >");
    (void) printf (" size: %d x %d", box->size[0], box->size[1]);
    (void) printf (" natural: %d x %d ", box->natural[0], box->natural[1]);
    switch (box->type) {
    case BoxBox:
	PrintDirection (box->u.box.dir);
	(void) printf ("%s\n", "");
	for (child = box->u.box.firstChild; child; child = child->nextSibling)
	    PrintBox (child, level+1);
	break;
    case WidgetBox:
	(void) printf (" %s\n", XrmQuarkToString (box->u.widget.quark));
	break;
    case GlueBox:
	(void) printf ("%s\n", " glue");
	break;
    case VariableBox:
	(void) printf (" variable %s\n", XrmQuarkToString (box->u.variable.quark));
	break;
    }
}

static ExprPtr
LookupVariable (child, quark)
    BoxPtr	child;
    XrmQuark	quark;
{
    BoxPtr	parent, box;

    DBUG_ENTER("LookupVariable");
    DBUG_PRINT("name = <%s>",XrmQuarkToString(quark));
    DBUG_PRINT("child = %p",child);
    while ((parent = child->parent))
    {
	for (box = parent->u.box.firstChild; 
	     box != child; 
	     box = box->nextSibling)
	{
	    if (box->type == VariableBox && box->u.variable.quark == quark)
		DBUG_RETURN("%p", box->u.variable.expr);
	}
	child = parent;
    }
    DBUG_RETURN("failure -> %d",0);
}
		
static double
Evaluate (l, box, expr, natural)
    LayoutWidget    l;
    BoxPtr	    box;
    ExprPtr	    expr;
    double	    natural;
{
    double	left, right, down;
    Widget	widget;
    SubInfoPtr	info;

    DBUG_PRINT("Evaluate %d", expr->type);
    switch (expr->type) {
    case Constant:
	return expr->u.constant;
    case Binary:
	left = Evaluate (l, box, expr->u.binary.left, natural);
	right = Evaluate (l, box, expr->u.binary.right, natural);
	switch (expr->u.binary.op) {
        case Plus:
	    return left + right;
	case Minus:
	    return left - right;
	case Times:
	    return left * right;
	case Divide:
	    return left / right;
	case Percent:
	    return right * left / 100.0;
	}
    case Unary:
	down = Evaluate (l, box, expr->u.unary.down, natural);
	switch (expr->u.unary.op) {
	case Percent:
	    return natural * down / 100.0;
	case Minus:
	    return -down;
	case Plus:
	case Times:
	case Divide:
	    break;
	}
    case Width:
	widget = QuarkToWidget (l, expr->u.width);
	if (!widget)
	    return 0;
	info = SubInfo (widget);
	return info->naturalSize[LayoutHorizontal];
    case Height:
	widget = QuarkToWidget (l, expr->u.height);
	if (!widget)
	    return 0;
	info = SubInfo (widget);
	return info->naturalSize[LayoutVertical];
    case Variable: 
        {
	/* in the original code there was no nexpr, 
	   expr was overwritten by LookupVariable and 
	   the expression "expr->u.variable" to obtain the
	   variable name for the errormessage cause a segmentation
	   violation */
	ExprPtr    nexpr;
	nexpr = LookupVariable (box, expr->u.variable);
	if (!nexpr)
	    {
	    char    buf[256];
	    (void) sprintf (buf, "Layout: undefined variable %s\n",
			    XrmQuarkToString (expr->u.variable));
	    XtError (buf);
	    return 0.0;
	    }
	return Evaluate (l, box, nexpr, natural);
	}
    }
    return 0.0;
}

static void
DisposeExpr (expr)
    ExprPtr expr;
{
    if (!expr)
	return;
    switch (expr->type) {
    case Constant:
	break;
    case Binary:
	DisposeExpr (expr->u.binary.left);
	DisposeExpr (expr->u.binary.right);
	break;
    case Unary:
	DisposeExpr (expr->u.unary.down);
	break;
    case Width:
    case Height:
    case Variable:
	break;
    }
    Dispose (expr);
}

#define CheckGlue(l, box, glue, n) { \
    if (glue.expr) \
	glue.value = Evaluate (l, box, glue.expr, n); \
    if (glue.order == 0 && glue.value == 0) \
	glue.order = -1; \
    else if (glue.order == -1 && glue.value != 0) \
	glue.order = 0; \
}

#define DoStretch(l, box, dir) \
    CheckGlue (l, box, box->params.stretch[dir], (double) box->natural[dir]);
	
#define DoShrink(l, box, dir) \
    CheckGlue (l, box, box->params.shrink[dir], (double) box->natural[dir])

/* compute the natural sizes of a box */
static void
ComputeNaturalSizes (l, box, dir)
    LayoutWidget    l;
    BoxPtr	    box;
    LayoutDirection dir;
{
    BoxPtr	child;
    Widget	w;
    SubInfoPtr	info;
    int		minStretchOrder, minShrinkOrder;
    LayoutDirection thisDir;

    DBUG_ENTER("ComputeNaturalSizes");
    DBUG_PRINT("box->type=%d",box->type);
    switch (box->type) {
    case WidgetBox:
	w = box->u.widget.widget = QuarkToWidget (l, box->u.widget.quark);
	if (!w)
	{
	    box->natural[LayoutHorizontal] = 0;
	    box->natural[LayoutVertical] = 0;
	}
	else 
	{
	    info = SubInfo (w);
	    box->natural[LayoutHorizontal] = info->naturalSize[LayoutHorizontal];
	    box->natural[LayoutVertical] = info->naturalSize[LayoutVertical];
	}
	DoStretch (l, box, dir);
	DoShrink (l, box, dir);
	DoStretch (l, box, !dir);
	DoShrink (l, box, !dir);
	break;
    case GlueBox:
	box->natural[dir] = Evaluate (l, box, box->u.glue.expr, 0.0);
	box->natural[!dir] = 0;
	DoStretch (l, box, dir);
	DoShrink (l, box, dir);
	break;
    case BoxBox:
	thisDir = box->u.box.dir;
	box->natural[0] = 0;
	box->natural[1] = 0;
	minStretchOrder = 100000;
	minShrinkOrder = 100000;
	ZeroGlue (box->params.shrink[thisDir]);
	ZeroGlue (box->params.stretch[thisDir]);
	box->params.shrink[!thisDir].order = 100000;
	box->params.stretch[!thisDir].order = 100000;
	for (child = box->u.box.firstChild; child; child = child->nextSibling) 
	{
	    ComputeNaturalSizes (l, child, thisDir);
	    /*
	     * along box axis:
	     *  normal size += child normal size
	     *  shrink += child shrink
	     *  stretch += child stretch
	     */
	    box->natural[thisDir] += child->natural[thisDir];
	    AddGlue (box->params.shrink[thisDir],
		     box->params.shrink[thisDir],
		     child->params.shrink[thisDir]);
	    AddGlue (box->params.stretch[thisDir],
		     box->params.stretch[thisDir],
		     child->params.stretch[thisDir]);
	    /*
	     * normal to box axis:
	     *  normal size = maximum child normal size of minimum shrink order
	     *  shrink = difference between normal size and minimum shrink
	     *  stretch = minimum child stretch
	     */
	    if (box->natural[!thisDir] >= child->natural[!thisDir])
	    {
		if (child->params.stretch[!thisDir].order < minShrinkOrder)
		{
		    box->natural[!thisDir] = child->natural[!thisDir];
		    minStretchOrder = child->params.stretch[!thisDir].order;
		    if (child->params.shrink[!thisDir].order < minShrinkOrder)
			minShrinkOrder = child->params.shrink[!thisDir].order;
		}
	    }
	    else
	    {
		if (child->params.shrink[!thisDir].order <= minStretchOrder)
		{
		    box->natural[!thisDir] = child->natural[!thisDir];
		    minShrinkOrder = child->params.shrink[!thisDir].order;
		    if (child->params.stretch[!thisDir].order < minStretchOrder)
			minStretchOrder = child->params.stretch[!thisDir].order;
		}
	    }
	    MinGlue (box->params.stretch[!thisDir],
		     box->params.stretch[!thisDir],
		     child->params.stretch[!thisDir]);
	    MinGlue (box->params.shrink[!thisDir],
		     box->params.shrink[!thisDir],
		     child->params.shrink[!thisDir]);
	}
	if (box->params.shrink[!thisDir].order <= 0)
	{
	    int	    minSize;
	    int	    largestMinSize;
	    
	    largestMinSize = 0;
	    for (child = box->u.box.firstChild; child; child = child->nextSibling) 
	    {
		if (child->params.shrink[!thisDir].order <= 0)
		{
		    minSize = child->natural[!thisDir] -
			      child->params.shrink[!thisDir].value;
		    if (minSize > largestMinSize)
			largestMinSize = minSize;
		}
	    }
	    box->params.shrink[!thisDir].value = box->natural[!thisDir] -
						 largestMinSize;
	    if (box->params.shrink[!thisDir].value == 0)
		box->params.shrink[!thisDir].order = -1;
	    else
		box->params.shrink[!thisDir].order = 0;
	}
        break;
    case VariableBox:
        break;
    }
    DBUG_VOID_RETURN;
}

/* given the boxs geometry, set the geometry of the pieces */

#define GluePart(a,b,dist)	((a) ? ((int) (((a) * (dist)) / (b) + \
					((dist >= 0) ? 0.5 : -0.5))) : 0)

static Bool
ComputeSizes (box)
    BoxPtr	    box;
{
    LayoutDirection dir;
    BoxPtr	    child;
    GlueRec	    stretch;
    GlueRec	    shrink;
    GlueRec	    totalGlue[2];
    double	    remainingGlue;
    GluePtr	    glue;
    int		    size;
    int		    totalSizes;
    int		    totalChange[2];
    int		    change;
    int		    remainingChange;
    Bool	    shrinking;
    Bool	    happy;
    int		    i;
    int		    maxGlue;

    dir = box->u.box.dir;
    size = box->size[dir];
    
    stretch = box->params.stretch[dir];
    shrink = box->params.shrink[dir];
    
    /* pick the correct adjustment parameters based on the change direction */

    totalChange[0] = size - box->natural[dir];

    shrinking = totalChange[0] < 0;
    
    totalChange[1] = 0;
    totalGlue[1].order = 100000;
    totalGlue[1].value = 0;
    maxGlue = 1;
    if (shrinking) 
    {
	totalGlue[0] = shrink;
	/* for first-order infinites, shrink it to zero and then
	 * shrink the zero-orders
	 */
	if (shrink.order == 1) {
	    totalSizes = 0;
	    remainingGlue = 0;
	    for (child = box->u.box.firstChild; 
		 child; 
		 child = child->nextSibling) 
	    {
		switch (child->params.shrink[dir].order) {
		case 0:
		    remainingGlue += child->params.shrink[dir].value;
		    break;
		case 1:
		    totalSizes += child->natural[dir];
		    break;
		}
	    }
	    if (totalSizes < -totalChange[0])
	    {
		totalGlue[1] = shrink;
		totalGlue[0].order = 0;
		totalGlue[0].value = remainingGlue;
		totalChange[1] = -totalSizes;
		totalChange[0] = totalChange[0] - totalChange[1];
		maxGlue = 2;
	    }
	}
	if (totalGlue[0].order <= 0 && 
	    totalChange[0] > totalGlue[0].value)
	{
	    totalChange[0] = totalGlue[0].value;
	}
    }
    else
	totalGlue[0] = stretch;

    /* adjust each box */
    totalSizes = 0;
    remainingGlue = totalGlue[0].value + totalGlue[1].value;
    remainingChange = totalChange[0] + totalChange[1];
    happy = True;
    for (child = box->u.box.firstChild; child; child = child->nextSibling) 
    {
        /* never add glue or stretch to a VariableBox! */
        if (child->type == VariableBox) continue;

	if (shrinking)
	    glue = &child->params.shrink[dir];
	else
	    glue = &child->params.stretch[dir];
    
	child->size[dir] = child->natural[dir];
	for (i = 0; i < maxGlue; i++)
	{
	    if (glue->order == totalGlue[i].order)
	    {
		remainingGlue -= glue->value;
		if (remainingGlue <= 0)
		    change = remainingChange;
		else
		    change = GluePart (glue->value, 
				       totalGlue[i].value, 
				       totalChange[i]);
		child->size[dir] += change;
		remainingChange -= change;
	    }
	}
	child->size[!dir] = box->size[!dir];
	totalSizes += child->size[dir];
	if (child->type == BoxBox)
	    if (!ComputeSizes (child))
		happy = False;
    }
    return totalSizes == box->size[dir] && happy;
}

static void
SetSizes (box, x, y)
    BoxPtr	box;
    Position	x, y;
{
    BoxPtr	child;
    int		width, height;
    int		bw;
    Widget	w;
    SubInfoPtr	info;
    
    switch (box->type) {
    case WidgetBox:
	w = box->u.widget.widget;
	if (w)
	{
	    info = SubInfo(w);
	    /* info = (SubInfoPtr) w->core.constraints; */
	    width = box->size[LayoutHorizontal];
	    height = box->size[LayoutVertical];
	    bw = info->naturalBw;
	    width -= bw * 2;
	    height -= bw * 2;
	    /* Widgets which grow too small are placed off screen */
	    if (width <= 0 || height <= 0) 
	    {
		width = 1;
		height = 1;
		bw = 0;
		x = -1;
		y = -1;
	    }
	    XtConfigureWidget (w, x, y, 
			      (Dimension)width, (Dimension)height, 
			      (Dimension)bw);
	}
	break;
    case BoxBox:
	for (child = box->u.box.firstChild; child; child = child->nextSibling) 
	{
	    SetSizes (child, x, y);
	    if (box->u.box.dir == LayoutHorizontal)
		x += child->size[LayoutHorizontal];
	    else
		y += child->size[LayoutVertical];
	}
	break;
    case GlueBox:
    case VariableBox:
	break;
    }
}

static void
LayoutFreeLayout (box)
    BoxPtr  box;
{
    BoxPtr  child, next;
    
    switch (box->type) {
    case BoxBox:
	for (child = box->u.box.firstChild; child; child = next)
	{
	    next = child->nextSibling;
	    LayoutFreeLayout (child);
	}
	break;
    case GlueBox:
	DisposeExpr (box->u.glue.expr);
	break;
    case WidgetBox:
    case VariableBox:
    default: 
	break;
    }
    DisposeExpr (box->params.stretch[LayoutHorizontal].expr);
    DisposeExpr (box->params.shrink[LayoutHorizontal].expr);
    DisposeExpr (box->params.stretch[LayoutVertical].expr);
    DisposeExpr (box->params.shrink[LayoutVertical].expr);
    Dispose (box);
}


static void
LayoutGetNaturalSize (l, widthp, heightp)
    LayoutWidget    l;
    Dimension	    *widthp, *heightp;
{
    BoxPtr		box;

    DBUG_ENTER("LayoutGetNaturalSize");
    box = l->layout.layout;
    if (box) 
    {
	ComputeNaturalSizes (l, box, LayoutHorizontal);
	*widthp = box->natural[LayoutHorizontal];
	*heightp = box->natural[LayoutVertical];
    }
    else
    {
	*widthp = 0;
	*heightp = 0;
    }
    DBUG_VOID_RETURN;
}

static void
LayoutLayout (l, attemptResize)
    LayoutWidget    l;
    Bool	    attemptResize;
{
    BoxPtr		box;
    Dimension		width, height;
    Dimension		prefered_width, prefered_height;

    DBUG_ENTER("LayoutLayout");
    box = l->layout.layout;
    if (!box)
	return;
    LayoutGetNaturalSize (l, &prefered_width, &prefered_height);
    if (l->core.width == 0 || l->core.height == 0)
    {
	l->core.width = prefered_width;
	l->core.height = prefered_height;
    }
    box->size[LayoutHorizontal] = l->core.width;
    box->size[LayoutVertical] = l->core.height;
    if (!ComputeSizes (box) && attemptResize)
    {
	XtMakeResizeRequest ((Widget) l,
			    prefered_width, prefered_height,
			    &width, &height);
	if (width != box->size[LayoutHorizontal] ||
	    height != box->size[LayoutVertical])
	{
	    box->size[LayoutHorizontal] = width;
	    box->size[LayoutVertical] = height;
	    ComputeSizes (box);
	}
    }
    if (l->layout.debug)
    {
	PrintBox (box, 0);
	fflush (stdout);
    }
    SetSizes (box, 0, 0);
    DBUG_VOID_RETURN;
}

