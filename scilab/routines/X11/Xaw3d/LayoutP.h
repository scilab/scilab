/*
 * $XConsortium: LayoutP.h,v 1.2 92/01/22 18:03:08 keith Exp $
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

#ifndef _XawLayoutP_h
#define _XawLayoutP_h

#if defined(LAYOUT)
# include "Layout.h"
#else
# include <X11/Xaw3d/Layout.h>
#endif

#include <X11/ConstrainP.h>

#ifdef MOTIF
# include "Xm/ManagerP.h"
#endif

#define GlueEqual(a,b)	((a).order == (b).order && (a).value == (b).value)

#define AddGlue(r,a,b)	if (a.order == b.order) { \
			    r.order = a.order; \
			    r.value = a.value + b.value; \
			} else { \
			    if (a.order > b.order) \
				r = a; \
			    else \
				r = b; \
			}

#define MinGlue(r,a,b)	if (a.order == b.order) { \
			    r.order = a.order; \
			    if (a.value > b.value) \
				r.value = b.value; \
			    else \
				r.value = a.value; \
			} else { \
			    if (a.order > b.order) \
				r = b; \
			    else \
				r = a; \
			}

#define SubGlue(r,a,b)	if (a.order == b.order) { \
			    r.order = a.order; \
			    r.value = a.value - b.value; \
			} else { \
			    if (a.order > b.order) \
				r = a; \
			    else { \
				r.order = b.order; \
				r.value = -b.value; \
			    } \
			}

#define ZeroGlue(g)	((g).value = 0, (g).order = 0, (g).expr = 0)
#define IsZeroGlue(g)	((g).value == 0)

#define QuarkToWidget(l,q)  XtNameToWidget((Widget) l, \
					   (char *) XrmQuarkToString(q));
					   
typedef enum _BoxType { BoxBox, WidgetBox, GlueBox, VariableBox } BoxType;
    
typedef enum _LayoutDirection {
    LayoutHorizontal = 0, LayoutVertical = 1
} LayoutDirection;

typedef enum _Operator {
    Plus, Minus, Times, Divide, Percent
} Operator;

typedef enum _ExprType {
    Constant,
    Binary,
    Unary,
    Width,
    Height,
    Variable
} ExprType;

typedef struct _Expr *ExprPtr;

typedef struct _Expr {
    ExprType	type;
    union {
	double	    constant;
	struct {
	    Operator	op;
	    ExprPtr	left, right;
	} binary;
	struct {
	    Operator    op;
	    ExprPtr	down;
	} unary;
	XrmQuark    width;
	XrmQuark    height;
	XrmQuark    variable;
    } u;
} ExprRec;

typedef struct _Glue {
    int		order;
    double	value;
    ExprPtr	expr;
} GlueRec, *GluePtr;

typedef struct _BoxParams {
    GlueRec stretch[2];
    GlueRec shrink[2];
} BoxParamsRec, *BoxParamsPtr;

typedef struct _Box *BoxPtr;

typedef BoxPtr	LayoutPtr;

typedef struct _Box {
    BoxPtr	    nextSibling;
    BoxPtr	    parent;
    BoxParamsRec    params;
    int		    size[2];
    int		    natural[2];
    BoxType	    type;
    union {
	struct {
	    BoxPtr	    firstChild;
	    LayoutDirection dir;
	} box;
	struct {
	    XrmQuark	    quark;
	    Widget	    widget;
	} widget;
	struct {
	    ExprPtr	    expr;
	} glue;
	struct {
	    XrmQuark	    quark;
	    ExprPtr	    expr;
	} variable;
    } u;
} LBoxRec; /* this conflicted with Box's BoxRec, besides, it's not used anywhere */

typedef struct _SubInfo {
    int	    naturalSize[2];
    int	    naturalBw;
} SubInfoRec, *SubInfoPtr;

/* #define New(t) (t *) malloc(sizeof (t)) */
#define New(t)      (t *) XtCalloc(1,sizeof (t))
#define Dispose(p)  XtFree((char *) p)
#define Some(t,n)   (t*) XtMalloc(sizeof(t) * n)
#define More(p,t,n) ((p)? (t *) XtRealloc((char *) p, sizeof(t)*n):Some(t,n)

/*********************************************************************
 *
 * Layout Widget Private Data
 *
 *********************************************************************/

/* New fields for the Layout widget class record */

typedef struct _LayoutClassPart {
    int foo;			/* keep compiler happy. */
} LayoutClassPart;

/* Full Class record declaration */
typedef struct _LayoutClassRec {
    CoreClassPart       core_class;
    CompositeClassPart  composite_class;
    ConstraintClassPart constraint_class;
#ifdef MOTIF
    XmManagerClassPart  manager_class;
#endif
    LayoutClassPart     layout_class;
} LayoutClassRec;

extern LayoutClassRec layoutClassRec;

typedef struct _LayoutConstraintsRec {
#ifdef MOTIF
    XmManagerConstraintPart  manager;
#endif
    SubInfoRec	layout;
} LayoutConstraintsRec, *LayoutConstraints;

#define SubInfo(w)  (&(((LayoutConstraints) (w)->core.constraints)->layout))

/* New Fields for the Layout widget record */

typedef struct {
    /* resources */
    LayoutPtr	layout;
    Boolean	debug;
} LayoutPart;

/**************************************************************************
 *
 * Full instance record declaration
 *
 **************************************************************************/

typedef struct _LayoutRec {
    CorePart       core;
    CompositePart  composite;
    ConstraintPart constraint;
#ifdef MOTIF
    XmManagerPart  manager;
#endif    
    LayoutPart     layout;
} LayoutRec;
#endif
