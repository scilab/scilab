getf SCI/util/testexamples.sci
reinit_for_test()
%U=mopen('SCI/tests/automatic_tests/diag_data.ref','rb');
%ans = diag([1,2]);
if load_ref('%ans') then   pause,end,


A = [1,2;3,4];
%ans = diag(A);
if load_ref('%ans') then   pause,end,
// main diagonal
%ans = diag(A, 1);
if load_ref('%ans') then   pause,end,


%ans = diag(sparse(1:10));
if load_ref('%ans') then   pause,end,
// sparse diagonal matrix

// form a tridiagonal matrix of size 2*m+1
m = 5;%ans = diag(-m:m) + diag(ones(2 * m, 1), 1) + diag(ones(2 * m, 1), -1);
if load_ref('%ans') then   pause,end,

xdel_run(winsid());

mclose(%U);
