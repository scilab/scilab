// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2008 - DIGITEO - Allan CORNET
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- Non-regression test for bug 3757 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=3757
//
// <-- Short Description -->
// Under Scilex, the completion of paths is incorrect

// <-- ENGLISH IMPOSED -->
// <-- JVM NOT MANDATORY -->

exec('SCI/modules/completion/tests/utilities/build_primitives.sce',-1);
exec('SCI/modules/completion/tests/utilities/loader.sce',-1);


// cd d:\Pro[TAB]
currentline = 'cd d:\Pro';
newline = completeline(currentline,'Projects',getfilepartlevel(currentline),getpartlevel(currentline),%t);
if newline <> 'cd d:\Projects' then pause,end
