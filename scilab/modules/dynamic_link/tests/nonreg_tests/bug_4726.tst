// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2009 - DIGITEO - Sylvestre LEDRU
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- ENGLISH IMPOSED -->
// <-- CLI SHELL MODE -->

// <-- Non-regression test for bug 4726 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=4726
// <-- Short Description -->
// Under !Windows, when the link dynamic was copying the files,
// in some cases, it was going to copy backup files
// (for example, sci_luinc.c.bak instead of sci_luinc.c)
  ilib_verbose(0);
  currentpath = pwd();

  TEST_DIR=TMPDIR+"/bug_4726";
  mkdir(TEST_DIR);
  chdir(TEST_DIR);
  f1=['int ext1c(int *n, double *a, double *b, double *c)'
    '{return 1;}'];
  mputl(f1,'sci_luinc.c');
  
  f1back=['fake content with nothing in it... Should not be compiled'];
  mputl(f1back,'sci_luinc.c.back');
  
  ilib_for_link('ext1c','sci_luinc.c',[],"c");
  if fileinfo("loader.sce") == [], then pause, end
	
  rmdir(TEST_DIR,'s');
  chdir(currentpath);


// =============================================================================
