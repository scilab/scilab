getf SCI/util/testexamples.sci
reinit_for_test()
%U=mopen('SCI/tests/graphical_tests/leastsq_data.ref','rb');
// We will show different calling possibilities of leastsq on one (trivial) example
// which is non linear but doesn't really need to be solved with leastsq (applying
// log linearizes the model and the problem may be solved with linear algebra).
// In this example we look for the 2 parameters x(1) and x(2) of a simple
// exponential decay model (x(1) being the unknow initial value and x(2) the
// decay constant):
function y = yth(t, x)
     y  = x(1)*exp(-x(2)*t)
endfunction
if load_ref('%ans') then   pause,end,


// we have the m measures (ti, yi):
m = 10;
tm = [0.25,0.5,0.75,1,1.25,1.5,1.75,2,2.25,2.5]';
ym = [0.79,0.59,0.47,0.36,0.29,0.23,0.17,0.15,0.12,0.08]';
wm = ones(m, 1);// measure weights (here all equal to 1...)

// and we want to find the parameters x such that the model fits the given
// datas in the least square sense:
//
//  minimize  f(x) = sum_i  wm(i)^2 ( yth(tm(i),x) - ym(i) )^2

// initial parameters guess
x0 = [1.5;0.8];

// in the first examples, we define the function fun and dfun
// in scilab language
function e = myfun(x, tm, ym, wm)
     e = wm.*( yth(tm, x) - ym )
endfunction
if load_ref('%ans') then   pause,end,


function g = mydfun(x, tm, ym, wm)
     v = wm.*exp(-x(2)*tm)
     g = [v , -x(1)*tm.*v]
endfunction
if load_ref('%ans') then   pause,end,


// now we could call leastsq:

// 1- the simplest call
[f,xopt,gropt] = leastsq(list(myfun, tm, ym, wm), x0);
if load_ref('gropt') then   pause,end,
if load_ref('xopt') then   pause,end,
if load_ref('f') then   pause,end,


// 2- we provide the Jacobian
[f,xopt,gropt] = leastsq(list(myfun, tm, ym, wm), mydfun, x0);
if load_ref('gropt') then   pause,end,
if load_ref('xopt') then   pause,end,
if load_ref('f') then   pause,end,


// a small graphic (before showing other calling features)
tt = linspace(0, 1.1 * max(tm), 100)';
yy = yth(tt, xopt);
%ans = xbasc_run();
if load_ref('%ans') then   pause,end,

%ans = plot2d(tm, ym, style=-2);
if load_ref('%ans') then   pause,end,

%ans = plot2d(tt, yy, style=2);
if load_ref('%ans') then   pause,end,

legend(['measure points','fitted curve']);
%ans = xtitle('a simple fit with leastsq');
if load_ref('%ans') then   pause,end,


// 3- how to get some informations (we use imp=1)
[f,xopt,gropt] = leastsq(1, list(myfun, tm, ym, wm), mydfun, x0);
if load_ref('gropt') then   pause,end,
if load_ref('xopt') then   pause,end,
if load_ref('f') then   pause,end,


// 4- using the conjugate gradient (instead of quasi Newton)
[f,xopt,gropt] = leastsq(1, list(myfun, tm, ym, wm), mydfun, x0, 'gc');
if load_ref('gropt') then   pause,end,
if load_ref('xopt') then   pause,end,
if load_ref('f') then   pause,end,


// 5- how to provide bound constraints (not useful here !)
xinf = [-%inf,-%inf];xsup = [%inf,%inf];
[f,xopt,gropt] = leastsq(list(myfun, tm, ym, wm), 'b', xinf, xsup, x0);
if load_ref('gropt') then   pause,end,
if load_ref('xopt') then   pause,end,
if load_ref('f') then   pause,end,
// without Jacobian
[f,xopt,gropt] = leastsq(list(myfun, tm, ym, wm), mydfun, 'b', xinf, xsup, x0);
if load_ref('gropt') then   pause,end,
if load_ref('xopt') then   pause,end,
if load_ref('f') then   pause,end,
// with Jacobian

// 6- playing with some stopping parameters of the algorithm
//    (allows only 40 function calls, 8 iterations and set epsg=0.01, epsf=0.1)
[f,xopt,gropt] = leastsq(1, list(myfun, tm, ym, wm), mydfun, x0, 'ar', 40, 8, 0.01, 0.1);
if load_ref('gropt') then   pause,end,
if load_ref('xopt') then   pause,end,
if load_ref('f') then   pause,end,



// 7 and 8: now we want to define fun and dfun in fortran then in C
//          Note that the "compile and link to scilab" method used here
//          is believed to be OS independant (but there are some requirements,
//          in particular you need a C and a fortran compiler, and they must
//          be compatible with the ones used to build your scilab binary).

// 7- fun and dfun in fortran

// 7-1/ Let 's Scilab write the fortran code (in the TMPDIR directory):
f_code = ['      subroutine myfun(m,n,x,param,f)';
  '*     param(i) = tm(i), param(m+i) = ym(i), param(2m+i) = wm(i)';
  '      implicit none';
  '      integer n,m';
  '      double precision x(n), param(*), f(m)';
  '      integer i';
  '      do i = 1,m';
  '         f(i) = param(2*m+i)*( x(1)*exp(-x(2)*param(i)) - param(m+i) )';
  '      enddo';
  '      end ! subroutine fun';
  '';
  '      subroutine mydfun(m,n,x,param,df)';
  '*     param(i) = tm(i), param(m+i) = ym(i), param(2m+i) = wm(i)';
  '      implicit none';
  '      integer n,m';
  '      double precision x(n), param(*), df(m,n)';
  '      integer i';
  '      do i = 1,m';
  '         df(i,1) =  param(2*m+i)*exp(-x(2)*param(i))';
  '         df(i,2) = -x(1)*param(i)*df(i,1)';
  '      enddo';
  '      end ! subroutine dfun'];
%ans = mputl(f_code, TMPDIR + '/myfun.f');
if load_ref('%ans') then   pause,end,


// 7-2/ compiles it. You need a fortran compiler !
names = ['myfun','mydfun'];
if load_ref('names') then   pause,end,

flibname = ilib_for_link(names, 'myfun.o', [], 'f', TMPDIR + '/Makefile');

// 7-3/ link it to scilab (see link help page)
%ans = link(flibname, names, 'f');
if load_ref('%ans') then   pause,end,


// 7-4/ ready for the leastsq call: be carreful don't forget to
//      give the dimension m after the routine name !
[f,xopt,gropt] = leastsq(list('myfun', m, tm, ym, wm), x0);
if load_ref('gropt') then   pause,end,
if load_ref('xopt') then   pause,end,
if load_ref('f') then   pause,end,
// without Jacobian
[f,xopt,gropt] = leastsq(list('myfun', m, tm, ym, wm), 'mydfun', x0);
if load_ref('gropt') then   pause,end,
if load_ref('xopt') then   pause,end,
if load_ref('f') then   pause,end,
// with Jacobian


// 8- last example: fun and dfun in C

// 8-1/ Let 's Scilab write the C code (in the TMPDIR directory):
c_code = ['#include <math.h>';
  'void myfunc(int *m,int *n, double *x, double *param, double *f)';
  '{';
  '  /*  param[i] = tm[i], param[m+i] = ym[i], param[2m+i] = wm[i] */';
  '  int i;';
  '  for ( i = 0 ; i < *m ; i++ )';
  '    f[i] = param[2*(*m)+i]*( x[0]*exp(-x[1]*param[i]) - param[(*m)+i] );';
  '  return;';
  '}';
  '';
  'void mydfunc(int *m,int *n, double *x, double *param, double *df)';
  '{';
  '  /*  param[i] = tm[i], param[m+i] = ym[i], param[2m+i] = wm[i] */';
  '  int i;';
  '  for ( i = 0 ; i < *m ; i++ )';
  '    {';
  '      df[i] = param[2*(*m)+i]*exp(-x[1]*param[i]);';
  '      df[i+(*m)] = -x[0]*param[i]*df[i];';
  '    }';
  '  return;';
  '}'];
%ans = mputl(c_code, TMPDIR + '/myfunc.c');
if load_ref('%ans') then   pause,end,


// 8-2/ compiles it. You need a C compiler !
names = ['myfunc','mydfunc'];
if load_ref('names') then   pause,end,

clibname = ilib_for_link(names, 'myfunc.o', [], 'c', TMPDIR + '/Makefile');

// 8-3/ link it to scilab (see link help page)
%ans = link(clibname, names, 'c');
if load_ref('%ans') then   pause,end,


// 8-4/ ready for the leastsq call
[f,xopt,gropt] = leastsq(list('myfunc', m, tm, ym, wm), 'mydfunc', x0);
if load_ref('gropt') then   pause,end,
if load_ref('xopt') then   pause,end,
if load_ref('f') then   pause,end,

xdel_run(winsid());

mclose(%U);
