/* $XConsortium: VisCmap.c,v 1.10 89/10/08 15:05:47 rws Exp $ 
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
#include <math.h>
#include <X11/Xlib.h>
#include <X11/Xatom.h>
#include <X11/Xutil.h>
#include <X11/Xmu/StdCmap.h>

/*
 * To create all of the appropriate standard colormaps for a given visual on
 * a given screen, use XmuVisualStandardColormaps.
 * 
 * Define all appropriate standard colormap properties for the given visual.
 * If replace is true, any previous definition will be removed.
 * If retain is true, new properties will be retained for the duration of
 * the server session.  Return 0 on failure, non-zero on success.
 * On failure, no new properties will be defined, and, old ones may have
 * been removed if replace was True.
 *
 * Not all standard colormaps are meaningful to all visual classes.  This
 * routine will check and define the following properties for the following
 * classes, provided that the size of the colormap is not too small.
 *
 *	DirectColor and PseudoColor
 *	    RGB_DEFAULT_MAP
 *	    RGB_BEST_MAP
 *	    RGB_RED_MAP
 *	    RGB_GREEN_MAP
 * 	    RGB_BLUE_MAP
 *          RGB_GRAY_MAP
 *
 *	TrueColor and StaticColor
 *	    RGB_BEST_MAP
 *
 *	GrayScale and StaticGray
 *	    RGB_GRAY_MAP
 */

Status XmuVisualStandardColormaps(dpy, screen, visualid, depth, replace,
				  retain)
    Display		*dpy;		/* specifies server connection */
    int			screen;		/* specifies screen number */
    VisualID		visualid;	/* specifies the visual */
    unsigned int	depth;		/* specifies the visual */
    Bool		replace;	/* specifies whether to replace */
    Bool		retain;		/* specifies whether to retain */
{
    Status		status;
    int			n;
    long		vinfo_mask;
    XVisualInfo		vinfo_template, *vinfo;
        
    vinfo_template.screen = screen;
    vinfo_template.visualid = visualid;
    vinfo_template.depth = depth;
    vinfo_mask = VisualScreenMask | VisualIDMask | VisualDepthMask;
    if ((vinfo = XGetVisualInfo(dpy, vinfo_mask, &vinfo_template, &n)) == NULL)
	return 0;

    if (vinfo->colormap_size <= 2) {
	/* Monochrome visuals have no standard maps; considered successful */
	XFree((char *) vinfo);
	return 1;
    }

    switch (vinfo->class)
    {
      case PseudoColor:
      case DirectColor:
	status = XmuLookupStandardColormap(dpy, screen, visualid, depth,
					   XA_RGB_DEFAULT_MAP, replace,retain);
	if (!status) break;

	status = XmuLookupStandardColormap(dpy, screen, visualid, depth,
					   XA_RGB_GRAY_MAP, replace, retain);
	if (!status) {
	    XmuDeleteStandardColormap(dpy, screen, XA_RGB_DEFAULT_MAP);
	    break;
	}

	status = XmuLookupStandardColormap(dpy, screen, visualid, depth,
					   XA_RGB_RED_MAP, replace, retain);
	if (!status) {
	    XmuDeleteStandardColormap(dpy, screen, XA_RGB_DEFAULT_MAP);
	    XmuDeleteStandardColormap(dpy, screen, XA_RGB_GRAY_MAP);
	    break;
	}

	status = XmuLookupStandardColormap(dpy, screen, visualid, depth,
					   XA_RGB_GREEN_MAP, replace, retain);
	if (!status) {
	    XmuDeleteStandardColormap(dpy, screen, XA_RGB_DEFAULT_MAP);
	    XmuDeleteStandardColormap(dpy, screen, XA_RGB_GRAY_MAP);
	    XmuDeleteStandardColormap(dpy, screen, XA_RGB_RED_MAP);
	    break;
	}

	status = XmuLookupStandardColormap(dpy, screen, visualid, depth,
					   XA_RGB_BLUE_MAP, replace, retain);
	if (!status) {
	    XmuDeleteStandardColormap(dpy, screen, XA_RGB_DEFAULT_MAP);
	    XmuDeleteStandardColormap(dpy, screen, XA_RGB_GRAY_MAP);
	    XmuDeleteStandardColormap(dpy, screen, XA_RGB_RED_MAP);
	    XmuDeleteStandardColormap(dpy, screen, XA_RGB_GREEN_MAP);
	    break;
	}
	/* fall through */

      case StaticColor:
      case TrueColor:

	status = XmuLookupStandardColormap(dpy, screen, visualid, depth,
					   XA_RGB_BEST_MAP, replace, retain);
	if (!status && (vinfo->class == PseudoColor || 
			vinfo->class == DirectColor)) {
	    XmuDeleteStandardColormap(dpy, screen, XA_RGB_DEFAULT_MAP);
	    XmuDeleteStandardColormap(dpy, screen, XA_RGB_GRAY_MAP);
	    XmuDeleteStandardColormap(dpy, screen, XA_RGB_RED_MAP);
	    XmuDeleteStandardColormap(dpy, screen, XA_RGB_GREEN_MAP);
	    XmuDeleteStandardColormap(dpy, screen, XA_RGB_BLUE_MAP);
	}
	break;
	/* the end for PseudoColor, DirectColor, StaticColor, and TrueColor */

      case GrayScale:
	status = XmuLookupStandardColormap(dpy, screen, visualid, depth,
					   XA_RGB_DEFAULT_MAP, replace,
					   retain);
	if (! status) break;
	/* fall through */

      case StaticGray:

	status = XmuLookupStandardColormap(dpy, screen, visualid, depth,
					   XA_RGB_GRAY_MAP, replace, retain);
	if (! status && vinfo->class == GrayScale) {
	    XmuDeleteStandardColormap(dpy, screen, XA_RGB_DEFAULT_MAP);
	    break;
	}
    }

    XFree((char *) vinfo);
    return status;
}
