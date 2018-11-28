// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2018 - ESI - Antoine ELIAS
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================
//
// <-- CLI SHELL MODE -->
// <-- NO CHECK REF -->
//
// <-- Non-regression test for bug 15885 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/15885
//
// <-- Short Description -->
// break in an else block is broken

j = 0;
for i = 1:4
    if %f
    else
        break
    end
    j = j + 1;
end

assert_checktrue(j == 0);

j = 0;
for i = 1:4
    if %t
        break
    else
    end
    j = j + 1;
end

assert_checktrue(j == 0);
