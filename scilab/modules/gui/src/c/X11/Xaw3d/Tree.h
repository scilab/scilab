/*
 * $XConsortium: Tree.h,v 1.13 94/04/17 20:13:22 kaleb Exp $
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


#ifndef _XawTree_h
#define _XawTree_h

#include <X11/Xmu/Converters.h>

/******************************************************************************
 * 
 * Tree Widget (subclass of ConstraintClass)
 * 
 ******************************************************************************
 * 
 * Parameters:
 * 
 *  Name                Class              Type            Default
 *  ----                -----              ----            -------
 * 
 *  autoReconfigure     AutoReconfigure    Boolean         FALSE
 *  background          Background         Pixel           XtDefaultBackground
 *  foreground          Foreground         Pixel           XtDefaultForeground
 *  gravity             Gravity            XtGravity       West
 *  hSpace              HSpace             Dimension       20
 *  lineWidth           LineWidth          Dimension       0
 *  vSpace              VSpace             Dimension       6
 * 
 * 
 * Constraint Resources attached to children:
 * 
 *  treeGC              TreeGC             GC              NULL
 *  treeParent          TreeParent         Widget          NULL
 * 
 * 
 *****************************************************************************/

                                        /* new instance field names */
#ifndef _XtStringDefs_h_
#define XtNhSpace "hSpace"
#define XtNvSpace "vSpace"
#define XtCHSpace "HSpace"
#define XtCVSpace "VSpace"
#endif

#define XtNautoReconfigure "autoReconfigure"
#define XtNlineWidth "lineWidth"
#define XtNtreeGC "treeGC"
#define XtNtreeParent "treeParent"
#define XtNgravity "gravity"

                                        /* new class field names */
#define XtCAutoReconfigure "AutoReconfigure"
#define XtCLineWidth "LineWidth"
#define XtCTreeGC "TreeGC"
#define XtCTreeParent "TreeParent"
#define XtCGravity "Gravity"

#define XtRGC "GC"
                                        /* external declarations */
extern WidgetClass treeWidgetClass;

typedef struct _TreeClassRec *TreeWidgetClass;
typedef struct _TreeRec      *TreeWidget;

_XFUNCPROTOBEGIN

extern void XawTreeForceLayout (
#if NeedFunctionPrototypes
    Widget /* tree */
#endif
);

_XFUNCPROTOEND

#endif /* _XawTree_h */
