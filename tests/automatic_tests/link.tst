getf SCI/util/testexamples.sci
reinit_for_test()
%U=mopen('SCI/tests/automatic_tests/link_data.ref','rb');

//Example of the use of ilib_for_link with  a simple C code
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
//generated the code is compileda nd a shared library built.
%ans = ilib_for_link('fooc', 'fooc.o', [], 'c');
if load_ref('%ans') then   pause,end,


// display the loader.sce file which calls link
%ans = mprintf('%s\n', mgetl('loader.sce'));
if load_ref('%ans') then   pause,end,

// load the shared library
%ans = exec('loader.sce');
if load_ref('%ans') then   pause,end,

xdel_run(winsid());

mclose(%U);
