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

cd TMPDIR;
TEST_NAME = "optim_external_fortran";
mkdir(TEST_NAME);
cd(TEST_NAME);   
mputl(F,  TMPDIR + '/' + TEST_NAME + '/rosenf.f');
// compile the Fortran code
l = ilib_for_link('rosenf', 'rosenf.f', [], 'f');
assert_checkequal(isfile(l), %t);
// incremental linking
assert_checkequal(link(l, 'rosenf', 'f'), 0);
// solve the problem
x0 = [40; 10; 50];
p = 100;
[f, xo, go] = optim('rosenf', x0, 'td', p);
assert_checkequal(f, 1);
assert_checkequal(norm(xo - [-1;1;1]) > 1000*%eps, %f);
// =============================================================================
