/* $XConsortium: sharedlib.c,v 1.6 94/04/17 20:13:34 kaleb Exp $ */
/*

Copyright (c) 1991, 1994  X Consortium

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

*/

#if defined(SUNSHLIB) && !defined(SHAREDCODE)

#include <X11/IntrinsicP.h>
#include <X11/Xaw3d/AsciiSinkP.h>
#include <X11/Xaw3d/AsciiSrcP.h>
#include <X11/Xaw3d/AsciiTextP.h>
#include <X11/Xaw3d/MultiSinkP.h>
#include <X11/Xaw3d/MultiSrcP.h>
#include <X11/Xaw3d/BoxP.h>
#include <X11/Xaw3d/CommandP.h>
#include <X11/Xaw3d/DialogP.h>
#include <X11/Xaw3d/FormP.h>
#include <X11/Xaw3d/GripP.h>
#include <X11/Xaw3d/LabelP.h>
#include <X11/Xaw3d/ListP.h>
#include <X11/Xaw3d/MenuButtoP.h>
#include <X11/Xaw3d/PanedP.h>
#include <X11/Xaw3d/PannerP.h>
#include <X11/Xaw3d/PortholeP.h>
#include <X11/Xaw3d/RepeaterP.h>
#include <X11/Xaw3d/ScrollbarP.h>
#include <X11/Xaw3d/SimpleP.h>
#include <X11/Xaw3d/SimpleMenP.h>
#include <X11/Xaw3d/SmeP.h>
#include <X11/Xaw3d/SmeThreeDP.h>
#include <X11/Xaw3d/SmeBSBP.h>
#include <X11/Xaw3d/SmeLineP.h>
#include <X11/Xaw3d/StripCharP.h>
#include <X11/Xaw3d/TextP.h>
#include <X11/Xaw3d/TextSinkP.h>
#include <X11/Xaw3d/TextSrcP.h>
#include <X11/Xaw3d/ThreeDP.h>
#include <X11/Xaw3d/ToggleP.h>
#include <X11/Xaw3d/TreeP.h>
#include <X11/VendorP.h>
#include <X11/Xaw3d/ViewportP.h>

extern AsciiSinkClassRec asciiSinkClassRec;
WidgetClass asciiSinkObjectClass = (WidgetClass)&asciiSinkClassRec;

extern AsciiSrcClassRec asciiSrcClassRec;
WidgetClass asciiSrcObjectClass = (WidgetClass)&asciiSrcClassRec;

extern AsciiTextClassRec asciiTextClassRec;
WidgetClass asciiTextWidgetClass = (WidgetClass)&asciiTextClassRec;

#ifdef ASCII_STRING
extern AsciiStringClassRec asciiStringClassRec;
WidgetClass asciiStringWidgetClass = (WidgetClass)&asciiStringClassRec;
#endif

#ifdef ASCII_DISK
extern AsciiDiskClassRec asciiDiskClassRec;
WidgetClass asciiDiskWidgetClass = (WidgetClass)&asciiDiskClassRec;
#endif

extern MultiSinkClassRec multiSinkClassRec;
WidgetClass multiSinkObjectClass = (WidgetClass)&multiSinkClassRec;

extern MultiSrcClassRec multiSrcClassRec;
WidgetClass multiSrcObjectClass = (WidgetClass)&multiSrcClassRec;

extern BoxClassRec boxClassRec;
WidgetClass boxWidgetClass = (WidgetClass)&boxClassRec;

extern CommandClassRec commandClassRec;
WidgetClass commandWidgetClass = (WidgetClass) &commandClassRec;

extern DialogClassRec dialogClassRec;
WidgetClass dialogWidgetClass = (WidgetClass)&dialogClassRec;

extern FormClassRec formClassRec;
WidgetClass formWidgetClass = (WidgetClass)&formClassRec;

extern GripClassRec gripClassRec;
WidgetClass gripWidgetClass = (WidgetClass) &gripClassRec;

extern LabelClassRec labelClassRec;
WidgetClass labelWidgetClass = (WidgetClass)&labelClassRec;

extern ListClassRec listClassRec;
WidgetClass listWidgetClass = (WidgetClass)&listClassRec;

extern MenuButtonClassRec menuButtonClassRec;
WidgetClass menuButtonWidgetClass = (WidgetClass) &menuButtonClassRec;

extern PanedClassRec panedClassRec;
WidgetClass panedWidgetClass = (WidgetClass) &panedClassRec;
WidgetClass vPanedWidgetClass = (WidgetClass) &panedClassRec;

extern PannerClassRec pannerClassRec;
WidgetClass pannerWidgetClass = (WidgetClass) &pannerClassRec;

extern PortholeClassRec portholeClassRec;
WidgetClass portholeWidgetClass = (WidgetClass) &portholeClassRec;

extern RepeaterClassRec repeaterClassRec;
WidgetClass repeaterWidgetClass = (WidgetClass) &repeaterClassRec;

extern ScrollbarClassRec scrollbarClassRec;
WidgetClass scrollbarWidgetClass = (WidgetClass)&scrollbarClassRec;

extern SimpleClassRec simpleClassRec;
WidgetClass simpleWidgetClass = (WidgetClass)&simpleClassRec;

extern SimpleMenuClassRec simpleMenuClassRec;
WidgetClass simpleMenuWidgetClass = (WidgetClass)&simpleMenuClassRec;

extern SmeClassRec smeClassRec;
WidgetClass smeObjectClass = (WidgetClass) &smeClassRec;

extern smeThreeDClassRec smeThreeDClassRec;
WidgetClass smeThreeDObjectClass = (WidgetClass) &smeThreeDClassRec;

WidgetClass smeBSBObjectClass = (WidgetClass) &smeBSBClassRec;

extern SmeLineClassRec smeLineClassRec;
WidgetClass smeLineObjectClass = (WidgetClass) &smeLineClassRec;

extern StripChartClassRec stripChartClassRec;
WidgetClass stripChartWidgetClass = (WidgetClass) &stripChartClassRec;

extern TextClassRec textClassRec;
WidgetClass textWidgetClass = (WidgetClass)&textClassRec;

unsigned long FMT8BIT = 0L;
unsigned long XawFmt8Bit = 0L;
unsigned long XawFmtWide = 0L;

extern TextSinkClassRec textSinkClassRec;
WidgetClass textSinkObjectClass = (WidgetClass)&textSinkClassRec;

extern TextSrcClassRec textSrcClassRec;
WidgetClass textSrcObjectClass = (WidgetClass)&textSrcClassRec;

extern ThreeDClassRec threeDClassRec;
WidgetClass threeDClass = (WidgetClass)&threeDClassRec;

extern ToggleClassRec toggleClassRec;
WidgetClass toggleWidgetClass = (WidgetClass) &toggleClassRec;

extern TreeClassRec treeClassRec;
WidgetClass treeWidgetClass = (WidgetClass) &treeClassRec;

extern VendorShellClassRec vendorShellClassRec;
WidgetClass vendorShellWidgetClass = (WidgetClass) &vendorShellClassRec;

extern ViewportClassRec viewportClassRec;
WidgetClass viewportWidgetClass = (WidgetClass)&viewportClassRec;

#endif /* SUNSHLIB */
