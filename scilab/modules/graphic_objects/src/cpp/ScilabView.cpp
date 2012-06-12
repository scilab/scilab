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
#include "createGraphicObject.h"
#include "setGraphicObjectProperty.h"
#include "getGraphicObjectProperty.h"
#include "graphicObjectProperties.h"
#include "getScilabJavaVM.h"
}

/**
 * C Wrapping functions
 * \{
 */
void ScilabNativeView__createObject(char *pstId)
{
    ScilabView::createObject(pstId);
}

void ScilabNativeView__deleteObject(char *pstId)
{
    ScilabView::deleteObject(pstId);
}

void ScilabNativeView__updateObject(char *pstId, char *pstProperty)
{
    ScilabView::updateObject(pstId, pstProperty);
}

/**
 * \}
 */

bool ScilabView::isEmptyFigureList()
{
    return m_figureList.empty();
}

char *ScilabView::getFigureFromIndex(int figNum)
{
    __figureList_iterator it;

    for (it = m_figureList.begin(); it != m_figureList.end(); ++it)
    {
        if (it->second == figNum)
        {
            return it->first;
        }
    }
    return NULL;
}

bool ScilabView::existsFigureId(int id)
{
    __figureList_iterator it;

    for (it = m_figureList.begin(); it != m_figureList.end(); ++it)
    {
        if (it->second == id)
        {
            return true;
        }
    }
    return false;
}

void ScilabView::getFiguresId(int ids[])
{
    __figureList_iterator it;
    int i = (int)(m_figureList.size() - 1);

    for (it = m_figureList.begin(); it != m_figureList.end(); ++it, --i)
    {
        //std::cerr << "[ScilabView] DEBUG " << it->first << " <-> " << it->second << std::endl;
        ids[i] = it->second;
    }
}

int ScilabView::getNbFigure(void)
{
    return (int)m_figureList.size();

}

void ScilabView::createObject(char *pstId)
{
    //std::cerr << "[ScilabView] ++ createObject UID=" << pstId << std::endl;
    char *pstType = NULL;

    getGraphicObjectProperty(pstId, __GO_TYPE__, jni_string, (void **)&pstType);
    if (pstType != NULL && strcmp(pstType, __GO_FIGURE__) == 0)
    {
        m_figureList[strdup(pstId)] = -1;
        setCurrentFigure(pstId);
    }

    // Register object handle.
    getObjectHandle(pstId);
}

void ScilabView::deleteObject(char *pstId)
{
    //std::cerr << "[ScilabView] -- deleteObject UID=" << pstId << std::endl;
    char *pstType = NULL;
    char *pstParentUID = NULL;
    char *pstCurrentFigure = getCurrentFigure();
    char *pstCurrentObject = getCurrentObject();

    getGraphicObjectProperty(pstId, __GO_TYPE__, jni_string, (void **)&pstType);

    /*
    ** If deleting a figure, remove from figure list.
    */
    if (pstType != NULL && strcmp(pstType, __GO_FIGURE__) == 0)
    {
        m_figureList.erase(pstId);
    }

    /*
    ** If deleting current figure find another current one,
    ** if there is no more figure : NULL
    */
    if (pstCurrentFigure != NULL
            && strcmp(pstId, getCurrentFigure()) == 0) // Deleting current figure
    {
        if (getNbFigure() != 0)
        {
            setCurrentFigure(m_figureList.rbegin()->first);
        }
        else
        {
            setCurrentFigure(NULL);
            setCurrentSubWin(NULL);
        }
    }

    /*
    ** If deleting current entity, set parent as new current.
    */
    if (pstCurrentObject != NULL
            && strcmp(pstId, getCurrentObject()) == 0) // Deleting current object
    {
        getGraphicObjectProperty(pstId, __GO_PARENT__, jni_string, (void **)&pstParentUID);
        setCurrentObject(pstParentUID);
    }

    // Remove the corresponding handle.
    m_handleList.erase(pstId);
}

void ScilabView::updateObject(char *pstId, char *pstProperty)
{
    //std::cerr << "[ScilabView] == updateObject UID=" << pstId << " PROPERTY=" << pstProperty << std::endl;

    /*
     ** Take care of update if the value update is ID and object type is a Figure I manage.
     */
    if (strcmp(pstProperty, __GO_ID__) == 0 && m_figureList.find(pstId) != m_figureList.end())
    {
        int iNewId = 0;
        int *piNewId = &iNewId;

        getGraphicObjectProperty(pstId, __GO_ID__, jni_int, (void **)&piNewId);

        m_figureList[strdup(pstId)] = iNewId;
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
** Reove ScilabView from Controller.
*/
void ScilabView::unregisterToController(void)
{
    org_scilab_modules_graphic_objects::CallGraphicController::unregisterScilabView(getScilabJavaVM());
}

/*
** Set Current Figure UID
*/
void ScilabView::setCurrentFigure(char *UID)
{
    if (UID != NULL)
    {
        if (m_currentFigure != NULL)
        {
            free(m_currentFigure);
        }
        m_currentFigure = strdup(UID);
    }
    else
    {
        m_currentFigure = NULL;
    }
}

/*
** Get Current Figure UID
*/
char *ScilabView::getCurrentFigure()
{
    //std::cerr << "[ScilaView] currentFigure = " << (m_currentFigure == NULL ? "NULL !!" : m_currentFigure) << std::endl;
    return m_currentFigure;
}

/*
** Set Current Object UID
*/
void ScilabView::setCurrentObject(char *UID)
{
    m_currentObject = strdup(UID);
}

/*
** Get Current Figure UID
*/
char *ScilabView::getCurrentObject()
{
    //std::cerr << "[ScilaView] currentObject = " << m_currentObject << std::endl;
    return m_currentObject;
}

/*
** Set Current SubWin UID
*/
void ScilabView::setCurrentSubWin(char *UID)
{
    m_currentSubWin = UID;
}

/*
** Get Current Figure UID
*/
char *ScilabView::getCurrentSubWin()
{
    //std::cerr << "[ScilaView] currentSubWin = " << m_currentSubWin << std::endl;
    return m_currentSubWin;
}

/*
** Scilab only can store long as handle
*/
long ScilabView::getObjectHandle(char *UID)
{
    /*
     * if (UID != NULL)
     * {
     * std::cerr << "UID = " << UID << std::endl;
     * }
     * else
     * {
     * std::cerr << "UID is null :-S" << std::endl;
     * }
     * __handleList_iterator it2;
     * std::cerr << "[DEBUG] +++ handleMap +++" << std::endl;
     * for (it2 = m_handleList.begin() ; it2 != m_handleList.end() ; ++it2)
     * {
     * std::cerr << "UID " << it2->first << " <-> handle " << it2->second << std::endl;
     * }
     * std::cerr << "[DEBUG] +++ handleMap +++" << std::endl;
     */
    __handleList_iterator it = m_handleList.find(UID);

    if (it != m_handleList.end())
    {
        return it->second;
    }

    // increase maximum value
    // register new handle and return it.
    m_topHandleValue++;
    m_handleList[strdup(UID)] = m_topHandleValue;

    return m_topHandleValue;
}

char *ScilabView::getObjectFromHandle(long handle)
{
    __handleList_iterator it;

    for (it = m_handleList.begin(); it != m_handleList.end(); ++it)
    {
        if (it->second == handle)
        {
            return it->first;
        }
    }
    return NULL;
}

char *ScilabView::getFigureModel(void)
{
    //std::cerr << "[ScilabView] getFigureModel = " << (m_figureModel == NULL ? "!! NULL !!" : m_figureModel) << std::endl;
    return m_figureModel;
}

void ScilabView::setFigureModel(char *UID)
{
    m_figureModel = UID;
}

char *ScilabView::getAxesModel(void)
{
    //std::cerr << "[ScilabView] getAxesModel = " << (m_axesModel == NULL ? "!! NULL !!" : m_axesModel) << std::endl;
    return m_axesModel;
}

void ScilabView::setAxesModel(char *UID)
{
    m_axesModel = UID;
}

/*
** Allocate static class variable.
*/
ScilabView::__figureList ScilabView::m_figureList = *new __figureList();
ScilabView::__handleList ScilabView::m_handleList = *new __handleList();
long ScilabView::m_topHandleValue = 0;
char *ScilabView::m_currentFigure = NULL;
char *ScilabView::m_currentObject = NULL;
char *ScilabView::m_currentSubWin = NULL;
char *ScilabView::m_figureModel = NULL;
char *ScilabView::m_axesModel = NULL;
