getf SCI/util/testexamples.sci
reinit_for_test()
%U=mopen('SCI/tests/automatic_tests/asinhm_data.ref','rb');
A = [1,2;2,3];
if load_ref('A') then   pause,end,

%ans = sinhm(asinhm(A));
if load_ref('%ans') then   pause,end,

xdel_run(winsid());

mclose(%U);
