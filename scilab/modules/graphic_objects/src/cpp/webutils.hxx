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
#include <sstream>

typedef void(*setFunction)(int, std::ostringstream&);

class WebUtils
{
    WebUtils() {}
    ~WebUtils() {}

    typedef std::unordered_map<int, setFunction> SETTER;
    static SETTER setterUIControl;
    static SETTER setterFigure;
    static SETTER setterAxes;

    typedef std::unordered_map<int, std::list<int>> WAITING_PROP;
    static WAITING_PROP waitprop;

    static bool hasStyle(int uid, int style);
    static bool isInputType(int uid);

    static std::string getColor(const std::vector<double>& c);

    static std::string imagePath;
public:

    static void setImagePath(const std::string& path);
    static std::string getImagePath();

    static bool hasValidParent(int uid);
    static void addInWaitingQueue(int uid, int prop);
    static void setWaitingProperties(int uid, std::ostringstream& ostr);

    static void fillSetters();
    static bool setUIControlProperty(int prop, int uid, std::ostringstream& ostr);
    static bool setFigureProperty(int prop, int uid, std::ostringstream& ostr);
    static bool setAxesProperty(int prop, int uid, std::ostringstream& ostr);
    static bool setProperty(const SETTER& setter, int prop, int uid, std::ostringstream& ostr);

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
    static void setParent(int uid, std::ostringstream& ostr);
    static void getUIUnits(int uid, std::string& val);
    static void getUIPosition(int uid, std::vector<double>& vect);
    static void setUIPosition(int uid, std::ostringstream& ostr);

    static void getFigureSize(int uid, std::vector<int>& vect);
    static void setFigureSize(int uid, std::ostringstream& ostr);

    static void getFigureName(int uid, std::string& val);
    static void setFigureName(int uid, std::ostringstream& ostr);

    static void getUIString(int uid, std::vector<std::string>& vect);
    static void setUIString(int uid, std::ostringstream& ostr);

    static bool getVisible(int uid);
    static void setVisible(int uid, std::ostringstream& ostr);

    static bool getUIEnable(int uid);
    static void setUIEnable(int uid, std::ostringstream& ostr);

    static bool getUIBackgroundColor(int uid, std::vector<double>& vect);
    static void setUIBackgroundColor(int uid, std::ostringstream& ostr);

    static bool getUIFontAngle(int uid, std::string& val);
    static void setUIFontAngle(int uid, std::ostringstream& ostr);

    static bool getUIFontName(int uid, std::string& val);
    static void setUIFontName(int uid, std::ostringstream& ostr);

    static bool getUIFontUnits(int uid, std::string& val);
    static double getUIFontSize(int uid);
    static void setUIFontSize(int uid, std::ostringstream& ostr);

    static bool getUIFontWeight(int uid, std::string& val);
    static void setUIFontWeight(int uid, std::ostringstream& ostr);

    static bool getUIForegroundColor(int uid, std::vector<double>& vect);
    static void setUIForegroundColor(int uid, std::ostringstream& ostr);

    static bool getUIHorizontalAlignment(int uid, std::string& val);
    static void setUIHorizontalAlignment(int uid, std::ostringstream& ostr);

    static bool getUIRelief(int uid, std::string& val);
    static void setUIRelief(int uid, std::ostringstream& ostr);

    static bool getUIVerticalAlignment(int uid, std::string& val);
    static void setUIVerticalAlignment(int uid, std::ostringstream& ostr);

    static int getUILayout(int uid);
    static void setUILayout(int uid, std::ostringstream& ostr);

    static double getUIMin(int uid);
    static void setUIMin(int uid, std::ostringstream& ostr);

    static double getUIMax(int uid);
    static void setUIMax(int uid, std::ostringstream& ostr);

    static double getUIStep(int uid);
    static void setUIStep(int uid, std::ostringstream& ostr);

    static bool getUIValue(int uid, std::vector<double>& vect);
    static void setUIValue(int uid, std::ostringstream& ostr);

    static bool getUIGroupName(int uid, std::string& val);
    static void setUIGroupName(int uid, std::ostringstream& ostr);

    //border layout
    static int getUIBorderPosition(int uid);
    static void getUIBorderPadding(int uid, std::vector<int>& vect);
    static void getUIBorderPreferredSize(int uid, std::vector<int>& vect);
    static void setUIBorder(int uid, std::ostringstream& ostr);

    //gridbag layout
    static void getUIGridBagGrid(int uid, std::vector<int>& vect);
    static void setUIGridBag(int uid, std::ostringstream& ostr);

    //static void getUIFrameBorder(int uid, std::vector<int>& vect);
    static void setUIFrameBorder(int uid, std::ostringstream& ostr);

    static bool getUIIcon(int uid, std::string& val);
    static void setUIIcon(int uid, std::ostringstream& ostr);

    static bool hasCallback(int uid);
    static void setCallback(int uid, std::ostringstream& ostr);

    //is
    static bool isManaged(int uid);
    static bool isFigure(int uid);
    static bool isUIcontrol(int uid);
    static bool isButton(int uid);

    //create
    static bool createFigure(int uid, std::ostringstream& ostr);

    static bool createAxes(int uid, std::ostringstream& ostr);

    static bool createUIControl(int uid, std::ostringstream& ostr);
    static bool createPushButton(int uid, std::ostringstream& ostr);
    static bool createFrame(int uid, std::ostringstream& ostr);
    static bool createText(int uid, std::ostringstream& ostr);
    static bool createEdit(int uid, std::ostringstream& ostr);
    static bool createCheckbox(int uid, std::ostringstream& ostr);
    static bool createRadio(int uid, std::ostringstream& ostr);
    static bool createSlider(int uid, std::ostringstream& ostr);
    static bool createListbox(int uid, std::ostringstream& ostr);
    static bool createCombobox(int uid, std::ostringstream& ostr);
    static bool createSpinner(int uid, std::ostringstream& ostr);
    static bool createTab(int uid, std::ostringstream& ostr);
    static bool addTabChild(int uid, int child, std::ostringstream& ostr);

    //update
    static bool updateDefaultProperties(int uid, std::ostringstream& ostr);
    //delete
    static bool deleteObject(int uid, std::ostringstream& ostr);
    static void deleteGraphicObject(int uid);
};
#endif /* !__WEB_UTILS_HXX__ */

