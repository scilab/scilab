getf SCI/util/testexamples.sci
reinit_for_test()
%U=mopen('SCI/tests/automatic_tests/exists_data.ref','rb');
%ans = deff('foo(x)', ['disp([exists(''a12''),exists(''a12'',''local'')])';
  'disp([exists(''x''),exists(''x'',''local'')])']);
if load_ref('%ans') then   pause,end,

%ans = foo(1);
if load_ref('%ans') then   pause,end,

a12 = [];%ans = foo(1);
if load_ref('%ans') then   pause,end,

xdel_run(winsid());

mclose(%U);
