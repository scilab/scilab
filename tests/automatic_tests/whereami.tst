getf SCI/util/testexamples.sci
reinit_for_test()
%U=mopen('/usr/local/lib/scilab/tests/Examples/other/whereami_data.ref','r');
%ans = deff('y=test(a)', ['y=sin(a)+1';
  'y=t1(y)';
  'y=y+1']);
if load_ref('%ans') then   pause,end,

%ans = deff('y=t1(y)', ['y=y^2';'whereami()']);
if load_ref('%ans') then   pause,end,

%ans = test(1);
if load_ref('%ans') then   pause,end,

xdel_run(winsid());

mclose(%U);
