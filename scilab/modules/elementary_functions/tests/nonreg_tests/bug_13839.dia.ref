// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2016 - Samuel GOUGEON
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================
// <-- CLI SHELL MODE -->
//
// <-- Non-regression test for bug 13839 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=13839
//
// <-- Short Description -->
//    sign() did not accept sparse matrices
m = sprand(100,10,0.007);
i = find(m~=0 & m<0.5);
m(i) = m(i)-0.5;
try
    sign(m);
catch
    pause
end
try
    sign(m+%i*m);
catch
    pause
end
