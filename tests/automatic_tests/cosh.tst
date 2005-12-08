getf SCI/util/testexamples.sci
reinit_for_test()
%U=mopen('SCI/tests/automatic_tests/cosh_data.ref','rb');
x = [0,1,%i];
if load_ref('x') then   pause,end,

%ans = acosh(cosh(x));
if load_ref('%ans') then   pause,end,

xdel_run(winsid());

mclose(%U);
