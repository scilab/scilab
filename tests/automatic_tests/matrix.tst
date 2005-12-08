getf SCI/util/testexamples.sci
reinit_for_test()
%U=mopen('SCI/tests/automatic_tests/matrix_data.ref','rb');
a = [1,2,3;4,5,6];
if load_ref('a') then   pause,end,

%ans = matrix(a, 1, 6);
if load_ref('%ans') then   pause,end,

%ans = matrix(a, 1, -1);
if load_ref('%ans') then   pause,end,

%ans = matrix(a, 3, 2);
if load_ref('%ans') then   pause,end,

xdel_run(winsid());

mclose(%U);
