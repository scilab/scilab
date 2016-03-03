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

#include <string.h>
#include <iostream>
#include <cmath>
#include "webutils.hxx"

extern "C"
{
#include "setGraphicObjectProperty.h"
#include "getGraphicObjectProperty.h"
#include "graphicObjectProperties.h"
#include "LayoutType.h"
#include "BorderLayoutType.h"
}

WebUtils::SETTER WebUtils::setter;
WebUtils::WAITING_PROP WebUtils::waitprop;

int WebUtils::getIntProperty(int uid, int prop)
{
    int val = 0;
    int* pVal = &val;
    getGraphicObjectProperty(uid, prop, jni_int, (void **)&pVal);
    return val;
}

bool WebUtils::getBoolProperty(int uid, int prop)
{
    int val = 0;
    int* pVal = &val;
    getGraphicObjectProperty(uid, prop, jni_bool, (void **)&pVal);
    return val != 0;
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

bool WebUtils::setStringProperty(int uid, int prop, const std::string& value)
{
    setGraphicObjectProperty(uid, prop, value.data(), jni_string, 1);
    return true;
}

bool WebUtils::setStringVectorProperty(int uid, int prop, const std::vector<std::string>& values)
{
    int size = (int)values.size();
    std::vector<const char*> c(size, NULL);
    for (int i = 0; i < size; ++i)
    {
        c[i] = values[i].data();
    }

    setGraphicObjectProperty(uid, prop, (void**)c.data(), jni_string_vector, size);
    return true;
}

bool WebUtils::setDoubleVectorProperty(int uid, int prop, const std::vector<double>& values)
{
    setGraphicObjectProperty(uid, prop, values.data(), jni_double_vector, (int)values.size());
    return true;
}

std::string WebUtils::getIdString(int uid, const std::string& suffix)
{
    return "'uid" + std::to_string(uid) + suffix + "'";
}

std::string WebUtils::getElementById(int uid, const std::string& suffix)
{
    return "document.getElementById(" + getIdString(uid, suffix) + ");";
}

std::string WebUtils::createElement(const std::string& type)
{
    return "document.createElement('" + type + "');";
}

std::string WebUtils::getColor(const std::vector<double>& c)
{
    if (c[0] == -1)
    {
        return "''";
    }

    std::string str("'rgb(");

    //use abs to convert [-1 -1 -1 ] to (255, 255, 255)
    str += std::to_string((int)std::abs(c[0] * 255));
    for (int i = 1; i < c.size(); ++i)
    {
        str += ", " + std::to_string((int)std::abs(c[i] * 255));
    }

    str += ")'";

    return str;
}

std::string WebUtils::getSubPadding(int val)
{
    if (val)
    {
        return std::to_string(val) + "px ";
    }

    return "0 ";
}

std::string WebUtils::getPadding(int t, int r, int b, int l)
{
    std::string ret;

    ret += getSubPadding(t);
    ret += getSubPadding(r);
    ret += getSubPadding(b);
    ret += getSubPadding(l);

    return ret;
}

bool WebUtils::isManaged(int uid)
{
    int type = getType(uid);
    switch (type)
    {
        case __GO_FIGURE__:
        case __GO_UICONTROL__:
            return true;
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

bool WebUtils::hasValidParent(int uid)
{
    return getParent(uid) != 0;
}

int WebUtils::getParent(int uid)
{
    return getIntProperty(uid, __GO_PARENT__);
}

void WebUtils::setParent(int uid, std::string& str, bool append)
{
    int parent = getParent(uid);

    if (append == false)
    {
        str = "var __child__ = " + getElementById(uid);
    }

    str += "var __parent__ = " + getElementById(parent);
    str += "__parent__.appendChild(__child__);";

    if (getUILayout(parent) == LAYOUT_BORDER)
    {
        //force update of border position, especially for center
        addInWaitingQueue(uid, __GO_UI_BORDER_POSITION__);
    }

    setWaitingProperties(uid, str, true);
}

void WebUtils::getFigureSize(int uid, std::vector<int>& vect)
{
    int fig = getFigureId(uid);
    vect.resize(2, 0);
    getIntVectorProterty(fig, __GO_AXES_SIZE__, vect);
}

void WebUtils::setFigureSize(int uid, std::string& str, bool append)
{
    std::vector<int> size;
    getFigureSize(uid, size);

    if (append == false)
    {
        str = "var __child__ = " + getElementById(uid);
    }
    str += "__child__.style.width = '" + std::to_string(size[0]) + "px';";
    str += "__child__.style.height = '" + std::to_string(size[1]) + "px';";
}

int WebUtils::getFigureId(int uid)
{
    int id = uid;
    while (isFigure(id) == false)
    {
        id = getParent(id);
    }

    return id;
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


void WebUtils::setUIPosition(int uid, std::string& str, bool append)
{
    std::vector<double> pos;
    getUIPosition(uid, pos);

    if (append == false)
    {
        str = "var __child__ = " + getElementById(uid);
    }
    str += "__child__.style.left = '" + std::to_string((int)pos[0]) + "px';";
    str += "__child__.style.bottom = '" + std::to_string((int)pos[1]) + "px';";
    str += "__child__.style.width = '" + std::to_string((int)pos[2]) + "px';";
    str += "__child__.style.height = '" + std::to_string((int)pos[3]) + "px';";

    //to ensure vertical alignement, adapt style.line-height
    if (hasStyle(uid, __GO_UI_TEXT__) || hasStyle(uid, __GO_UI_CHECKBOX__) || hasStyle(uid, __GO_UI_RADIOBUTTON__))
    {
        std::string v;
        getUIVerticalAlignment(uid, v);
        if (v == "middle")
        {
            str += "__child__.style.lineHeight = '" + std::to_string((int)pos[3]) + "px';";
        }
        else
        {
            str += "__child__.style.lineHeight = 'initial';";
        }
    }
}

void WebUtils::setUIString(int uid, std::string& str, bool append)
{
    std::vector<std::string> s;
    getUIString(uid, s);
    if (s.size() == 0)
    {
        s.push_back("");
    }

    if (hasStyle(uid, __GO_UI_CHECKBOX__) || hasStyle(uid, __GO_UI_RADIOBUTTON__))
    {
        if (append == false)
        {
            str == "";
        }

        str += "var __label__ = " + getElementById(uid, "_label");
        str += "__label__.innerHTML = '" + s[0] + "';";
    }
    else if (hasStyle(uid, __GO_UI_POPUPMENU__) || hasStyle(uid, __GO_UI_LISTBOX__))
    {
        if (append == false)
        {
            str = "var __child__ = " + getElementById(uid);
        }

        //remove previous values
        str += "while (__child__.length) {__child__.remove(0);}";

        str += "var option;";
        int size = (int)s.size();
        for (int i = 0; i < size; ++i)
        {
            str += "option = new Option('" + s[i] + "');";
            str += "__child__.add(option);";
        }

        if (hasStyle(uid, __GO_UI_LISTBOX__))
        {
            //switch to listbox instead of combobox
            str += "__child__.size = 2;";
        }
    }
    else
    {
        if (append == false)
        {
            str = "var __child__ = " + getElementById(uid);
        }

        if (isInputType(uid))
        {
            str += "__child__.value = '" + s[0] + "';";
        }
        else
        {
            str += "__child__.innerHTML = '" + s[0] + "';";
        }
    }
}

bool WebUtils::getVisible(int uid)
{
    return getBoolProperty(uid, __GO_VISIBLE__);
}

void WebUtils::setVisible(int uid, std::string& str, bool append)
{
    //reverse flag value
    std::string v = getVisible(uid) ? "inherit" : "hidden";
    if (append == false)
    {
        str = "var __child__ = " + getElementById(uid);
    }

    str += "__child__.style.visibility  = '" + v + "';";
}

bool WebUtils::getUIEnable(int uid)
{
    return getBoolProperty(uid, __GO_UI_ENABLE__);
}

void WebUtils::setUIEnable(int uid, std::string& str, bool append)
{
    //reverse flag value
    std::string v = getUIEnable(uid) ? "false" : "true";
    if (append == false)
    {
        str = "var __child__ = " + getElementById(uid);
    }

    str += "__child__.disabled  = " + v + ";";
}

bool WebUtils::getUIBackgroundColor(int uid, std::vector<double>& vect)
{
    vect.resize(3, 0.);
    getDoubleVectorProterty(uid, __GO_UI_BACKGROUNDCOLOR__, vect);
    return true;
}

void WebUtils::setUIBackgroundColor(int uid, std::string& str, bool append)
{
    std::vector<double> c;
    getUIBackgroundColor(uid, c);
    if (append == false)
    {
        str = "var __child__ = " + getElementById(uid);
    }

    str += "__child__.style.backgroundColor  = " + getColor(c) + ";";
}

bool WebUtils::getUIFontAngle(int uid, std::string& str)
{
    str = getStringProperty(uid, __GO_UI_FONTANGLE__);
    return true;
}

void WebUtils::setUIFontAngle(int uid, std::string& str, bool append)
{
    std::string angle;
    getUIFontAngle(uid, angle);
    if (append == false)
    {
        str = "var __child__ = " + getElementById(uid);
    }

    str += "__child__.style.fontStyle  = '" + angle + "';";
}

bool WebUtils::getUIFontName(int uid, std::string& str)
{
    str = getStringProperty(uid, __GO_UI_FONTNAME__);
    return true;
}

void WebUtils::setUIFontName(int uid, std::string& str, bool append)
{
    std::string font;
    getUIFontName(uid, font);
    if (append == false)
    {
        str = "var __child__ = " + getElementById(uid);
    }

    str += "__child__.style.fontFamily  = '" + font + "','serif' ;";
}

bool WebUtils::getUIFontUnits(int uid, std::string& str)
{
    str = getStringProperty(uid, __GO_UI_FONTUNITS__);
    return true;
}

double WebUtils::getUIFontSize(int uid)
{
    return getDoubleProperty(uid, __GO_UI_FONTSIZE__);
}

void WebUtils::setUIFontSize(int uid, std::string& str, bool append)
{
    int size = (int)getUIFontSize(uid);

    std::string units;
    getUIFontUnits(uid, units);

    if (units == "pixels")
    {
        units = "px";
    }
    else //"points"
    {
        units = "pt";
        size = (int)(size * 0.75);
    }

    if (append == false)
    {
        str = "var __child__ = " + getElementById(uid);
    }

    str += "__child__.style.fontSize  = '" + std::to_string(size) + units + "';";
}

bool WebUtils::getUIFontWeight(int uid, std::string& str)
{
    str = getStringProperty(uid, __GO_UI_FONTWEIGHT__);
    return true;
}

void WebUtils::setUIFontWeight(int uid, std::string& str, bool append)
{
    std::string weight;
    getUIFontWeight(uid, weight);

    if (append == false)
    {
        str = "var __child__ = " + getElementById(uid);
    }

    str += "__child__.style.fontWeight  = '" + weight + "';";
}

bool WebUtils::getUIForegroundColor(int uid, std::vector<double>& vect)
{
    vect.resize(3, 0.);
    getDoubleVectorProterty(uid, __GO_UI_FOREGROUNDCOLOR__, vect);
    return true;
}

void WebUtils::setUIForegroundColor(int uid, std::string& str, bool append)
{
    std::vector<double> c;
    getUIForegroundColor(uid, c);

    if (append == false)
    {
        str = "var __child__ = " + getElementById(uid);
    }

    str += "__child__.style.color  = " + getColor(c) + ";";
}

bool WebUtils::getUIHorizontalAlignment(int uid, std::string& str)
{
    str = getStringProperty(uid, __GO_UI_HORIZONTALALIGNMENT__);
    return true;
}

void WebUtils::setUIHorizontalAlignment(int uid, std::string& str, bool append)
{
    std::string align;
    getUIHorizontalAlignment(uid, align);

    if (append == false)
    {
        str = "var __child__ = " + getElementById(uid);
    }

    str += "__child__.style.textAlign  = '" + align + "';";
}

bool WebUtils::getUIRelief(int uid, std::string& str)
{
    str = getStringProperty(uid, __GO_UI_RELIEF__);
    return true;
}

void WebUtils::setUIRelief(int uid, std::string& str, bool append)
{
}

bool WebUtils::getUIVerticalAlignment(int uid, std::string& str)
{
    str = getStringProperty(uid, __GO_UI_VERTICALALIGNMENT__);
    return true;
}

void WebUtils::setUIVerticalAlignment(int uid, std::string& str, bool append)
{
}

int WebUtils::getUILayout(int uid)
{
    return getIntProperty(uid, __GO_LAYOUT__);
    return true;
}

void WebUtils::setUILayout(int uid, std::string& str, bool append)
{
    if (isFigure(uid) || hasStyle(uid, __GO_UI_FRAME__))
    {
        //TODO clean previous layout before create new one

        if (append == false)
        {
            str = "var __child__ = " + getElementById(uid) + ";";
        }

        int layout = getUILayout(uid);
        switch (layout)
        {
            case LAYOUT_BORDER:
            {
                //create a table ( ascii art powa ! )
                //+---------------------+
                //|         TOP         |
                //+---+-------------+---+
                //| L |             | R |
                //| E |             | I |
                //| F |   CENTER    | G |
                //| T |             | H |
                //|   |             | T |
                //+---+-------------+---+
                //|       BOTTOM        |
                //+---------------------+

                //table
                str += "var __table__ = " + createElement("TABLE");
                str += "__table__.id = " + getIdString(uid, "_table") + ";";
                str += "__table__.style.margin = '0';";
                str += "__table__.style.padding = '0';";
                str += "__table__.style.width = '100%';";
                str += "__table__.style.height = '100%';";
                str += "__table__.style.borderCollapse = 'collapse';";

                //rows
                str += "var __tr_top__ = " + createElement("TR");
                str += "var __tr_middle__ = " + createElement("TR");
                str += "var __tr_bottom__ = " + createElement("TR");

                //cell in rows
                str += "var __td_top__ = " + createElement("TD");
                str += "__td_top__.colSpan = '3';";
                str += "__td_top__.id = " + getIdString(uid, "_top") + ";";
                str += "__td_top__.style.width = '100%';";

                str += "var __td_left__ = " + createElement("TD");
                str += "__td_left__.id = " + getIdString(uid, "_left") + ";";
                str += "__td_left__.style.height = '100%';";

                str += "var __td_center__ = " + createElement("TD");
                str += "__td_center__.id = " + getIdString(uid, "_center") + ";";
                str += "__td_center__.style.width = '100%';";
                str += "__td_center__.style.height = '100%';";

                str += "var __td_right__ = " + createElement("TD");
                str += "__td_right__.id = " + getIdString(uid, "_right") + ";";
                str += "__td_right__.style.height = '100%';";

                str += "var __td_bottom__ = " + createElement("TD");
                str += "__td_bottom__.colSpan = '3';";
                str += "__td_bottom__.id = " + getIdString(uid, "_bottom") + ";";
                str += "__td_bottom__.style.width = '100%';";

                //hierarchy

                //td in tr
                str += "__tr_top__.appendChild(__td_top__);";
                str += "__tr_middle__.appendChild(__td_left__);";
                str += "__tr_middle__.appendChild(__td_center__);";
                str += "__tr_middle__.appendChild(__td_right__);";
                str += "__tr_bottom__.appendChild(__td_bottom__);";

                //tr in table
                str += "__table__.appendChild(__tr_top__);";
                str += "__table__.appendChild(__tr_middle__);";
                str += "__table__.appendChild(__tr_bottom__);";

                //table in parent
                str += "__child__.appendChild(__table__);";

                break;
            }
            case LAYOUT_GRID:
            {
                break;
            }
            case LAYOUT_GRIDBAG:
            {
                //add empty table
                str += "var __table__ = " + createElement("TABLE");
                str += "__table__.id = " + getIdString(uid, "_table") + ";";
                str += "__table__.style.margin = '0';";
                str += "__table__.style.padding = '0';";
                str += "__table__.style.width = '100%';";
                str += "__table__.style.height = '100%';";
                str += "__table__.style.borderCollapse = 'collapse';";

                //table in parent
                str += "__child__.appendChild(__table__);";
                break;
            }
            default:
            case LAYOUT_NONE:
            {
                break;
            }

        }
    }
}

double WebUtils::getUIMin(int uid)
{
    return getDoubleProperty(uid, __GO_UI_MIN__);
}

void WebUtils::setUIMin(int uid, std::string& str, bool append)
{
    if (hasStyle(uid, __GO_UI_SPINNER__) || hasStyle(uid, __GO_UI_SLIDER__))
    {
        double min = getUIMin(uid);

        if (append == false)
        {
            str = "var __child__ = " + getElementById(uid);
        }

        str += "__child__.min  = '" + std::to_string(min) + "';";
    }
}

double WebUtils::getUIMax(int uid)
{
    return getDoubleProperty(uid, __GO_UI_MAX__);
}

void WebUtils::setUIMax(int uid, std::string& str, bool append)
{
    if (hasStyle(uid, __GO_UI_SPINNER__) || hasStyle(uid, __GO_UI_SLIDER__))
    {
        double max = getUIMax(uid);

        if (append == false)
        {
            str = "var __child__ = " + getElementById(uid);
        }

        str += "__child__.max  = '" + std::to_string(max) + "';";
    }
}

double WebUtils::getUIStep(int uid)
{
    std::vector<double> step(2, 0.);
    getDoubleVectorProterty(uid, __GO_UI_SLIDERSTEP__, step);
    return step[0];
}

void WebUtils::setUIStep(int uid, std::string& str, bool append)
{
    if (hasStyle(uid, __GO_UI_SPINNER__) || hasStyle(uid, __GO_UI_SLIDER__))
    {
        double step = getUIStep(uid);

        if (append == false)
        {
            str = "var __child__ = " + getElementById(uid);
        }

        str += "__child__.step  = '" + std::to_string(step) + "';";
    }
}

bool WebUtils::getUIValue(int uid, std::vector<double>& vect)
{
    int size = getIntProperty(uid, __GO_UI_VALUE_SIZE__);
    vect.resize(size, 0.);
    getDoubleVectorProterty(uid, __GO_UI_VALUE__, vect);
    return true;
}

void WebUtils::setUIValue(int uid, std::string& str, bool append)
{
    if (hasStyle(uid, __GO_UI_SPINNER__) || hasStyle(uid, __GO_UI_SLIDER__))
    {
        std::vector<double> values;
        getUIValue(uid, values);
        if (values.size() == 0)
        {
            values.push_back(0);
        }

        if (append == false)
        {
            str = "var __child__ = " + getElementById(uid);
        }

        str += "__child__.value  = '" + std::to_string(values[0]) + "';";
    }
}

void WebUtils::getUIBorderPreferredSize(int uid, std::vector<int>& vect)
{
    vect.resize(2, 0);
    getIntVectorProterty(uid, __GO_UI_BORDER_PREFERREDSIZE__, vect);
}

void WebUtils::getUIBorderPadding(int uid, std::vector<int>& vect)
{
    vect.resize(2, 0);
    getIntVectorProterty(uid, __GO_BORDER_OPT_PADDING__, vect);
}

int WebUtils::getUIBorderPosition(int uid)
{
    return getIntProperty(uid, __GO_UI_BORDER_POSITION__);
}

void WebUtils::setUIBorder(int uid, std::string& str, bool append)
{
    int parent = getParent(uid);
    std::string position;
    std::string padding;
    int border = getUIBorderPosition(uid);

    std::vector<int> pad;
    getUIBorderPadding(parent, pad);

    std::vector<int> size;
    getUIBorderPreferredSize(uid, size);



    if (append == false)
    {
        str = "var __child__ = " + getElementById(uid);
    }

    switch (border)
    {
        default:
        case BORDER_CENTER:
            position = "_center";
            str += "__child__.style.width = '100%';";
            str += "__child__.style.height = '100%';";
            padding = getPadding(0, 0, 0, 0);
            break;
        case BORDER_BOTTOM:
            position = "_bottom";
            str += "__child__.style.width = '100%';";

            if (size[1] == -1)
            {
                str += "__child__.style.height = 'inherit';";
            }
            else
            {
                str += "__child__.style.height = '" + getSubPadding(size[1]) + "';";
            }

            padding = getPadding(pad[1], 0, 0, 0);
            break;
        case BORDER_TOP:
            position = "_top";
            str += "__child__.style.width = '100%';";

            if (size[1] == -1)
            {
                str += "__child__.style.height = 'inherit';";
            }
            else
            {
                str += "__child__.style.height = '" + getSubPadding(size[1]) + "';";
            }

            padding = getPadding(0, 0, pad[1], 0);
            break;
        case BORDER_LEFT:
            position = "_left";

            if (size[0] == -1)
            {
                str += "__child__.style.width = 'inherit';";
            }
            else
            {
                str += "__child__.style.width = '" + getSubPadding(size[0]) + "';";
            }

            str += "__child__.style.height = '100%';";
            padding = getPadding(0, pad[0], 0, 0);
            break;
        case BORDER_RIGHT:
            position = "_right";

            if (size[0] == -1)
            {
                str += "__child__.style.width = 'inherit';";
            }
            else
            {
                str += "__child__.style.width = '" + getSubPadding(size[0]) + "';";
            }

            str += "__child__.style.height = '100%';";
            padding = getPadding(0, 0, 0, pad[0]);
            break;
    }


    //move child in targeted cell
    str += "__parent__ = " + getElementById(parent, position);
    str += "__parent__.appendChild(__child__);";
    str += "__parent__.style.padding = '" + padding + "';";

    //to well perform positionning, we must clear some default properties
    //position left, right, width, height,

    str += "__child__.style.position = 'inherit';";
    str += "__child__.style.left = 'inherit';";
    str += "__child__.style.bottom = 'inherit';";
}

void WebUtils::getUIGridBagGrid(int uid, std::vector<int>& vect)
{
    vect.resize(4, 0);
    getIntVectorProterty(uid, __GO_UI_GRIDBAG_GRID__, vect);
}


void WebUtils::setUIGridBag(int uid, std::string& str, bool append)
{
    int parent = getParent(uid);
    std::vector<int> grid;
    getUIGridBagGrid(uid, grid);

    if (append == false)
    {
        str = "var __child__ = " + getElementById(uid);
    }

    str += "__child__.style.width = '100%';";
    str += "__child__.style.position = 'inherit';";
    str += "__child__.style.left = 'inherit';";
    str += "__child__.style.bottom = 'inherit';";
    str += "__child__.style.height = '100%';";
    //we have to create a td with grid information and add it to the good cell ( or create if not exist ! )

    //build td child
    std::string td;
    td = "var __td__ = " + createElement("TD");

    std::string name("_" + std::to_string(grid[0]) + "_" + std::to_string(grid[1]));
    td += "__td__.id = " + getIdString(parent, name) + ";";

    if (grid[2] != 1)
    {
        td += "__td__.colSpan = '" + std::to_string(grid[2]) + "';";
    }

    if (grid[3] != 1)
    {
        td += "__td__.rowSpan = '" + std::to_string(grid[3]) + "';";
    }

    td += "__td__.appendChild(__child__);";

    //build or get tr
    name = "_" + std::to_string(grid[1]);
    std::string tr;
    tr = "var __tr__ = " + getElementById(parent, name);
    tr += "var __table__ = " + getElementById(parent, "_table");
    tr += "if(__tr__ == null){";
    tr += "__tr__ = " + createElement("TR");
    tr += "__tr__.id = " + getIdString(parent, name) + ";";
    tr += "gridbagHelperTR(__table__, __tr__, " + std::to_string(grid[1]) + ");";
    tr += "}";
    tr += "gridbagHelperTD(__tr__,__td__, " + std::to_string(grid[0]) + ");";

    //to force refresh of table, move it to another component and restore it
    std::string tricktoforcerefresh("\nvar __scilab__ = document.getElementById('scilab');\n");
    tricktoforcerefresh += "__scilab__.appendChild(__table__);\n";
    tricktoforcerefresh += "var __parent__ = " + getElementById(parent) + "\n";
    tricktoforcerefresh += "__parent__.appendChild(__table__);";


    str += td;
    str += tr;
    str += tricktoforcerefresh;
}

bool WebUtils::hasCallback(int uid)
{
    return getStringProperty(uid, __GO_CALLBACK__) != "";
}

void WebUtils::setCallback(int uid, std::string& str, bool append)
{
    if (append == false)
    {
        str = "var __child__ = " + getElementById(uid);
    }

    std::string event;
    std::string func;
    switch (getStyle(uid))
    {
        case __GO_UI_PUSHBUTTON__ :
            event = "click";
            func = "onPushButton";
            break;
        case __GO_UI_CHECKBOX__:
            event = "click";
            func = "onCheckBox";
            break;
        case __GO_UI_RADIOBUTTON__:
            event = "click";
            func = "onRadioButton";
            break;
        case __GO_UI_LISTBOX__:
            event = "change";
            func = "onListBox";
            break;
        case __GO_UI_POPUPMENU__:
            event = "change";
            func = "onComboBox";
            break;
        /*
                case __GO_UI_TAB__:
                    event = "click";
                    func = "onPushButton";
                    break;
        */
        case __GO_UI_SLIDER__:
            event = "input";
            func = "onSlider";
            break;
        case __GO_UI_EDIT__:
            event = "input";
            func = "onEditBox";
            break;
        case __GO_UI_SPINNER__:
            event = "input";
            func = "onSpinner";
            break;
        default :
            return;
    }

    //add callback listener
    str += "__child__.addEventListener('" + event + "', " + func + ");";
}

//is
bool WebUtils::isInputType(int uid)
{
    switch (getStyle(uid))
    {
        case __GO_UI_EDIT__ :
        case __GO_UI_SLIDER__:
        case __GO_UI_SPINNER__ :
            return true;
        default :
            return false;
    }
}

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
    //set figure uid to help children to find it
    str = "var __parent__ = document.getElementById('scilab');";
    str += "var __temp__ = " + createElement("DIV");
    str += "__temp__.id = " + getIdString(uid) + ";";
    str += "__temp__.className = 'GO_FIGURE';";
    str += "__parent__.innerHTML += '<br>';";
    str += "__parent__.appendChild(__temp__);";
    str += "__parent__.innerHTML += '<br>';";
    return true;
}

bool WebUtils::createUIControl(int uid, std::string& str)
{
    int style = getStyle(uid);
    switch (style)
    {
        case __GO_UI_FRAME__:
            return createFrame(uid, str);
        case __GO_UI_PUSHBUTTON__:
            return createPushButton(uid, str);
        case __GO_UI_TEXT__:
            return createText(uid, str);
        case __GO_UI_EDIT__:
            return createEdit(uid, str);
        case __GO_UI_CHECKBOX__:
            return createCheckbox(uid, str);
        case __GO_UI_RADIOBUTTON__:
            return createRadio(uid, str);
        case __GO_UI_SLIDER__:
            return createSlider(uid, str);
        case __GO_UI_LISTBOX__:
            return createListbox(uid, str);
        case __GO_UI_POPUPMENU__:
            return createCombobox(uid, str);
        case __GO_UI_SPINNER__:
            return createSpinner(uid, str);
        default:
            return false;
    }
}

bool WebUtils::createCommonIUControl(int uid, const std::string& htmlType, const std::string& cssClass, std::string& str)
{
    //create a button, with no parent, wait update with _GO_PARENT to update it
    str = "var __temp__ = " + createElement(htmlType);
    str += "__temp__.id = " + getIdString(uid) + ";";
    str += "__temp__.className = '" + cssClass + "';";

    return true;
}

bool WebUtils::createPushButton(int uid, std::string& str)
{
    //create a <button>
    createCommonIUControl(uid, "BUTTON", "GO_PUSHBUTTON", str);
    //add item temporary in main scilabview div waiting __GO_PARENT__ update
    str += "var __parent__ = document.getElementById('scilab');";
    str += "__parent__.appendChild(__temp__);";

    return true;
}

bool WebUtils::createFrame(int uid, std::string& str)
{
    //create a <div>
    createCommonIUControl(uid, "DIV", "GO_FRAME", str);
    //add item temporary in main div waiting __GO_PARENT__ update
    str += "var __parent__ = document.getElementById('scilab');";
    str += "__parent__.appendChild(__temp__);";

    return true;
}

bool WebUtils::createText(int uid, std::string& str)
{
    //create a <div>
    createCommonIUControl(uid, "LABEL", "GO_TEXT", str);
    //add item temporary in main div waiting __GO_PARENT__ update
    str += "var __parent__ = document.getElementById('scilab');";
    str += "__parent__.appendChild(__temp__);";

    return true;
}

bool WebUtils::createEdit(int uid, std::string& str)
{
    //create a <input>
    createCommonIUControl(uid, "INPUT", "GO_EDIT", str);
    str += "__temp__.type = 'text';";
    //add item temporary in main div waiting __GO_PARENT__ update
    str += "var __parent__ = document.getElementById('scilab');";
    str += "__parent__.appendChild(__temp__);";

    return true;
}

bool WebUtils::createCheckbox(int uid, std::string& str)
{
    //create a <input>

    //for checkbox we need to create 3 elements.

    //a div to enclose others
    str = "var __main__ = " + createElement("DIV");
    str += "__main__.id = " + getIdString(uid) + ";";
    str += "__main__.className = 'GO_CHECKBOX';";
    //add item temporary in main div waiting __GO_PARENT__ update
    str += "var __parent__ = document.getElementById('scilab');";
    str += "__parent__.appendChild(__main__);";

    //the checkbox itself
    str += "var __temp__ = " + createElement("INPUT");
    str += "__temp__.className = 'GO_CHECKBOX_CHECKBOX';";
    str += "__temp__.type = 'checkbox';";
    str += "__temp__.id = " + getIdString(uid, "_checkbox") + ";";
    //add item temporary in main div waiting __GO_PARENT__ update
    str += "__main__.appendChild(__temp__);";

    //the label of the checkbox
    str += "var __label__ = " + createElement("LABEL");
    str += "__label__.id = " + getIdString(uid, "_label") + ";";
    str += "__label__.className = 'GO_CHECKBOX_LABEL';";
    str += "__label__.htmlFor = " + getIdString(uid, "_checkbox") + ";";
    //add item temporary in main div waiting __GO_PARENT__ update
    str += "__main__.appendChild(__label__);";

    return true;
}

bool WebUtils::createRadio(int uid, std::string& str)
{
    //create a <input>

    //for radio we need to create 3 elements.

    //a div to enclose others
    str = "var __main__ = " + createElement("DIV");
    str += "__main__.id = " + getIdString(uid) + ";";
    str += "__main__.className = 'GO_RADIO';";
    //add item temporary in main div waiting __GO_PARENT__ update
    str += "var __parent__ = document.getElementById('scilab');";
    str += "__parent__.appendChild(__main__)";;

    //the radio itself
    str += "var __temp__ = " + createElement("INPUT");
    str += "__temp__.className = 'GO_RADIO_RADIO';";
    str += "__temp__.type = 'radio';";
    str += "__temp__.id = " + getIdString(uid, "_radio") + ";";
    //add item temporary in main div waiting __GO_PARENT__ update
    str += "__main__.appendChild(__temp__);";

    //the label of the checkbox
    str += "var __label__ = " + createElement("LABEL");
    str += "__label__.id = " + getIdString(uid, "_label") + ";";
    str += "__label__.className = 'GO_RADIO_LABEL';";
    str += "__label__.htmlFor = " + getIdString(uid, "_radio") + ";";
    //add item temporary in main div waiting __GO_PARENT__ update
    str += "__main__.appendChild(__label__);";

    return true;
}

bool WebUtils::createSlider(int uid, std::string& str)
{
    //create a <input>
    createCommonIUControl(uid, "INPUT", "GO_SLIDER", str);
    str += "__temp__.type = 'range';";
    //add item temporary in main div waiting __GO_PARENT__ update
    str += "var __parent__ = document.getElementById('scilab');";
    str += "__parent__.appendChild(__temp__);";

    return true;
}

bool WebUtils::createListbox(int uid, std::string& str)
{
    //create a <input>
    createCommonIUControl(uid, "SELECT", "GO_LISTBOX", str);
    //add item temporary in main div waiting __GO_PARENT__ update
    str += "var __parent__ = document.getElementById('scilab');";
    str += "__parent__.appendChild(__temp__);";

    return true;
}

bool WebUtils::createCombobox(int uid, std::string& str)
{
    //create a <input>
    createCommonIUControl(uid, "SELECT", "GO_COMBOBOX", str);
    str += "__temp__.size = 1;";
    //add item temporary in main div waiting __GO_PARENT__ update
    str += "var __parent__ = document.getElementById('scilab');";
    str += "__parent__.appendChild(__temp__);";

    return true;
}

bool WebUtils::createSpinner(int uid, std::string& str)
{
    //create a <input>
    createCommonIUControl(uid, "INPUT", "GO_SPINNER", str);
    str += "__temp__.type = 'number';";
    //add item temporary in main div waiting __GO_PARENT__ update
    str += "var __parent__ = document.getElementById('scilab');";
    str += "__parent__.appendChild(__temp__);";

    return true;
}

bool WebUtils::deleteObject(int uid, std::string& str)
{
    str = "var __child__ = " + getElementById(uid);
    str += "__child__.parentNode.removeChild(__child__);";

    return true;
}

bool WebUtils::updateDefaultProperties(int uid, std::string& update)
{
    std::string str;

    //visible
    setVisible(uid, str);

    //backgoundcolor
    setUIBackgroundColor(uid, str, true);

    //enable
    setUIEnable(uid, str, true);

    //fontangle
    setUIFontAngle(uid, str, true);

    //fontname
    setUIFontName(uid, str, true);

    //fontsize & fontunits
    setUIFontSize(uid, str, true);

    //fontweight
    setUIFontWeight(uid, str, true);

    //foregroundcolor
    setUIForegroundColor(uid, str, true);

    //horizontalalignment
    setUIHorizontalAlignment(uid, str, true);

    //relief ?
    setUIRelief(uid, str, true);

    //string
    setUIString(uid, str, true);

    //verticalalignment
    setUIVerticalAlignment(uid, str, true);

    //position
    setUIPosition(uid, str, true);

    //layout ?
    setUILayout(uid, str, true);

    //min
    setUIMin(uid, str, true);

    //max
    setUIMax(uid, str, true);

    //step
    setUIStep(uid, str, true);

    //vaue
    setUIValue(uid, str, true);

    //set callback uses to update values from web view
    setCallback(uid, str, true);

    update += str;

    return true;
}

bool WebUtils::set(int prop, int uid, std::string& str, bool append)
{
    SETTER::iterator it = setter.find(prop);
    if (it != setter.end())
    {
        it->second(uid, str, append);

        return true;
    }

    return false;
}

void WebUtils::fillSetter()
{
    setter[__GO_PARENT__] = WebUtils::setParent;
    setter[__GO_POSITION__] = WebUtils::setUIPosition;
    setter[__GO_SIZE__] = WebUtils::setFigureSize;
    setter[__GO_UI_STRING__] = WebUtils::setUIString;
    setter[__GO_VISIBLE__] = WebUtils::setVisible;
    setter[__GO_UI_ENABLE__] = WebUtils::setUIEnable;
    setter[__GO_UI_BACKGROUNDCOLOR__] = WebUtils::setUIBackgroundColor;
    setter[__GO_UI_FONTANGLE__] = WebUtils::setUIFontAngle;
    setter[__GO_UI_FONTNAME__] = WebUtils::setUIFontName;
    setter[__GO_UI_FONTSIZE__] = WebUtils::setUIFontSize;
    setter[__GO_UI_FONTWEIGHT__] = WebUtils::setUIFontWeight;
    setter[__GO_UI_FOREGROUNDCOLOR__] = WebUtils::setUIForegroundColor;
    setter[__GO_UI_HORIZONTALALIGNMENT__] = WebUtils::setUIHorizontalAlignment;
    setter[__GO_UI_RELIEF__] = WebUtils::setUIRelief;
    setter[__GO_UI_VERTICALALIGNMENT__] = WebUtils::setUIVerticalAlignment;
    setter[__GO_LAYOUT__] = WebUtils::setUILayout;
    setter[__GO_UI_MIN__] = WebUtils::setUIMin;
    setter[__GO_UI_MAX__] = WebUtils::setUIMax;
    setter[__GO_UI_SLIDERSTEP__] = WebUtils::setUIStep;
    setter[__GO_UI_VALUE__] = WebUtils::setUIValue;
    //preferred size is the last property to be set for border constraints
    setter[__GO_UI_BORDER_PREFERREDSIZE__] = WebUtils::setUIBorder;
    //preferred size is the last property to be set for gridbag constraints
    setter[__GO_UI_GRIDBAG_PREFERREDSIZE__] = WebUtils::setUIGridBag;
    //setter[__GO_CALLBACK__] = WebUtils::setCallback;
}

bool WebUtils::updateValue(int uid, const std::string& value)
{
    if (hasStyle(uid, __GO_UI_EDIT__))
    {
        std::vector<std::string> v(1, value);
        setStringVectorProperty(uid, __GO_UI_STRING__, v);
    }

    return false;
}

bool WebUtils::updateValue(int uid, const std::vector<double>& values)
{
    setDoubleVectorProperty(uid, __GO_UI_VALUE__, values);
    return false;
}

bool WebUtils::updateValue(int uid, double value)
{
    std::vector<double> v(1, value);
    setDoubleVectorProperty(uid, __GO_UI_VALUE__, v);
    return false;
}

bool WebUtils::updateValue(int uid, bool value)
{
    std::vector<double> v(1, value ? getUIMax(uid) : getUIMin(uid));
    setDoubleVectorProperty(uid, __GO_UI_VALUE__, v);
    return false;
}

void WebUtils::addInWaitingQueue(int uid, int prop)
{
    waitprop[uid].push_back(prop);
}

void WebUtils::setWaitingProperties(int uid, std::string& str, bool append)
{
    WAITING_PROP::iterator it = waitprop.find(uid);
    if (it != waitprop.end())
    {
        for (int prop : it->second)
        {
            set(prop, uid, str, append);
        }
        waitprop.erase(it);
    }
}
