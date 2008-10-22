getf SCI/util/testexamples.sci
reinit_for_test()
%U=mopen('SCI/tests/automatic_tests/ilib_for_link_data.ref','rb');

f1 = ['int ext1c(n, a, b, c)';
  'int *n;     double *a, *b, *c;';
  '{int k;';
  '  for (k = 0; k < *n; ++k) ';
  '      c[k] = a[k] + b[k];';
  '  return(0);}'];

%ans = mputl(f1, 'fun1.c');
if load_ref('%ans') then   pause,end,


//creating the shared library (a gateway, a Makefile and a loader are
//generated.

%ans = ilib_for_link('ext1c', 'fun1.o', [], 'c');
if load_ref('%ans') then   pause,end,


// load the shared library

%ans = exec('loader.sce');
if load_ref('%ans') then   pause,end,


//using the new primitive
a = [1,2,3];b = [4,5,6];n = 3;
c = call('ext1c', n, 1, 'i', a, 2, 'd', b, 3, 'd', 'out', [1,3], 4, 'd');
if norm(c - (a + b)) > %eps then   pause,end,

xdel_run(winsid());

mclose(%U);
