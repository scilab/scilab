//<-- CLI SHELL MODE -->
// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2005-2008 - INRIA - Serge STEER <serge.steer@inria.fr>
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- Non-regression test for bug 3164 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=3164
//
// <-- Short Description -->
//If varargout is not a list, Scilab produces strange error messages or crashes


function varargout = test1();
    s.a=1;s.b=3;varargout = s
endfunction
if execstr("test1()","errcatch") == 0 then pause,end

function varargout = test2();
    varargout = 1
endfunction
if execstr("test2()","errcatch") == 0 then pause,end

