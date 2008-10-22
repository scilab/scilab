// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) ????-2008 - INRIA
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

//============================================
// external with optim 
// dynamic link test
//============================================
// External function written in C (C compiler required)
// write down the C code (Rosenbrock problem)
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
mputl(C,TMPDIR+'/rosenc.c')
// compile the C code
l=ilib_for_link('rosenc','rosenc.o',[],'c',TMPDIR+'/Makefile');
// incremental linking
link(l,'rosenc','c');
//solve the problem
x0=[40;10;50];
p=100;
[f,xo,go]=optim('rosenc',x0,'td',p);
if f <> 1 then pause,end;
if norm(xo - [-1;1;1]) > 1000*%eps then pause,end;
//============================================
// External function written in Fortran (Fortran compiler required)
// write down the Fortran  code (Rosenbrock problem)
 F=[ '      subroutine rosenf(ind, n, x, f, g, ti, tr, td)'
     '      integer ind,n,ti(*)'
     '      double precision x(n),f,g(n),td(*)'
     '      real tr(*)'
     'c'
     '      double precision y,p'
     '      p=td(1)'
     '      if (ind.eq.2.or.ind.eq.4) then'
     '        f=1.0d0'
     '        do i=2,n'
     '          f=f+p*(x(i)-x(i-1)**2)**2+(1.0d0-x(i))**2'
     '        enddo'
     '      endif'
     '      if (ind.eq.3.or.ind.eq.4) then'
     '        g(1)=-4.0d0*p*(x(2)-x(1)**2)*x(1)'
     '        if(n.gt.2) then'
     '          do i=2,n-1'
     '            g(i)=2.0d0*p*(x(i)-x(i-1)**2)-4.0d0*p*(x(i+1)-x(i)**2)*x(i)'
     '     &           -2.0d0*(1.0d0-x(i))'
     '          enddo'
     '        endif'
     '        g(n)=2.0d0*p*(x(n)-x(n-1)**2)-2.0d0*(1.0d0-x(n))'
     '      endif'
     '      return'
     '      end'];

mputl(F,TMPDIR+'/rosenf.f');
// compile the Fortran code
l=ilib_for_link('rosenf','rosenf.o',[],'f',TMPDIR+'/Makefile');
// incremental linking
link(l,'rosenf','f');
//solve the problem
x0=[40;10;50];
p=100;
[f,xo,go]=optim('rosenf',x0,'td',p);
if f <> 1 then pause,end;
if norm(xo - [-1;1;1]) > 1000*%eps then pause,end;
//============================================
