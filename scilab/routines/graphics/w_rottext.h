/* ************************************************************************ */

/* Header file for the `xvertext 5.0' routines.
 *
 *  Copyright (c) 1993 Alan Richardson (mppa3@uk.ac.sussex.syma)
 *
 * Any party obtaining a copy of these files is granted, free of charge, a
 * full and unrestricted irrevocable, world-wide, paid up, royalty-free,
 * nonexclusive right and license to deal in this software and
 * documentation files (the "Software"), including without limitation the
 * rights to use, copy, modify, merge, publish and/or distribute copies of
 * the Software, and to permit persons who receive copies from any such 
 * party to do so, with the only requirement being that this copyright 
 * notice remain intact.
 *
 */

/* ************************************************************************ */

#ifndef W_ROTTEXT_H
#define W_ROTTEXT_H

#ifndef _XVERTEXT_INCLUDED_ 
#define _XVERTEXT_INCLUDED_


#define XV_VERSION      5.0
#define XV_COPYRIGHT \
      "xvertext routines Copyright (c) 1993 Alan Richardson"


/* ---------------------------------------------------------------------- */


/* text alignment */

#define NONE             0
#define TLEFT            1
#define TCENTRE          2
#define TRIGHT           3
#define MLEFT            4
#define MCENTRE          5
#define MRIGHT           6
#define BLEFT            7
#define BCENTRE          8
#define BRIGHT           9


/* ---------------------------------------------------------------------- */

/* this shoulf be C++ compliant, thanks to 
     vlp@latina.inesc.pt (Vasco Lopes Paulo) */

#if defined(__cplusplus) || defined(c_plusplus)

extern "C" {
float   XRotVersion(char*, int);
void    XRotSetMagnification(float);
void    XRotSetBoundingBoxPad(int);
int     XRotDrawString(Display*, XFontStruct*, float,
                       Drawable, GC, int, int, char*);
int     XRotDrawImageString(Display*, XFontStruct*, float,
                            Drawable, GC, int, int, char*);
int     XRotDrawAlignedString(Display*, XFontStruct*, float,
                              Drawable, GC, int, int, char*, int);
int     XRotDrawAlignedImageString(Display*, XFontStruct*, float,
                                   Drawable, GC, int, int, char*, int);
XPoint *XRotTextExtents(XFontStruct*, float,
			int, int, char*, int);
}

#else

extern float   XRotVersion();
extern void    XRotSetMagnification();
extern void    XRotSetBoundingBoxPad();
extern int     XRotDrawString();
extern int     XRotDrawImageString();
extern int     XRotDrawAlignedString();
extern int     XRotDrawAlignedImageString();
extern XPoint *XRotTextExtents();

#endif /* __cplusplus */

/* ---------------------------------------------------------------------- */


#endif /* _XVERTEXT_INCLUDED_ */


#endif /* W_ROTTEXT_H */
