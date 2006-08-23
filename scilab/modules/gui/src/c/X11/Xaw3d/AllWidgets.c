/* $XConsortium: AllWidgets.c,v 1.8 94/04/17 20:11:39 kaleb Exp $ */

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

#include <X11/IntrinsicP.h>
#include <X11/Xmu/WidgetNode.h>

extern WidgetClass applicationShellWidgetClass;
extern WidgetClass asciiSinkObjectClass;
extern WidgetClass asciiSrcObjectClass;
extern WidgetClass asciiTextWidgetClass;
extern WidgetClass boxWidgetClass;
extern WidgetClass commandWidgetClass;
extern WidgetClass compositeWidgetClass;
extern WidgetClass constraintWidgetClass;
extern WidgetClass coreWidgetClass;
extern WidgetClass dialogWidgetClass;
extern WidgetClass formWidgetClass;
extern WidgetClass gripWidgetClass;
extern WidgetClass labelWidgetClass;
extern WidgetClass layoutWidgetClass;
extern WidgetClass listWidgetClass;
extern WidgetClass menuButtonWidgetClass;
extern WidgetClass objectClass;
extern WidgetClass overrideShellWidgetClass;
extern WidgetClass panedWidgetClass;
extern WidgetClass pannerWidgetClass;
extern WidgetClass portholeWidgetClass;
extern WidgetClass rectObjClass;
extern WidgetClass repeaterWidgetClass;
extern WidgetClass scrollbarWidgetClass;
extern WidgetClass shellWidgetClass;
extern WidgetClass simpleMenuWidgetClass;
extern WidgetClass simpleWidgetClass;
extern WidgetClass smeBSBObjectClass;
extern WidgetClass smeLineObjectClass;
extern WidgetClass smeObjectClass;
extern WidgetClass smeThreeDObjectClass;
extern WidgetClass stripChartWidgetClass;
extern WidgetClass textSinkObjectClass;
extern WidgetClass textSrcObjectClass;
extern WidgetClass textWidgetClass;
extern WidgetClass threeDWidgetClass;
extern WidgetClass toggleWidgetClass;
extern WidgetClass topLevelShellWidgetClass;
extern WidgetClass transientShellWidgetClass;
extern WidgetClass treeWidgetClass;
extern WidgetClass vendorShellWidgetClass;
extern WidgetClass viewportWidgetClass;
extern WidgetClass wmShellWidgetClass;

XmuWidgetNode XawWidgetArray[] = {
{ "applicationShell", &applicationShellWidgetClass },
{ "asciiSink", &asciiSinkObjectClass },
{ "asciiSrc", &asciiSrcObjectClass },
{ "asciiText", &asciiTextWidgetClass },
{ "box", &boxWidgetClass },
{ "command", &commandWidgetClass },
{ "composite", &compositeWidgetClass },
{ "constraint", &constraintWidgetClass },
{ "core", &coreWidgetClass },
{ "dialog", &dialogWidgetClass },
{ "form", &formWidgetClass },
{ "grip", &gripWidgetClass },
{ "label", &labelWidgetClass },
{ "layout", &layoutWidgetClass },
{ "list", &listWidgetClass },
{ "menuButton", &menuButtonWidgetClass },
{ "object", &objectClass },
{ "overrideShell", &overrideShellWidgetClass },
{ "paned", &panedWidgetClass },
{ "panner", &pannerWidgetClass },
{ "porthole", &portholeWidgetClass },
{ "rect", &rectObjClass },
{ "repeater", &repeaterWidgetClass },
{ "scrollbar", &scrollbarWidgetClass },
{ "shell", &shellWidgetClass },
{ "simpleMenu", &simpleMenuWidgetClass },
{ "simple", &simpleWidgetClass },
{ "smeBSB", &smeBSBObjectClass },
{ "smeLine", &smeLineObjectClass },
{ "smeThreeD", &smeThreeDObjectClass },
{ "sme", &smeObjectClass },
{ "stripChart", &stripChartWidgetClass },
{ "textSink", &textSinkObjectClass },
{ "textSrc", &textSrcObjectClass },
{ "text", &textWidgetClass },
{ "threeD", &threeDWidgetClass },
{ "toggle", &toggleWidgetClass },
{ "topLevelShell", &topLevelShellWidgetClass },
{ "transientShell", &transientShellWidgetClass },
{ "tree", &treeWidgetClass },
{ "vendorShell", &vendorShellWidgetClass },
{ "viewport", &viewportWidgetClass },
{ "wmShell", &wmShellWidgetClass },
};

int XawWidgetCount = XtNumber(XawWidgetArray);

