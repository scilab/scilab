getf SCI/util/testexamples.sci
reinit_for_test()
%U=mopen('SCI/tests/automatic_tests/sylv_data.ref','rb');
A = rand(4, 4);C = rand(4, 3);B = rand(3, 3);
X = sylv(A, B, C, 'c');
%ans = norm(A * X + X * B - C);
if load_ref('%ans') then   pause,end,

X = sylv(A, B, C, 'd');
if load_ref('X') then   pause,end,

%ans = norm(A * X * B - X - C);
if load_ref('%ans') then   pause,end,

xdel_run(winsid());

mclose(%U);
