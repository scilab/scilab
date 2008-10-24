// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) INRIA
// 
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at    
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

function y=erfcx(x)
//erfcx  -  The scaled complementary error function.
//%Syntax
// y = erf(x).
//%Parameters
// x : real vector
// y : real vector (of same size)
//%Description
// erf computes the scaled complementary error function:
//
//      y = exp(x^2) * erfc(x) ~ (1/sqrt(pi)) * 1/x for large x.
//%See also 
// erfc erf calerf

y=calerf(x,2)
endfunction
