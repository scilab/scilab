// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2003-2008 - INRIA - Serge STEER <serge.steer@inria.fr>
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt

function remove_profiling(funname)
    //remove all profiling instruction bytecode
    if argn(2) < 1 then
        error(sprintf(_("%s: Wrong number of input argument(s): %d expected.\n"), "remove_profiling", 1));
    end
    nsiz=6
    execstr("code=bytecode("+funname+")")
    lc=1
    lc = lc + nsiz*double(code(lc)) + 1
    lc = lc + nsiz*double(code(lc)) + 1
    long=code(lc)
    lc = lc+1
    c=code(lc:$)
    c1=bytecodewalk(c,25,rmprof)
    code=[code(1:lc-2) int32(size(c1,"*")) c1]
    execstr(funname+" = resume(bytecode(code))")
endfunction
function [c,l]=rmprof(l)
    //remove profiling instruction bytecode
    c=[];l=l+3;
endfunction
