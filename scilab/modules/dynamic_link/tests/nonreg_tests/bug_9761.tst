// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2011 - DIGITEO - Allan CORNET
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================
//
// <-- CLI SHELL MODE -->
//
// <-- Non-regression test for bug 9761 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=9761
//
// <-- Short Description -->
// NDEBUG not defined by default in release mode on Windows for dynamic link

if getos() == "Windows" then
  setenv('DEBUG_SCILAB_DYNAMIC_LINK','NO');

  ilib_verbose(0);
  currentpath = pwd();
  cd TMPDIR;

  mkdir(TMPDIR,'bug_9761');
  TEST_DIR = TMPDIR + filesep() + 'bug_9761';
  copyfile(SCI+'/modules/dynamic_link/tests/nonreg_tests/bug_9761.c' , TEST_DIR + filesep() + 'bug_9761.c');
  chdir(TEST_DIR);
  files = ['bug_9761.c'];
  ierr = execstr("dll = ilib_for_link(""bug_9761"",[files],[],""c"");" , "errcatch");
  assert_checkequal(ierr, 0);
  mdelete(dll);
end
