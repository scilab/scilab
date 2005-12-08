getf SCI/util/testexamples.sci
reinit_for_test()
%U=mopen('SCI/tests/automatic_tests/des2ss_data.ref','rb');
s = poly(0, 's');G = [1/(s - 1),s;1,2/(s^3)];
S1 = tf2des(G);S2 = tf2des(G, 'withD');
W1 = des2ss(S1);W2 = des2ss(S2);
%ans = clean(ss2tf(W1));
if load_ref('%ans') then   pause,end,

%ans = clean(ss2tf(W2));
if load_ref('%ans') then   pause,end,

xdel_run(winsid());

mclose(%U);
