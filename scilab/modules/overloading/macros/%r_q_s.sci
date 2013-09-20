// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) INRIA
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt

function f=%r_q_s(f1,n2)
    //  r.\m
    //!

    if size(n2,"*")==0 then f=[],return,end
    f=rlist(f1("den").*n2,f1("num").*ones(n2),f1("dt"))
endfunction
