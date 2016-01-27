// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2008 - INRIA
//
// Copyright (C) 2012 - 2016 - Scilab Enterprises
//
// This file is hereby licensed under the terms of the GNU GPL v2.0,
// pursuant to article 5.3.4 of the CeCILL v.2.1.
// This file was originally licensed under the terms of the CeCILL v2.1,
// and continues to be available under such terms.
// For more information, see the COPYING file which you should have received
// along with this program.
// === LICENSE_END ===

function [flist,compiled,profilable,called]=listfunctions(scope)
    if ~exists("scope","local") then scope="local"; end
    nam=who(scope)'
    called=uint32(zeros(nam)); afun=(called==1); pfun=afun; cfun=pfun;
    for i=1:size(nam,2)
        clear rvar lst;
        // rvar is cleared to avoid function redefinition warning
        // lst (topmost, variable size) is cleared to speed up garbage collection
        if scope=="global" then global(nam(i)); end
        execstr("rvar="+nam(i));
        if type(rvar)==11 then afun(i)=%t; end
        if type(rvar)==13 then
            afun(i)=%t; cfun(i)=%t;
            lst=macr2lst(rvar)
            pfun(i)=and(lst(5)(1)=="25")
            if pfun(i) then execstr("called(i)="+lst(5)(2)); end
        end
    end
    flist=nam(afun)
    compiled=cfun(afun)
    profilable=pfun(afun)
    called=called(afun)
endfunction
