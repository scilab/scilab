// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) INRIA
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt

function f=%s_q_r(s,f)
    // %s_q_r(s,f)  f= s./f
    //!

    if size(s,"*")==0 then f=[],return,end
    f=rlist(f("num")./s,f("den").*ones(s),f("dt")),
endfunction
