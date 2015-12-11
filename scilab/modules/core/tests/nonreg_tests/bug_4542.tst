//<-- CLI SHELL MODE -->
// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2005-2009 - INRIA - Serge Steer
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- Non-regression test for bug 4542 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=4542
//
// <-- Short Description -->
// unexpected  interpreter warning.

function foo1()
for i=1
 disp('abcdefghijklmonoprstuvwxyz')
end
endfunction 


function foo2()
while 1
 disp('abcdefghijklmonoprstuvwxyz')
end
endfunction 
