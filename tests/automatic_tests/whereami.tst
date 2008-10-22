getf SCI/util/testexamples.sci
reinit_for_test()
%U=mopen('SCI/tests/automatic_tests/whereami_data.ref','rb');
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
