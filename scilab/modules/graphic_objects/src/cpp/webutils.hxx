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

class WebUtils
{
    WebUtils() {}
    ~WebUtils() {}

private:

    static bool hasStyle(int uid, int style);

    static std::string getIdString(int uid);
    static std::string getElementById(int uid);
    static std::string createElement(const std::string& type);

public:

    static int getIntProperty(int uid, int prop);
    static double getDoubleProperty(int uid, int prop);
    static std::string getStringProperty(int uid, int prop);

    static void getIntVectorProterty(int uid, int prop, std::vector<int>& vect);
    static void getDoubleVectorProterty(int uid, int prop, std::vector<double>& vect);
    static void  getStringVectorProperty(int uid, int prop, std::vector<std::string>& vect);


    //commoms
    static int getType(int uid);
    static int getStyle(int uid);
    static int getParent(int uid);
    static void setParent(int uid, std::string& str);
    static void getUIPosition(int uid, std::vector<double>& vect);
    static void setUIPosition(int uid, std::string& str);

    static void getFigureSize(int uid, std::vector<int>& vect);
    static void setFigureSize(int uid, std::string& str);

    static void getUIString(int uid, std::vector<std::string>& vect);
    static void setUIString(int uid, std::string& str);

    //is
    static bool isManaged(int uid);
    static bool isFigure(int uid);
    static bool isUIcontrol(int uid);
    static bool isButton(int uid);
    
    //create
    static bool createFigure(int uid, std::string& str);
    static bool createUIControl(int uid, std::string& str);
    static bool createPushButton(int uid, std::string& str);

    //delete


    //cupdate
};
#endif /* !__WEB_UTILS_HXX__ */

