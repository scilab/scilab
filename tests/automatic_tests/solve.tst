getf SCI/util/testexamples.sci
reinit_for_test()
%U=mopen('SCI/tests/automatic_tests/solve_data.ref','rb');
A = ['1','a';'0','2'];//Upper triangular
b = ['x';'y'];
w = solve(A, b);
if load_ref('w') then   pause,end,

a = 1;x = 2;y = 5;
%ans = evstr(w);
if load_ref('%ans') then   pause,end,

%ans = inv([1,1;0,2]) * [2;5];
if load_ref('%ans') then   pause,end,

xdel_run(winsid());

mclose(%U);
