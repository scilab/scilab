getf SCI/util/testexamples.sci
reinit_for_test()
%U=mopen('SCI/tests/automatic_tests/ss2des_data.ref','rb');
s = poly(0, 's');
G = [1/(s + 1),s;1 + s^2,3 * (s^3)];Sl = tf2ss(G);
S = ss2des(Sl);
if load_ref('S') then   pause,end,

S1 = ss2des(Sl, 'withD');
if load_ref('S1') then   pause,end,

Des = des2ss(S);Des(5) = clean(Des(5));
if load_ref('Des') then   pause,end,

Des1 = des2ss(S1);
if load_ref('Des1') then   pause,end,

xdel_run(winsid());

mclose(%U);
