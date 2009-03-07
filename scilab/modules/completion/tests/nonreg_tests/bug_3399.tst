// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2008 - DIGITEO - Allan CORNET
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- Non-regression test for bug 3399 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=3399
//
// <-- Short Description -->
// Completion is eating a character.

// <-- JVM NOT MANDATORY -->

ilib_verbose(0);

exec('SCI/modules/completion/tests/utilities/build_primitives.sce',-1);
exec('SCI/modules/completion/tests/utilities/loader.sce',-1);

//cd /h<TAB>

currentline = 'cd /h';
newline = completeline(currentline,'home',getfilepartlevel(currentline),getpartlevel(currentline),%t);
if newline <> 'cd /home' then pause,end
