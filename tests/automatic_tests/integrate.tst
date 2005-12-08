getf SCI/util/testexamples.sci
reinit_for_test()
%U=mopen('SCI/tests/automatic_tests/integrate_data.ref','rb');
%ans = integrate('sin(x)', 'x', 0, %pi);
if load_ref('%ans') then   pause,end,

%ans = integrate(['if x==0 then 1,';
  'else sin(x)/x,end'], 'x', 0, %pi);
if load_ref('%ans') then   pause,end,

xdel_run(winsid());

mclose(%U);
