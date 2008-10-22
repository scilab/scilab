// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2005-2008 - INRIA
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- Non-regression test for bug 1107 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=1107
//
// <-- Short Description -->
//    wrong use of = not detected in if conditions

//    the bug was due to a misinterpretation of syntax like (x=2) in
//    factor. It was treated as a named argument of a function instead of a
//    factor equivalent to (x==2)
warning('off');
x=5;
if (x = 4) then
	x=6;
end

ok=x==5;

if (execstr('x=5;z=2*(x=4);',"errcatch","n")<>0) | (z<>0) then pause,end
if (execstr('x=5;z=2*(x-1=4);',"errcatch","n")<>0) | (z<>2) then pause,end
warning('on');
