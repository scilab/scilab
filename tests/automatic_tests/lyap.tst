getf SCI/util/testexamples.sci
reinit_for_test()
%U=mopen('SCI/tests/automatic_tests/lyap_data.ref','rb');
A = rand(4, 4);C = rand(A);C = C + C';
X = lyap(A, C, 'c');
%ans = A' * X + X * A - C;
if load_ref('%ans') then   pause,end,

X = lyap(A, C, 'd');
%ans = A' * X * A - X - C;
if load_ref('%ans') then   pause,end,

xdel_run(winsid());

mclose(%U);
