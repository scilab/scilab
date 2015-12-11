// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) Samuel GOUGEON <sgougeon@free.fr>
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt

function  rep = %r_varn(p, varname)
    if argn(2)==1 then
        rep = varn(p("num"));
    else
        rep = rlist(varn(p.num, varname), varn(p.den, varname), p.dt);
    end
endfunction
