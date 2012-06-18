// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2009 - DIGITEO - Allan CORNET
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- Non-regression test for bug 4737 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=4737
//
// <-- Short Description -->
// bug with the completion


// <-- CLI SHELL MODE -->

ilib_verbose(0);
ierr = exec(SCI+"/modules/completion/tests/utilities/build_primitives.sce","errcatch",-1);
if ierr<>0 then pause, end
ierr = exec(TMPDIR + "/completion/loader.sce","errcatch",-1);
if ierr<>0 then pause, end

cd TMPDIR;
mkdir('modules');
cd (TMPDIR+'/modules');
mkdir('string');
cd (TMPDIR+'/modules/string');
mkdir('macros');
cd (TMPDIR+'/modules/string/macros');
mputl('TEST',TMPDIR+'/modules/string/macros/finddeps.sci');
cd TMPDIR;
currentline = 'ls modules/string/macros/finddeps.';
r = completeline(currentline,'finddeps.sci',getfilepartlevel(currentline),getpartlevel(currentline),%t);
if r <> 'ls modules/string/macros/finddeps.sci' then pause,end
