//<-- CLI SHELL MODE -->
// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2007-2008 - INRIA - Serge STEER <serge.steer@inria.fr>
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- Non-regression test for bug 2396 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=2396
//
// <-- Short Description -->
//    Scilab crashes on call to feval. I might have defined a weird function
//    to be feval'ed, but scilab should not crash. The steps to reproduce the
//    bug cause scilab to hang with the error message below, I have to kill
//    it.
//
//    Matthias

a1 = [1 2 3];
a2 = [1 3 4 5 1 6 7];

function y=elfind(x)
    y = (find(a2 == x) ~= []);
endfunction

if execstr("z=feval(a1,elfind)","errcatch")<>999 then pause,end
