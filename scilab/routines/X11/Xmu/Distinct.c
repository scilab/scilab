/*
 * $XConsortium: Distinct.c,v 1.3 90/12/28 19:12:21 gildea Exp $
 *
 * Copyright 1990 Massachusetts Institute of Technology
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

# include   <X11/Xlib.h>

/*
 * Distinguishable colors routine.  Determines if two colors are
 * distinguishable or not.  Somewhat arbitrary meaning.
 */

#define MIN_DISTINGUISH	10000.0

Bool
XmuDistinguishableColors (colors, count)
XColor	*colors;
int	count;
{
    double	    deltaRed, deltaGreen, deltaBlue;
    double	    dist;
    int		    i, j;

    for (i = 0; i < count - 1; i++)
	for (j = i + 1; j < count; j++)
	{
     	    deltaRed = (double)colors[i].red - (double)colors[j].red;
    	    deltaGreen = (double)colors[i].green - (double)colors[j].green;
    	    deltaBlue = (double)colors[i].blue - (double)colors[j].blue;
    	    dist = deltaRed * deltaRed +
	       	   deltaGreen * deltaGreen +
 	       	   deltaBlue * deltaBlue;
	    if (dist <= MIN_DISTINGUISH * MIN_DISTINGUISH)
		return False;
	}
    return True;
}

Bool
XmuDistinguishablePixels (dpy, cmap, pixels, count)
    Display	    *dpy;
    Colormap	    cmap;
    unsigned long   *pixels;
    int		    count;
{
    XColor  *defs;
    int	    i, j;
    Bool    ret;

    for (i = 0; i < count - 1; i++)
	for (j = i + 1; j < count; j++)
	    if (pixels[i] == pixels[j])
		return False;
    defs = (XColor *) malloc (count * sizeof (XColor));
    if (!defs)
	return False;
    for (i = 0; i < count; i++)
	defs[i].pixel = pixels[i];
    XQueryColors (dpy, cmap, defs, count);
    ret = XmuDistinguishableColors (defs, count);
    free ((char *) defs);
    return ret;
}
