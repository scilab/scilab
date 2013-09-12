// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) INRIA
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt

function r=%r_d_s(r,m)
    // r=r./m
    //!

    if size(m,"*")==0 then r=[],return,end
    r=rlist(r.num./m,r.den.*ones(m),r.dt)
endfunction
