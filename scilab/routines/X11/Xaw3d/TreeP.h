/*
 * $XConsortium: TreeP.h,v 1.14 94/04/17 20:13:24 jim Exp $
 *

Copyright (c) 1990  X Consortium

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


#ifndef _XawTreeP_h
#define _XawTreeP_h

#include <X11/Xaw3d/Tree.h>

typedef struct _TreeClassPart {
    int ignore;
} TreeClassPart;

typedef struct _TreeClassRec {
    CoreClassPart core_class;
    CompositeClassPart composite_class;
    ConstraintClassPart constraint_class;
    TreeClassPart tree_class;
} TreeClassRec;

extern TreeClassRec treeClassRec;

typedef struct {
    /* fields available through resources */
    Dimension hpad;			/* hSpace/HSpace */
    Dimension vpad;			/* vSpace/VSpace */
    Dimension line_width;		/* lineWidth/LineWidth */
    Pixel foreground;			/* foreground/Foreground */
    XtGravity gravity;			/* gravity/Gravity */
    Boolean auto_reconfigure;		/* autoReconfigure/AutoReconfigure */
    /* private fields */
    GC gc;				/* used to draw lines */
    Widget tree_root;			/* hidden root off all children */
    Dimension *largest;			/* list of largest per depth */
    int n_largest;			/* number of elements in largest */
    Dimension maxwidth, maxheight;	/* for shrink wrapping */
} TreePart;


typedef struct _TreeRec {
    CorePart core;
    CompositePart composite;
    ConstraintPart constraint;
    TreePart tree;
}  TreeRec;


/*
 * structure attached to all children
 */
typedef struct _TreeConstraintsPart {
    /* resources */
    Widget parent;			/* treeParent/TreeParent */
    GC gc;				/* treeGC/TreeGC */
    /* private data */
    Widget *children;
    int n_children;
    int max_children;
    Dimension bbsubwidth, bbsubheight;	/* bounding box of sub tree */
    Dimension bbwidth, bbheight;	/* bounding box including node */
    Position x, y;
} TreeConstraintsPart;

typedef struct _TreeConstraintsRec {
   TreeConstraintsPart tree;
} TreeConstraintsRec, *TreeConstraints;


/*
 * useful macros
 */

#define TREE_CONSTRAINT(w) \
                   ((TreeConstraints)((w)->core.constraints))

#define TREE_INITIAL_DEPTH 10		/* for allocating largest array */
#define TREE_HORIZONTAL_DEFAULT_SPACING 20
#define TREE_VERTICAL_DEFAULT_SPACING 6

#endif /* _XawTreeP_h */



