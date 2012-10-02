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
#include "deleteGraphicObject.h"
}

/**
 * C Wrapping functions
 * \{
 */
void ScilabNativeView__createObject(char const* pstId)
{
    ScilabView::createObject(pstId);
}

void ScilabNativeView__deleteObject(char const*pstId)
{
    ScilabView::deleteObject(pstId);
}

void ScilabNativeView__updateObject(char const* pstId, int iProperty)
{
    ScilabView::updateObject(pstId, iProperty);
}

/**
 * \}
 */

bool ScilabView::isEmptyFigureList()
{
    return m_figureList.empty();
}

char const* ScilabView::getFigureFromIndex(int figNum)
{
    __figureList_iterator it;

    for (it = m_figureList.begin(); it != m_figureList.end(); ++it)
    {
        if (it->second == figNum)
        {
            return it->first.c_str();
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

void ScilabView::createObject(char const* pstId)
{
    //std::cerr << "[ScilabView] ++ createObject UID=" << pstId << std::endl;
    int iType = -1;
    int *piType = &iType;

    getGraphicObjectProperty(pstId, __GO_TYPE__, jni_int, (void **)&piType);
    if (iType != -1 && iType == __GO_FIGURE__)
    {
        m_figureList[pstId] = -1;
        setCurrentFigure(pstId);
    }

    // Register object handle.
    getObjectHandle(pstId);
}

void ScilabView::deleteObject(char const* pstId)
{
    //std::cerr << "[ScilabView] -- deleteObject UID=" << pstId << std::endl;
    int iType = -1;
    int *piType = &iType;
    char *pstParentUID = NULL;

    getGraphicObjectProperty(pstId, __GO_TYPE__, jni_int, (void **)&piType);

    /*
    ** If deleting a figure, remove from figure list.
    */
    if (iType != -1 && iType == __GO_FIGURE__)
    {
        m_figureList.erase(pstId);
    }

    /*
    ** If deleting current figure find another current one,
    ** if there is no more figure : NULL
    */
    if (m_currentFigure == pstId) // Deleting current figure
    {
        char* pstrAxesUID = NULL;

        if (getNbFigure() != 0)
        {
            m_currentFigure = m_figureList.rbegin()->first;
            getGraphicObjectProperty(m_currentFigure.c_str(), __GO_SELECTED_CHILD__, jni_string,  (void**)&pstrAxesUID);
            setCurrentSubWin(pstrAxesUID);
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
    if (m_currentObject == pstId) // Deleting current object
    {
        getGraphicObjectProperty(pstId, __GO_PARENT__, jni_string, (void **)&pstParentUID);
        setCurrentObject(pstParentUID);
    }

    // Remove the corresponding handle.
    m_uidList.erase(m_handleList.find(pstId)->second);
    m_handleList.erase(pstId);

    deleteDataObject(pstId);
}

void ScilabView::updateObject(char const* pstId, int iProperty)
{
    //std::cerr << "[ScilabView] == updateObject UID=" << pstId << " PROPERTY=" << pstProperty << std::endl;

    /*
     ** Take care of update if the value update is ID and object type is a Figure I manage.
     */
    if (iProperty == __GO_ID__ && m_figureList.find(pstId) != m_figureList.end())
    {
        int iNewId = 0;
        int *piNewId = &iNewId;

        getGraphicObjectProperty(pstId, __GO_ID__, jni_int, (void **)&piNewId);

        m_figureList[pstId] = iNewId;
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
void ScilabView::setCurrentFigure(char const* UID)
{
    if (UID == NULL)
    {
        m_currentFigure = std::string();
    }
    else
    {
        m_currentFigure = UID;
    }
}

/*
** Get Current Figure UID
*/
char const* ScilabView::getCurrentFigure()
{
    //std::cerr << "[ScilaView] currentFigure = " << (m_currentFigure == NULL ? "NULL !!" : m_currentFigure) << std::endl;
    if (m_currentFigure.length() == 0)
    {
        return NULL;
    }
    return m_currentFigure.c_str();
}

/*
** Set Current Object UID
*/
void ScilabView::setCurrentObject(char const* UID)
{
    if (UID == NULL)
    {
        m_currentObject = std::string();
    }
    else
    {
        m_currentObject = UID;
    }
}

/*
** Get Current Figure UID
*/
char const* ScilabView::getCurrentObject()
{
    //std::cerr << "[ScilaView] currentObject = " << m_currentObject << std::endl;
    if (m_currentFigure.length() == 0)
    {
        return NULL;
    }
    return m_currentObject.c_str();
}

/*
** Set Current SubWin UID
*/
void ScilabView::setCurrentSubWin(char const* UID)
{
    if (UID == NULL)
    {
        m_currentSubWin = std::string();
    }
    else
    {
        m_currentSubWin = UID;
    }
}

/*
** Get Current Figure UID
*/
char const* ScilabView::getCurrentSubWin()
{
    //std::cerr << "[ScilaView] currentSubWin = " << m_currentSubWin << std::endl;
    if (m_currentSubWin.length() == 0)
    {
        return NULL;
    }
    return m_currentSubWin.c_str();
}

/*
** Scilab only can store long as handle
*/
long ScilabView::getObjectHandle(char const* UID)
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

char const* ScilabView::getObjectFromHandle(long handle)
{
    __uidList_iterator it = m_uidList.find(handle);
    if (it == m_uidList.end())
    {
        return NULL;
    }

    return it->second.c_str();
}

char const* ScilabView::getFigureModel(void)
{
    //std::cerr << "[ScilabView] getFigureModel = " << (m_figureModel == NULL ? "!! NULL !!" : m_figureModel) << std::endl;
    if (m_figureModel.length() == 0)
    {
        return NULL;
    }
    return m_figureModel.c_str();
}

void ScilabView::setFigureModel(char const* UID)
{
    m_figureModel = UID;
}

char const* ScilabView::getAxesModel(void)
{
    //std::cerr << "[ScilabView] getAxesModel = " << (m_axesModel == NULL ? "!! NULL !!" : m_axesModel) << std::endl;
    if (m_axesModel.length() == 0)
    {
        return NULL;
    }
    return m_axesModel.c_str();
}

void ScilabView::setAxesModel(char const* UID)
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
std::string ScilabView::m_currentFigure;
std::string ScilabView::m_currentObject;
std::string ScilabView::m_currentSubWin;
std::string ScilabView::m_figureModel;
std::string ScilabView::m_axesModel;
