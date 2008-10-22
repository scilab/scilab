getf SCI/util/testexamples.sci
reinit_for_test()
%U=mopen('SCI/tests/automatic_tests/tanhm_data.ref','rb');
A = [1,2;3,4];
%ans = tanhm(A);
if load_ref('%ans') then   pause,end,

xdel_run(winsid());

mclose(%U);
