#ifndef GD_H
#define GD_H 1

/* gd.h: declarations file for the gifdraw module.

	Written by Tom Boutell, 5/94.
	Copyright 1994, Cold Spring Harbor Labs.
	Permission granted to use this code in any fashion provided
	that this notice is retained and any alterations are
	labeled as such. It is requested, but not required, that
	you share extensions to this module with us so that we
	can incorporate them into new versions. */

/* stdio is needed for file I/O. */
#include <stdio.h>
#include "../machine.h"

/* This can't be changed, it's part of the GIF specification. */

#define gdMaxColors 256

/* Image type. See functions below; you will not need to change
	the elements directly. Use the provided macros to
	access sx, sy, the color table, and colorsTotal for 
	read-only purposes. */

typedef struct  gdImageStruct { 
	unsigned char ** pixels;
	int sx;
	int sy;
	int colorsTotal;
	int red[gdMaxColors];
	int green[gdMaxColors];
	int blue[gdMaxColors]; 
	int open[gdMaxColors];
	int transparent;
	int *polyInts;
	int polyAllocated;
 	struct gdImageStruct *brush;
	struct gdImageStruct *tile;	 
	int brushColorMap[gdMaxColors];
	int tileColorMap[gdMaxColors];
	int styleLength;
	int stylePos;
	int *style;
	int interlace;
        int alu;
        int clipping;
        int cliprect[4];
        int background;
} gdImage;

typedef gdImage * gdImagePtr;

typedef struct {
	/* # of characters in font */
	int nchars;
	/* First character is numbered... (usually 32 = space) */
	int offset;
	/* Character width and height */
	int w;
	int h;
        int fixed;
	/* Font data; array of characters, one row after another.
		Easily included in code, also easily loaded from
		data files. */
	char *data;
} gdFont;

/* Text functions take these. */
typedef gdFont *gdFontPtr;

/* For backwards compatibility only. Use gdImageSetStyle()
	for MUCH more flexible line drawing. Also see
	gdImageSetBrush(). */
#define gdDashSize 4

/* Special colors. */

#define gdStyled (-2)
#define gdBrushed (-3)
#define gdStyledBrushed (-4)
#define gdTiled (-5)

/* NOT the same as the transparent color index.
	This is used in line styles only. */
#define gdTransparent (-6)

/* functions to handle alu modes and clipping */
void gdSetClipping __PARAMS((gdImagePtr im , int xmin, int ymin, int xmax, int ymax));
void gdUnsetClipping  __PARAMS((gdImagePtr im)); 
void gdSetAlu __PARAMS((gdImagePtr im, int alu));

/* functions to handle dashes */
void gdResetDash();
/* Functions to manipulate images. */

gdImagePtr gdImageCreate __PARAMS((int sx, int sy));
gdImagePtr gdImageCreateFromGif __PARAMS((FILE *fd));
gdImagePtr gdImageCreateFromGd __PARAMS((FILE *in));
gdImagePtr gdImageCreateFromXbm __PARAMS((FILE *fd));
void gdImageDestroy __PARAMS((gdImagePtr im));
void gdImageSetPixel __PARAMS((gdImagePtr im, int x, int y, int color));
int gdImageGetPixel __PARAMS((gdImagePtr im, int x, int y));
void gdImageLine __PARAMS((gdImagePtr im, int x1, int y1, int x2, int y2, int color));
/* For backwards compatibility only. Use gdImageSetStyle()
	for much more flexible line drawing. */

/* Corners specified (not width and height). Upper left first, lower right
 	second. */
void gdImageRectangle __PARAMS((gdImagePtr im, int x1, int y1, int x2, int y2, int color));
void gdImageThickRectangle __PARAMS((gdImagePtr im, int x1, int y1, int x2, int y2, int color, int thick));

/* Solid bar. Upper left corner first, lower right corner second. */
void gdImageFilledRectangle __PARAMS((gdImagePtr im, int x1, int y1, int x2, int y2, int color));
int gdImageBoundsSafe __PARAMS((gdImagePtr im, int x, int y));
int gdImageChar __PARAMS((gdImagePtr im, gdFontPtr f, int x, int y, int c, int color));
void gdImageCharUp __PARAMS((gdImagePtr im, gdFontPtr f, int x, int y, int c, int color));
void gdImageString __PARAMS((gdImagePtr im, gdFontPtr f, int x, int y, unsigned char *s, int color));
void gdImageStringUp __PARAMS((gdImagePtr im, gdFontPtr f, int x, int y, unsigned char *s, int color));
void gdImageString16 __PARAMS((gdImagePtr im, gdFontPtr f, int x, int y, unsigned short *s, int color));
void gdImageStringUp16 __PARAMS((gdImagePtr im, gdFontPtr f, int x, int y, unsigned short *s, int color));

/* Point type for use in polygon drawing. */

typedef struct {
	int x, y;
} gdPoint, *gdPointPtr;

void gdImagePolygon __PARAMS((gdImagePtr im, gdPointPtr p, int n, int c));
void gdImageFilledPolygon __PARAMS((gdImagePtr im, gdPointPtr p, int n, int c));

int gdImageColorAllocate __PARAMS((gdImagePtr im, int r, int g, int b));
int gdImageColorClosest __PARAMS((gdImagePtr im, int r, int g, int b));
int gdImageColorExact __PARAMS((gdImagePtr im, int r, int g, int b));
void gdImageColorDeallocate __PARAMS((gdImagePtr im, int color));
void gdImageColorTransparent __PARAMS((gdImagePtr im, int color));
void gdImageGif __PARAMS((gdImagePtr im, FILE *out));
void gdImagePPM __PARAMS((gdImagePtr im, FILE *out));
void gdImageGd __PARAMS((gdImagePtr im, FILE *out));
void gdImageArc __PARAMS((gdImagePtr im, int cx, int cy, int w, int h, int s, int e, int color));
void gdImageFillToBorder __PARAMS((gdImagePtr im, int x, int y, int border, int color));
void gdImageFill __PARAMS((gdImagePtr im, int x, int y, int color));
void gdImageCopy __PARAMS((gdImagePtr dst, gdImagePtr src, int dstX, int dstY, int srcX, int srcY, int w, int h));
/* Stretches or shrinks to fit, as needed */
void gdImageCopyResized __PARAMS((gdImagePtr dst, gdImagePtr src, int dstX, int dstY, int srcX, int srcY, int dstW, int dstH, int srcW, int srcH));
void gdImageSetBrush __PARAMS((gdImagePtr im, gdImagePtr brush));
void gdImageSetTile __PARAMS((gdImagePtr im, gdImagePtr tile));
void gdImageSetStyle __PARAMS((gdImagePtr im, int *style, int noOfPixels));
/* On or off (1 or 0) */
void gdImageInterlace __PARAMS((gdImagePtr im, int interlaceArg));
void gdImageChangeColor __PARAMS((gdImagePtr im, int old, int new));
void gdSetBackground __PARAMS((gdImagePtr im, int background));
int gdCharWidth __PARAMS((gdFontPtr f, int c));
void gdImageThickLine __PARAMS((gdImagePtr im, int x1, int y1, int x2, int y2, int color, int thick));
void gdImagePolyLine __PARAMS((gdImagePtr im, int *X, int *Y, int n, int color, int thick, int close));

/* Macros to access information about images. READ ONLY. Changing
	these values will NOT have the desired result. */
#define gdImageSX(im) ((im)->sx)
#define gdImageSY(im) ((im)->sy)
#define gdImageColorsTotal(im) ((im)->colorsTotal)
#define gdImageRed(im, c) ((im)->red[(c)])
#define gdImageGreen(im, c) ((im)->green[(c)])
#define gdImageBlue(im, c) ((im)->blue[(c)])
#define gdImageGetTransparent(im) ((im)->transparent)
#define gdImageGetInterlaced(im) ((im)->interlace)
#endif
