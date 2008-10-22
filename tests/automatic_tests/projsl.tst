getf SCI/util/testexamples.sci
reinit_for_test()
%U=mopen('SCI/tests/automatic_tests/projsl_data.ref','rb');
rand('seed', 0);sl = ssrand(2, 2, 5);[A,B,C,D] = abcd(sl);poles = spec(A);
if load_ref('poles') then   pause,end,

[Q,M] = pbig(A, 0, 'c');//keeping unstable poles
slred = projsl(sl, Q, M);%ans = spec(slred('A'));
if load_ref('%ans') then   pause,end,

sl('D') = rand(2, 2);//making proper system
%ans = trzeros(sl);
if load_ref('%ans') then   pause,end,
//zeros of sl
wi = inv(sl);//wi=inverse in state-space
[q,m] = psmall(wi('A'), 2, 'd');//keeping small zeros (poles of wi) i.e. abs(z)<2
slred2 = projsl(sl, q, m);
%ans = trzeros(slred2);
if load_ref('%ans') then   pause,end,
//zeros of slred2 = small zeros of sl
//  Example keeping second order modes
A = diag([-1,-2,-3]);
sl = syslin('c', A, rand(3, 2), rand(2, 3));[nk2,W] = hankelsv(sl);
if load_ref('W') then   pause,end,
if load_ref('nk2') then   pause,end,

[Q,M] = pbig(W, nk2(2) - %eps, 'c');//keeping 2 eigenvalues of W
slr = projsl(sl, Q, M);//reduced model
%ans = hankelsv(slr);
if load_ref('%ans') then   pause,end,

xdel_run(winsid());

mclose(%U);
