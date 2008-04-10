// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) ????-2008 - INRIA
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- Non-regression test for bug 610 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=610
//
// <-- Short Description -->
//    In a function with a varargout list I want to call another
//    function having a varargout list.  How should I code it
//    (this is simple in Matlab)?    The following is my example
//    which does not work.  Any suggestions?

function varargout = mysum(a, b)
	varargout = list(a + b, a - b, a * b);
endfunction

if execstr('function [y, varargout] = MyNew(a,b) ..
   [y, varargout] = mysum(a,b) ..
   endfunction' ..
,'errcatch' ) <> 37 then pause,end
