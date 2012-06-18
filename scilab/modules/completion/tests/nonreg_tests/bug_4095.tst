// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2008 - DIGITEO - Allan CORNET
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- Non-regression test for bug 4095 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=4095
//
// <-- Short Description -->
// Completion delete some characters of the string we are typing (With NW mode).

// <-- CLI SHELL MODE -->

ilib_verbose(0);
ierr = exec(SCI+"/modules/completion/tests/utilities/build_primitives.sce","errcatch",-1);
if ierr<>0 then pause, end
ierr = exec(TMPDIR + "/completion/loader.sce","errcatch",-1);
if ierr<>0 then pause, end


// cd SCI/mod<TAB>
currentline = 'cd SCI/mod';
r = getfilepartlevel(currentline);
if r <> 'SCI/mod' then pause,end
r = completion(getfilepartlevel(currentline),'files');
if r <> 'modules' + filesep() then pause,end

newline = completeline(currentline,r,getfilepartlevel(currentline),getpartlevel(currentline),%t);
if newline <> 'cd SCI/modules' + filesep() then pause,end

// cd SCI/modules/file<TAB>
currentline = 'cd SCI/modules/file';
r = getfilepartlevel(currentline);
if r <> 'SCI/modules/file' then pause,end
r = completion(getfilepartlevel(currentline),'files');
if r <> 'fileio' + filesep() then pause,end

newline = completeline(currentline,r,getfilepartlevel(currentline),getpartlevel(currentline),%t);
if newline <> 'cd SCI/modules/fileio' + filesep() then pause,end

// cd SCI/modules/fileio/te<TAB>
currentline = 'cd SCI/modules/fileio/te';
r = getfilepartlevel(currentline);
if r <> 'SCI/modules/fileio/te' then pause,end
r = completion(getfilepartlevel(currentline),'files');
if r <> 'tests' + filesep() then pause,end

newline = completeline(currentline,r,getfilepartlevel(currentline),getpartlevel(currentline),%t);
if newline <> 'cd SCI/modules/fileio/tests' + filesep() then pause,end

// cd SCI/modules/fileio/tests/u<TAB> 
currentline = 'cd SCI/modules/fileio/tests/u';
r = getfilepartlevel(currentline);
if r <> 'SCI/modules/fileio/tests/u' then pause,end
r = completion(getfilepartlevel(currentline),'files');
if r <> 'unit_tests' + filesep() then pause,end

newline = completeline(currentline,r,getfilepartlevel(currentline),getpartlevel(currentline),%t);
if newline <> 'cd SCI/modules/fileio/tests/unit_tests' + filesep() then pause,end
