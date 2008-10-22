getf SCI/util/testexamples.sci
reinit_for_test()
%U=mopen('SCI/tests/automatic_tests/sqrt_data.ref','rb');
%ans = sqrt([2,4]);
if load_ref('%ans') then   pause,end,

%ans = sqrt(-1);
if load_ref('%ans') then   pause,end,

xdel_run(winsid());

mclose(%U);
