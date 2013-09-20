// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) INRIA
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt

function r=%r_o_r(l1,l2)
    // l1==l2 with l1 and l2 rationals
    //!
    //r=l1('dt')==l2('dt')&l1('num')==l2('num')&(l1('den')==l2('den')|l1('num')==0)
    r = and((l1.num == l2.num))&and((l1.den==l2.den))&(l1.dt==l2.dt);
    
//    if varn([l1("num"),l1("den")])~=varn([l2("num"),l2("den")]) then
//        r=%f;
//    else
//        r=(l1("dt")==l2("dt"))&(numer(l1-l2)==0)
//    end
endfunction
