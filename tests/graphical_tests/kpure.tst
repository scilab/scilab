getf SCI/util/testexamples.sci
reinit_for_test()
%U=mopen('SCI/tests/graphical_tests/kpure_data.ref','rb');
s = poly(0, 's');
h = syslin('c', (s - 1)/(1 + 5 * s + s^2 + s^3));
if load_ref('h') then   pause,end,

xbasc_run();%ans = evans(h);
if load_ref('%ans') then   pause,end,

g = kpure(h);
if load_ref('g') then   pause,end,

hf = h /. g(1);
if load_ref('hf') then   pause,end,

%ans = roots(denom(hf));
if load_ref('%ans') then   pause,end,

xdel_run(winsid());

mclose(%U);
