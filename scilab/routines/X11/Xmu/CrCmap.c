/* $XConsortium: CrCmap.c,v 1.3 91/04/10 16:45:46 converse Exp $
 *
 * CreateCmap.c - given a standard colormap description, make the map.
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
#include <X11/Xutil.h>

extern char	*calloc();

static int	ROmap();		/* allocate entire map Read Only */
static Status	ROorRWcell();		/* allocate a cell, prefer Read Only */
static Status	RWcell();		/* allocate a cell Read Write */
static int	compare();		/* for quicksort */
static Status 	contiguous();		/* find contiguous sequence of cells */
static void	free_cells();		/* frees resources before quitting */
static Status	readonly_map();		/* create a map in a RO visual type */
static Status	readwrite_map();	/* create a map in a RW visual type */

/*
 * To create any one colormap which is described by an XStandardColormap
 * structure, use XmuCreateColormap().
 *
 * Return 0 on failure, non-zero on success.
 * Resources created by this function are not made permanent.
 * No argument error checking is provided.  Use at your own risk.
 *
 * All colormaps are created with read only allocations, with the exception
 * of read only allocations of colors in the default map or otherwise
 * which fail to return the expected pixel value, and these are individually 
 * defined as read/write allocations.  This is done so that all the cells
 * defined in the default map are contiguous, for use in image processing.
 * This typically happens with White and Black in the default map.
 *
 * Colormaps of static visuals are considered to be successfully created if
 * the map of the static visual matches the definition given in the
 * standard colormap structure.
 */
   
Status XmuCreateColormap(dpy, colormap)
    Display		*dpy;		/* specifies the connection under 
					 * which the map is created */
    XStandardColormap	*colormap;	/* specifies the map to be created,
					 * and returns, particularly if the
					 * map is created as a subset of the
					 * default colormap of the screen,
					 * the base_pixel of the map.
					 */
{
    XVisualInfo		vinfo_template;	/* template visual information */
    XVisualInfo		*vinfo;		/* matching visual information */
    XVisualInfo		*vpointer;	/* for freeing the entire list */
    long		vinfo_mask;	/* specifies the visual mask value */
    int 		n;		/* number of matching visuals */
    int			status;		

    vinfo_template.visualid = colormap->visualid;
    vinfo_mask = VisualIDMask;
    if ((vinfo = XGetVisualInfo(dpy, vinfo_mask, &vinfo_template, &n)) == NULL)
	return 0;

    /* A visual id may be valid on multiple screens.  Also, there may 
     * be multiple visuals with identical visual ids at different depths.  
     * If the colormap is the Default Colormap, use the Default Visual.
     * Otherwise, arbitrarily, use the deepest visual.
     */
    vpointer = vinfo;
    if (n > 1)
    {
	register int	i;
	register int	screen_number;
	Bool 		def_cmap;

	def_cmap = False;
	for (screen_number = ScreenCount(dpy); --screen_number >= 0; )
	    if (colormap->colormap == DefaultColormap(dpy, screen_number)) {
		def_cmap = True;
		break;
	    }

	if (def_cmap) {
	    for (i=0; i < n; i++, vinfo++) {
		if (vinfo->visual == DefaultVisual(dpy, screen_number))
			break;
	    }
	} else {
	    unsigned int	maxdepth = 0;
	    XVisualInfo		*v;

	    for (i=0; i < n; i++, vinfo++)
		if (vinfo->depth > maxdepth) {
		    maxdepth = vinfo->depth;
		    v = vinfo;
		}
	    vinfo = v;
	}
    }

    if (vinfo->class == PseudoColor || vinfo->class == DirectColor ||
	vinfo->class == GrayScale)
	status = readwrite_map(dpy, vinfo, colormap);
    else 
	status = readonly_map(dpy, vinfo, colormap);
    
    XFree((char *) vpointer);
    return status;
}

/****************************************************************************/
static Status readwrite_map(dpy, vinfo, colormap)
    Display		*dpy;
    XVisualInfo		*vinfo;
    XStandardColormap	*colormap;
{
    register int	i, n;		/* index counters */
    int			ncolors;	/* number of colors to be defined */
    int			npixels;	/* number of pixels allocated R/W */
    int			first_index;	/* first index of pixels to use */
    int			remainder;	/* first index of remainder */
    XColor		color;		/* the definition of a color */
    unsigned long	*pixels;	/* array of colormap pixels */

    
    /* Determine ncolors, the number of colors to be defined.
     * Insure that 1 < ncolors <= the colormap size.
     */
    ncolors = colormap->red_max * colormap->red_mult +
	      colormap->green_max * colormap->green_mult +
	      colormap->blue_max * colormap->blue_mult + 1;
    if (ncolors <= 1 || ncolors > vinfo->colormap_size)	return 0;

    /* Allocate Read/Write as much of the colormap as we can possibly get.
     * Then insure that the pixels we were allocated are given in 
     * monotonically increasing order, using a quicksort.  Next, insure
     * that our allocation includes a subset of contiguous pixels at least
     * as long as the number of colors to be defined.  Now we know that 
     * these conditions are met:
     *	1) There are no free cells in the colormap.
     *  2) We have a contiguous sequence of pixels, monotonically 
     *     increasing, of length >= the number of colors requested.
     *
     * One cell at a time, we will free, compute the next color value, 
     * then allocate read only.  This takes a long time.
     * This is done to insure that cells are allocated read only in the
     * contiguous order which we prefer.  If the server has a choice of
     * cells to grant to an allocation request, the server may give us any
     * cell, so that is why we do these slow gymnastics.
     */

    if ((pixels = (unsigned long *) calloc((unsigned) vinfo->colormap_size,
				      sizeof(unsigned long))) == NULL)
	return 0;

    if ((npixels = ROmap(dpy, colormap->colormap, pixels,
			   vinfo->colormap_size, ncolors)) == 0) {
	free((char *) pixels);
	return 0;
    }

    qsort((char *) pixels, npixels, sizeof(unsigned long), compare);

    if (! contiguous(pixels, npixels, ncolors, &first_index, &remainder))
    {
	/* can't find enough contiguous cells, give up */
	XFreeColors(dpy, colormap->colormap, pixels, npixels,
		    (unsigned long) 0);
	free((char *) pixels);
	return 0;
    }
    colormap->base_pixel = pixels[first_index];

    /* construct a gray map */
    if (colormap->red_mult == 1 && colormap->green_mult == 1 &&
	colormap->blue_mult == 1)
	for (n=colormap->base_pixel, i=0; i < ncolors; i++, n++)
	{
	    color.pixel = (unsigned long) n;
	    color.blue = color.green = color.red =
		(unsigned short) ((i * 65535) / (colormap->red_max +
						 colormap->green_max +
						 colormap->blue_max));

	    if (! ROorRWcell(dpy, colormap->colormap, pixels, npixels, &color,
			     first_index + i, n))
		return 0;
	}

    /* construct a red ramp map */
    else if (colormap->green_max == 0 && colormap->blue_max == 0)
    	for (n=colormap->base_pixel, i=0; i < ncolors; i++, n++)
	{
	    color.pixel = (unsigned long) n;
	    color.red = (unsigned short) ((i * 65535) / colormap->red_max);
	    color.green = color.blue = 0;

	    if (! ROorRWcell(dpy, colormap->colormap, pixels, npixels, &color,
			     first_index + i, n))
		return 0;
	}

    /* construct a green ramp map */
    else if (colormap->red_max == 0 && colormap->blue_max == 0)
    	for (n=colormap->base_pixel, i=0; i < ncolors; i++, n++)
	{
	    color.pixel = (unsigned long) n;
	    color.green = (unsigned short) ((i * 65535) / colormap->green_max);
	    color.red = color.blue = 0;

	    if (! ROorRWcell(dpy, colormap->colormap, pixels, npixels, &color,
			     first_index + i, n))
		return 0;
	}

    /* construct a blue ramp map */
    else if (colormap->red_max == 0 && colormap->green_max == 0)
    	for (n=colormap->base_pixel, i=0; i < ncolors; i++, n++)
	{
	    color.pixel = (unsigned long) n;
	    color.blue = (unsigned short) ((i * 65535) / colormap->blue_max);
	    color.red = color.green = 0;

	    if (! ROorRWcell(dpy, colormap->colormap, pixels, npixels, &color,
			     first_index + i, n))
		return 0;
	}

    /* construct a standard red green blue cube map */
    else
    {
	int r = colormap->red_mult;
	int g = colormap->green_mult;
	int gg = colormap->green_max + 1;

    	for (n=colormap->base_pixel, i=0; i < ncolors; i++, n++)
	{
	    color.pixel = (unsigned long) n;
	    color.red = (unsigned short) (((i/r) * 65535) / colormap->red_max);
	    color.green = (unsigned short) ((((i/g)%gg) * 65535) /
					    colormap->green_max);
	    color.blue = (unsigned short) (((i%g) * 65535) /
					   colormap->blue_max);
	    if (! ROorRWcell(dpy, colormap->colormap, pixels, npixels, &color,
			     first_index + i, n))
		return 0;
	}
    }
    /* We have a read-only map defined.  Now free unused cells,
     * first those occuring before the contiguous sequence begins,
     * then any following the contiguous sequence.
     */

    if (first_index)
	XFreeColors(dpy, colormap->colormap, pixels, first_index, 
		    (unsigned long) 0);
    if (remainder)
	XFreeColors(dpy, colormap->colormap,
		    &(pixels[first_index + ncolors]), remainder,
		    (unsigned long) 0);

    free((char *) pixels);
    return 1;
}


/****************************************************************************/
static int ROmap(dpy, cmap, pixels, m, n)
    Display		*dpy;		/* the X server connection */
    Colormap		cmap;		/* specifies colormap ID */
    unsigned long	pixels[];	/* returns pixel allocations */
    int			m;		/* specifies colormap size */
    int			n;		/* specifies number of colors */
{
    register int	p;

    /* first try to allocate the entire colormap */
    if (XAllocColorCells(dpy, cmap, 1, (unsigned long *) NULL, 
			 (unsigned) 0, pixels, (unsigned) m))
	return m;

    /* Allocate all available cells in the colormap, using a binary
     * algorithm to discover how many cells we can allocate in the colormap.
     */
    m--;
    while (n <= m) {
	p = n + ((m - n + 1) / 2);
	if (XAllocColorCells(dpy, cmap, 1, (unsigned long *) NULL,
			     (unsigned) 0, pixels, (unsigned) p)) {
	    if (p == m)
		return p;
	    else {
		XFreeColors(dpy, cmap, pixels, p, (unsigned long) 0);
		n = p;
	    }
	}
	else
	    m = p - 1;
    }
    return 0;
}
      

/****************************************************************************/
static Status contiguous(pixels, npixels, ncolors, first, rem)
    unsigned long	pixels[];	/* specifies allocated pixels */
    int			npixels;	/* specifies count of alloc'd pixels */
    int			ncolors;	/* specifies needed sequence length */
    int			*first;		/* returns first index of sequence */
    int			*rem;		/* returns first index after sequence,
					 * or 0, if none follow */
{
    register int i = 1;		/* walking index into the pixel array */
    register int count = 1;	/* length of sequence discovered so far */

    *rem = npixels - 1;
    *first = 0;
    while (count < ncolors && ncolors - count <= *rem)
    {
	if (pixels[i-1] + 1 == pixels[i])
	    count++;
	else {
	    count = 1;
	    *first = i;
	}
	i++;
	(*rem)--;
    }
    if (count != ncolors)
	return 0;
    return 1;
}


/****************************************************************************/
static Status ROorRWcell(dpy, cmap, pixels, npixels, color, p, n)
    Display		*dpy;
    Colormap		cmap;
    unsigned long	pixels[];
    int			npixels;
    XColor		*color;
    int			p;
    int			n;
{
    unsigned long	pixel;
    XColor		request;

    /* Free the read/write allocation of one cell in the colormap.
     * Request a read only allocation of one cell in the colormap.
     * If the read only allocation cannot be granted, give up, because
     * there must be no free cells in the colormap.
     * If the read only allocation is granted, but gives us a cell which
     * is not the one that we just freed, it is probably the case that
     * we are trying allocate White or Black or some other color which
     * already has a read-only allocation in the map.  So we try to 
     * allocate the previously freed cell with a read/write allocation,
     * because we want contiguous cells for image processing algorithms.
     */
     
    pixel = (unsigned long) n;
    request.red = color->red;
    request.green = color->green;
    request.blue = color->blue;

    XFreeColors(dpy, cmap, &pixel, 1, (unsigned long) 0);
    if (! XAllocColor(dpy, cmap, color) 
	|| (color->pixel != pixel &&
	    (!RWcell(dpy, cmap, color, &request, &pixel, n)))) 
    {
	free_cells(dpy, cmap, pixels, npixels, p);
	return 0;
    }
    return 1;
}


/****************************************************************************/
static void free_cells(dpy, cmap, pixels, npixels,  p)
    Display		*dpy;
    Colormap		cmap;
    unsigned long	pixels[];	/* to be freed */
    int			npixels;        /* original number allocated */
    int			p;	  
{
    /* One of the npixels allocated has already been freed.
     * p is the index of the freed pixel.
     * First free the pixels preceeding p, and there are p of them;
     * then free the pixels following p, there are npixels - p - 1 of them.
     */
    XFreeColors(dpy, cmap, pixels, p, (unsigned long) 0);
    XFreeColors(dpy, cmap, &(pixels[p+1]), npixels - p - 1, (unsigned long) 0);
    free((char *) pixels);
}


/****************************************************************************/
static Status RWcell(dpy, cmap, color, request, pixel, n)
    Display		*dpy;
    Colormap		cmap;
    XColor		*color;
    XColor		*request;
    unsigned long	*pixel;
    int			n;
{
    XFreeColors(dpy, cmap, &(color->pixel), 1, (unsigned long)0);
    if (! XAllocColorCells(dpy, cmap, (Bool) 0, (unsigned long *) NULL,
			   (unsigned) 0, pixel, (unsigned) 1))
	return 0;
    if (*pixel != n)
    {
	XFreeColors(dpy, cmap, pixel, 1, (unsigned long) 0);
	return 0;
    }
    color->pixel = *pixel;
    color->flags = DoRed | DoGreen | DoBlue;
    color->red = request->red;
    color->green = request->green;
    color->blue = request->blue;
    XStoreColors(dpy, cmap, color, 1);
    return 1;
}


/****************************************************************************/
static int compare(e1, e2)
    unsigned long	*e1, *e2;
{
    if (*e1 < *e2)	return -1;
    if (*e1 > *e2)	return 1;
    return 0;
}


/****************************************************************************/
static Status readonly_map(dpy, vinfo, colormap)
    Display		*dpy;
    XVisualInfo		*vinfo;
    XStandardColormap	*colormap;
{
    int			i, last_pixel;
    XColor		color;

    last_pixel = (colormap->red_max + 1) * (colormap->green_max + 1) * 
	(colormap->blue_max + 1) + colormap->base_pixel - 1;

    for(i=colormap->base_pixel; i <= last_pixel; i++) {

	color.pixel = (unsigned long) i;
	color.red = (unsigned short)
	    (((i/colormap->red_mult) * 65535) / colormap->red_max);

	if (vinfo->class == StaticColor || vinfo->class == TrueColor) {
	    color.green = (unsigned short)
		((((i/colormap->green_mult) % (colormap->green_max + 1)) *
		  65535) / colormap->green_max);
	    color.blue = (unsigned short)
		(((i%colormap->green_mult) * 65535) / colormap->blue_max);
	}
	else	/* vinfo->class == GrayScale, old style allocation XXX */
	    color.green = color.blue = color.red;

	XAllocColor(dpy, colormap->colormap, &color);
	if (color.pixel != (unsigned long) i)
	    return 0;
    }
    return 1;
}
