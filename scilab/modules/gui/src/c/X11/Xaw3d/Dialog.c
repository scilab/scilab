/* $XConsortium: Dialog.c,v 1.48 94/04/17 20:12:02 kaleb Exp $ */

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

/* NOTE: THIS IS NOT A WIDGET!  Rather, this is an interface to a widget.
   It implements policy, and gives a (hopefully) easier-to-use interface
   than just directly making your own form. */


#include <X11/IntrinsicP.h>
#include <X11/Xos.h>
#include <X11/StringDefs.h>
#include <X11/Xmu/Misc.h>

#include <X11/Xaw3d/XawInit.h>
#include <X11/Xaw3d/AsciiText.h>
#include <X11/Xaw3d/Command.h>	
#include <X11/Xaw3d/Label.h>
#include <X11/Xaw3d/DialogP.h>
#include <X11/Xaw3d/Cardinals.h>

/*
 * After we have set the string in the value widget we set the
 * string to a magic value.  So that when a SetValues request is made
 * on the dialog value we will notice it, and reset the string.
 */

#define MAGIC_VALUE ((char *) 3)

#define streq(a,b) (strcmp( (a), (b) ) == 0)

static XtResource resources[] = {
  {XtNlabel, XtCLabel, XtRString, sizeof(String),
     XtOffsetOf(DialogRec, dialog.label), XtRString, NULL},
  {XtNvalue, XtCValue, XtRString, sizeof(String),
     XtOffsetOf(DialogRec, dialog.value), XtRString, NULL},
  {XtNicon, XtCIcon, XtRBitmap, sizeof(Pixmap),
     XtOffsetOf(DialogRec, dialog.icon), XtRImmediate, 0},
};

static void Initialize(), ConstraintInitialize(), CreateDialogValueWidget(),
            GetValuesHook();

static Boolean SetValues();

DialogClassRec dialogClassRec = {
  { /* core_class fields */
    /* superclass         */    (WidgetClass) &formClassRec,
    /* class_name         */    "Dialog",
    /* widget_size        */    sizeof(DialogRec),
    /* class_initialize   */    XawInitializeWidgetSet,
    /* class_part init    */    NULL,
    /* class_inited       */    FALSE,
    /* initialize         */    Initialize,
    /* initialize_hook    */    NULL,
    /* realize            */    XtInheritRealize,
    /* actions            */    NULL,
    /* num_actions        */    0,
    /* resources          */    resources,
    /* num_resources      */    XtNumber(resources),
    /* xrm_class          */    NULLQUARK,
    /* compress_motion    */    TRUE,
    /* compress_exposure  */    TRUE,
    /* compress_enterleave*/    TRUE,
    /* visible_interest   */    FALSE,
    /* destroy            */    NULL,
    /* resize             */    XtInheritResize,
    /* expose             */    XtInheritExpose,
    /* set_values         */    SetValues,
    /* set_values_hook    */    NULL,
    /* set_values_almost  */    XtInheritSetValuesAlmost,
    /* get_values_hook    */    GetValuesHook,
    /* accept_focus       */    NULL,
    /* version            */    XtVersion,
    /* callback_private   */    NULL,
    /* tm_table           */    NULL,
    /* query_geometry     */	XtInheritQueryGeometry,
    /* display_accelerator*/	XtInheritDisplayAccelerator,
    /* extension          */	NULL
  },
  { /* composite_class fields */
    /* geometry_manager   */   XtInheritGeometryManager,
    /* change_managed     */   XtInheritChangeManaged,
    /* insert_child       */   XtInheritInsertChild,
    /* delete_child       */   XtInheritDeleteChild,
    /* extension          */   NULL
  },
  { /* constraint_class fields */
    /* subresourses       */   NULL,
    /* subresource_count  */   0,
    /* constraint_size    */   sizeof(DialogConstraintsRec),
    /* initialize         */   ConstraintInitialize,
    /* destroy            */   NULL,
    /* set_values         */   NULL,
    /* extension          */   NULL
  },
  { /* form_class fields */
    /* layout             */   XtInheritLayout
  },
  { /* dialog_class fields */
    /* empty              */   0
  }
};

WidgetClass dialogWidgetClass = (WidgetClass)&dialogClassRec;

/* ARGSUSED */
static void Initialize(request, new, args, num_args)
Widget request, new;
ArgList args;
Cardinal *num_args;
{
    DialogWidget dw = (DialogWidget)new;
    Arg arglist[9];
    Cardinal arg_cnt = 0;

    XtSetArg(arglist[arg_cnt], XtNborderWidth, 0); arg_cnt++;
    XtSetArg(arglist[arg_cnt], XtNleft, XtChainLeft); arg_cnt++;

    if (dw->dialog.icon != (Pixmap)0) {
	XtSetArg(arglist[arg_cnt], XtNbitmap, dw->dialog.icon); arg_cnt++;
	XtSetArg(arglist[arg_cnt], XtNright, XtChainLeft); arg_cnt++;
	dw->dialog.iconW =
	    XtCreateManagedWidget( "icon", labelWidgetClass,
				   new, arglist, arg_cnt );
	arg_cnt = 2;
	XtSetArg(arglist[arg_cnt], XtNfromHoriz, dw->dialog.iconW);arg_cnt++;
    } else dw->dialog.iconW = (Widget)NULL;

    XtSetArg(arglist[arg_cnt], XtNlabel, dw->dialog.label); arg_cnt++;
    XtSetArg(arglist[arg_cnt], XtNright, XtChainRight); arg_cnt++;

    dw->dialog.labelW = XtCreateManagedWidget( "label", labelWidgetClass,
					      new, arglist, arg_cnt);

    if (dw->dialog.iconW != (Widget)NULL &&
	(dw->dialog.labelW->core.height < dw->dialog.iconW->core.height)) {
	XtSetArg( arglist[0], XtNheight, dw->dialog.iconW->core.height );
	XtSetValues( dw->dialog.labelW, arglist, ONE );
    }
    if (dw->dialog.value != NULL) 
        CreateDialogValueWidget( (Widget) dw);
    else
        dw->dialog.valueW = NULL;
}

/* ARGSUSED */
static void ConstraintInitialize(request, new, args, num_args)
Widget request, new;
ArgList args;
Cardinal *num_args;
{
    DialogWidget dw = (DialogWidget)new->core.parent;
    DialogConstraints constraint = (DialogConstraints)new->core.constraints;

    if (!XtIsSubclass(new, commandWidgetClass))	/* if not a button */
	return;					/* then just use defaults */

    constraint->form.left = constraint->form.right = XtChainLeft;
    if (dw->dialog.valueW == NULL) 
      constraint->form.vert_base = dw->dialog.labelW;
    else
      constraint->form.vert_base = dw->dialog.valueW;

    if (dw->composite.num_children > 1) {
	WidgetList children = dw->composite.children;
	Widget *childP;
        for (childP = children + dw->composite.num_children - 1;
	     childP >= children; childP-- ) {
	    if (*childP == dw->dialog.labelW || *childP == dw->dialog.valueW)
	        break;
	    if (XtIsManaged(*childP) &&
		XtIsSubclass(*childP, commandWidgetClass)) {
	        constraint->form.horiz_base = *childP;
		break;
	    }
	}
    }
}

#define ICON 0
#define LABEL 1
#define NUM_CHECKS 2

/* ARGSUSED */
static Boolean SetValues(current, request, new, in_args, in_num_args)
Widget current, request, new;
ArgList in_args;
Cardinal *in_num_args;
{
    DialogWidget w = (DialogWidget)new;
    DialogWidget old = (DialogWidget)current;
    Arg args[5];
    Cardinal num_args;
    int i;
    Boolean checks[NUM_CHECKS];

    for (i = 0; i < NUM_CHECKS; i++)
	checks[i] = FALSE;

    for (i = 0; i < *in_num_args; i++) {
	if (streq(XtNicon, in_args[i].name))
	    checks[ICON] = TRUE;
	if (streq(XtNlabel, in_args[i].name))
	    checks[LABEL] = TRUE;
    }

    if (checks[ICON]) {
	if (w->dialog.icon != (Pixmap)0) {
	    XtSetArg( args[0], XtNbitmap, w->dialog.icon );
	    if (old->dialog.iconW != (Widget)NULL) {
		XtSetValues( old->dialog.iconW, args, ONE );
	    } else {
		XtSetArg( args[1], XtNborderWidth, 0);
		XtSetArg( args[2], XtNleft, XtChainLeft);
		XtSetArg( args[3], XtNright, XtChainLeft);
		w->dialog.iconW =
		    XtCreateWidget( "icon", labelWidgetClass,
				    new, args, FOUR );
		((DialogConstraints)w->dialog.labelW->core.constraints)->
		    form.horiz_base = w->dialog.iconW;
		XtManageChild(w->dialog.iconW);
	    }
	} else if (old->dialog.icon != (Pixmap)0) {
	    ((DialogConstraints)w->dialog.labelW->core.constraints)->
		    form.horiz_base = (Widget)NULL;
	    XtDestroyWidget(old->dialog.iconW);
	    w->dialog.iconW = (Widget)NULL;
	}
    }

    if ( checks[LABEL] ) {
        num_args = 0;
        XtSetArg( args[num_args], XtNlabel, w->dialog.label ); num_args++;
	if (w->dialog.iconW != (Widget)NULL &&
	    (w->dialog.labelW->core.height <= w->dialog.iconW->core.height)) {
	    XtSetArg(args[num_args], XtNheight, w->dialog.iconW->core.height);
	    num_args++;
	}
	XtSetValues( w->dialog.labelW, args, num_args );
    }

    if ( w->dialog.value != old->dialog.value ) {
        if (w->dialog.value == NULL)  /* only get here if it
					  wasn't NULL before. */
	    XtDestroyWidget(old->dialog.valueW);
	else if (old->dialog.value == NULL) { /* create a new value widget. */
	    w->core.width = old->core.width;
	    w->core.height = old->core.height;
#ifdef notdef
/* this would be correct if Form had the same semantics on Resize
 * as on MakeGeometryRequest.  Unfortunately, Form botched it, so
 * any subclasses will currently have to deal with the fact that
 * we're about to change our real size.
 */
	    w->form.resize_in_layout = False; 
	    CreateDialogValueWidget( (Widget) w);
	    w->core.width = w->form.preferred_width;
	    w->core.height = w->form.preferred_height;
	    w->form.resize_in_layout = True;
#else /*notdef*/
	    CreateDialogValueWidget( (Widget) w);
#endif /*notdef*/
	}
	else {			/* Widget ok, just change string. */
	    Arg args[1];
	    XtSetArg(args[0], XtNstring, w->dialog.value);
	    XtSetValues(w->dialog.valueW, args, ONE);
	    w->dialog.value = MAGIC_VALUE;
	}
    }
    return False;
}

/*	Function Name: GetValuesHook
 *	Description: This is a get values hook routine that gets the
 *                   values in the dialog.
 *	Arguments: w - the Text Widget.
 *                 args - the argument list.
 *                 num_args - the number of args.
 *	Returns: none.
 */

static void
GetValuesHook(w, args, num_args)
Widget w;
ArgList args;
Cardinal * num_args;
{
  Arg a[1];
  String s;
  DialogWidget src = (DialogWidget) w;
  int i;
  
  for (i=0; i < *num_args; i++)
    if (streq(args[i].name, XtNvalue)) {
      XtSetArg(a[0], XtNstring, &s);
      XtGetValues(src->dialog.valueW, a, 1);
      *((char **) args[i].value) = s;
    }
}


/*	Function Name: CreateDialogValueWidget
 *	Description: Creates the dialog widgets value widget.
 *	Arguments: w - the dialog widget.
 *	Returns: none.
 *
 *	must be called only when w->dialog.value is non-nil.
 */

static void
CreateDialogValueWidget(w)
Widget w;
{
    DialogWidget dw = (DialogWidget) w;    
    Arg arglist[10];
    Cardinal num_args = 0;

#ifdef notdef
    XtSetArg(arglist[num_args], XtNwidth,
	     dw->dialog.labelW->core.width); num_args++; /* ||| hack */
#endif /*notdef*/
    XtSetArg(arglist[num_args], XtNstring, dw->dialog.value);     num_args++;
    XtSetArg(arglist[num_args], XtNresizable, True);              num_args++;
    XtSetArg(arglist[num_args], XtNresize, XawtextResizeBoth);    num_args++;
    XtSetArg(arglist[num_args], XtNeditType, XawtextEdit);        num_args++;
    XtSetArg(arglist[num_args], XtNfromVert, dw->dialog.labelW);  num_args++;
    XtSetArg(arglist[num_args], XtNleft, XtChainLeft);            num_args++;
    XtSetArg(arglist[num_args], XtNright, XtChainRight);          num_args++;

    dw->dialog.valueW = XtCreateWidget("value", asciiTextWidgetClass,
				     w, arglist, num_args);

    /* if the value widget is being added after buttons,
     * then the buttons need new layout constraints.
     */
    if (dw->composite.num_children > 1) {
	WidgetList children = dw->composite.children;
	Widget *childP;
        for (childP = children + dw->composite.num_children - 1;
	     childP >= children; childP-- ) {
	    if (*childP == dw->dialog.labelW || *childP == dw->dialog.valueW)
		continue;
	    if (XtIsManaged(*childP) &&
		XtIsSubclass(*childP, commandWidgetClass)) {
	        ((DialogConstraints)(*childP)->core.constraints)->
		    form.vert_base = dw->dialog.valueW;
	    }
	}
    }
    XtManageChild(dw->dialog.valueW);

/* 
 * Value widget gets the keyboard focus.
 */

    XtSetKeyboardFocus(w, dw->dialog.valueW);
    dw->dialog.value = MAGIC_VALUE;
}


void
#if NeedFunctionPrototypes
XawDialogAddButton(Widget dialog, _Xconst char* name, XtCallbackProc function,
		   XtPointer param)
#else
XawDialogAddButton(dialog, name, function, param)
Widget dialog;
String name;
XtCallbackProc function;
XtPointer param;
#endif
{
/*
 * Correct Constraints are all set in ConstraintInitialize().
 */
    Widget button;

    button = XtCreateManagedWidget( name, commandWidgetClass, dialog, 
				    (ArgList)NULL, (Cardinal)0 );

    if (function != NULL)	/* don't add NULL callback func. */
        XtAddCallback(button, XtNcallback, function, param);
}


char *
#if NeedFunctionPrototypes
XawDialogGetValueString(Widget w)
#else
XawDialogGetValueString(w)
Widget w;
#endif
{
    Arg args[1];
    char * value;

    XtSetArg(args[0], XtNstring, &value);
    XtGetValues( ((DialogWidget)w)->dialog.valueW, args, ONE);
    return(value);
}
