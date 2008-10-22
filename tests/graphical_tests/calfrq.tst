getf SCI/util/testexamples.sci
reinit_for_test()
%U=mopen('SCI/tests/graphical_tests/calfrq_data.ref','rb');
s = poly(0, 's');
if load_ref('s') then   pause,end,

h = syslin('c', (s^2 + 2 * 0.9 * 10 * s + 100)/(s^2 + 2 * 0.3 * 10.1 * s + 102.01));
if load_ref('h') then   pause,end,

h1 = h * syslin('c', (s^2 + 2 * 0.1 * 15.1 * s + 228.01)/(s^2 + 2 * 0.9 * 15 * s + 225));
if load_ref('h1') then   pause,end,

[f1,spl] = calfrq(h1, 0.01, 1000);
rf = repfreq(h1, f1);
%ans = plot2d(real(rf)', imag(rf)');
if load_ref('%ans') then   pause,end,

xdel_run(winsid());

mclose(%U);
