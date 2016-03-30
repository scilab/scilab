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

#ifdef _MSC_VER
#include <io.h>
#endif
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
#include "FrameBorderType.h"
#include "copyfile.h"
#include "charEncoding.h"
#include "sci_malloc.h"
}

WebUtils::SETTER WebUtils::setter;
WebUtils::WAITING_PROP WebUtils::waitprop;
std::string WebUtils::imagePath;

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

void WebUtils::setImagePath(const std::string& path)
{
    imagePath = path;
}

std::string WebUtils::getImagePath()
{
    return imagePath;
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

void WebUtils::setParent(int uid, std::ostringstream& ostr)
{
    int parent = getParent(uid);

    ostr << "setParent(" << uid << ", " << parent << ");";

    if (hasStyle(parent, __GO_UI_TAB__))
    {
        //add button and properties for tab
        addTabChild(parent, uid, ostr);
    }

    if (getUILayout(parent) == LAYOUT_BORDER && hasStyle(parent, __GO_UI_TAB__) == false)
    {
        //force update of border position, especially for center
        addInWaitingQueue(uid, __GO_UI_BORDER_POSITION__);
    }

    setWaitingProperties(uid, ostr);
}

void WebUtils::getFigureSize(int uid, std::vector<int>& vect)
{
    int fig = getFigureId(uid);
    vect.resize(2, 0);
    getIntVectorProterty(fig, __GO_AXES_SIZE__, vect);
}

void WebUtils::setFigureSize(int uid, std::ostringstream& ostr)
{
    std::vector<int> size;
    getFigureSize(uid, size);

    ostr << "setFigureSize('" << uid << "', [" << size[0] << ", " << size[1] << "]);";
}

void WebUtils::getFigureName(int uid, std::string& val)
{
    val = getStringProperty(uid, __GO_NAME__);
}

void WebUtils::setFigureName(int uid, std::ostringstream& ostr)
{
    std::string val;
    getFigureName(uid, val);

    ostr << "setFigureName('" << uid << "', '" << val << "');";
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

void WebUtils::getUIUnits(int uid, std::string& val)
{
    val = getStringProperty(uid, __GO_UI_UNITS__);
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


void WebUtils::setUIPosition(int uid, std::ostringstream& ostr)
{
    std::string units;
    getUIUnits(uid, units);
    std::vector<double> pos;
    getUIPosition(uid, pos);

    ostr << "setUIPosition('" << uid << "', [" << pos[0] << ", " << pos[1] << ", " << pos[2] << ", " << pos[3] << "], '" << units << "');";

    //to ensure vertical alignement, adapt style.line-height
    if (hasStyle(uid, __GO_UI_TEXT__) || hasStyle(uid, __GO_UI_CHECKBOX__) || hasStyle(uid, __GO_UI_RADIOBUTTON__))
    {
        std::string v;
        getUIVerticalAlignment(uid, v);
        std::string value;
        if (v == "middle")
        {
            value = std::to_string((int)pos[3]) + "px";
        }
        else
        {
            value = "initial";
        }

        ostr << "setUIVerticalAlignment('" << uid << "', '" << value << "');";
    }
}

void WebUtils::setUIString(int uid, std::ostringstream& ostr)
{
    std::vector<std::string> s;
    getUIString(uid, s);
    int size = (int)s.size();

    std::string strings = "[";
    if (size)
    {
        strings += ("'" + s[0] + "'");
        for (int i = 1; i < size; ++i)
        {
            strings += ", ";
            strings += ("'" + s[i] + "'");
        }
    }
    else
    {
        strings += "''";
    }

    strings += "]";

    int parent = getParent(uid);

    ostr << "setUIString('" << uid << "', " + strings + ", '" << parent << "');";
}

bool WebUtils::getVisible(int uid)
{
    return getBoolProperty(uid, __GO_VISIBLE__);
}

void WebUtils::setVisible(int uid, std::ostringstream& ostr)
{
    //reverse flag value
    std::string v = getVisible(uid) ? "inherit" : "hidden";
    ostr << "setVisible('" << uid << "', '" + v << "');";
}

bool WebUtils::getUIEnable(int uid)
{
    return getBoolProperty(uid, __GO_UI_ENABLE__);
}

void WebUtils::setUIEnable(int uid, std::ostringstream& ostr)
{
    //reverse flag value
    std::string v = getUIEnable(uid) ? "true" : "false";
    ostr << "setUIEnable('" << uid << "', " + v << ");";
}

bool WebUtils::getUIBackgroundColor(int uid, std::vector<double>& vect)
{
    vect.resize(3, 0.);
    getDoubleVectorProterty(uid, __GO_UI_BACKGROUNDCOLOR__, vect);
    return true;
}

void WebUtils::setUIBackgroundColor(int uid, std::ostringstream& ostr)
{
    std::vector<double> c;
    getUIBackgroundColor(uid, c);
    ostr << "setUIBackgroundColor('" << uid << "', " + getColor(c) << ");";
}

bool WebUtils::getUIFontAngle(int uid, std::string& val)
{
    val = getStringProperty(uid, __GO_UI_FONTANGLE__);
    return true;
}

void WebUtils::setUIFontAngle(int uid, std::ostringstream& ostr)
{
    std::string angle;
    getUIFontAngle(uid, angle);
    ostr << "setUIFontAngle('" << uid << "', '" + angle << "');";
}

bool WebUtils::getUIFontName(int uid, std::string& val)
{
    val = getStringProperty(uid, __GO_UI_FONTNAME__);
    return true;
}

void WebUtils::setUIFontName(int uid, std::ostringstream& ostr)
{
    std::string font;
    getUIFontName(uid, font);
    ostr << "setUIFontName('" << uid << "', '" + font << "');";
}

bool WebUtils::getUIFontUnits(int uid, std::string& val)
{
    val =  getStringProperty(uid, __GO_UI_FONTUNITS__);
    return true;
}

double WebUtils::getUIFontSize(int uid)
{
    return getDoubleProperty(uid, __GO_UI_FONTSIZE__);
}

void WebUtils::setUIFontSize(int uid, std::ostringstream& ostr)
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

    ostr << "setUIFontSize('" << uid << "', '" << size << units << "');";

}

bool WebUtils::getUIFontWeight(int uid, std::string& val)
{
    val = getStringProperty(uid, __GO_UI_FONTWEIGHT__);
    return true;
}

void WebUtils::setUIFontWeight(int uid, std::ostringstream& ostr)
{
    std::string weight;
    getUIFontWeight(uid, weight);
    ostr << "setUIFontWeight('" << uid << "', '" << weight << "');";
}

bool WebUtils::getUIForegroundColor(int uid, std::vector<double>& vect)
{
    vect.resize(3, 0.);
    getDoubleVectorProterty(uid, __GO_UI_FOREGROUNDCOLOR__, vect);
    return true;
}

void WebUtils::setUIForegroundColor(int uid, std::ostringstream& ostr)
{
    std::vector<double> c;
    getUIForegroundColor(uid, c);
    ostr << "setUIForegroundColor('" << uid << "', " + getColor(c) << ");";
}

bool WebUtils::getUIHorizontalAlignment(int uid, std::string& val)
{
    val = getStringProperty(uid, __GO_UI_HORIZONTALALIGNMENT__);
    return true;
}

void WebUtils::setUIHorizontalAlignment(int uid, std::ostringstream& ostr)
{
    std::string align;
    getUIHorizontalAlignment(uid, align);
    ostr << "setUIHorizontalAlignment('" << uid << "', '" + align << "');";
}

bool WebUtils::getUIRelief(int uid, std::string& val)
{
    val = getStringProperty(uid, __GO_UI_RELIEF__);
    return true;
}

void WebUtils::setUIRelief(int uid, std::ostringstream& ostr)
{
}

bool WebUtils::getUIVerticalAlignment(int uid, std::string& val)
{
    val = getStringProperty(uid, __GO_UI_VERTICALALIGNMENT__);
    return true;
}

void WebUtils::setUIVerticalAlignment(int uid, std::ostringstream& ostr)
{
    std::string align;
    getUIVerticalAlignment(uid, align);
    ostr << "setUIVerticalAlignment('" << uid << "', '" + align << "');";
}

int WebUtils::getUILayout(int uid)
{
    return getIntProperty(uid, __GO_LAYOUT__);
}

void WebUtils::setUILayout(int uid, std::ostringstream& ostr)
{
    if (isFigure(uid) || hasStyle(uid, __GO_UI_FRAME__))
    {
        //TODO clean previous layout before create new one
        ostr << "setUILayout('" << uid << "', " << getUILayout(uid) << ");";
    }
}

double WebUtils::getUIMin(int uid)
{
    return getDoubleProperty(uid, __GO_UI_MIN__);
}

void WebUtils::setUIMin(int uid, std::ostringstream& ostr)
{
    if (hasStyle(uid, __GO_UI_SPINNER__) || hasStyle(uid, __GO_UI_SLIDER__))
    {
        ostr << "setUIMin('" << uid << "', '" << getUIMin(uid) << "');";
    }
}

double WebUtils::getUIMax(int uid)
{
    return getDoubleProperty(uid, __GO_UI_MAX__);
}

void WebUtils::setUIMax(int uid, std::ostringstream& ostr)
{
    if (hasStyle(uid, __GO_UI_SPINNER__) || hasStyle(uid, __GO_UI_SLIDER__))
    {
        ostr << "setUIMax('" << uid << "', '" << getUIMax(uid) << "');";
    }
}

double WebUtils::getUIStep(int uid)
{
    std::vector<double> step(2, 0.);
    getDoubleVectorProterty(uid, __GO_UI_SLIDERSTEP__, step);
    return step[0];
}

void WebUtils::setUIStep(int uid, std::ostringstream& ostr)
{
    if (hasStyle(uid, __GO_UI_SPINNER__) || hasStyle(uid, __GO_UI_SLIDER__))
    {
        ostr << "setUIStep('" << uid << "', '" << getUIStep(uid) << "');";
    }
}

bool WebUtils::getUIValue(int uid, std::vector<double>& vect)
{
    int size = getIntProperty(uid, __GO_UI_VALUE_SIZE__);
    vect.resize(size, 0.);
    getDoubleVectorProterty(uid, __GO_UI_VALUE__, vect);
    return true;
}

void WebUtils::setUIValue(int uid, std::ostringstream& ostr)
{
    std::vector<double> values;
    getUIValue(uid, values);
    if (values.size() == 0)
    {
        values.push_back(0);
    }

    ostr << "setUIValue('" << uid << "', " << values[0] << ");";
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

void WebUtils::setUIBorder(int uid, std::ostringstream& ostr)
{
    std::vector<int> pad;
    std::vector<int> size;
    int parent = getParent(uid);
    int border = getUIBorderPosition(uid);
    getUIBorderPadding(parent, pad);
    getUIBorderPreferredSize(uid, size);

    ostr << "setUIBorder('" << uid << "', '" << parent << "', " << border << ", [";
    ostr << pad[0] << ", " << pad[1];
    ostr << "], [" << size[0] << ", " << size[1] << "]);";
}

void WebUtils::getUIGridBagGrid(int uid, std::vector<int>& vect)
{
    vect.resize(4, 0);
    getIntVectorProterty(uid, __GO_UI_GRIDBAG_GRID__, vect);
}


void WebUtils::setUIGridBag(int uid, std::ostringstream& ostr)
{
    int parent = getParent(uid);
    std::vector<int> grid;
    getUIGridBagGrid(uid, grid);

    ostr << "setUIGridBag('" << uid << "', '" << parent << "', ['";
    ostr << grid[0] << "', '" << grid[1] << "', '" << grid[2] << "', '" << grid[3] << "']);";
}

void WebUtils::setUIFrameBorder(int uid, std::ostringstream& ostr)
{
    std::string text;
    std::string position;
    std::string alignment;
    std::string fontName;
    std::string fontStyle;
    std::string fontSize;
    std::string fontWeight;
    std::string fontColor;

    int border = getIntProperty(uid, __GO_UI_FRAME_BORDER__);
    int style = getIntProperty(border, __GO_UI_FRAME_BORDER_STYLE__);
    int parent = getParent(uid);

    if (style == TITLED)
    {
        text = getStringProperty(border, __GO_TITLE__);
        int pos = getIntProperty(border, __GO_UI_FRAME_BORDER_POSITION__);
        switch (pos)
        {
            case 0:
            case 1:
            case 2:
                position = "top";
                break;
            case 3:
            case 4:
            case 5:
                position = "bottom";
                break;
        }

        int justification = getIntProperty(border, __GO_UI_FRAME_BORDER_JUSTIFICATION__);
        switch (justification)
        {
            default:
            case 1:
                alignment = "left";
                break;
            case 2:
                alignment = "center";
                break;
            case 3:
                alignment = "right";
                break;
        }

        //fontname
        fontName = getStringProperty(border, __GO_UI_FONTNAME__);

        //font style/angle
        fontStyle = getStringProperty(border, __GO_UI_FONTANGLE__);

        //fontSize
        int size = getIntProperty(border, __GO_UI_FONTSIZE__);
        fontSize = std::to_string(size);

        //fontWeight
        fontWeight = getStringProperty(border, __GO_UI_FONTWEIGHT__);

        //color
        fontColor = getStringProperty(border, __GO_UI_FRAME_BORDER_COLOR__);

    }

    //build struct
    std::string var = "{";
    var += "text:'" + text + "',";
    var += "position:'" + position + "',";
    var += "alignment:'" + alignment + "',";
    var += "fontName:'" + fontName + "',";
    var += "fontStyle:'" + fontStyle + "',";
    var += "fontSize:'" + fontSize + "',";
    var += "fontWeight:'" + fontWeight + "',";
    var += "fontColor:'" + fontColor + "'}";

    ostr << "setUIFrameBorder('" << uid << "', " << var << ");";
}

bool WebUtils::getUIIcon(int uid, std::string& val)
{
    val = getStringProperty(uid, __GO_UI_ICON__);
    return true;
}

void WebUtils::setUIIcon(int uid, std::ostringstream& ostr)
{
    std::string icon;
    getUIIcon(uid, icon);
    std::vector<std::string> str;
    getUIString(uid, str);
    if (str.empty())
    {
        str.push_back("");
    }

    size_t found = icon.find_last_of(".");
    std::string tmpname = icon;

    if (found != std::string::npos)
    {
        std::string ext = icon.substr(found);

        tmpname = std::tmpnam(nullptr);
        tmpname = "images/" + tmpname.substr(1) + ext;

        //copy image file to web server path
        wchar_t* src = to_wide_string(icon.data());
        wchar_t* dst = to_wide_string((getImagePath() + tmpname).data());

        CopyFileFunction(dst, src);
        FREE(src);
        FREE(dst);
    }
    ostr << "setUIIcon('" << uid << "', '" << tmpname << "', '" << str[0] << "');";
}

bool WebUtils::hasCallback(int uid)
{
    return getStringProperty(uid, __GO_CALLBACK__) != "";
}

void WebUtils::setCallback(int uid, std::ostringstream& ostr)
{
    ostr << "setCallback('" << uid << "');";
}

bool WebUtils::getUIGroupName(int uid, std::string& val)
{
    val = getStringProperty(uid, __GO_UI_GROUP_NAME__);
    return true;
}

void WebUtils::setUIGroupName(int uid, std::ostringstream& ostr)
{
    std::string val;
    getUIGroupName(uid, val);

    ostr << "setUIGroupName('" << uid << "', '" << val << "');";
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
bool WebUtils::createFigure(int uid, std::ostringstream& ostr)
{
    ostr << "__temp__ = createFigure('" << uid << "');";
    return true;
}

bool WebUtils::createUIControl(int uid, std::ostringstream& ostr)
{
    int style = getStyle(uid);
    switch (style)
    {
        case __GO_UI_FRAME__:
            return createFrame(uid, ostr);
        case __GO_UI_PUSHBUTTON__:
            return createPushButton(uid, ostr);
        case __GO_UI_TEXT__:
            return createText(uid, ostr);
        case __GO_UI_EDIT__:
            return createEdit(uid, ostr);
        case __GO_UI_CHECKBOX__:
            return createCheckbox(uid, ostr);
        case __GO_UI_RADIOBUTTON__:
            return createRadio(uid, ostr);
        case __GO_UI_SLIDER__:
            return createSlider(uid, ostr);
        case __GO_UI_LISTBOX__:
            return createListbox(uid, ostr);
        case __GO_UI_POPUPMENU__:
            return createCombobox(uid, ostr);
        case __GO_UI_SPINNER__:
            return createSpinner(uid, ostr);
        case __GO_UI_TAB__:
            return createTab(uid, ostr);
        default:
            return false;
    }
}

bool WebUtils::createPushButton(int uid, std::ostringstream& ostr)
{
    ostr << "var __temp__ = createPushButton(" + std::to_string(uid) + ");" << std::endl;
    return true;
}

bool WebUtils::createFrame(int uid, std::ostringstream& ostr)
{
    ostr << "var __temp__ = createFrame(" + std::to_string(uid) + ");" << std::endl;
    return true;
}

bool WebUtils::createText(int uid, std::ostringstream& ostr)
{
    ostr << "var __temp__ = createText(" + std::to_string(uid) + ");" << std::endl;
    return true;
}

bool WebUtils::createEdit(int uid, std::ostringstream& ostr)
{
    ostr << "var __temp__ = createEdit(" + std::to_string(uid) + ");" << std::endl;
    return true;
}

bool WebUtils::createCheckbox(int uid, std::ostringstream& ostr)
{
    ostr << "var __temp__ = createCheckbox(" + std::to_string(uid) + ");" << std::endl;
    return true;
}

bool WebUtils::createRadio(int uid, std::ostringstream& ostr)
{
    ostr << "var __temp__ = createRadio(" + std::to_string(uid) + ");" << std::endl;
    return true;
}

bool WebUtils::createSlider(int uid, std::ostringstream& ostr)
{
    ostr << "var __temp__ = createSlider(" + std::to_string(uid) + ");" << std::endl;
    return true;
}

bool WebUtils::createListbox(int uid, std::ostringstream& ostr)
{
    ostr << "var __temp__ = createListbox(" + std::to_string(uid) + ");" << std::endl;
    return true;
}

bool WebUtils::createCombobox(int uid, std::ostringstream& ostr)
{
    ostr << "var __temp__ = createCombobox(" + std::to_string(uid) + ");" << std::endl;
    return true;
}

bool WebUtils::createSpinner(int uid, std::ostringstream& ostr)
{
    ostr << "var __temp__ = createSpinner(" + std::to_string(uid) + ");" << std::endl;
    return true;
}

bool WebUtils::createTab(int uid, std::ostringstream& ostr)
{
    ostr << "var __temp__ = createTab(" << uid << ");" << std::endl;
    return true;
}

bool WebUtils::addTabChild(int uid, int child, std::ostringstream& ostr)
{
    ostr << "addTabHelper(" << uid << ", " << child << ");";
    setUIValue(uid, ostr);
    return true;
}


bool WebUtils::deleteObject(int uid, std::ostringstream& ostr)
{
    ostr << "deleteObject(" << uid << ");";
    return true;
}

bool WebUtils::updateDefaultProperties(int uid, std::ostringstream& ostr)
{
    //visible
    setVisible(uid, ostr);

    //backgoundcolor
    setUIBackgroundColor(uid, ostr);

    //enable
    setUIEnable(uid, ostr);

    //fontangle
    setUIFontAngle(uid, ostr);

    //fontname
    setUIFontName(uid, ostr);

    //fontsize & fontunits
    setUIFontSize(uid, ostr);

    //fontweight
    setUIFontWeight(uid, ostr);

    //foregroundcolor
    setUIForegroundColor(uid, ostr);

    //horizontalalignment
    setUIHorizontalAlignment(uid, ostr);

    //relief ?
    setUIRelief(uid, ostr);

    //string
    setUIString(uid, ostr);

    //verticalalignment
    setUIVerticalAlignment(uid, ostr);

    //position
    setUIPosition(uid, ostr);

    //layout ?
    setUILayout(uid, ostr);

    //min
    setUIMin(uid, ostr);

    //max
    setUIMax(uid, ostr);

    //step
    setUIStep(uid, ostr);

    //vaue
    setUIValue(uid, ostr);

    //set callback uses to update values from web view
    setCallback(uid, ostr);

    return true;
}

bool WebUtils::set(int prop, int uid, std::ostringstream& ostr)
{
    SETTER::iterator it = setter.find(prop);
    if (it != setter.end())
    {
        it->second(uid, ostr);

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
    setter[__GO_UI_BORDER_POSITION__] = WebUtils::setUIBorder;
    //preferred size is the last property to be set for gridbag constraints
    setter[__GO_UI_GRIDBAG_GRID__] = WebUtils::setUIGridBag;
    setter[__GO_UI_FRAME_BORDER__] = WebUtils::setUIFrameBorder;
    setter[__GO_UI_ICON__] = WebUtils::setUIIcon;
    setter[__GO_UI_GROUP_NAME__] = WebUtils::setUIGroupName;
    //setter[__GO_CALLBACK__] = WebUtils::setCallback;
}

bool WebUtils::updateValue(int uid, const std::string& value)
{
    if (hasStyle(uid, __GO_UI_EDIT__))
    {
        std::vector<std::string> v(1, value);
        setStringVectorProperty(uid, __GO_UI_STRING__, v);
    }

    return true;
}

bool WebUtils::updateValue(int uid, const std::vector<double>& values)
{
    setDoubleVectorProperty(uid, __GO_UI_VALUE__, values);
    return true;
}

bool WebUtils::updateValue(int uid, double value)
{
    std::vector<double> v(1, value);
    setDoubleVectorProperty(uid, __GO_UI_VALUE__, v);
    return true;
}

bool WebUtils::updateValue(int uid, bool value)
{
    std::vector<double> v(1, value ? getUIMax(uid) : getUIMin(uid));
    setDoubleVectorProperty(uid, __GO_UI_VALUE__, v);
    return true;
}

void WebUtils::addInWaitingQueue(int uid, int prop)
{
    waitprop[uid].push_back(prop);
}

void WebUtils::setWaitingProperties(int uid, std::ostringstream& ostr)
{
    WAITING_PROP::iterator it = waitprop.find(uid);
    if (it != waitprop.end())
    {
        for (int prop : it->second)
        {
            set(prop, uid, ostr);
        }
        waitprop.erase(it);
    }
}
