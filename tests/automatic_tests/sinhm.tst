getf SCI/util/testexamples.sci
reinit_for_test()
%U=mopen('SCI/tests/automatic_tests/sinhm_data.ref','rb');
A = [1,2;2,3];
if load_ref('A') then   pause,end,

%ans = asinhm(sinhm(A));
if load_ref('%ans') then   pause,end,

A(1, 1) = %i;%ans = sinhm(A) - (expm(A) - expm(-A))/2;
if load_ref('%ans') then   pause,end,
//Complex case
xdel_run(winsid());

mclose(%U);
