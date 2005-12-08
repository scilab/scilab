getf SCI/util/testexamples.sci
reinit_for_test()
%U=mopen('SCI/tests/automatic_tests/mean_data.ref','rb');

A = [1,2,10;7,7.1,7.01];
%ans = mean(A);
if load_ref('%ans') then   pause,end,

%ans = mean(A, 'r');
if load_ref('%ans') then   pause,end,

%ans = mean(A, 'c');
if load_ref('%ans') then   pause,end,

A = matrix(1:12, [1,1,2,3,2]);
// in this case mean(A,'m') is equivalent to mean(A,3), the first non singleton dimension of A
y = mean(A, 'm');
if load_ref('y') then   pause,end,


xdel_run(winsid());

mclose(%U);
