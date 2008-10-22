getf SCI/util/testexamples.sci
reinit_for_test()
%U=mopen('SCI/tests/automatic_tests/pbig_data.ref','rb');
A = diag([1,2,3]);X = rand(A);A = inv(X) * A * X;
[Q,M] = pbig(A, 1.5, 'd');
%ans = spec(M * A * Q);
if load_ref('%ans') then   pause,end,

[Q1,M1] = fullrf(eye() - Q * M);
%ans = spec(M1 * A * Q1);
if load_ref('%ans') then   pause,end,

xdel_run(winsid());

mclose(%U);
