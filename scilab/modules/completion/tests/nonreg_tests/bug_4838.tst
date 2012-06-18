// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2008 - DIGITEO - Allan CORNET
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- Non-regression test for bug 4838 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=4838
//
// <-- Short Description -->
// completion returns wrong value with '..' 
// cd SCI/modules
// cd ../et[tab]

// <-- CLI SHELL MODE -->

ilib_verbose(0);
ierr = exec(SCI+"/modules/completion/tests/utilities/build_primitives.sce","errcatch",-1);
if ierr<>0 then pause, end
ierr = exec(TMPDIR + "/completion/loader.sce","errcatch",-1);
if ierr<>0 then pause, end


currentline = 'cd ../et';
r = completeline(currentline,'etc/',getfilepartlevel(currentline),getpartlevel(currentline),%t);
if r <> 'cd ../etc/' then pause,end
