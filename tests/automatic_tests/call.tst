getf SCI/util/testexamples.sci
reinit_for_test()
%U=mopen('SCI/tests/automatic_tests/call_data.ref','rb');
//Example 1 with  a simple C code
f1 = ['#include <math.h>';
  'void fooc(c,a,b,m,n)';
  'double a[],*b,c[];';
  'int *m,*n;';
  '{';
  '   int i;';
  '   for ( i =0 ; i < (*m)*(*n) ; i++) ';
  '     c[i] = sin(a[i]) + *b; ';
  '}'];

%ans = mputl(f1, 'fooc.c');
if load_ref('%ans') then   pause,end,


//creating the shared library (a gateway, a Makefile and a loader are
//generated.

%ans = ilib_for_link('fooc', 'fooc.o', [], 'c');
if load_ref('%ans') then   pause,end,


// load the shared library

%ans = exec('loader.sce');
if load_ref('%ans') then   pause,end,


//using the new primitive
a = [1,2,3;4,5,6];b = %pi;
[m,n] = size(a);

// Inputs:
// a is in position 2 and double
// b                3     double
// n                4     integer
// m                5     integer
// Outputs:
// c is in position 1 and double with size [m,n]
c = call('fooc', a, 2, 'd', b, 3, 'd', m, 4, 'i', n, 5, 'i', 'out', [m,n], 1, 'd');

//Example 2 with  a simple Fortran code
f1 = ['      subroutine foof(c,a,b,n,m)';
  '      integer n,m';
  '      double precision a(*),b,c(*)';
  '      do 10 i=1,m*n ';
  '        c(i) = sin(a(i))+b';
  '   10 continue';
  '      end'];
%ans = mputl(f1, 'foof.f');
if load_ref('%ans') then   pause,end,


//creating the shared library (a gateway, a Makefile and a loader are
//generated.

%ans = ilib_for_link('foof', 'foof.o', [], 'f');
if load_ref('%ans') then   pause,end,


// load the shared library

%ans = exec('loader.sce');
if load_ref('%ans') then   pause,end,


//using the new primitive
a = [1,2,3;4,5,6];b = %pi;
[m,n] = size(a);
c = call('foof', a, 2, 'd', b, 3, 'd', m, 4, 'i', n, 5, 'i', 'out', [m,n], 1, 'd');


xdel_run(winsid());

mclose(%U);
