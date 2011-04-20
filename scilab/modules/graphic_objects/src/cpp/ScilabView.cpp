/*
 *  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 *  Copyright (C) 2011-2011 - DIGITEO - Bruno JOFRET
 *
 *  This file must be used under the terms of the CeCILL.
 *  This source file is licensed as described in the file COPYING, which
 *  you should have received as part of this distribution.  The terms
 *  are also available at
 *  http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#include <iostream>
#include <map>
#include <cstring>

#include "ScilabView.hxx"
#include "CallGraphicController.hxx"

extern "C"
{
#include "FigureList.h"
#include "getGraphicObjectProperty.h"
#include "graphicObjectProperties.h"
#include "getScilabJavaVM.h"
}

void ScilabNativeView__createObject(char* pstId)
{
    ScilabView::createObject(pstId);
}

void ScilabNativeView__deleteObject(char* pstId)
{
    ScilabView::deleteObject(pstId);
}

void ScilabNativeView__updateObject(char* pstId, char* pstProperty)
{
    ScilabView::updateObject(pstId, pstProperty);
}

int __sciGetNbFigure(void)
{
    return ScilabView::getNbFigure();
}

void __sciGetFiguresId(int ids[])
{
    return ScilabView::getFiguresId(ids);
}

void __registerToController()
{
    ScilabView::registerToController();
}

void ScilabView::getFiguresId(int ids[])
{
    std::map<char*, int, ScilabView::cmp_str>::iterator it;
    int i = 0;

    for(it = figureList.begin() ; it != figureList.end() ; ++it, ++i)
    {
        //std::cerr << "[ScilabView] DEBUG " << it->first << " <-> " << it->second << std::endl;
        ids[i] = it->second;
    }
}

int ScilabView::getNbFigure(void)
{
    return figureList.size();
}

void ScilabView::createObject(char* pstId)
{
    //std::cerr << "[ScilabView] ++ createObject UID=" << pstId << std::endl;
    char *pstType;

    getGraphicObjectProperty(pstId, __GO_TYPE__, jni_string, (void**) &pstType);
    if(pstType != NULL && strcmp(pstType, __GO_FIGURE__) == 0)
    {
        ScilabView::figureList[strdup(pstId)] = -1;
    }
}

void ScilabView::deleteObject(char* pstId)
{
    //std::cerr << "[ScilabView] -- deleteObject UID=" << pstId << std::endl;
    char *pstType;

    getGraphicObjectProperty(pstId, __GO_TYPE__, jni_string, (void**) &pstType);
    if(pstType != NULL && strcmp(pstType, __GO_FIGURE__) == 0)
    {
        figureList.erase(pstId);
    }
}

void ScilabView::updateObject(char* pstId, char* pstProperty)
{
    //std::cerr << "[ScilabView] == updateObject UID=" << pstId << " PROPERTY=" << pstProperty << std::endl;

    /*
    ** Take care of update if the value update is ID and object type is a Figure I manage.
    */
    if (strcmp(pstProperty, __GO_ID__) == 0
        && figureList.find(pstId) != figureList.end())
    {
        int iNewId = 0;
        int *piNewId = &iNewId;

        getGraphicObjectProperty(pstId, __GO_ID__, jni_int, (void**) &piNewId);

        figureList[strdup(pstId)] = iNewId;
        //std::cerr << "### [ScilabView] updateMap UID=" << pstId << " id=" << iNewId << std::endl;
    }
}

/*
** Register ScilabView to Controller.
** Must be done after Graphics models are created.
*/
void ScilabView::registerToController(void)
{
    org_scilab_modules_graphic_objects::CallGraphicController::registerScilabView(getScilabJavaVM());
}

/*
** Allocate static class variable.
*/
std::map<char*, int, ScilabView::cmp_str> ScilabView::figureList;
