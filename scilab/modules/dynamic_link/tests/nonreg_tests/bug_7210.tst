// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2010 - DIGITEO - Allan CORNET
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================
//
// <-- CLI SHELL MODE -->
//
// <-- Non-regression test for bug 7210 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=7210
// <-- Short Description -->
//
// 'call' was broken on Windows 64 bits (master)
//
ilib_verbose(0);
foo=['void foo(double *a,double *b,double *c)';
     '{ *c = *a + *b; }'  ];

// we use TMPDIR for compilation

if ~c_link('foo') then
  curPath = pwd();
  chdir(TMPDIR);
  mputl(foo,'foo.c');

  ilib_for_link(['foo'],'foo.c',[],"c");

  // load the shared library
  exec loader.sce ;
  chdir(curPath) ;
end

//5+7 by C function
v = call('foo',5, 1, 'd', 7, 2, 'd', 'out', [1,1], 3, 'd');
if v <> 12 then pause, end
// =============================================================================
