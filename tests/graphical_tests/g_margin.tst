getf SCI/util/testexamples.sci
reinit_for_test()
%U=mopen('SCI/tests/graphical_tests/g_margin_data.ref','rb');
h = syslin('c', -1 + %s, 3 + 2 * %s + %s^2);
if load_ref('h') then   pause,end,

[g,fr] = g_margin(h);
if load_ref('fr') then   pause,end,
if load_ref('g') then   pause,end,

[g,fr] = g_margin(h - 10);
if load_ref('fr') then   pause,end,
if load_ref('g') then   pause,end,

%ans = nyquist(h - 10);
if load_ref('%ans') then   pause,end,

xdel_run(winsid());

mclose(%U);
