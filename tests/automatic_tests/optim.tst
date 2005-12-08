getf SCI/util/testexamples.sci
reinit_for_test()
%U=mopen('SCI/tests/automatic_tests/optim_data.ref','rb');
// External function written in Scilab
xref = [1;2;3];x0 = [1;-1;1];
if load_ref('x0') then   pause,end,

deff('[f,g,ind]=cost(x,ind)', 'f=0.5*norm(x-xref)^2,g=x-xref');
[f,xopt] = optim(cost, x0);
if load_ref('xopt') then   pause,end,
if load_ref('f') then   pause,end,
//Simplest call
[f,xopt,gopt] = optim(cost, x0, 'gc');
if load_ref('gopt') then   pause,end,
if load_ref('xopt') then   pause,end,
if load_ref('f') then   pause,end,
// By conjugate gradient
[f,xopt,gopt] = optim(cost, x0, 'nd');
if load_ref('gopt') then   pause,end,
if load_ref('xopt') then   pause,end,
if load_ref('f') then   pause,end,
//Seen as non differentiable
[f,xopt,gopt] = optim(cost, 'b', [-1;0;2], [0.5;1;4], x0);
if load_ref('gopt') then   pause,end,
if load_ref('xopt') then   pause,end,
if load_ref('f') then   pause,end,
//  Bounds on x
[f,xopt,gopt] = optim(cost, 'b', [-1;0;2], [0.5;1;4], x0, 'gc');
if load_ref('gopt') then   pause,end,
if load_ref('xopt') then   pause,end,
if load_ref('f') then   pause,end,
//  Bounds on x
[f,xopt,gopt] = optim(cost, 'b', [-1;0;2], [0.5;1;4], x0, 'gc', 'ar', 3);
if load_ref('gopt') then   pause,end,
if load_ref('xopt') then   pause,end,
if load_ref('f') then   pause,end,


// External function written in C (C compiler required)
// write down the C code (Rosenbrock problem)
C = ['#include <math.h>';
  'double sq(double x)';
  '{ return x*x;}';
  'void rosenc(int *ind, int *n, double *x, double *f, double *g, ';
  '                                int *ti, float *tr, double *td)';
  '{';
  '  double p;';
  '  int i;';
  '  p=td[0];';
  '  if (*ind==2||*ind==4) {';
  '    *f=1.0;';
  '    for (i=1;i<*n;i++)';
  '      *f+=p*sq(x[i]-sq(x[i-1]))+sq(1.0-x[i]);';
  '  }';
  '  if (*ind==3||*ind==4) {';
  '    g[0]=-4.0*p*(x[1]-sq(x[0]))*x[0];';
  '    for (i=1;i<*n-1;i++)';
  '      g[i]=2.0*p*(x[i]-sq(x[i-1]))-4.0*p*(x[i+1]-sq(x[i]))*x[i]-2.0*(1.0-x[i]);';
  '    g[*n-1]=2.0*p*(x[*n-1]-sq(x[*n-2]))-2.0*(1.0-x[*n-1]);';
  '  }';
  '}'];
%ans = mputl(C, TMPDIR + '/rosenc.c');
if load_ref('%ans') then   pause,end,

// compile the C code
l = ilib_for_link('rosenc', 'rosenc.o', [], 'c', TMPDIR + '/Makefile');
// incremental linking
%ans = link(l, 'rosenc', 'c');
if load_ref_nocheck('%ans') then   pause,end,

//solve the problem
x0 = [40;10;50];
p = 100;
[f,xo,go] = optim('rosenc', x0, 'td', p);
if load_ref('go') then   pause,end,
if load_ref('xo') then   pause,end,
if load_ref('f') then   pause,end,


// External function written in Fortran (Fortran compiler required)
// write down the Fortran  code (Rosenbrock problem)
F = ['      subroutine rosenf(ind, n, x, f, g, ti, tr, td)';
  '      integer ind,n,ti(*)';
  '      double precision x(n),f,g(n),td(*)';
  '      real tr(*)';
  'c';
  '      double precision y,p';
  '      p=td(1)';
  '      if (ind.eq.2.or.ind.eq.4) then';
  '        f=1.0d0';
  '        do i=2,n';
  '          f=f+p*(x(i)-x(i-1)**2)**2+(1.0d0-x(i))**2';
  '        enddo';
  '      endif';
  '      if (ind.eq.3.or.ind.eq.4) then';
  '        g(1)=-4.0d0*p*(x(2)-x(1)**2)*x(1)';
  '        if(n.gt.2) then';
  '          do i=2,n-1';
  '            g(i)=2.0d0*p*(x(i)-x(i-1)**2)-4.0d0*p*(x(i+1)-x(i)**2)*x(i)';
  '     &           -2.0d0*(1.0d0-x(i))';
  '          enddo';
  '        endif';
  '        g(n)=2.0d0*p*(x(n)-x(n-1)**2)-2.0d0*(1.0d0-x(n))';
  '      endif';
  '      return';
  '      end'];

%ans = mputl(F, TMPDIR + '/rosenf.f');
if load_ref('%ans') then   pause,end,

// compile the Fortran code
l = ilib_for_link('rosenf', 'rosenf.o', [], 'f', TMPDIR + '/Makefile');
// incremental linking
%ans = link(l, 'rosenf', 'f');
if load_ref_nocheck('%ans') then   pause,end,

//solve the problem
x0 = [40;10;50];
p = 100;
[f,xo,go] = optim('rosenf', x0, 'td', p);
if load_ref('go') then   pause,end,
if load_ref('xo') then   pause,end,
if load_ref('f') then   pause,end,



xdel_run(winsid());

mclose(%U);
