/* $XConsortium: StrToShap.c,v 1.3 90/12/20 13:36:01 converse Exp $ */

/* 
 * Copyright 1988 by the Massachusetts Institute of Technology
 *
 * Permission to use, copy, modify, and distribute this software and its
 * documentation for any purpose and without fee is hereby granted, provided 
 * that the above copyright notice appear in all copies and that both that 
 * copyright notice and this permission notice appear in supporting 
 * documentation, and that the name of M.I.T. not be used in advertising
 * or publicity pertaining to distribution of the software without specific, 
 * written prior permission. M.I.T. makes no representations about the 
 * suitability of this software for any purpose.  It is provided "as is"
 * without express or implied warranty.
 *
 */

#include <X11/Intrinsic.h>
#include "Converters.h"
#include "CharSet.h"

/* ARGSUSED */
#define	done(type, value) \
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
		toVal->size = sizeof(type);			\
		toVal->addr = (XtPointer)&static_val;		\
	    }							\
	    return True;					\
	}


Boolean XmuCvtStringToShapeStyle(dpy, args, num_args, from, toVal, data)
    Display *dpy;
    XrmValue *args;		/* unused */
    Cardinal *num_args;		/* unused */
    XrmValue *from;
    XrmValue *toVal;
    XtPointer *data;		/* unused */
{
    if (   XmuCompareISOLatin1((char*)from->addr, XtERectangle) == 0
	|| XmuCompareISOLatin1((char*)from->addr, "ShapeRectangle") == 0)
	done( int, XmuShapeRectangle );
    if (   XmuCompareISOLatin1((char*)from->addr, XtEOval) == 0
	|| XmuCompareISOLatin1((char*)from->addr, "ShapeOval") == 0)
	done( int, XmuShapeOval );
    if (   XmuCompareISOLatin1((char*)from->addr, XtEEllipse) == 0
	|| XmuCompareISOLatin1((char*)from->addr, "ShapeEllipse") == 0)
	done( int, XmuShapeEllipse );
    if (   XmuCompareISOLatin1((char*)from->addr, XtERoundedRectangle) == 0
	|| XmuCompareISOLatin1((char*)from->addr, "ShapeRoundedRectangle") == 0)
	done( int, XmuShapeRoundedRectangle );
    {
	int style = 0;
	char ch, *p = (char*)from->addr;
	while (ch = *p++) {
	    if (ch >= '0' && ch <= '9') {
		style *= 10;
		style += ch - '0';
	    }
	    else break;
	}
	if (ch == '\0' && style <= XmuShapeRoundedRectangle)
	    done( int, style );
    }
    XtDisplayStringConversionWarning( dpy, (char*)from->addr, XtRShapeStyle );
    return False;
}
