// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2008 - DIGITEO - Allan CORNET
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- Non-regression test for bug 4039 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=4039
//
// <-- Short Description -->
// Problem of completion with version 5.1:
// (With NW mode).

// <-- CLI SHELL MODE -->

ilib_verbose(0);
ierr = exec(SCI+"/modules/completion/tests/utilities/build_primitives.sce","errcatch",-1);
if ierr<>0 then pause, end
ierr = exec(TMPDIR + "/completion/loader.sce","errcatch",-1);
if ierr<>0 then pause, end

// wavread("/ho[TAB]
currentline = 'wavread(""/ho';
r = getfilepartlevel(currentline);
if r <> '/ho' then pause,end

newline = completeline(currentline,'home',getfilepartlevel(currentline),getpartlevel(currentline),%t);
if newline <> 'wavread(""/home' then pause,end

// wavread("/home/sy[TAB]
currentline = 'wavread(""/home/sy';
r = getfilepartlevel(currentline);
if r <> '/home/sy' then pause,end

newline = completeline(currentline,'sylvestre',getfilepartlevel(currentline),getpartlevel(currentline),%t);
if newline <> 'wavread(""/home/sylvestre' then pause,end
