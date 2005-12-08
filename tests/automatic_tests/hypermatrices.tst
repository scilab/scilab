getf SCI/util/testexamples.sci
reinit_for_test()
%U=mopen('SCI/tests/automatic_tests/hypermatrices_data.ref','rb');
a(1, 1, 1, 1:2) = [1,2];
if load_ref('a') then   pause,end,

a = [1,2;3,4];a(eye(), eye(), 2) = rand(2, 2);
if load_ref('a') then   pause,end,

%ans = a(1, 1, :);
if load_ref('%ans') then   pause,end,

%ans = [a,a];
if load_ref('%ans') then   pause,end,

xdel_run(winsid());

mclose(%U);
