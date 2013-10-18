/*
 *  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 *  Copyright (C) 2011-2011 - DIGITEO - Bruno JOFRET
 *
 *  This file must be used under the terms of the CeCILL.
 *  This source file is licensed as described in the file COPYING, which
 *  you should have received as part of this distribution.  The terms
 *  are also available at
 *  http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
 *
 */

#include <iostream>
#include <map>
#include <cstring>
#include <limits.h>

#include "ScilabView.hxx"
#include "CallGraphicController.hxx"

extern "C"
{
#include "CurrentObject.h"
#include "createGraphicObject.h"
#include "setGraphicObjectProperty.h"
#include "getGraphicObjectProperty.h"
#include "graphicObjectProperties.h"
#include "getScilabJavaVM.h"
#include "deleteGraphicObject.h"
}

/**
 * C Wrapping functions
 * \{
 */
void ScilabNativeView__createObject(int iUID)
{
    ScilabView::createObject(iUID);
}

void ScilabNativeView__deleteObject(int iUID)
{
    ScilabView::deleteObject(iUID);
}

void ScilabNativeView__updateObject(int iUID, int iProperty)
{
    ScilabView::updateObject(iUID, iProperty);
}

void ScilabNativeView__setCurrentFigure(int iUID)
{
    ScilabView::setCurrentFigure(iUID);
}

void ScilabNativeView__setCurrentSubWin(int iUID)
{
    ScilabView::setCurrentSubWin(iUID);
}

void ScilabNativeView__setCurrentObject(int iUID)
{
    ScilabView::setCurrentObject(iUID);
}

int ScilabNativeView__getValidDefaultFigureId()
{
    return ScilabView::getValidDefaultFigureId();
}

/**
 * \}
 */

int ScilabView::getValidDefaultFigureId()
{
    if (m_figureList.empty())
    {
        return 0;
    }
    else
    {
        int max = INT_MIN;
        for (__figureList_iterator it = m_figureList.begin(); it != m_figureList.end(); ++it)
        {
            if (it->second > max)
            {
                max = it->second;
            }
        }

        return max + 1;
    }
}

bool ScilabView::isEmptyFigureList()
{
    return m_figureList.empty();
}

int ScilabView::getFigureFromIndex(int figNum)
{
    __figureList_iterator it;

    for (it = m_figureList.begin(); it != m_figureList.end(); ++it)
    {
        if (it->second == figNum)
        {
            return it->first;
        }
    }
    return 0;
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
    //__figureList_iterator it;
    //int i = (int)(m_figureList.size() - 1);

    //for (it = m_figureList.begin(); it != m_figureList.end(); ++it, --i)
    //{
    //    //std::cerr << "[ScilabView] DEBUG " << it->first << " <-> " << it->second << std::endl;
    //    ids[i] = it->second;
    //}

    __figureList_reverse_iterator it;
    int i = (int)(m_figureList.size() - 1);

    for (it = m_figureList.rbegin(); it != m_figureList.rend(); ++it, --i)
    {
        //std::cerr << "[ScilabView] DEBUG " << it->first << " <-> " << it->second << std::endl;
        ids[i] = it->second;
    }
}

int ScilabView::getNbFigure(void)
{
    return (int)m_figureList.size();
}

void ScilabView::createObject(int iUID)
{
    //std::cerr << "[ScilabView] ++ createObject UID=" << iUID << std::endl;
    int iType = -1;
    int *piType = &iType;

    getGraphicObjectProperty(iUID, __GO_TYPE__, jni_int, (void **)&piType);
    if (iType != -1 && iType == __GO_FIGURE__)
    {
        m_figureList[iUID] = -1;
        setCurrentFigure(iUID);
    }

    // Register object handle.
    getObjectHandle(iUID);
}

void ScilabView::deleteObject(int iUID)
{
    //std::cerr << "[ScilabView] -- deleteObject UID=" << iUID << std::endl;
    int iType = -1;
    int *piType = &iType;
    int iParentUID = 0;

    /*
    ** If deleting a figure, remove from figure list.
    */
    m_figureList.erase(iUID);

    /*
    ** If deleting current figure find another current one,
    ** if there is no more figure : NULL
    */
    if (m_currentFigure == iUID) // Deleting current figure
    {
        int iAxesUID = 0;
        int* piAxesUID = &iAxesUID;

        if (getNbFigure() != 0)
        {
            m_currentFigure = m_figureList.begin()->first;
            getGraphicObjectProperty(m_currentFigure, __GO_SELECTED_CHILD__, jni_int,  (void**)&piAxesUID);
            setCurrentSubWin(iAxesUID);
        }
        else
        {
            setCurrentFigure(0);
            setCurrentSubWin(0);
        }
    }

    /*
    ** If deleting current entity, set parent as new current.
    */
    if (m_currentObject == iUID) // Deleting current object
    {
        iParentUID = getParentObject(iUID);
        setCurrentObject(iParentUID);
    }

    // Remove the corresponding handle.
    __handleList_iterator it = m_handleList.find(iUID);
    if (it != m_handleList.end())
    {
        m_uidList.erase(it->second);
        m_handleList.erase(it);
    }

    deleteDataObject(iUID);
}

void ScilabView::updateObject(int iUID, int iProperty)
{
    //std::cerr << "[ScilabView] == updateObject UID=" << iUID << " PROPERTY=" << pstProperty << std::endl;

    /*
    ** Take care of update if the value update is ID and object type is a Figure I manage.
    */
    if (iProperty == __GO_ID__ && m_figureList.find(iUID) != m_figureList.end())
    {
        int iNewId = 0;
        int *piNewId = &iNewId;

        getGraphicObjectProperty(iUID, __GO_ID__, jni_int, (void **)&piNewId);

        m_figureList[iUID] = iNewId;
        //std::cerr << "### [ScilabView] updateMap UID=" << iUID << " id=" << iNewId << std::endl;
    }
}

/*
** Register ScilabView to Controller.
** Must be done after Graphics models are created.
*/
void ScilabView::registerToController(void)
{
    org_scilab_modules_graphic_objects::CallGraphicController::registerScilabView(getScilabJavaVM());
    m_figureList.get_allocator().allocate(4096);
    m_handleList.get_allocator().allocate(4096);
    m_uidList.get_allocator().allocate(4096);
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
void ScilabView::setCurrentFigure(int UID)
{
    m_currentFigure = UID;
}

/*
** Get Current Figure UID
*/
int ScilabView::getCurrentFigure()
{
    //std::cerr << "[ScilaView] currentFigure = " << (m_currentFigure == 0 ? "NULL !!" : m_currentFigure) << std::endl;
    return m_currentFigure;
}

/*
** Set Current Object UID
*/
void ScilabView::setCurrentObject(int UID)
{
    m_currentObject = UID;
}

/*
** Get Current Figure UID
*/
int ScilabView::getCurrentObject()
{
    //std::cerr << "[ScilaView] currentObject = " << m_currentObject << std::endl;
    return m_currentObject;
}

/*
** Set Current SubWin UID
*/
void ScilabView::setCurrentSubWin(int UID)
{
    m_currentSubWin = UID;
}

/*
** Get Current Figure UID
*/
int ScilabView::getCurrentSubWin()
{
    //std::cerr << "[ScilaView] currentSubWin = " << m_currentSubWin << std::endl;
    return m_currentSubWin;
}

/*
** Scilab only can store long as handle
*/
long ScilabView::getObjectHandle(int UID)
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
    m_handleList[UID] = m_topHandleValue;
    m_uidList[m_topHandleValue] = UID;

    return m_topHandleValue;
}

int ScilabView::getObjectFromHandle(long handle)
{
    __uidList_iterator it = m_uidList.find(handle);
    if (it == m_uidList.end())
    {
        return 0;
    }

    return it->second;
}

int ScilabView::getFigureModel(void)
{
    //std::cerr << "[ScilabView] getFigureModel = " << (m_figureModel == 0 ? "!! NULL !!" : m_figureModel) << std::endl;
    return m_figureModel;
}

void ScilabView::setFigureModel(int UID)
{
    m_figureModel = UID;
}

int ScilabView::getAxesModel(void)
{
    //std::cerr << "[ScilabView] getAxesModel = " << (m_axesModel == 0 ? "!! NULL !!" : m_axesModel) << std::endl;
    return m_axesModel;
}

void ScilabView::setAxesModel(int UID)
{
    m_axesModel = UID;
}

/*
** Allocate static class variable.
*/
ScilabView::__figureList ScilabView::m_figureList = *new __figureList();
ScilabView::__handleList ScilabView::m_handleList = *new __handleList();
ScilabView::__uidList ScilabView::m_uidList = *new __uidList();
long ScilabView::m_topHandleValue = 0;
int ScilabView::m_currentFigure;
int ScilabView::m_currentObject;
int ScilabView::m_currentSubWin;
int ScilabView::m_figureModel;
int ScilabView::m_axesModel;
