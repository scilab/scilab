// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2008 - INRIA - Michael Baudin
// Copyright (C) 2009 - DIGITEO - Michael Baudin
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- CLI SHELL MODE -->

// optim.tst --
//   Test the optim command with the Rosenbrock test case
//   in the case where the cost function is provided as a Fortran
//   routine and the parameter is given as a scilab variable, using the "td" option.
//
//   Note : the following source code was copied from optimization/sci_gateway/fortran/Ex-optim.f
//   Thus, the "genros" function from Ex-optim.f is not needed anymore.
//
// This is the precision measured with experiments.

ilib_verbose(0);

Leps = 10^12 * %eps;
n = 3;
xopt = ones(n, 1);
// Move into the temporary directory to create the temporary files there
cur_dir = pwd();
chdir(TMPDIR);
//
// Define a fortran source code and compile it (fortran compiler required)
//
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


mputl(F,TMPDIR + '/rosenf.f');
// compile the Fortran code
ilib_for_link('rosenf', 'rosenf.f', [], 'f');
exec loader.sce;
chdir(cur_dir);
//solve the problem
x0 = 1.2*ones(n, 1);
valtd = 100;
[f, xo, go] = optim('rosenf', x0, 'td', valtd);
// Test with all solvers
solverlist = ["gc" "qn" "nd"];
for solver = solverlist
  [f, x, g] = optim('rosenf', x0, solver, 'td', valtd);
  if abs(f - 1 + norm(x - xopt) ) > Leps then pause, end
end
// Test all verbose levels with all possible solvers
verboselevels = [0];
for verbose = verboselevels
  for solver = solverlist
    [f,x,g] = optim('rosenf', x0, solver, 'td', valtd, imp = verbose);
    if abs(f - 1 + norm(x - xopt) ) > Leps then pause,end
  end
end
// Clean-up
ulink();

