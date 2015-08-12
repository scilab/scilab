/*
* Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
* Copyright (C) 2009 - DIGITEO - Allan CORNET
*
* This file must be used under the terms of the CeCILL.
* This source file is licensed as described in the file COPYING, which
* you should have received as part of this distribution.  The terms
* are also available at
* http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
*
*/

#ifndef __INITTYPENAMES_H__
#define __INITTYPENAMES_H__

#include "machine.h"

/**
* initialize hard coded type names
* see stack-c.h for defined types
* called only by initsci.f
*/
int C2F(inittypenames)();

#endif /* __INITTYPENAMES_H__ */