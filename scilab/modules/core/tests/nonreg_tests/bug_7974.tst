// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2010 - DIGITEO - Allan CORNET
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================
//
// <-- Non-regression test for bug 7974 -->
//
// <-- CLI SHELL MODE -->
// 
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=7974
//
// <-- Short Description -->
// xerbla.f did not return enough information.

ilib_verbose(0);
if haveacompiler() then
  cd TMPDIR;
  mkdir("bug_7974");
  cd("bug_7974");
  copyfile(SCI + "/modules/core/tests/nonreg_tests/bug_7974.c", TMPDIR + "/bug_7974/bug_7974.c");
  if ~isfile(TMPDIR + "/bug_7974/bug_7974.c") then pause, end
  ilib_build("build_c",["call_xerbla","sci_call_xerbla"], "bug_7974.c", []);
  exec loader.sce;
  
  if execstr("call_xerbla(""FUN123"", 10)", "errcatch") <> 998 then pause, end
  [msg, err] = lasterror();
  if err <> 998 then pause, end
  refline = "  if execstr(""call_xerbla(""""FUN123"""", 10)"", ""errcatch"") <> 998 then pause, end";
  if msg <> refline then pause, end

end
