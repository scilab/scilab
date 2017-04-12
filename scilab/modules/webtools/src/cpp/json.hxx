/*
*  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
*  Copyright (C) 2016 - Scilab Enterprises - Antoine ELIAS
*
* Copyright (C) 2012 - 2016 - Scilab Enterprises
*
*/

#ifndef __JSON_HXX__
#define __JSON_HXX__

#include "api_scilab.h"
#include "internal.hxx"
#include "dynlib_webtools.h"

//api C++
WEBTOOLS_IMPEXP std::string toJSON(types::InternalType* it, int indent = 0);
WEBTOOLS_IMPEXP types::InternalType* fromJSON(const std::string& s);

//api C
WEBTOOLS_IMPEXP std::string toJSON(scilabEnv env, scilabVar var, int indent = 0);
WEBTOOLS_IMPEXP scilabVar fromJSON(scilabEnv env, const std::string& s);

#endif /* !__JSON_HXX__ */