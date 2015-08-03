/*
* Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
* Copyright (C) 2015 - Scilab Enterprises - Cedric Delamarre
*
* This file must be used under the terms of the CeCILL.
* This source file is licensed as described in the file COPYING, which
* you should have received as part of this distribution.  The terms
* are also available at
* http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
*
*/

#ifndef __READWRITE_H__
#define __READWRITE_H__

#include "internal.hxx"

types::InternalType::ScilabType checkformat(const char* format);
void closeFile(types::InternalType* _pIT, int _iID);


#endif /* !__READWRITE_H__ */
