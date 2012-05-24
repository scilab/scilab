// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2009 - DIGITEO - Allan CORNET
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- CLI SHELL MODE -->
// <-- Non-regression test for bug 4515 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=4515
//
// <-- Short Description -->
// linking and loading some external functions already used crashs scilab
// exec SCI/modules/dynamic_link/tests/nonreg_tests/bug_4515.tst;

ilib_verbose(0);
chdir(TMPDIR);

f1 = ['int ext1c(int *n, double *a, double *b, double *c)',
'{int k;',
' for (k = 0; k < *n; ++k) ',
' c[k] = a[k] + b[k];',
' return(0);}'];

mputl(f1,'fun1.c');

ilib_for_link('ext1c','fun1.c',[],"c");
exec('loader.sce');

a=[1,2,3];b=[4,5,6];n=3;
c=call('ext1c',n,1,'i',a,2,'d',b,3,'d','out',[1,3],4,'d');     
if or(c <> [5 7 9]) <> %f then pause,end

f2 = ['#include <math.h>',
'#include <stdlib.h>';
'int ext1c(int *n, double *a, double *b, double *c, double *g)',
'{int k;',
'  for (k = 0; k < *n; ++k){ ',
'      c[k] = 2.0 *a[k] + b[k];',
'      g[k] = 3.0 + a[k] * b[k];',
'      a[k] = 2.5 * b[k];',
'      g[k] = 3.0 + a[k] * b[k];}',
'  return(0);}'];

ilib_for_link('ext1c','fun1.c',[],"c");
exec loader.sce;

c=call('ext1c',n,1,'i',a,2,'d',b,3,'d','out',[1,3],4,'d');     
if or(c <> [5 7 9]) <> %f then pause,end

// =============================================================================
