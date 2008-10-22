#ifndef __CALLJXCLICK_H__
#define __CALLJXCLICK_H__
/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2007 - INRIA - Vincent COUVERT
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

void CallJxclick(void);

int getJxclickMouseButtonNumber(void);

double getJxclickXCoordinate(void);

double getJxclickYCoordinate(void);

int getJxclickWindowID(void);

char * getJxclickMenuCallback(void);

/**
 * Delete the menu call back which is allocated by a C++ new.
 */
void deleteMenuCallBack(char * menuCallBack);

#endif

