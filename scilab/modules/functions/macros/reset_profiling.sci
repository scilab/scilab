// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2003-2008 - INRIA - Serge STEER <serge.steer@inria.fr>
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

function reset_profiling(funname)
    //reset profiling data in bytecode
    if argn(2) < 1 then
        error(sprintf(_("%s: Wrong number of input argument(s): %d expected.\n"), "reset_profiling", 1));
    end
    nsiz=6
    execstr("code=bytecode("+funname+")")
    lc=1
    lc = lc + nsiz*double(code(lc)) + 1
    lc = lc + nsiz*double(code(lc)) + 1
    long=code(lc)
    lc = lc+1
    c=code(lc:$)
    c1=bytecodewalk(c,25,resetprof)
    code=[code(1:lc-2) int32(size(c1,"*")) c1]
    execstr(funname+" = resume(bytecode(code))")
endfunction
function [c,l]=resetprof(l)
    //reset profiling data in bytecode
    c=int32([25 0 0]);l=l+3;
endfunction
