/*
 *  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 *  Copyright (C) 2010-2010 - DIGITEO - Bruno JOFRET
 *
 *  This file must be used under the terms of the CeCILL.
 *  This source file is licensed as described in the file COPYING, which
 *  you should have received as part of this distribution.  The terms
 *  are also available at
 *  http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
 *
 */

#ifndef __CREATEGRAPHICOBJECT_H__
#define __CREATEGRAPHICOBJECT_H__

#include "dynlib_graphic_objects.h"
#include "BOOL.h"

/*
** Ask Controller to create a graphic object
** of the given _sType.
*/
GRAPHIC_OBJECTS_IMPEXP int createGraphicObject(int _iType);

/*
** Ask Controller to clone a graphic object
** referenced the given _sId.
*/
GRAPHIC_OBJECTS_IMPEXP int cloneGraphicObject(int iId);

/*
** Ask Controller to create a data object
** referenced by the given _sId and of the given _sType
*/
GRAPHIC_OBJECTS_IMPEXP int createDataObject(int iId, int _iType);

/*
** Ask Controller to create a figure menubar
** referenced by the given _sFigureId and of the given _sType
*/
GRAPHIC_OBJECTS_IMPEXP void buildFigureMenuBar(int iFigureId);

GRAPHIC_OBJECTS_IMPEXP int buildRect(int iParentsubwinUID, double x, double y, double height, double width, int foreground, int background, int isfilled, int isline);
GRAPHIC_OBJECTS_IMPEXP int cloneGraphicContext(int iSourceIdentifier, int iDestIdentifier);
GRAPHIC_OBJECTS_IMPEXP int cloneFontContext(int iSourceIdentifier, int iDestIdentifier);

GRAPHIC_OBJECTS_IMPEXP int createHiddenLabel(int iParent);
GRAPHIC_OBJECTS_IMPEXP BOOL isAxesRedrawing(int iSubWin);
GRAPHIC_OBJECTS_IMPEXP int createLabel(int iParent, int type);
GRAPHIC_OBJECTS_IMPEXP int createNewFigureWithAxes();
GRAPHIC_OBJECTS_IMPEXP void cloneMenus(int iModel, int iNewParent);
GRAPHIC_OBJECTS_IMPEXP int cloneAxesModel(int iParent);
GRAPHIC_OBJECTS_IMPEXP int createSubWin(int iParent);
GRAPHIC_OBJECTS_IMPEXP int allocateText(int iParentsubwinUID, char** text, int nbRow, int nbCol, double x, double y, BOOL autoSize, double* userSize, int  centerPos, int *foreground, int *background, BOOL isboxed, BOOL isline, BOOL isfilled, int align);
GRAPHIC_OBJECTS_IMPEXP int createArc(int parent, double x, double y, double h, double w, double start, double end, int* foreground, int* background, BOOL filled, BOOL line);
GRAPHIC_OBJECTS_IMPEXP int createAxis(int parent, int dir, int tics, double* vx, int nx, double* vy, int ny, int subint, char* format, int fontSize, int textColor, int ticsColor, BOOL seg);
GRAPHIC_OBJECTS_IMPEXP int createCompound(int parent, int* children, int childrenCount);
GRAPHIC_OBJECTS_IMPEXP int createCompoundSeq(int parent, int childrenCount);
GRAPHIC_OBJECTS_IMPEXP int createFec(int parent, double* zminmax, int zminmaxsize, int* colminmax, int colminmaxSize, int* colout, int coloutSize, BOOL with_mesh);
GRAPHIC_OBJECTS_IMPEXP int createGrayplot(int parent, int type, double* pvecx, int pvecxSize, int n1, int n2);

#endif /* !__CREATEGRAPHICOBJECT_H__ */
