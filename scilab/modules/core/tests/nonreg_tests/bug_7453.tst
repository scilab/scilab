// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2010 - DIGITEO - Bernard HUGUENEY
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================
//
// <-- CLI SHELL MODE -->
//
// <-- Non-regression test for bug 7453 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=7453
//
// <-- Short Description -->
// after hashtable rewrite, it was not possible anymore to call newfun()
// after removing the funptr with clearfun()
//
//

id = funptr('user');
if id == 0 then pause, end
if clearfun('user') <> %t then pause, end
newfun('user', id);
if user(3) <> 3 then pause, end

