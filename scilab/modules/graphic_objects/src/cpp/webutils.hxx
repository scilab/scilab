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

#ifndef __WEB_UTILS_HXX__
#define __WEB_UTILS_HXX__

#include <string>
#include <vector>
#include <list>
#include <unordered_map>

typedef void(*setFunction)(int, std::string&, bool);

class WebUtils
{
    WebUtils() {}
    ~WebUtils() {}

    typedef std::unordered_map<int, setFunction> SETTER;
    static SETTER setter;

    typedef std::unordered_map<int, std::list<int>> WAITING_PROP;
    static WAITING_PROP waitprop;

    static bool hasStyle(int uid, int style);
    static bool isInputType(int uid);

    static std::string getIdString(int uid, const std::string& suffix = "");
    static std::string getElementById(int uid, const std::string& suffix = "");
    static std::string createElement(const std::string& type);

    static std::string getColor(const std::vector<double>& c);

    static std::string getSubPadding(int val);
    static std::string getPadding(int t, int r, int b, int l);
public:

    static bool hasValidParent(int uid);
    static void addInWaitingQueue(int uid, int prop);
    static void setWaitingProperties(int uid, std::string& str, bool append);

    static void fillSetter();
    static bool set(int prop, int uid, std::string& str, bool append = false);

    static bool updateValue(int uid, bool value);
    static bool updateValue(int uid, double value);
    static bool updateValue(int uid, const std::string& value);
    static bool updateValue(int uid, const std::vector<double>& values);
    static int getIntProperty(int uid, int prop);
    static bool getBoolProperty(int uid, int prop);
    static double getDoubleProperty(int uid, int prop);
    static std::string getStringProperty(int uid, int prop);

    static void getIntVectorProterty(int uid, int prop, std::vector<int>& vect);
    static void getDoubleVectorProterty(int uid, int prop, std::vector<double>& vect);
    static void  getStringVectorProperty(int uid, int prop, std::vector<std::string>& vect);

    static bool setStringProperty(int uid, int prop, const std::string& value);
    static bool setStringVectorProperty(int uid, int prop, const std::vector<std::string>& values);

    static bool setDoubleProperty(int uid, int prop, double value);
    static bool setDoubleVectorProperty(int uid, int prop, const std::vector<double>& values);


    //commoms
    static int getType(int uid);
    static int getStyle(int uid);
    static int getParent(int uid);
    static int getFigureId(int uid);
    static void setParent(int uid, std::string& str, bool append = false);
    static void getUIPosition(int uid, std::vector<double>& vect);
    static void setUIPosition(int uid, std::string& str, bool append = false);

    static void getFigureSize(int uid, std::vector<int>& vect);
    static void setFigureSize(int uid, std::string& str, bool append = false);

    static void getUIString(int uid, std::vector<std::string>& vect);
    static void setUIString(int uid, std::string& str, bool append = false);

    static bool getVisible(int uid);
    static void setVisible(int uid, std::string& str, bool append = false);

    static bool getUIEnable(int uid);
    static void setUIEnable(int uid, std::string& str, bool append = false);

    static bool getUIBackgroundColor(int uid, std::vector<double>& vect);
    static void setUIBackgroundColor(int uid, std::string& str, bool append = false);

    static bool getUIFontAngle(int uid, std::string& str);
    static void setUIFontAngle(int uid, std::string& str, bool append = false);

    static bool getUIFontName(int uid, std::string& str);
    static void setUIFontName(int uid, std::string& str, bool append = false);

    static bool getUIFontUnits(int uid, std::string& str);
    static double getUIFontSize(int uid);
    static void setUIFontSize(int uid, std::string& str, bool append = false);

    static bool getUIFontWeight(int uid, std::string& str);
    static void setUIFontWeight(int uid, std::string& str, bool append = false);

    static bool getUIForegroundColor(int uid, std::vector<double>& vect);
    static void setUIForegroundColor(int uid, std::string& str, bool append = false);

    static bool getUIHorizontalAlignment(int uid, std::string& str);
    static void setUIHorizontalAlignment(int uid, std::string& str, bool append = false);

    static bool getUIRelief(int uid, std::string& str);
    static void setUIRelief(int uid, std::string& str, bool append = false);

    static bool getUIVerticalAlignment(int uid, std::string& str);
    static void setUIVerticalAlignment(int uid, std::string& str, bool append = false);

    static int getUILayout(int uid);
    static void setUILayout(int uid, std::string& str, bool append = false);

    static double getUIMin(int uid);
    static void setUIMin(int uid, std::string& str, bool append = false);

    static double getUIMax(int uid);
    static void setUIMax(int uid, std::string& str, bool append = false);

    static double getUIStep(int uid);
    static void setUIStep(int uid, std::string& str, bool append = false);

    static bool getUIValue(int uid, std::vector<double>& vect);
    static void setUIValue(int uid, std::string& str, bool append = false);

    //border layout
    static int getUIBorderPosition(int uid);
    static void getUIBorderPadding(int uid, std::vector<int>& vect);
    static void getUIBorderPreferredSize(int uid, std::vector<int>& vect);
    static void setUIBorder(int uid, std::string& str, bool append = false);

    //gridbag layout
    static void getUIGridBagGrid(int uid, std::vector<int>& vect);
    static void setUIGridBag(int uid, std::string& str, bool append = false);

    static bool hasCallback(int uid);
    static void setCallback(int uid, std::string& str, bool append = false);

    //is
    static bool isManaged(int uid);
    static bool isFigure(int uid);
    static bool isUIcontrol(int uid);
    static bool isButton(int uid);

    //create
    static bool createFigure(int uid, std::string& str);

    static bool createCommonIUControl(int uid, const std::string& htmlType, const std::string& cssClass, std::string& str);
    static bool createUIControl(int uid, std::string& str);
    static bool createPushButton(int uid, std::string& str);
    static bool createFrame(int uid, std::string& str);
    static bool createText(int uid, std::string& str);
    static bool createEdit(int uid, std::string& str);
    static bool createCheckbox(int uid, std::string& str);
    static bool createRadio(int uid, std::string& str);
    static bool createSlider(int uid, std::string& str);
    static bool createListbox(int uid, std::string& str);
    static bool createCombobox(int uid, std::string& str);
    static bool createSpinner(int uid, std::string& str);

    //update
    static bool updateDefaultProperties(int uid, std::string& str);
    //delete
    static bool deleteObject(int uid, std::string& str);
};
#endif /* !__WEB_UTILS_HXX__ */

