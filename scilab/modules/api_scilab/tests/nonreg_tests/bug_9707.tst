// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2011 - DIGITEO - Allan CORNET
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================
//
// <-- ENGLISH IMPOSED -->
//
// <-- CLI SHELL MODE -->
//
// <-- Non-regression test for bug 9707 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/9707
//
// <-- Short Description -->
// createNamedXXX did not check variable name

BUGNAME = "bug_9707";
ilib_verbose(0);
if haveacompiler() then
  cd TMPDIR;
  mkdir(BUGNAME);
  cd(BUGNAME);
  DEST = TMPDIR + "/" + BUGNAME + "/" + BUGNAME + ".c";
  copyfile(SCI + "/modules/api_scilab/tests/nonreg_tests/" + BUGNAME + ".c", DEST);
  assert_checkequal(isfile(DEST), %T);

  gw_functions = [BUGNAME+ "_1","sci_" + BUGNAME + "_1";
                  BUGNAME+ "_2","sci_" + BUGNAME + "_2";
                  BUGNAME+ "_3","sci_" + BUGNAME + "_3";
                  BUGNAME+ "_4","sci_" + BUGNAME + "_4";
                  BUGNAME+ "_OK","sci_" + BUGNAME + "_OK"];

  ilib_build("lib" + BUGNAME, gw_functions, BUGNAME + ".c", []);
  exec loader.sce;
  assert_checkequal(execstr("bug_9707_1()", "errcatch"), 999);
  assert_checkequal(execstr("bug_9707_2()", "errcatch"), 999);
  assert_checkequal(execstr("bug_9707_3()", "errcatch"), 999);
  assert_checkequal(execstr("bug_9707_4()", "errcatch"), 999);
  assert_checkequal(execstr("bug_9707_OK()", "errcatch"), 0);
  assert_checkequal(VAR_9707 == "bug_9707_OK", %t);
end
