getf SCI/util/testexamples.sci
reinit_for_test()
%U=mopen('SCI/tests/automatic_tests/pol2des_data.ref','rb');
s = poly(0, 's');
G = [1,s;1 + s^2,3 * (s^3)];[N,B,C] = pol2des(G);
G1 = clean(C * inv(s * N - eye()) * B);
if load_ref('G1') then   pause,end,
G2 = numer(G1);
if load_ref('G2') then   pause,end,

xdel_run(winsid());

mclose(%U);
