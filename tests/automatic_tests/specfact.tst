getf SCI/util/testexamples.sci
reinit_for_test()
%U=mopen('SCI/tests/automatic_tests/specfact_data.ref','rb');
A = diag([-1,-2]);B = [1;1];C = [1,1];D = 1;s = poly(0, 's');
W1 = syslin('c', A, B, C, D);
phi = gtild(W1, 'c') + W1;
phis = clean(ss2tf(phi));
if load_ref('phis') then   pause,end,

clean(phis - horner(phis, -s)');//check this is 0...
[A,B,C,D] = abcd(W1);
[W0,L] = specfact(A, B, C, D);
W = syslin('c', A, B, L, W0);
if load_ref('W') then   pause,end,

Ws = ss2tf(W);
%ans = horner(Ws, -s) * Ws;
if load_ref('%ans') then   pause,end,

xdel_run(winsid());

mclose(%U);
