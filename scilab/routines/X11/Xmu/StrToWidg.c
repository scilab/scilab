/* $XConsortium: StrToWidg.c,v 1.8 91/07/23 15:19:48 converse Exp $ */

/* Copyright 1988, 1991 Massachusetts Institute of Technology
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
 *
 * XmuCvtStringToWidget
 *
 *   static XtConvertArgRec parentCvtArgs[] = {
 *	{XtBaseOffset, (caddr_t)XtOffset(Widget, core.parent), sizeof(Widget)},
 *   };
 *
 * matches the string against the name of the immediate children (normal
 * or popup) of the parent.  If none match, compares string to classname
 * & returns first match.  Case is significant.
 */
#include <X11/IntrinsicP.h>
#include <X11/StringDefs.h>
#include <X11/ObjectP.h>

#define	done(address, type) \
	{ toVal->size = sizeof(type); \
	  toVal->addr = (XPointer) address; \
	  return; \
	}

/* ARGSUSED */
void XmuCvtStringToWidget(args, num_args, fromVal, toVal)
    XrmValuePtr args;		/* parent */
    Cardinal    *num_args;      /* 1 */
    XrmValuePtr fromVal;
    XrmValuePtr toVal;
{
    static Widget widget, *widgetP, parent;
    XrmName name = XrmStringToName(fromVal->addr);
    int i;

    if (*num_args != 1)
	XtErrorMsg("wrongParameters", "cvtStringToWidget", "xtToolkitError",
		   "StringToWidget conversion needs parent arg", NULL, 0);

    parent = *(Widget*)args[0].addr;
    /* try to match names of normal children */
    if (XtIsComposite(parent)) {
	i = ((CompositeWidget)parent)->composite.num_children;
	for (widgetP = ((CompositeWidget)parent)->composite.children;
	     i; i--, widgetP++) {
	    if ((*widgetP)->core.xrm_name == name) {
		widget = *widgetP;
		done(&widget, Widget);
	    }
	}
    }
    /* try to match names of popup children */
    i = parent->core.num_popups;
    for (widgetP = parent->core.popup_list; i; i--, widgetP++) {
	if ((*widgetP)->core.xrm_name == name) {
	    widget = *widgetP;
	    done(&widget, Widget);
	}
    }
    /* try to match classes of normal children */
    if (XtIsComposite(parent)) {
	i = ((CompositeWidget)parent)->composite.num_children;
	for (widgetP = ((CompositeWidget)parent)->composite.children;
	     i; i--, widgetP++) {
	    if ((*widgetP)->core.widget_class->core_class.xrm_class == name) {
		widget = *widgetP;
		done(&widget, Widget);
	    }
	}
    }
    /* try to match classes of popup children */
    i = parent->core.num_popups;
    for (widgetP = parent->core.popup_list; i; i--, widgetP++) {
	if ((*widgetP)->core.widget_class->core_class.xrm_class == name) {
	    widget = *widgetP;
	    done(&widget, Widget);
	}
    }
    XtStringConversionWarning(fromVal->addr, XtRWidget);
    toVal->addr = NULL;
    toVal->size = 0;
}

#undef done

#define	newDone(type, value) \
	{							\
	    if (toVal->addr != NULL) {				\
		if (toVal->size < sizeof(type)) {		\
		    toVal->size = sizeof(type);			\
		    return False;				\
		}						\
		*(type*)(toVal->addr) = (value);		\
	    }							\
	    else {						\
		static type static_val;				\
		static_val = (value);				\
		toVal->addr = (XtPointer)&static_val;		\
	    }							\
	    toVal->size = sizeof(type);				\
	    return True;					\
	}


/*ARGSUSED*/
Boolean XmuNewCvtStringToWidget(dpy, args, num_args, fromVal, toVal, 
				converter_data)
     Display *dpy;
     XrmValue *args;		/* parent */
     Cardinal *num_args;	/* 1 */
     XrmValue *fromVal;
     XrmValue *toVal;
     XtPointer *converter_data;
{
    Widget *widgetP, parent;
    XrmName name = XrmStringToName(fromVal->addr);
    int i;

    if (*num_args != 1)
	XtAppWarningMsg(XtDisplayToApplicationContext(dpy),
			"wrongParameters","cvtStringToWidget","xtToolkitError",
			"String To Widget conversion needs parent argument",
			(String *)NULL, (Cardinal *)NULL);

    parent = *(Widget*)args[0].addr;
    /* try to match names of normal children */
    if (XtIsComposite(parent)) {
	i = ((CompositeWidget)parent)->composite.num_children;
	for (widgetP = ((CompositeWidget)parent)->composite.children;
	     i; i--, widgetP++) {
	    if ((*widgetP)->core.xrm_name == name)
		newDone(Widget, *widgetP);
	}
    }
    /* try to match names of popup children */
    i = parent->core.num_popups;
    for (widgetP = parent->core.popup_list; i; i--, widgetP++) {
	if ((*widgetP)->core.xrm_name == name)
	    newDone(Widget, *widgetP);
    }
    /* try to match classes of normal children */
    if (XtIsComposite(parent)) {
	i = ((CompositeWidget)parent)->composite.num_children;
	for (widgetP = ((CompositeWidget)parent)->composite.children;
	     i; i--, widgetP++) {
	    if ((*widgetP)->core.widget_class->core_class.xrm_class == name)
		newDone(Widget, *widgetP);
	}
    }
    /* try to match classes of popup children */
    i = parent->core.num_popups;
    for (widgetP = parent->core.popup_list; i; i--, widgetP++) {
	if ((*widgetP)->core.widget_class->core_class.xrm_class == name)
	    newDone(Widget, *widgetP);
    }
    XtDisplayStringConversionWarning(dpy, (String)fromVal->addr, XtRWidget);
    return False;
}
