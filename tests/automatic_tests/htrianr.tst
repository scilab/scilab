getf SCI/util/testexamples.sci
reinit_for_test()
%U=mopen('SCI/tests/automatic_tests/htrianr_data.ref','rb');
x = poly(0, 'x');
M = [x;x^2;2 + x^3] * [1,x - 2,x^4];
[Mu,U,rk] = htrianr(M);
if load_ref('rk') then   pause,end,
if load_ref('U') then   pause,end,
if load_ref('Mu') then   pause,end,

%ans = det(U);
if load_ref('%ans') then   pause,end,

%ans = M * U(:, 1:2);
if load_ref('%ans') then   pause,end,

xdel_run(winsid());

mclose(%U);
