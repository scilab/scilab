/* $XConsortium: StrToCurs.c,v 1.16 91/07/25 17:48:36 converse Exp $ */

/***********************************************************
Copyright 1987, 1988 by Digital Equipment Corporation, Maynard, Massachusetts,
and the Massachusetts Institute of Technology, Cambridge, Massachusetts.

                        All Rights Reserved

Permission to use, copy, modify, and distribute this software and its 
documentation for any purpose and without fee is hereby granted, 
provided that the above copyright notice appear in all copies and that
both that copyright notice and this permission notice appear in 
supporting documentation, and that the names of Digital or MIT not be
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

#include	<X11/IntrinsicP.h>	/* 'cause CoreP.h needs it */
#include	<X11/CoreP.h>		/* just to do XtConvert() */
#include	<X11/StringDefs.h>
#include	<X11/Xmu/Converters.h>
#include	<X11/Xmu/Drawing.h>

#ifndef X_NOT_POSIX
#ifdef _POSIX_SOURCE
#include <limits.h>
#else
#define _POSIX_SOURCE
#include <limits.h>
#undef _POSIX_SOURCE
#endif
#endif /* X_NOT_POSIX */
#ifndef PATH_MAX
#include <sys/param.h>
#ifdef MAXPATHLEN
#define PATH_MAX MAXPATHLEN
#else
#define PATH_MAX 1024
#endif
#endif /* PATH_MAX */

/*
 * XmuConvertStringToCursor:
 *
 * allows String to specify a standard cursor name (from cursorfont.h), a
 * font name and glyph index of the form "FONT fontname index [[font] index]", 
 * or a bitmap file name (absolute, or relative to the global resource
 * bitmapFilePath, class BitmapFilePath).  If the resource is not
 * defined, the default value is the build symbol BITMAPDIR.
 *
 * shares lots of code with XmuCvtStringToPixmap, but unfortunately
 * can't use it as the hotspot info is lost.
 *
 * To use, include the following in your ClassInitialize procedure:

static XtConvertArgRec screenConvertArg[] = {
    {XtBaseOffset, (XtPointer) XtOffsetOf(WidgetRec, core.screen),
     sizeof(Screen *)}
};

    XtAddConverter(XtRString, XtRCursor, XmuCvtStringToCursor,      
		   screenConvertArg, XtNumber(screenConvertArg));
 *
 */

#define	done(address, type) \
	{ (*toVal).size = sizeof(type); (*toVal).addr = (XPointer) address; }

#define FONTSPECIFIER		"FONT "

/*ARGSUSED*/
void XmuCvtStringToCursor(args, num_args, fromVal, toVal)
    XrmValuePtr args;
    Cardinal    *num_args;
    XrmValuePtr	fromVal;
    XrmValuePtr	toVal;
{
    static Cursor cursor;		/* static for cvt magic */
    char *name = (char *)fromVal->addr;
    Screen *screen;
    register int i;
    char maskname[PATH_MAX];
    Pixmap source, mask;
    /* XXX - make fg/bg resources */
    static XColor bgColor = {0, 0xffff, 0xffff, 0xffff};
    static XColor fgColor = {0, 0, 0, 0};
    int xhot, yhot;
    int len;


    if (*num_args != 1)
     XtErrorMsg("wrongParameters","cvtStringToCursor","XtToolkitError",
             "String to cursor conversion needs screen argument",
              (String *)NULL, (Cardinal *)NULL);

    screen = *((Screen **) args[0].addr);

    if (0 == strncmp(FONTSPECIFIER, name, strlen(FONTSPECIFIER))) {
	char source_name[PATH_MAX], mask_name[PATH_MAX];
	int source_char, mask_char, fields;
	WidgetRec widgetRec;
	Font source_font, mask_font;
	XrmValue fromString, toFont;

	fields = sscanf(name, "FONT %s %d %s %d",
			source_name, &source_char,
			mask_name, &mask_char);
	if (fields < 2) {
	    XtStringConversionWarning(name, XtRCursor);
	    return;
	}

	/* widgetRec is stupid; we should just use XtDirectConvert,
	 * but the names in Xt/Converters aren't public. */
	widgetRec.core.screen = screen;
	fromString.addr = source_name;
	fromString.size = strlen(source_name);
	XtConvert(&widgetRec, XtRString, &fromString, XtRFont, &toFont);
	if (toFont.addr == NULL) {
	    XtStringConversionWarning(name, XtRCursor);
	    return;
	}
	source_font = *(Font*)toFont.addr;

	switch (fields) {
	  case 2:		/* defaulted mask font & char */
	    mask_font = source_font;
	    mask_char = source_char;
	    break;

	  case 3:		/* defaulted mask font */
	    mask_font = source_font;
	    mask_char = atoi(mask_name);
	    break;

	  case 4:		/* specified mask font & char */
	    fromString.addr = mask_name;
	    fromString.size = strlen(mask_name);
	    XtConvert(&widgetRec, XtRString, &fromString, XtRFont, &toFont);
	    if (toFont.addr == NULL) {
		XtStringConversionWarning(name, XtRCursor);
		return;
	    }
	    mask_font = *(Font*)toFont.addr;
	}

	cursor = XCreateGlyphCursor( DisplayOfScreen(screen), source_font,
				     mask_font, source_char, mask_char,
				     &fgColor, &bgColor );
	done(&cursor, Cursor);
	return;
    }

    i = XmuCursorNameToIndex (name);
    if (i != -1) {
	cursor = XCreateFontCursor (DisplayOfScreen(screen), i);
	done(&cursor, Cursor);
	return;
    }

    if ((source = XmuLocateBitmapFile (screen, name, 
				       maskname, (sizeof maskname) - 4,
				       NULL, NULL, &xhot, &yhot)) == None) {
	XtStringConversionWarning (name, XtRCursor);
    }
    len = strlen (maskname);
    for (i = 0; i < 2; i++) {
	strcpy (maskname + len, i == 0 ? "Mask" : "msk");
	if ((mask = XmuLocateBitmapFile (screen, maskname, NULL, 0, 
					 NULL, NULL, NULL, NULL)) != None)
	  break;
    }

    cursor = XCreatePixmapCursor( DisplayOfScreen(screen), source, mask,
				  &fgColor, &bgColor, xhot, yhot );
    XFreePixmap( DisplayOfScreen(screen), source );
    if (mask != None) XFreePixmap( DisplayOfScreen(screen), mask );

    done(&cursor, Cursor);
}

#define	new_done(type, value) \
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
		toVal->addr = (XPointer)&static_val;		\
	    }							\
	    toVal->size = sizeof(type);				\
	    return True;					\
	}

/*	Function Name: XmuCvtStringToColorCursor
 *	Description: Converts a string into a colored cursor.
 *	Arguments: dpy
 *		   args - an argument list (see below).
 *                 num_args - number of elements in the argument list.
 *                 fromVal - value to convert from.
 *                 toVal - value to convert to.
 *		   data
 *	Returns:   True or False
 */

/*ARGSUSED*/
Boolean
XmuCvtStringToColorCursor(dpy, args, num_args, fromVal, toVal, converter_data)
    Display     *dpy;
    XrmValuePtr args;
    Cardinal    *num_args;
    XrmValuePtr	fromVal;
    XrmValuePtr	toVal;
    XtPointer   *converter_data;	/* unused */
{
    Cursor cursor;
    Pixel fg, bg;
    Colormap c_map;
    XColor colors[2];
    Cardinal number;
    XrmValue ret_val;

    if (*num_args != 4) {
	XtAppWarningMsg(XtDisplayToApplicationContext(dpy),
	    "wrongParameters","cvtStringToColorCursor","XmuError",
            "String to color cursor conversion needs four arguments",
	    (String *)NULL, (Cardinal *)NULL);
	return False;
    }

    fg = *((Pixel *) args[1].addr);
    bg = *((Pixel *) args[2].addr);
    c_map = *((Colormap *) args[3].addr);

    number = 1;
    XmuCvtStringToCursor(args, &number, fromVal, &ret_val);
    
    cursor = *((Cursor *) ret_val.addr);

    if (cursor == None)
	new_done(Cursor, cursor);

    colors[0].pixel = fg;
    colors[1].pixel = bg;

    XQueryColors (dpy, c_map, colors, 2);
    XRecolorCursor(dpy, cursor, colors, colors + 1);
    new_done(Cursor, cursor);
}

    
    
