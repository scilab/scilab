// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2010 - DIGITEO - Allan CORNET
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- Non-regression test for bug 4625 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=4625
//
// <-- Short Description -->
// intersci-n was broken on Windows.
//

if getos() == 'Windows' then
  copyfile(SCI+"/modules/intersci/tests/nonreg_tests/bug_4625.desc", TMPDIR+"/bug_4625.desc");
  cd(TMPDIR);
  s = dos(SCI+'/modules/intersci/bin/intersci-n.exe bug_4625');
  if s <> %t then pause, end
  r = mgetl(TMPDIR + '/bug_4625.c');
  ref = mgetl(SCI+"/modules/intersci/tests/nonreg_tests/bug_4625.c");
  if ~and(r == ref) then pause, end
end
