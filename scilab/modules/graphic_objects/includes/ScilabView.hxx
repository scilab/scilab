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

#ifndef __SCILAB_VIEW_HXX__
#define __SCILAB_VIEW_HXX__

extern "C"
{
    void ScilabNativeView__createObject(char* pstId);
    void ScilabNativeView__deleteObject(char* pstId);
    void ScilabNativeView__updateObject(char* pstId, char* pstProperty);
}

class ScilabView
{
private :
    ScilabView() {}
    ~ScilabView() {}
    struct cmp_str
    {
        bool operator()(char const *a, char const *b)
        {
            return std::strcmp(a, b) < 0;
        }
    };
    static std::map<char*,int, cmp_str> figureList;

public :
    static void createObject(char* pstId);
    static void deleteObject(char* pstId);
    static void updateObject(char* pstId, char* pstProperty);
    static int  getNbFigure(void);
    static void getFiguresId(int ids[]);
    static void registerToController(void);
};

#endif /* !__SCILAB_VIEW_HXX__ */

