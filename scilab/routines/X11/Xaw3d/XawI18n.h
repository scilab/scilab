/* $XConsortium: XawI18n.h,v 1.12 95/01/25 00:48:44 kaleb Exp $ */

/************************************************************

Copyright (c) 1993, 1994  X Consortium

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.  IN NO EVENT SHALL THE
X CONSORTIUM BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN
AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

Except as contained in this notice, the name of the X Consortium shall not be
used in advertising or otherwise to promote the sale, use or other dealings
in this Software without prior written authorization from the X Consortium.

********************************************************/

#ifdef HAS_WCTYPE_H
#include <wctype.h>
#include <widec.h>
#define wcslen(c) wslen(c)
#define wcscpy(d,s) wscpy(d,s)
#define wcsncpy(d,s,l) wsncpy(d,s,l)
#endif

#ifdef HAS_WCHAR_H
#include <wchar.h>
#endif

#ifdef AIXV3
#include <ctype.h>
#endif

#ifdef NCR
#define iswspace(c) _Xaw_iswspace(c)
extern int _Xaw_iswspace(wchar_t);
#endif

#ifdef sony
#ifndef SVR4
#include <jctype.h>
#define iswspace(c) jisspace(c)
#endif
#endif

#ifdef USE_XWCHAR_STRING
#define wcslen(c) _Xwcslen(c)
#define wcscpy(d,s) _Xwcscpy(d,s)
#define wcsncpy(d,s,l) _Xwcsncpy(d,s,l)
#ifdef USE_XMBTOWC
#define mbtowc(wc,s,l) _Xmbtowc(wc,s,l)
#endif
#endif

extern wchar_t _Xaw_atowc (
#if NeedFunctionPrototypes
    unsigned char	c
#endif
);

#ifndef HAS_ISW_FUNCS
#include <ctype.h>
#ifndef iswspace
#define iswspace(c) (isascii(c) && isspace(toascii(c)))
#endif
#endif
