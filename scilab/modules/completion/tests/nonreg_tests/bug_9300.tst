// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2011 - Calixte DENIZET
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================
//
// <-- CLI SHELL MODE -->
//
// <-- Non-regression test for bug 9300 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/9300
//
// <-- Short Description -->
// Crash when try to complete a mlist without fields
//

ilib_verbose(0);
ierr = exec(SCI+"/modules/completion/tests/utilities/build_primitives.sce","errcatch",-1);
if ierr <> 0 then pause, end
ierr = exec(TMPDIR + "/completion/loader.sce","errcatch",-1);
if ierr <> 0 then pause, end

a = mlist(['V']);
if getfields('a.') <> '' then pause, end;

a = tlist(['V']);
if getfields('a.') <> '' then pause, end;

a = struct();
if getfields('a.') <> '' then pause, end;

a = mlist(['V' 'f']);
if getfields('a.') <> 'f' then pause, end;

a = tlist(['V' 'f']);
if getfields('a.') <> 'f' then pause, end;

a = struct('f', 'v');
if getfields('a.') <> 'f' then pause, end;
