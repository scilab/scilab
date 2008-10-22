getf SCI/util/testexamples.sci
reinit_for_test()
%U=mopen('SCI/tests/automatic_tests/logm_data.ref','rb');
A = [1,2;3,4];
%ans = logm(A);
if load_ref('%ans') then   pause,end,

%ans = expm(logm(A));
if load_ref('%ans') then   pause,end,

A1 = A * A';
%ans = logm(A1);
if load_ref('%ans') then   pause,end,

%ans = expm(logm(A1));
if load_ref('%ans') then   pause,end,

A1(1, 1) = %i;
%ans = expm(logm(A1));
if load_ref('%ans') then   pause,end,

xdel_run(winsid());

mclose(%U);
