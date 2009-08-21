/*
* Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
* Copyright (C) 2009 - DIGITEO - Allan CORNET
* 
* This file must be used under the terms of the CeCILL.
* This source file is licensed as described in the file COPYING, which
* you should have received as part of this distribution.  The terms
* are also available at    
* http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
*
*/
/*--------------------------------------------------------------------------*/
#ifndef __GETDIARYDATE_HXX__
#define __GETDIARYDATE_HXX__

#include <string>

/**
* get current date and time
* @param[in]: format_mode
* _mode 0: UNIX EPOCH
* _mode 1: ISO_8601
* @return std::wstring date & time
*/
std::wstring getDiaryDate(int format_mode);

#endif /* __GETDIARYDATE_HXX__ */
/*--------------------------------------------------------------------------*/
