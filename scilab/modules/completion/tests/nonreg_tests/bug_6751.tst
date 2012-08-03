// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2008 - DIGITEO - Allan CORNET
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- Non-regression test for bug 6751 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=6751
//
// <-- Short Description -->
// A completion bug:
// mkdir -p cudascilab/sci_gateway cudascilab/src
// scilab -nwni
// exec cudascilab/[TAB]s[TAB]c[TAB]
// shows cudascilab/ssci_gateway/
// instead of 
// cudascilab/sci_gateway/

// <-- CLI SHELL MODE -->

ilib_verbose(0);
ierr = exec(SCI+"/modules/completion/tests/utilities/build_primitives.sce","errcatch",-1);
if ierr<>0 then pause, end
ierr = exec(TMPDIR + "/completion/loader.sce","errcatch",-1);
if ierr<>0 then pause, end


currentline = 'cd cudascilab/sc';
r = completeline(currentline,'sci_gateway/',getfilepartlevel(currentline),getpartlevel(currentline),%t);
if r <> 'cd cudascilab/sci_gateway/' then pause,end


currentline = 'cd cudascilab/sc';
r = completeline(currentline,'sci_gateway/',getfilepartlevel(currentline),getpartlevel(currentline),%t);
if r <> 'cd cudascilab/sci_gateway/' then pause,end

currentline = 'cd repositories/';
r = completeline(currentline,'tooboxes_google/',getfilepartlevel(currentline),getpartlevel(currentline),%t);
if r <> 'cd repositories/tooboxes_google/' then pause,end

currentline = 'cd ~/scilab/';
r = completeline(currentline,'scilab/',getfilepartlevel(currentline),getpartlevel(currentline),%t);
if r <> 'cd ~/scilab/scilab/' then pause,end
