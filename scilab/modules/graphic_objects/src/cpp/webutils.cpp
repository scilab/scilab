/*
*  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
*  Copyright (C) 2016 - Scilab Enterprises - Antoine ELIAS
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

#include "webutils.hxx"

extern "C"
{
#include "getGraphicObjectProperty.h"
#include "graphicObjectProperties.h"
}

int WebUtils::getIntProperty(int uid, int prop)
{
    int val = 0;
    int* pVal = &val;
    getGraphicObjectProperty(uid, prop, jni_int, (void **)&pVal);
    return val;
}

double WebUtils::getDoubleProperty(int uid, int prop)
{
    double val = 0;
    double* pVal = &val;
    getGraphicObjectProperty(uid, prop, jni_double, (void **)&pVal);
    return val;
}

void WebUtils::getDoubleVectorProterty(int uid, int prop, std::vector<double>& vect)
{
    double* values = NULL;
    getGraphicObjectProperty(uid, prop, jni_double_vector, (void**)&values);

    int size = (int)vect.size();
    memcpy(vect.data(), values, size * sizeof(double));
    releaseGraphicObjectProperty(prop, values, jni_double_vector, size);
}

void WebUtils::getIntVectorProterty(int uid, int prop, std::vector<int>& vect)
{
    int* values = NULL;
    getGraphicObjectProperty(uid, prop, jni_int_vector, (void**)&values);

    int size = (int)vect.size();
    memcpy(vect.data(), values, size * sizeof(int));
    releaseGraphicObjectProperty(prop, values, jni_int_vector, size);
}

std::string WebUtils::getStringProperty(int uid, int prop)
{
    std::string str;
    char* val = NULL;
    getGraphicObjectProperty(uid, prop, jni_string, (void**)&val);
    str = val;
    releaseGraphicObjectProperty(prop, val, jni_string, 1);
    return str;
}

void WebUtils::getStringVectorProperty(int uid, int prop, std::vector<std::string>& vect)
{
    char **pstString = NULL;
    getGraphicObjectProperty(uid, prop, jni_string_vector, (void **)&pstString);

    int size = (int)vect.size();
    for (int i = 0; i < size; ++i)
    {
        vect[i] = pstString[i];
    }

    releaseGraphicObjectProperty(prop, pstString, jni_string_vector, size);
}

std::string WebUtils::getIdString(int uid)
{
    return "'uid" + std::to_string(uid) + "'";
}

std::string WebUtils::getElementById(int uid)
{
    return "document.getElementById(" + getIdString(uid) + ");";
}

std::string WebUtils::createElement(const std::string& type)
{
    return "document.createElement('" + type + "');";
}

bool WebUtils::isManaged(int uid)
{
    int type = getType(uid);
    switch (type)
    {
        case __GO_FIGURE__:
            return true;
        case __GO_UICONTROL__:
        {
            int style = getStyle(uid);
            switch (style)
            {
                case __GO_UI_PUSHBUTTON__:
                {
                    return true;
                }
                default:
                    return false;
            }
            break;
        }
        default:
            return false;
    }
}

//commons
int WebUtils::getType(int uid)
{
    return getIntProperty(uid, __GO_TYPE__);
}

int WebUtils::getStyle(int uid)
{
    return getIntProperty(uid, __GO_STYLE__);
}

int WebUtils::getParent(int uid)
{
    return getIntProperty(uid, __GO_PARENT__);
}

void WebUtils::setParent(int uid, std::string& str)
{
    int parent = getParent(uid);
    str = "__parent__ = " + getElementById(parent);
    str += "__child__ = " + getElementById(uid);
    str += "__parent__.appendChild(__child__);";
}

void WebUtils::getFigureSize(int uid, std::vector<int>& vect)
{
    vect.resize(2, 0);
    getIntVectorProterty(uid, __GO_SIZE__, vect);
}

void WebUtils::setFigureSize(int uid, std::string& str)
{
    std::vector<int> size;
    getFigureSize(uid, size);

    str = "__child__ = " + getElementById(uid);
    str += "__child__.style.width = '" + std::to_string(size[0]) + "px';";
    str += "__child__.style.height = '" + std::to_string(size[1]) + "px';";
}

void WebUtils::getUIPosition(int uid, std::vector<double>& vect)
{
    vect.resize(4, 0.);
    getDoubleVectorProterty(uid, __GO_POSITION__, vect);
}

void WebUtils::getUIString(int uid, std::vector<std::string> & vect)
{
    int size = getIntProperty(uid, __GO_UI_STRING_SIZE__);
    //int col = getIntProperty(uid, __GO_UI_STRING_COLNB__);
    vect.resize(size, "");
    getStringVectorProperty(uid, __GO_UI_STRING__, vect);
}


void WebUtils::setUIPosition(int uid, std::string& str)
{
    std::vector<int> size;
    getFigureSize(getParent(uid), size);
    std::vector<double> pos;
    getUIPosition(uid, pos);

    str = "__child__ = " + getElementById(uid);
    str += "__child__.style.left = '" + std::to_string((int)pos[0]) + "px';";
    str += "__child__.style.top = '" + std::to_string(size[1] - (int)pos[1] - (int)pos[3]) + "px';";
    str += "__child__.style.width = '" + std::to_string((int)pos[2]) + "px';";
    str += "__child__.style.height = '" + std::to_string((int)pos[3]) + "px';";
}

void WebUtils::setUIString(int uid, std::string& str)
{
    std::vector<std::string> s;
    getUIString(uid, s);

    str = "__child__ = " + getElementById(uid);
    str += "__child__.innerHTML = '" + s[0] + "';";
}

//is
bool WebUtils::hasStyle(int uid, int style)
{
    if (isUIcontrol(uid))
    {
        return getStyle(uid) == style;
    }

    return false;
}

bool WebUtils::isFigure(int uid)
{
    return getType(uid) == __GO_FIGURE__;
}

bool WebUtils::isUIcontrol(int uid)
{
    return getType(uid) == __GO_UICONTROL__;
}

bool WebUtils::isButton(int uid)
{
    return hasStyle(uid, __GO_UI_PUSHBUTTON__);
}


//create
bool WebUtils::createFigure(int uid, std::string& str)
{
    //for the moment figure is a div hardcoded in html page ( .GO_FIGURE )
    //this function just set figure uid to help children to find it
    str = "var __temp__ = " + createElement("DIV");
    str += "__temp__.id = " + getIdString(uid) + ";";
    str += "__temp__.className = 'GO_FIGURE';";
    str += "document.body.appendChild(__temp__);";
    return true;
}

bool WebUtils::createUIControl(int uid, std::string& str)
{
    int style = getStyle(uid);
    switch (style)
    {
        case __GO_UI_PUSHBUTTON__:
        {
            return createPushButton(uid, str);
        }
        default :
            return false;
    }
}

bool WebUtils::createPushButton(int uid, std::string& str)
{
    //create a button, with no parent, wait update with _GO_PARENT to update it
    str += "var __temp__ = " + createElement("BUTTON");
    str += "__temp__.id = " + getIdString(uid) + ";";
    str += "__temp__.className = 'GO_PUSHBUTTON';";
    str += "document.body.appendChild(__temp__);";
    str += "__temp__.onclick = function (){OnUIClick(this)};";


    return true;
}
