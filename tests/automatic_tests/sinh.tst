getf SCI/util/testexamples.sci
reinit_for_test()
%U=mopen('SCI/tests/automatic_tests/sinh_data.ref','rb');
%ans = asinh(sinh([0,1,%i]));
if load_ref('%ans') then   pause,end,

xdel_run(winsid());

mclose(%U);
