// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) INRIA
// 
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at    
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

function y=erfc(x)
//erfc  -  The complementary error function.
//%Syntax
// y = erf(x).
//%Parameters
// x : real vector
// y : real vector (of same size)
//%Description
// erf computes the complementary error function:
//
//                      /inf
//      y = 2/sqrt(pi) *|  exp(-t^2) dt
//                      /x
//      y = 1 - erf(x)
//%See also 
// erf erfcx calerf
//!

y=calerf(x,1)
endfunction
