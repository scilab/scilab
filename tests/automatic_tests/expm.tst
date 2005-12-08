getf SCI/util/testexamples.sci
reinit_for_test()
%U=mopen('SCI/tests/automatic_tests/expm_data.ref','rb');
X = [1,2;3,4];
if load_ref('X') then   pause,end,

%ans = expm(X);
if load_ref('%ans') then   pause,end,

%ans = logm(expm(X));
if load_ref('%ans') then   pause,end,

xdel_run(winsid());

mclose(%U);
