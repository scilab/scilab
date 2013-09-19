// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) INRIA
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt

function f=%r_l_s(f,m)
    //  f\m
    //!

    if size(f("num"),"*")<>1 then
        f=invr(f)*m
    else
        f=rlist(m*f("den"),ones(m)*f("num"),f("dt"))
    end
endfunction
