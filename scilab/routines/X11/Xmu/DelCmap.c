/* $XConsortium: DelCmap.c,v 1.1 89/05/19 14:37:16 converse Exp $
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

#include <X11/Xlib.h>
#include <X11/Xutil.h>

/* To remove any standard colormap property, use XmuDeleteStandardColormap().
 * XmuDeleteStandardColormap() will remove the specified property from the
 * specified screen, releasing any resources used by the colormap(s) of the
 * property if possible.
 */

void XmuDeleteStandardColormap(dpy, screen, property)
    Display	*dpy;		/* specifies the X server to connect to */
    int		screen;		/* specifies the screen of the display */
    Atom	property;	/* specifies the standard colormap property */
{
    XStandardColormap	*stdcmaps, *s;
    int			count = 0;

    if (XGetRGBColormaps(dpy, RootWindow(dpy, screen), &stdcmaps, &count,
			 property))
    {
	for (s=stdcmaps; count > 0; count--, s++) {
	    if ((s->killid == ReleaseByFreeingColormap) &&
		(s->colormap != None) &&
		(s->colormap != DefaultColormap(dpy, screen)))
		XFreeColormap(dpy, s->colormap);
	    else if (s->killid != None)
		XKillClient(dpy, s->killid);
	}
	XDeleteProperty(dpy, RootWindow(dpy, screen), property);
	XFree((char *) stdcmaps);
	XSync(dpy, False);
    }
}

