/*
* Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
* Copyright (C) 2011 - DIGITEO - Calixte DENIZET
*
* This file must be used under the terms of the CeCILL.
* This source file is licensed as described in the file COPYING, which
* you should have received as part of this distribution.  The terms
* are also available at
* http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
*
*/

/*------------------------------------------------------------------------*/
#ifndef __UI_DATA_C_H__
#define __UI_DATA_C_H__
/*------------------------------------------------------------------------*/
#ifdef __cplusplus
extern "C" {
#endif
    /*------------------------------------------------------------------------*/
    void putScilabVariable(char * name, char ** lines, int rows, int cols);
    char * getUnnamedVariable();
    /*------------------------------------------------------------------------*/
#ifdef __cplusplus
}
#endif
/*------------------------------------------------------------------------*/
#endif /* __UI_DATA_C_H__ */
/*------------------------------------------------------------------------*/
