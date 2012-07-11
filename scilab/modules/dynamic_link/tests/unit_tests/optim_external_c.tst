// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) ????-2008 - INRIA
// Copyright (C) 2009-2011 - DIGITEO
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================
//
// <-- CLI SHELL MODE -->
//
// =============================================================================
ilib_verbose(0);
// =============================================================================
// external C with optim 
// dynamic link test
// =============================================================================
// External function written in C (C compiler required)
// write down the C code (Rosenbrock problem)
// =============================================================================
 C=['#include <math.h>'
    'double sq(double x)'
    '{ return x*x;}'
    'void rosenc(int *ind, int *n, double *x, double *f, double *g, '
    '                                int *ti, float *tr, double *td)'
    '{'
    '  double p;'
    '  int i;'
    '  p=td[0];'
    '  if (*ind==2||*ind==4) {'
    '    *f=1.0;'
    '    for (i=1;i<*n;i++)'
    '      *f+=p*sq(x[i]-sq(x[i-1]))+sq(1.0-x[i]);'
    '  }'
    '  if (*ind==3||*ind==4) {'
    '    g[0]=-4.0*p*(x[1]-sq(x[0]))*x[0];'
    '    for (i=1;i<*n-1;i++)'
    '      g[i]=2.0*p*(x[i]-sq(x[i-1]))-4.0*p*(x[i+1]-sq(x[i]))*x[i]-2.0*(1.0-x[i]);'
    '    g[*n-1]=2.0*p*(x[*n-1]-sq(x[*n-2]))-2.0*(1.0-x[*n-1]);'
    '  }'
    '}'];
cd TMPDIR;
TEST_NAME = "optim_external_c";
mkdir(TEST_NAME);
cd(TEST_NAME);    
mputl(C, TMPDIR + '/' + TEST_NAME + '/rosenc.c');
// compile the C code
l = ilib_for_link('rosenc', 'rosenc.c', [], 'c');
assert_checkequal(isfile(l), %t);
// incremental linking
assert_checkequal(link(l, 'rosenc', 'c'), 0);
//solve the problem
x0 = [40;10;50];
p = 100;
[f, xo, go] = optim('rosenc', x0, 'td', p);
assert_checkequal(f, 1);
assert_checkequal(norm(xo - [-1;1;1]) > 1000*%eps, %f);
// =============================================================================
