getf SCI/util/testexamples.sci
reinit_for_test()
%U=mopen('SCI/tests/graphical_tests/p_margin_data.ref','rb');
h = syslin('c', -1 + %s, 3 + 2 * %s + %s^2);
if load_ref('h') then   pause,end,

[p,fr] = p_margin(h);
if load_ref('fr') then   pause,end,
if load_ref('p') then   pause,end,

[p,fr] = p_margin(h + 0.7);
if load_ref('fr') then   pause,end,
if load_ref('p') then   pause,end,

%ans = nyquist(h + 0.7);
if load_ref('%ans') then   pause,end,

t = (0:0.1:2 * %pi)';%ans = plot2d(sin(t), cos(t), -3, '000');
if load_ref('%ans') then   pause,end,

xdel_run(winsid());

mclose(%U);
