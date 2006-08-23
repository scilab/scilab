/*
 * $XConsortium: Tree.c,v 1.45 94/04/17 20:13:20 kaleb Exp $
 *

Copyright (c) 1990, 1994  X Consortium

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

 * Copyright 1989 Prentice Hall
 *
 * Permission to use, copy, modify, and distribute this software for any
 * purpose and without fee is hereby granted, provided that the above
 * copyright notice appear in all copies and that both the copyright notice
 * and this permission notice appear in supporting documentation.
 * 
 * Prentice Hall and the authors disclaim all warranties with regard
 * to this software, including all implied warranties of merchantability and
 * fitness.  In no event shall Prentice Hall or the authors be liable
 * for any special, indirect or cosequential damages or any damages whatsoever
 * resulting from loss of use, data or profits, whether in an action of
 * contract, negligence or other tortious action, arising out of or in
 * connection with the use or performance of this software.
 * 
 * Authors:  Jim Fulton, MIT X Consortium,
 *           based on a version by Douglas Young, Prentice Hall
 * 
 * This widget is based on the Tree widget described on pages 397-419 of
 * Douglas Young's book "The X Window System, Programming and Applications 
 * with Xt OSF/Motif Edition."  The layout code has been rewritten to use
 * additional blank space to make the structure of the graph easier to see
 * as well as to support vertical trees.
 */

#include <X11/IntrinsicP.h>
#include <X11/StringDefs.h>
#include <X11/Xaw3d/XawInit.h>
#include <X11/Xaw3d/Cardinals.h>
#include <X11/Xaw3d/TreeP.h>

#define IsHorizontal(tw) ((tw)->tree.gravity == WestGravity || \
			  (tw)->tree.gravity == EastGravity)


					/* widget class method */
static void             ClassInitialize();
static void             Initialize();
static void             ConstraintInitialize();
static void             ConstraintDestroy();
static Boolean          ConstraintSetValues();
static void             Destroy();
static Boolean          SetValues();
static XtGeometryResult GeometryManager();
static void             ChangeManaged();
static void             Redisplay();
static XtGeometryResult	QueryGeometry();

					/* utility routines */
static void             insert_node();
static void             delete_node();
static void             layout_tree();


/*
 * resources of the tree itself
 */
static XtResource resources[] = {
    { XtNautoReconfigure, XtCAutoReconfigure, XtRBoolean, sizeof (Boolean),
	XtOffsetOf(TreeRec, tree.auto_reconfigure), XtRImmediate,
	(XtPointer) FALSE },
    { XtNhSpace, XtCHSpace, XtRDimension, sizeof (Dimension),
	XtOffsetOf(TreeRec, tree.hpad), XtRImmediate, (XtPointer) 0 },
    { XtNvSpace, XtCVSpace, XtRDimension, sizeof (Dimension),
	XtOffsetOf(TreeRec, tree.vpad), XtRImmediate, (XtPointer) 0 },
    { XtNforeground, XtCForeground, XtRPixel, sizeof (Pixel),
	XtOffsetOf(TreeRec, tree.foreground), XtRString,
	XtDefaultForeground},
    { XtNlineWidth, XtCLineWidth, XtRDimension, sizeof (Dimension),
	XtOffsetOf(TreeRec, tree.line_width), XtRImmediate, (XtPointer) 0 },
    { XtNgravity, XtCGravity, XtRGravity, sizeof (XtGravity),
	XtOffsetOf(TreeRec, tree.gravity), XtRImmediate,
	(XtPointer) WestGravity },
};


/*
 * resources that are attached to all children of the tree
 */
static XtResource treeConstraintResources[] = {
    { XtNtreeParent, XtCTreeParent, XtRWidget, sizeof (Widget),
	XtOffsetOf(TreeConstraintsRec, tree.parent), XtRImmediate, NULL },
    { XtNtreeGC, XtCTreeGC, XtRGC, sizeof(GC),
	XtOffsetOf(TreeConstraintsRec, tree.gc), XtRImmediate, NULL },
};


TreeClassRec treeClassRec = {
  {
					/* core_class fields  */
    (WidgetClass) &constraintClassRec,	/* superclass         */
    "Tree",				/* class_name         */
    sizeof(TreeRec),			/* widget_size        */
    ClassInitialize,			/* class_init         */
    NULL,				/* class_part_init    */
    FALSE,				/* class_inited       */	
    Initialize,				/* initialize         */
    NULL,				/* initialize_hook    */	
    XtInheritRealize,			/* realize            */
    NULL,				/* actions            */
    0,					/* num_actions        */	
    resources,				/* resources          */
    XtNumber(resources),		/* num_resources      */
    NULLQUARK,				/* xrm_class          */
    TRUE,				/* compress_motion    */	
    TRUE,				/* compress_exposure  */	
    TRUE,				/* compress_enterleave*/	
    TRUE,				/* visible_interest   */
    Destroy,				/* destroy            */
    NULL,				/* resize             */
    Redisplay,				/* expose             */
    SetValues,				/* set_values         */
    NULL,				/* set_values_hook    */	
    XtInheritSetValuesAlmost,		/* set_values_almost  */
    NULL,				/* get_values_hook    */	
    NULL,				/* accept_focus       */
    XtVersion,				/* version            */	
    NULL,				/* callback_private   */
    NULL,				/* tm_table           */
    QueryGeometry,			/* query_geometry     */	
    NULL,				/* display_accelerator*/
    NULL,				/* extension          */
  },
  {
					/* composite_class fields */
    GeometryManager,			/* geometry_manager    */
    ChangeManaged,			/* change_managed      */
    XtInheritInsertChild,		/* insert_child        */	
    XtInheritDeleteChild,		/* delete_child        */	
    NULL,				/* extension           */
  },
  { 
					/* constraint_class fields */
   treeConstraintResources,		/* subresources        */
   XtNumber(treeConstraintResources),	/* subresource_count   */
   sizeof(TreeConstraintsRec),		/* constraint_size     */
   ConstraintInitialize,		/* initialize          */
   ConstraintDestroy,			/* destroy             */
   ConstraintSetValues,			/* set_values          */
   NULL,				/* extension           */
   },
  {
					/* Tree class fields */
    0,					/* ignore              */	
  }
};

WidgetClass treeWidgetClass = (WidgetClass) &treeClassRec;


/*****************************************************************************
 *                                                                           *
 *			     tree utility routines                           *
 *                                                                           *
 *****************************************************************************/

static void initialize_dimensions (listp, sizep, n)
    Dimension **listp;
    int *sizep;
    int n;
{
    int i;
    Dimension *l;

    if (!*listp) {
	*listp = (Dimension *) XtCalloc ((unsigned int) n,
					 (unsigned int) sizeof(Dimension));
	*sizep = ((*listp) ? n : 0);
	return;
    }
    if (n > *sizep) {
	*listp = (Dimension *) XtRealloc((char *) *listp,
					 (unsigned int) (n*sizeof(Dimension)));
	if (!*listp) {
	    *sizep = 0;
	    return;
	}
	for (i = *sizep, l = (*listp) + i; i < n; i++, l++) *l = 0;
	*sizep = n;
    }
    return;
}

static GC get_tree_gc (w)
    TreeWidget w;
{
    XtGCMask valuemask = GCBackground | GCForeground;
    XGCValues values;

    values.background = w->core.background_pixel;
    values.foreground = w->tree.foreground;
    if (w->tree.line_width != 0) {
	valuemask |= GCLineWidth;
	values.line_width = w->tree.line_width;
    }

    return XtGetGC ((Widget) w, valuemask, &values);
}

static void insert_node (parent, node)
     Widget parent, node;
{
    TreeConstraints pc;
    TreeConstraints nc = TREE_CONSTRAINT(node);
    int nindex;
  
    nc->tree.parent = parent;

    if (parent == NULL) return;

    /*
     * If there isn't more room in the children array, 
     * allocate additional space.
     */  
    pc = TREE_CONSTRAINT(parent);
    nindex = pc->tree.n_children;
  
    if (pc->tree.n_children == pc->tree.max_children) {
	pc->tree.max_children += (pc->tree.max_children / 2) + 2;
	pc->tree.children = (WidgetList) XtRealloc ((char *)pc->tree.children, 
						    (unsigned int)
						    ((pc->tree.max_children) *
						    sizeof(Widget)));
    } 

    /*
     * Add the sub_node in the next available slot and 
     * increment the counter.
     */
    pc->tree.children[nindex] = node;
    pc->tree.n_children++;
}

static void delete_node (parent, node)
    Widget parent, node;
{
    TreeConstraints pc;
    int pos, i;

    /*
     * Make sure the parent exists.
     */
    if (!parent) return;  
  
    pc = TREE_CONSTRAINT(parent);

    /*
     * Find the sub_node on its parent's list.
     */
    for (pos = 0; pos < pc->tree.n_children; pos++)
      if (pc->tree.children[pos] == node) break;

    if (pos == pc->tree.n_children) return;

    /*
     * Decrement the number of children
     */  
    pc->tree.n_children--;

    /*
     * Fill in the gap left by the sub_node.
     * Zero the last slot for good luck.
     */
    for (i = pos; i < pc->tree.n_children; i++) 
      pc->tree.children[i] = pc->tree.children[i+1];

    pc->tree.children[pc->tree.n_children]=0;
}

static void check_gravity (tw, grav)
    TreeWidget tw;
    XtGravity grav;
{
    switch (tw->tree.gravity) {
      case WestGravity: case NorthGravity: case EastGravity: case SouthGravity:
	break;
      default:
	tw->tree.gravity = grav;
	break;
    }
}


/*****************************************************************************
 *                                                                           *
 * 			      tree class methods                             *
 *                                                                           *
 *****************************************************************************/

static void ClassInitialize ()
{
    XawInitializeWidgetSet();
    XtAddConverter (XtRString, XtRGravity, XmuCvtStringToGravity,
		    (XtConvertArgList) NULL, (Cardinal) 0);
}


/*ARGSUSED*/
static void Initialize (grequest, gnew, args, num_args)
    Widget grequest, gnew;
    ArgList args;
    Cardinal *num_args;
{
    TreeWidget request = (TreeWidget) grequest, new = (TreeWidget) gnew;
    Arg arglist[2];

    /*
     * Make sure the widget's width and height are 
     * greater than zero.
     */
    if (request->core.width <= 0) new->core.width = 5;
    if (request->core.height <= 0) new->core.height = 5;

    /*
     * Set the padding according to the orientation
     */
    if (request->tree.hpad == 0 && request->tree.vpad == 0) {
	if (IsHorizontal (request)) {
	    new->tree.hpad = TREE_HORIZONTAL_DEFAULT_SPACING;
	    new->tree.vpad = TREE_VERTICAL_DEFAULT_SPACING;
	} else {
	    new->tree.hpad = TREE_VERTICAL_DEFAULT_SPACING;
	    new->tree.vpad = TREE_HORIZONTAL_DEFAULT_SPACING;
	}
    }

    /*
     * Create a graphics context for the connecting lines.
     */
    new->tree.gc = get_tree_gc (new);

    /*
     * Create the hidden root widget.
     */
    new->tree.tree_root = (Widget) NULL;
    XtSetArg(arglist[0], XtNwidth, 1);
    XtSetArg(arglist[1], XtNheight, 1);
    new->tree.tree_root = XtCreateWidget ("root", widgetClass, gnew,
					  arglist,TWO);

    /*
     * Allocate the array used to hold the widest values per depth
     */
    new->tree.largest = NULL;
    new->tree.n_largest = 0;
    initialize_dimensions (&new->tree.largest, &new->tree.n_largest, 
			   TREE_INITIAL_DEPTH);

    /*
     * make sure that our gravity is one of the acceptable values
     */
    check_gravity (new, WestGravity);
} 


/* ARGSUSED */
static void ConstraintInitialize (request, new, args, num_args)
     Widget request, new;
     ArgList args;
     Cardinal *num_args;
{
    TreeConstraints tc = TREE_CONSTRAINT(new);
    TreeWidget tw = (TreeWidget) new->core.parent;

    /*
     * Initialize the widget to have no sub-nodes.
     */
    tc->tree.n_children = 0;
    tc->tree.max_children = 0;
    tc->tree.children = (Widget *) NULL;
    tc->tree.x = tc->tree.y = 0; 
    tc->tree.bbsubwidth = 0;
    tc->tree.bbsubheight = 0;


    /*
     * If this widget has a super-node, add it to that 
     * widget' sub-nodes list. Otherwise make it a sub-node of 
     * the tree_root widget.
     */
    if (tc->tree.parent)
      insert_node (tc->tree.parent, new);
    else if (tw->tree.tree_root)
      insert_node (tw->tree.tree_root, new);
} 


/* ARGSUSED */
static Boolean SetValues (gcurrent, grequest, gnew, args, num_args)
    Widget gcurrent, grequest, gnew;
    ArgList args;
    Cardinal *num_args;
{
    TreeWidget current = (TreeWidget) gcurrent, new = (TreeWidget) gnew;
    Boolean redraw = FALSE;

    /*
     * If the foreground color has changed, redo the GC's
     * and indicate a redraw.
     */
    if (new->tree.foreground != current->tree.foreground ||
	new->core.background_pixel != current->core.background_pixel ||
	new->tree.line_width != current->tree.line_width) {
	XtReleaseGC (gnew, new->tree.gc);
	new->tree.gc = get_tree_gc (new);
	redraw = TRUE;     
    }

    /*
     * If the minimum spacing has changed, recalculate the
     * tree layout. layout_tree() does a redraw, so we don't
     * need SetValues to do another one.
     */
    if (new->tree.gravity != current->tree.gravity) {
	check_gravity (new, current->tree.gravity);
    }

    if (IsHorizontal(new) != IsHorizontal(current)) {
	if (new->tree.vpad == current->tree.vpad &&
	    new->tree.hpad == current->tree.hpad) {
	    new->tree.vpad = current->tree.hpad;
	    new->tree.hpad = current->tree.vpad;
	}
    }

    if (new->tree.vpad != current->tree.vpad ||
	new->tree.hpad != current->tree.hpad ||
	new->tree.gravity != current->tree.gravity) {
	layout_tree (new, TRUE);
	redraw = FALSE;
    }
    return redraw;
}


/* ARGSUSED */
static Boolean ConstraintSetValues (current, request, new, args, num_args)
    Widget current, request, new;
    ArgList args;
    Cardinal *num_args;
{
    TreeConstraints newc = TREE_CONSTRAINT(new);
    TreeConstraints curc = TREE_CONSTRAINT(current);
    TreeWidget tw = (TreeWidget) new->core.parent;

    /*
     * If the parent field has changed, remove the widget
     * from the old widget's children list and add it to the
     * new one.
     */
    if (curc->tree.parent != newc->tree.parent){
	if (curc->tree.parent)
	  delete_node (curc->tree.parent, new);
	if (newc->tree.parent)
	  insert_node(newc->tree.parent, new);

	/*
	 * If the Tree widget has been realized, 
	 * compute new layout.
	 */
	if (XtIsRealized((Widget)tw))
	  layout_tree (tw, FALSE);
    }
    return False;
}


static void ConstraintDestroy (w) 
    Widget w;
{ 
    TreeConstraints tc = TREE_CONSTRAINT(w);
    TreeWidget tw = (TreeWidget) XtParent(w);
    int i;

    /* 
     * Remove the widget from its parent's sub-nodes list and
     * make all this widget's sub-nodes sub-nodes of the parent.
     */
  
    if (tw->tree.tree_root == w) {
	if (tc->tree.n_children > 0)
	  tw->tree.tree_root = tc->tree.children[0];
	else
	  tw->tree.tree_root = NULL;
    }

    delete_node (tc->tree.parent, (Widget) w);
    for (i = 0; i< tc->tree.n_children; i++)
      insert_node (tc->tree.parent, tc->tree.children[i]);

    layout_tree ((TreeWidget) (w->core.parent), FALSE);
}

/* ARGSUSED */
static XtGeometryResult GeometryManager (w, request, reply)
    Widget w;
    XtWidgetGeometry *request;
    XtWidgetGeometry *reply;
{

    TreeWidget tw = (TreeWidget) w->core.parent;

    /*
     * No position changes allowed!.
     */
    if ((request->request_mode & CWX && request->x!=w->core.x)
	||(request->request_mode & CWY && request->y!=w->core.y))
      return (XtGeometryNo);

    /*
     * Allow all resize requests.
     */

    if (request->request_mode & CWWidth)
      w->core.width = request->width;
    if (request->request_mode & CWHeight)
      w->core.height = request->height;
    if (request->request_mode & CWBorderWidth)
      w->core.border_width = request->border_width;

    if (tw->tree.auto_reconfigure) layout_tree (tw, FALSE);
    return (XtGeometryYes);
}

static void ChangeManaged (gw)
    Widget gw;
{
    layout_tree ((TreeWidget) gw, FALSE);
}


static void Destroy (gw)
    Widget gw;
{
    TreeWidget w = (TreeWidget) gw;

    XtReleaseGC (gw, w->tree.gc);
    if (w->tree.largest) XtFree ((char *) w->tree.largest);
}


/* ARGSUSED */
static void Redisplay (gw, event, region)
     Widget gw;
     XEvent *event;
     Region region;
{
    TreeWidget tw = (TreeWidget) gw;

    /*
     * If the Tree widget is visible, visit each managed child.
     */
    if (tw->core.visible) {
	int i, j;
	Display *dpy = XtDisplay (tw);
	Window w = XtWindow (tw);

	for (i = 0; i < tw->composite.num_children; i++) {
	    Widget child = tw->composite.children[i];
	    TreeConstraints tc = TREE_CONSTRAINT(child);

	    /*
	     * Don't draw lines from the fake tree_root.
	     */
	    if (child != tw->tree.tree_root && tc->tree.n_children) {
		int srcx = child->core.x + child->core.border_width;
		int srcy = child->core.y + child->core.border_width;

		switch (tw->tree.gravity) {
		  case WestGravity:
		    srcx += child->core.width + child->core.border_width;
		    /* fall through */
		  case EastGravity:
		    srcy += child->core.height / 2;
		    break;

		  case NorthGravity:
		    srcy += child->core.height + child->core.border_width;
		    /* fall through */
		  case SouthGravity:
		    srcx += child->core.width / 2;
		    break;
		}

		for (j = 0; j < tc->tree.n_children; j++) {
		    Widget k = tc->tree.children[j];
		    GC gc = (tc->tree.gc ? tc->tree.gc : tw->tree.gc);

		    switch (tw->tree.gravity) {
		      case WestGravity:
			/*
			 * right center to left center
			 */
			XDrawLine (dpy, w, gc, srcx, srcy,
				   (int) k->core.x,
				   (k->core.y + ((int) k->core.border_width) +
				    ((int) k->core.height) / 2));
			break;

		      case NorthGravity:
			/*
			 * bottom center to top center
			 */
			XDrawLine (dpy, w, gc, srcx, srcy,
				   (k->core.x + ((int) k->core.border_width) +
				    ((int) k->core.width) / 2),
				   (int) k->core.y);
			break;

		      case EastGravity:
			/*
			 * left center to right center
			 */
			XDrawLine (dpy, w, gc, srcx, srcy,
				   (k->core.x +
				    (((int) k->core.border_width) << 1) +
				    (int) k->core.width),
				   (k->core.y + ((int) k->core.border_width) +
				    ((int) k->core.height) / 2));
			break;

		      case SouthGravity:
			/*
			 * top center to bottom center
			 */
			XDrawLine (dpy, w, gc, srcx, srcy,
				   (k->core.x + ((int) k->core.border_width) +
				    ((int) k->core.width) / 2),
				   (k->core.y +
				    (((int) k->core.border_width) << 1) +
				    (int) k->core.height));
			break;
		    }
		}
	    }
	}
    }
}

static XtGeometryResult QueryGeometry (w, intended, preferred)
    Widget w;
    XtWidgetGeometry *intended, *preferred;
{
    TreeWidget tw = (TreeWidget) w;

    preferred->request_mode = (CWWidth | CWHeight);
    preferred->width = tw->tree.maxwidth;
    preferred->height = tw->tree.maxheight;

    if (((intended->request_mode & (CWWidth | CWHeight)) ==
	 (CWWidth | CWHeight)) &&
	intended->width == preferred->width &&
	intended->height == preferred->height)
      return XtGeometryYes;
    else if (preferred->width == w->core.width &&
             preferred->height == w->core.height)
      return XtGeometryNo;
    else
      return XtGeometryAlmost;
}


/*****************************************************************************
 *                                                                           *
 *			     tree layout algorithm                           *
 *                                                                           *
 * Each node in the tree is "shrink-wrapped" with a minimal bounding         *
 * rectangle, laid next to its siblings (with a small about of padding in    *
 * between) and then wrapped with their parent.  Parents are centered about  *
 * their children (or vice versa if the parent is larger than the children). *
 *                                                                           *
 *****************************************************************************/

static void compute_bounding_box_subtree (tree, w, depth)
    TreeWidget tree;
    Widget w;
    int depth;
{
    TreeConstraints tc = TREE_CONSTRAINT(w);  /* info attached to all kids */
    int i;
    Bool horiz = IsHorizontal (tree);
    Dimension newwidth, newheight;
    Dimension bw2 = w->core.border_width * 2;

    /*
     * Set the max-size per level.
     */
    if (depth >= tree->tree.n_largest) {
	initialize_dimensions (&tree->tree.largest,
			       &tree->tree.n_largest, depth + 1);
    }
    newwidth = ((horiz ? w->core.width : w->core.height) + bw2);
    if (tree->tree.largest[depth] < newwidth)
      tree->tree.largest[depth] = newwidth;


    /*
     * initialize
     */
    tc->tree.bbwidth = w->core.width + bw2;
    tc->tree.bbheight = w->core.height + bw2;

    if (tc->tree.n_children == 0) return;

    /*
     * Figure the size of the opposite dimension (vertical if tree is 
     * horizontal, else vice versa).  The other dimension will be set 
     * in the second pass once we know the maximum dimensions.
     */
    newwidth = 0;
    newheight = 0;
    for (i = 0; i < tc->tree.n_children; i++) {
	Widget child = tc->tree.children[i];
	TreeConstraints cc = TREE_CONSTRAINT(child);
	    
	compute_bounding_box_subtree (tree, child, depth + 1);

	if (horiz) {
	    if (newwidth < cc->tree.bbwidth) newwidth = cc->tree.bbwidth;
	    newheight += tree->tree.vpad + cc->tree.bbheight;
	} else {
	    if (newheight < cc->tree.bbheight) newheight = cc->tree.bbheight;
	    newwidth += tree->tree.hpad + cc->tree.bbwidth;
	}
    }


    tc->tree.bbsubwidth = newwidth;
    tc->tree.bbsubheight = newheight;

    /*
     * Now fit parent onto side (or top) of bounding box and correct for
     * extra padding.  Be careful of unsigned arithmetic.
     */
    if (horiz) {
	tc->tree.bbwidth += tree->tree.hpad + newwidth;
	newheight -= tree->tree.vpad;
	if (newheight > tc->tree.bbheight) tc->tree.bbheight = newheight;
    } else {
	tc->tree.bbheight += tree->tree.vpad + newheight;
	newwidth -= tree->tree.hpad;
	if (newwidth > tc->tree.bbwidth) tc->tree.bbwidth = newwidth;
    }
}


static void set_positions (tw, w, level)
     TreeWidget tw;
     Widget w;
     int level;
{
    int i;
  
    if (w) {
	TreeConstraints tc = TREE_CONSTRAINT(w);

	if (level > 0) {
	    /*
	     * mirror if necessary
	     */
	    switch (tw->tree.gravity) {
	      case EastGravity:
		tc->tree.x = (((Position) tw->tree.maxwidth) -
			      ((Position) w->core.width) - tc->tree.x);
		break;

	      case SouthGravity:
		tc->tree.y = (((Position) tw->tree.maxheight) -
			      ((Position) w->core.height) - tc->tree.y);
		break;
	    }

	    /*
	     * Move the widget into position.
	     */
	    XtMoveWidget (w, tc->tree.x, tc->tree.y);
	}

	/*
	 * Set the positions of all children.
	 */
	for (i = 0; i < tc->tree.n_children; i++)
	  set_positions (tw, tc->tree.children[i], level + 1);
    }
}


static void arrange_subtree (tree, w, depth, x, y)
    TreeWidget tree;
    Widget w;
    int depth;
    Position x, y;
{
    TreeConstraints tc = TREE_CONSTRAINT(w);  /* info attached to all kids */
    TreeConstraints firstcc, lastcc;
    int i;
    int newx, newy;
    Bool horiz = IsHorizontal (tree);
    Widget child = NULL;
    Dimension tmp;
    Dimension bw2 = w->core.border_width * 2;
    Bool relayout = True;


    /*
     * If no children, then just lay out where requested.
     */
    tc->tree.x = x;
    tc->tree.y = y;

    if (horiz) {
	int myh = (w->core.height + bw2);

	if (myh > (int)tc->tree.bbsubheight) {
	    y += (myh - (int)tc->tree.bbsubheight) / 2;
	    relayout = False;
	}
    } else {
	int myw = (w->core.width + bw2);

	if (myw > (int)tc->tree.bbsubwidth) {
	    x += (myw - (int)tc->tree.bbsubwidth) / 2;
	    relayout = False;
	}
    }

    if ((tmp = ((Dimension) x) + tc->tree.bbwidth) > tree->tree.maxwidth)
      tree->tree.maxwidth = tmp;
    if ((tmp = ((Dimension) y) + tc->tree.bbheight) > tree->tree.maxheight)
      tree->tree.maxheight = tmp;

    if (tc->tree.n_children == 0) return;


    /*
     * Have children, so walk down tree laying out children, then laying
     * out parents.
     */
    if (horiz) {
	newx = x + tree->tree.largest[depth];
	if (depth > 0) newx += tree->tree.hpad;
	newy = y;
    } else {
	newx = x;
	newy = y + tree->tree.largest[depth];
	if (depth > 0) newy += tree->tree.vpad;
    }

    for (i = 0; i < tc->tree.n_children; i++) {
	TreeConstraints cc;

	child = tc->tree.children[i];	/* last value is used outside loop */
	cc = TREE_CONSTRAINT(child);

	arrange_subtree (tree, child, depth + 1, newx, newy);
	if (horiz) {
	    newy += tree->tree.vpad + cc->tree.bbheight;
	} else {
	    newx += tree->tree.hpad + cc->tree.bbwidth;
	}
    }

    /*
     * now layout parent between first and last children
     */
    if (relayout) {
	Position adjusted;
	firstcc = TREE_CONSTRAINT (tc->tree.children[0]);
	lastcc = TREE_CONSTRAINT (child);

	/* Adjustments are disallowed if they result in a position above
         * or to the left of the originally requested position, because
	 * this could collide with the position of the previous sibling.
	 */
	if (horiz) {
	    tc->tree.x = x;
	    adjusted = firstcc->tree.y +
	      ((lastcc->tree.y + (Position) child->core.height + 
		(Position) child->core.border_width * 2 -
		firstcc->tree.y - (Position) w->core.height - 
		(Position) w->core.border_width * 2 + 1) / 2);
	    if (adjusted > tc->tree.y) tc->tree.y = adjusted;
	} else {
	    adjusted = firstcc->tree.x +
	      ((lastcc->tree.x + (Position) child->core.width +
		(Position) child->core.border_width * 2 -
		firstcc->tree.x - (Position) w->core.width -
		(Position) w->core.border_width * 2 + 1) / 2);
	    if (adjusted > tc->tree.x) tc->tree.x = adjusted;
	    tc->tree.y = y;
	}
    }
}

static void set_tree_size (tw, insetvalues, width, height)
    TreeWidget tw;
    Boolean insetvalues;
    Dimension width, height;
{
    if (insetvalues) {
	tw->core.width = width;
	tw->core.height = height;
    } else {
	Dimension replyWidth = 0, replyHeight = 0;
	XtGeometryResult result = XtMakeResizeRequest ((Widget) tw,
						       width, height,
						       &replyWidth,
						       &replyHeight);
	/*
	 * Accept any compromise.
	 */
	if (result == XtGeometryAlmost)
	  XtMakeResizeRequest ((Widget) tw, replyWidth, replyHeight,
			       (Dimension *) NULL, (Dimension *) NULL);
    }
    return;
}

static void layout_tree (tw, insetvalues)
    TreeWidget tw;
    Boolean insetvalues;
{
    int i;
    Dimension *dp;

    /*
     * Do a depth-first search computing the width and height of the bounding
     * box for the tree at that position (and below).  Then, walk again using
     * this information to layout the children at each level.
     */

    if (tw->tree.tree_root == NULL)
	return;

    tw->tree.maxwidth = tw->tree.maxheight = 0;
    for (i = 0, dp = tw->tree.largest; i < tw->tree.n_largest; i++, dp++)
      *dp = 0;
    initialize_dimensions (&tw->tree.largest, &tw->tree.n_largest, 
			   tw->tree.n_largest);
    compute_bounding_box_subtree (tw, tw->tree.tree_root, 0);

   /*
    * Second pass to do final layout.  Each child's bounding box is stacked
    * on top of (if horizontal, else next to) on top of its siblings.  The
    * parent is centered between the first and last children.
    */
    arrange_subtree (tw, tw->tree.tree_root, 0, 0, 0);

    /*
     * Move each widget into place.
     */
    set_tree_size (tw, insetvalues, tw->tree.maxwidth, tw->tree.maxheight);
    set_positions (tw, tw->tree.tree_root, 0);

    /*
     * And redisplay.
     */
    if (XtIsRealized ((Widget) tw)) {
	XClearArea (XtDisplay(tw), XtWindow((Widget)tw), 0, 0, 0, 0, True);
    }
}



/*****************************************************************************
 *                                                                           *
 * 				Public Routines                              *
 *                                                                           *
 *****************************************************************************/

void
#if NeedFunctionPrototypes
XawTreeForceLayout (Widget tree)
#else
XawTreeForceLayout (tree)
    Widget tree;
#endif
{
    layout_tree ((TreeWidget) tree, FALSE);
}

