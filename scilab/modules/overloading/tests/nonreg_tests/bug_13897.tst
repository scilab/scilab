// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2016 - Samuel GOUGEON
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================
//<-- CLI SHELL MODE -->
//
// <-- Non-regression test for bug 13897 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=13897
//
// <-- Short Description -->
// 2 arrays of structures with same fields but in different order
//   could not be concatenated
clear p q
p(1,1).a = %pi;
p(1,1).b = %z;
p(1,2).b = (1-%z)^2;

q.b = %s;
q.a = %e;
try
    [p q];
catch
    pause
end
