getf SCI/util/testexamples.sci
reinit_for_test()
%U=mopen('SCI/tests/automatic_tests/hankelsv_data.ref','rb');
A = diag([-1,-2,-3]);
sl = syslin('c', A, rand(3, 2), rand(2, 3));[nk2,W] = hankelsv(sl);
if load_ref('W') then   pause,end,
if load_ref('nk2') then   pause,end,

[Q,M] = pbig(W, nk2(2) - %eps, 'c');
slr = projsl(sl, Q, M);%ans = hankelsv(slr);
if load_ref('%ans') then   pause,end,

xdel_run(winsid());

mclose(%U);
