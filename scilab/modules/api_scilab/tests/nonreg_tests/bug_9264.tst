// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2011 - Calixte DENIZET
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================
// <-- CLI SHELL MODE -->
//
// <-- Non-regression test for bug 9264 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/9264
//
// <-- Short Description -->
// Big loss of performance on mputl

ilib_verbose(0);
if haveacompiler() then
  cd TMPDIR;
  mkdir("bug_9264");
  cd("bug_9264");
  copyfile(SCI + "/modules/api_scilab/tests/nonreg_tests/bug_9264.c", TMPDIR + "/bug_9264/bug_9264.c");
  if ~isfile(TMPDIR + "/bug_9264/bug_9264.c") then pause, end
  ilib_build("build_c",["bug9264","sci_bug9264"], "bug_9264.c", []);
  exec loader.sce;
  
  M = string(rand(200,1000));
  tic();
  bug9264(M);
  T = toc();
  // More than 2 seconds to read a matrix is not normal
  if T > 2 then pause, end
end
