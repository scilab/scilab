// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2011 - Calixte DENIZET
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================
//
// <-- CLI SHELL MODE -->
//
// <-- Non-regression test for bug 9755 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/9755
//
// <-- Short Description -->
// Completion on paths was not case insensitive on Windows
//

if getos() == 'Windows' then
  ilib_verbose(0);
  ierr = exec(SCI+"/modules/completion/tests/utilities/build_primitives.sce","errcatch",-1);
  if ierr <> 0 then pause, end
  ierr = exec(TMPDIR + "/completion/loader.sce","errcatch",-1);
  if ierr <> 0 then pause, end

  currentline = 'cd c:/u';
  r = completeline(currentline,'Users',getfilepartlevel(currentline),getpartlevel(currentline),%t);
  if r <> 'cd c:/Users' then pause,end
  
  currentline = 'cd c:/programs Files/f';
  r = completeline(currentline,'Fichiers',getfilepartlevel(currentline),getpartlevel(currentline),%t);
  if r <> 'cd c:/programs Files/Fichiers' then pause,end
end
