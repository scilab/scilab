getf SCI/util/testexamples.sci
reinit_for_test()
%U=mopen('SCI/tests/graphical_tests/freson_data.ref','rb');
h = syslin('c', -1 + %s, (3 + 2 * %s + %s^2) * (50 + 0.1 * %s + %s^2));
if load_ref('h') then   pause,end,

fr = freson(h);
if load_ref('fr') then   pause,end,

%ans = bode(h);
if load_ref('%ans') then   pause,end,

g = 20 * log(abs(repfreq(h, fr)))/log(10);
if load_ref('g') then   pause,end,

xdel_run(winsid());

mclose(%U);
