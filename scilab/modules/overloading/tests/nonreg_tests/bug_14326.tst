// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2015 - Scilab Enterprises - Pierre-Aimé AGNEL
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================
// <-- CLI SHELL MODE -->
//
// <-- Non-regression test for bug 14326 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/14326
//
// <-- Short Description -->
// deletion of a element in a structure was impossible

for i=1:10
    demo(i)=struct("a",[i i*2],"b","demo "+string(i));
end

ierr = execstr("demo(4) = [];", "errcatch");

assert_checkequal(ierr, 0);
