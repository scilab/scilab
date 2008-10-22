getf SCI/util/testexamples.sci
reinit_for_test()
%U=mopen('SCI/tests/automatic_tests/contr_data.ref','rb');
W = ssrand(2, 3, 5, list('co', 3));//cont. subspace has dim 3.
A = W('A');B = W('B');
[n,U] = contr(A, B);n;
if load_ref('n') then   pause,end,

A1 = U' * A * U;
%ans = spec(A1(n + 1:$, n + 1:$));
if load_ref('%ans') then   pause,end,
//uncontrollable modes
%ans = spec(A + B * rand(3, 5));
if load_ref('%ans') then   pause,end,

xdel_run(winsid());

mclose(%U);
