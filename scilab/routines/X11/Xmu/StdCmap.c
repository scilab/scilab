/* $XConsortium: StdCmap.c,v 1.11 89/10/08 15:04:52 rws Exp $ 
 * 
 * Copyright 1989 by the Massachusetts Institute of Technology
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
 * M.I.T. DISCLAIMS ALL WARRANTIES WITH REGARD TO THIS SOFTWARE, INCLUDING ALL
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS, IN NO EVENT SHALL M.I.T.
 * BE LIABLE FOR ANY SPECIAL, INDIRECT OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
 * WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN ACTION
 * OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF OR IN
 * CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
 *
 * Author:  Donna Converse, MIT X Consortium
 */

#include <stdio.h>
#include <X11/Xlib.h>
#include <X11/Xatom.h>
#include <X11/Xutil.h>
#include <X11/Xmu/StdCmap.h>

static Status valid_args();		/* argument restrictions */

/*
 * To create any one standard colormap, use XmuStandardColormap().
 *
 * Create a standard colormap for the given screen, visualid, and visual
 * depth, with the given red, green, and blue maximum values, with the
 * given standard property name.  Return a pointer to an XStandardColormap
 * structure which describes the newly created colormap, upon success.
 * Upon failure, return NULL.
 * 
 * XmuStandardColormap() calls XmuCreateColormap() to create the map.
 *
 * Resources created by this function are not made permanent; that is the
 * caller's responsibility.
 */

XStandardColormap *XmuStandardColormap(dpy, screen, visualid, depth, property,
				       cmap, red_max, green_max, blue_max)
    Display		*dpy;		/* specifies X server connection */
    int			screen; 	/* specifies display screen */
    VisualID		visualid;	/* identifies the visual type */
    unsigned int	depth;		/* identifies the visual type */
    Atom		property;	/* a standard colormap property */
    Colormap		cmap;		/* specifies colormap ID or None */
    unsigned long	red_max, green_max, blue_max;	/* allocations */
{
    XStandardColormap	*stdcmap;
    Status		status;
    XVisualInfo		vinfo_template, *vinfo;
    long		vinfo_mask;
    int			n;

    /* Match the required visual information to an actual visual */
    vinfo_template.visualid = visualid;	
    vinfo_template.screen = screen;
    vinfo_template.depth = depth;
    vinfo_mask = VisualIDMask | VisualScreenMask | VisualDepthMask;
    if ((vinfo = XGetVisualInfo(dpy, vinfo_mask, &vinfo_template, &n)) == NULL)
	return 0;

    /* Check the validity of the combination of visual characteristics,
     * allocation, and colormap property.  Create an XStandardColormap
     * structure.
     */

    if (! valid_args(vinfo, red_max, green_max, blue_max, property)
	|| ((stdcmap = XAllocStandardColormap()) == NULL)) {
	XFree((char *) vinfo);
	return 0;
    }

    /* Fill in the XStandardColormap structure */

    if (cmap == DefaultColormap(dpy, screen)) {
	/* Allocating out of the default map, cannot use XFreeColormap() */
	Window win = XCreateWindow(dpy, RootWindow(dpy, screen), 1, 1, 1, 1,
				   0, 0, InputOnly, vinfo->visual,
				   (unsigned long) 0,
				   (XSetWindowAttributes *)NULL);
	stdcmap->killid  = (XID) XCreatePixmap(dpy, win, 1, 1, depth);
	XDestroyWindow(dpy, win);
	stdcmap->colormap = cmap;
    } else {
	stdcmap->killid = ReleaseByFreeingColormap;
	stdcmap->colormap = XCreateColormap(dpy, RootWindow(dpy, screen),
					    vinfo->visual, AllocNone);
    }
    stdcmap->red_max = red_max;
    stdcmap->green_max = green_max;
    stdcmap->blue_max = blue_max;
    if (property == XA_RGB_GRAY_MAP) 
	stdcmap->red_mult = stdcmap->green_mult = stdcmap->blue_mult = 1;
    else {
	stdcmap->red_mult = (red_max > 0)
	    ? (green_max + 1) * (blue_max + 1) : 0;
	stdcmap->green_mult = (green_max > 0) ? blue_max + 1 : 0;
	stdcmap->blue_mult = (blue_max > 0) ? 1 : 0;
    }
    stdcmap->base_pixel = 0;			/* base pixel may change */
    stdcmap->visualid = vinfo->visualid;

    /* Make the colormap */

    status = XmuCreateColormap(dpy, stdcmap);

    /* Clean up */

    XFree((char *) vinfo);
    if (!status) {

	/* Free the colormap or the pixmap, if we created one */
	if (stdcmap->killid == ReleaseByFreeingColormap)
	    XFreeColormap(dpy, stdcmap->colormap);
	else if (stdcmap->killid != None)
	    XFreePixmap(dpy, stdcmap->killid);
	
	XFree((char *) stdcmap);
	return (XStandardColormap *) NULL;
    }
    return stdcmap;
}

/****************************************************************************/
static Status valid_args(vinfo, red_max, green_max, blue_max, property)
    XVisualInfo		*vinfo;		/* specifies visual */
    unsigned long	red_max, green_max, blue_max;	/* specifies alloc */
    Atom		property;	/* specifies property name */
{
    unsigned long	ncolors;	/* number of colors requested */

    /* Determine that the number of colors requested is <= map size */

    if ((vinfo->class == DirectColor) || (vinfo->class == TrueColor)) {
	unsigned long mask;

	mask = vinfo->red_mask;
	while (!(mask & 1))
	    mask >>= 1;
	if (red_max > mask)
	    return 0;
	mask = vinfo->green_mask;
	while (!(mask & 1))
	    mask >>= 1;
	if (green_max > mask)
	    return 0;
	mask = vinfo->blue_mask;
	while (!(mask & 1))
	    mask >>= 1;
	if (blue_max > mask)
	    return 0;
    } else if (property == XA_RGB_GRAY_MAP) {
	ncolors = red_max + green_max + blue_max + 1;
	if (ncolors > vinfo->colormap_size)
	    return 0;
    } else {
	ncolors = (red_max + 1) * (green_max + 1) * (blue_max + 1);
	if (ncolors > vinfo->colormap_size)
	    return 0;
    }
    
    /* Determine that the allocation and visual make sense for the property */

    switch (property)
    {
      case XA_RGB_DEFAULT_MAP:
	if ((red_max == 0 || green_max == 0 || blue_max == 0) ||
	    (vinfo->class != PseudoColor && vinfo->class != DirectColor &&
	     vinfo->class != GrayScale))
	    return 0;
	break;
      case XA_RGB_RED_MAP:
	if ((vinfo->class != PseudoColor && vinfo->class != DirectColor) ||
	    (red_max == 0))
	    return 0;
	break;
      case XA_RGB_GREEN_MAP:
	if ((vinfo->class != PseudoColor && vinfo->class != DirectColor) ||
	    (green_max == 0))
	    return 0;
	break;
      case XA_RGB_BLUE_MAP:	
	if ((vinfo->class != PseudoColor && vinfo->class != DirectColor) ||
	    blue_max == 0)
	    return 0;
	break;
      case XA_RGB_BEST_MAP:
	if (vinfo->class == GrayScale || vinfo->class == StaticGray ||
	    red_max == 0 || green_max == 0 || blue_max == 0)
	    return 0;
	break;
      case XA_RGB_GRAY_MAP:
	if (vinfo->class == StaticColor || vinfo->class == TrueColor ||
	    red_max == 0 || blue_max == 0 || green_max == 0)
	    return 0;
	break;
      default:
	return 0;
    }
    return 1;
}
