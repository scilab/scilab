/*
* Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
* Copyright (C) 2009 - DIGITEO - Allan CORNET
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
