// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) INRIA
// Copyright (C) 2015 - Samuel GOUGEON
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt

function r = %r_o_r(l1, l2)
    r = ((l1.num == l2.num  & l1.den==l2.den)   | ..
    (l1.num == -l2.num & l1.den==-l2.den)       | ..
    (l1.num==0 & l2.num==0)) .. // when simp_mode(%f)
    & (l1.dt==l2.dt)
endfunction
