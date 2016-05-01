/*
 *  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 *  Copyright (C) 2010-2010 - DIGITEO - Bruno JOFRET
 *
 * Copyright (C) 2012 - 2016 - Scilab Enterprises
 *
 * This file is hereby licensed under the terms of the GNU GPL v2.0,
 * pursuant to article 5.3.4 of the CeCILL v.2.1.
 * This file was originally licensed under the terms of the CeCILL v2.1,
 * and continues to be available under such terms.
 * For more information, see the COPYING file which you should have received
 * along with this program.
 *
 */

extern "C"
{
#include "createGraphicObject.h"
#include "getScilabJavaVM.h"
#include "getGraphicObjectProperty.h"
#include "setGraphicObjectProperty.h"
#include "FigureModel.h"
#include "AxesModel.h"
#include "CurrentSubwin.h"
#include "api_scilab.h"
}

#include "CallGraphicController.hxx"
#include "CallXmlLoader.hxx"
#include "Builder.hxx"
#include "DataController.hxx"

using namespace org_scilab_modules_graphic_objects_builder;
using namespace org_scilab_modules_graphic_objects;

int createGraphicObject(int _iType)
{
    return CallGraphicController::askGraphicObject(getScilabJavaVM(), _iType);
}

int cloneGraphicObject(int _sId)
{
    return CallGraphicController::cloneGraphicObject(getScilabJavaVM(), _sId);
}

int createDataObject(int _sId, int _iType)
{
    return DataController::createDataObject(_sId, _iType);
}

void buildFigureMenuBar(int _sFigureId)
{
    CallGraphicController::buildFigureMenuBar(getScilabJavaVM(), _sFigureId);
}

int createRect(int iParentsubwinUID, double x, double y, double height, double width, int foreground, int background, int isfilled, int isline)
{
    return Builder::createRect(getScilabJavaVM(), iParentsubwinUID, x, y, height, width, foreground, background, isfilled, isline);
}

int cloneGraphicContext(int sourceIdentifier, int destIdentifier)
{
    return Builder::cloneGraphicContext(getScilabJavaVM(), sourceIdentifier, destIdentifier);
}

int cloneFontContext(int sourceIdentifier, int destIdentifier)
{
    return Builder::cloneFontContext(getScilabJavaVM(), sourceIdentifier, destIdentifier);
}

int createHiddenLabel(int parent)
{
    return Builder::createHiddenLabel(getScilabJavaVM(), parent);
}

BOOL isAxesRedrawing(int subWin)
{
    return booltoBOOL(Builder::isAxesRedrawing(getScilabJavaVM(), subWin));
}

int createLabel(int parent, int type)
{
    return Builder::createLabel(getScilabJavaVM(), parent, type);
}

int createNewFigureWithAxes()
{
    int iUserDataSize = 0;
    int* piUserDataSize = &iUserDataSize;
    int id = 0;
    int on = 1;
    int off = 0;

    id = Builder::createNewFigureWithAxes(getScilabJavaVM());

    //clone user_data is needed
    getGraphicObjectProperty(getFigureModel(), __GO_USER_DATA_SIZE__, jni_int, (void**)&piUserDataSize);
    if (iUserDataSize != 0)
    {
        int* pUserData = NULL;

        getGraphicObjectProperty(getFigureModel(), __GO_USER_DATA__, jni_int_vector, (void**)&pUserData);

        if (sizeof(void*) == 4) //32 bits
        {
            increaseValRef(NULL, (int*) * (int*)pUserData);
        }
        else //64 bits
        {
            increaseValRef(NULL, (int*) * (long long*)pUserData);
        }

        setGraphicObjectProperty(id, __GO_USER_DATA__, pUserData, jni_int_vector, iUserDataSize);
    }

    //clone gda user_data is needed
    getGraphicObjectProperty(getAxesModel(), __GO_USER_DATA_SIZE__, jni_int, (void**)&piUserDataSize);
    if (iUserDataSize != 0)
    {
        int* pUserData = NULL;
        getGraphicObjectProperty(getAxesModel(), __GO_USER_DATA__, jni_int_vector, (void**)&pUserData);
        setGraphicObjectProperty(getCurrentSubWin(), __GO_USER_DATA__, pUserData, jni_int_vector, iUserDataSize);
    }

    setGraphicObjectProperty(id, __GO_MENUBAR_VISIBLE__, (void*)&off, jni_bool, 1);
    setGraphicObjectProperty(id, __GO_TOOLBAR_VISIBLE__, (void*)&off, jni_bool, 1);
    setGraphicObjectProperty(id, __GO_INFOBAR_VISIBLE__, (void*)&off, jni_bool, 1);

    setGraphicObjectProperty(id, __GO_MENUBAR_VISIBLE__, (void*)&on, jni_bool, 1);
    setGraphicObjectProperty(id, __GO_TOOLBAR_VISIBLE__, (void*)&on, jni_bool, 1);
    setGraphicObjectProperty(id, __GO_INFOBAR_VISIBLE__, (void*)&on, jni_bool, 1);

    return id;
}

int createFigure(int iDockable, int iMenubarType, int iToolbarType, int iDefaultAxes, int iVisible)
{
    int id = 0;
    int iUserDataSize = 0;
    int* piUserDataSize = &iUserDataSize;

    id = Builder::createFigure(getScilabJavaVM(), iDockable != 0, iMenubarType, iToolbarType, iDefaultAxes != 0, iVisible != 0);

    //clone gdf user_data is needed
    getGraphicObjectProperty(getFigureModel(), __GO_USER_DATA_SIZE__, jni_int, (void**)&piUserDataSize);
    if (iUserDataSize != 0)
    {
        int* pUserData = NULL;
        getGraphicObjectProperty(getFigureModel(), __GO_USER_DATA__, jni_int_vector, (void**)&pUserData);
        setGraphicObjectProperty(id, __GO_USER_DATA__, pUserData, jni_int_vector, iUserDataSize);
    }

    //clone gda user_data is needed
    getGraphicObjectProperty(getAxesModel(), __GO_USER_DATA_SIZE__, jni_int, (void**)&piUserDataSize);
    if (iUserDataSize != 0)
    {
        int* pUserData = NULL;
        getGraphicObjectProperty(getAxesModel(), __GO_USER_DATA__, jni_int_vector, (void**)&pUserData);
        setGraphicObjectProperty(getCurrentSubWin(), __GO_USER_DATA__, pUserData, jni_int_vector, iUserDataSize);
    }

    return id;
}

void cloneMenus(int model, int newParent)
{
    Builder::cloneMenus(getScilabJavaVM(), model, newParent);
}

int cloneAxesModel(int parent)
{
    int id = Builder::cloneAxesModel(getScilabJavaVM(), parent);
    int iUserDataSize = 0;
    int* piUserDataSize = &iUserDataSize;

    //clone user_data is needed
    getGraphicObjectProperty(getAxesModel(), __GO_USER_DATA_SIZE__, jni_int, (void**)&piUserDataSize);
    if (iUserDataSize != 0)
    {
        int* pUserData = NULL;
        getGraphicObjectProperty(getAxesModel(), __GO_USER_DATA__, jni_int_vector, (void**)&pUserData);
        setGraphicObjectProperty(id, __GO_USER_DATA__, pUserData, jni_int_vector, iUserDataSize);
    }

    return id;
}

int createSubWin(int parent)
{
    int id = 0;
    int iUserDataSize = 0;
    int* piUserDataSize = &iUserDataSize;

    id = Builder::createSubWin(getScilabJavaVM(), parent);

    //clone user_data is needed
    getGraphicObjectProperty(getAxesModel(), __GO_USER_DATA_SIZE__, jni_int, (void**)&piUserDataSize);
    if (iUserDataSize != 0)
    {
        int* pUserData = NULL;
        getGraphicObjectProperty(getAxesModel(), __GO_USER_DATA__, jni_int_vector, (void**)&pUserData);
        setGraphicObjectProperty(id, __GO_USER_DATA__, pUserData, jni_int_vector, iUserDataSize);
    }

    return id;
}

int createText(int iParentsubwinUID, char** text, int nbRow, int nbCol, double x, double y, BOOL autoSize, double* userSize, int  centerPos, int *foreground, int *background, BOOL isboxed, BOOL isline, BOOL isfilled, int align)
{
    return Builder::createText(getScilabJavaVM(), iParentsubwinUID, text, nbRow * nbCol, nbRow, nbCol, x, y, autoSize == TRUE,
                               userSize, userSize == NULL ? 0 : 2,
                               centerPos,
                               foreground != NULL ? *foreground : 0, foreground != NULL,
                               background != NULL ? *background : 0, background != NULL,
                               isboxed == TRUE, isline == TRUE, isfilled == TRUE, align);
}

int createArc(int parent, double x, double y, double h, double w, double start, double end, int* foreground, int* background, BOOL filled, BOOL line)
{
    return Builder::createArc(getScilabJavaVM(), parent, x, y, h, w, start, end,
                              foreground != NULL ? *foreground : 0, foreground != NULL,
                              background != NULL ? *background : 0, background != NULL,
                              filled == TRUE, line == TRUE);
}

int createAxis(int parent, int dir, int tics, double* vx, int nx, double* vy, int ny, int subint, char* format, int fontSize, int textColor, int ticsColor, BOOL seg)
{
    return Builder::createAxis(getScilabJavaVM(), parent, dir, tics, vx, nx, vy, ny, subint, format, fontSize, textColor, ticsColor, seg == TRUE);
}

int createCompound(int parent, int* children, int childrenCount)
{
    return Builder::createCompound(getScilabJavaVM(), parent, children, childrenCount);
}

int createCompoundSeq(int parent, int childrenCount)
{
    return Builder::createCompoundSeq(getScilabJavaVM(), parent, childrenCount);
}

int createFec(int parent, double* zminmax, int zminmaxsize, int* colminmax, int colminmaxSize, int* colout, int coloutSize, BOOL with_mesh)
{
    return Builder::createFec(getScilabJavaVM(), parent, zminmax, zminmaxsize, colminmax, colminmaxSize, colout, coloutSize, with_mesh == TRUE);
}

int createGrayplot(int parent, int type, double* pvecx, int pvecxSize, int n1, int n2)
{
    return Builder::createGrayplot(getScilabJavaVM(), parent, type, pvecx, pvecxSize, n1, n2);
}

int createPolyline(int parent, BOOL closed, int plot, int* foreground, int* background, int backgroundSize,
                   int* mark_style, int* mark_foreground, int* mark_background, BOOL isline, BOOL isfilled, BOOL ismark, BOOL isinterp)
{
    bool isForeground       = foreground != NULL;
    bool isMarkStyle        = mark_style != NULL;
    bool isMarkForeground   = mark_foreground != NULL;
    bool isMarkBackground   = mark_background != NULL;

    return Builder::createPolyline(getScilabJavaVM(), parent, closed == TRUE, plot,
                                   isForeground ? *foreground : 0, isForeground,
                                   background, backgroundSize,
                                   isMarkStyle ? *mark_style : 0, isMarkStyle,
                                   isMarkForeground ? *mark_foreground : 0, isMarkForeground,
                                   isMarkBackground ? *mark_background : 0, isMarkBackground,
                                   isline == TRUE, isfilled == TRUE, ismark == TRUE, isinterp == TRUE);
}

int createLegend(int parent, char** text, int iTextSize, int* handles, int iHandlesSize)
{
    return Builder::createLegend(getScilabJavaVM(), parent, text, iTextSize, handles, iHandlesSize);
}

int createSegs(int parent, double* vx, int vxSize, double* vy, int vySize, double* vz, int vzSize, int* style, int styleSize, double arsize)
{
    return Builder::createSegs(getScilabJavaVM(), parent, vx, vxSize, vy, vySize, vz, vzSize, vzSize != 0, style, styleSize, arsize);
}

int createChamp(int parent, double* vx, int vxSize, double* vy, int vySize, double* vfx, int vfxSize, double* vfy, int vfySize, double arsize, BOOL typeofchamp)
{
    return Builder::createChamp(getScilabJavaVM(), parent, vx, vxSize, vy, vySize, vfx, vfxSize, vfy, vfySize, arsize, typeofchamp == TRUE);
}

int createSurface(int parent, int typeof3d, int colorFlag, int colorMode)
{
    return Builder::createSurface(getScilabJavaVM(), parent, typeof3d, colorFlag, colorMode);
}

void initSubWinTo3d(int subwin, char* legend, int* flag, double alpha, double theta,
                    double* ebox, double* x, int xSize, double* y, int ySize, double* z, int zSize)
{
    Builder::initSubWinTo3d(getScilabJavaVM(), subwin, legend, legend != NULL, flag, 3, alpha, theta,
                            ebox, 6, x, xSize, y, ySize, z, zSize);
}

int createLight(int parent, int type, BOOL visible, double* pos, double* dir, double* ambient, double* diffuse, double* specular)
{
    return Builder::createLight(getScilabJavaVM(), parent, type, visible == TRUE,
                                pos, pos == NULL ? 0 : 3,
                                dir, dir == NULL ? 0 : 3,
                                ambient, ambient == NULL ? 0 : 3,
                                diffuse, diffuse == NULL ? 0 : 3,
                                specular, specular == NULL ? 0 : 3);
}

int xmlload(char* xmlfile)
{
    return CallXmlLoader::Load(getScilabJavaVM(), xmlfile);
}

int xmldomload(char* xmlfile)
{
    return CallXmlLoader::DomLoad(getScilabJavaVM(), xmlfile);
}

char* xmlsave(int id, char* xmlfile, BOOL isReverse)
{
    return CallXmlLoader::Save(getScilabJavaVM(), id, xmlfile, isReverse == 1);
}
