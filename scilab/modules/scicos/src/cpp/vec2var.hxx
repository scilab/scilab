/*
*  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
*  Copyright (C) 2015 - Scilab Enterprises - Paul Bignier
*
*  This file must be used under the terms of the CeCILL.
*  This source file is licensed as described in the file COPYING, which
*  you should have received as part of this distribution.  The terms
*  are also available at
*  http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
*
*/

#ifndef VEC2VAR_HXX_
#define VEC2VAR_HXX_

#include <vector>

#include "types.hxx"
#include "internal.hxx"

bool vec2var(const std::vector<double> in, types::InternalType* &out);

#endif /* VEC2VAR_HXX_ */
