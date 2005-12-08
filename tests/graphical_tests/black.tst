getf SCI/util/testexamples.sci
reinit_for_test()
%U=mopen('SCI/tests/graphical_tests/black_data.ref','rb');

s = poly(0, 's');
if load_ref('s') then   pause,end,

h = syslin('c', (s^2 + 2 * 0.9 * 10 * s + 100)/(s^2 + 2 * 0.3 * 10.1 * s + 102.01));
if load_ref('h') then   pause,end,

chart();
sstr = '(s^2+2*0.9*10*s+100)/(s^2+2*0.3*10.1*s+102.01)';
black(h, 0.01, 100, sstr);
h1 = h * syslin('c', (s^2 + 2 * 0.1 * 15.1 * s + 228.01)/(s^2 + 2 * 0.9 * 15 * s + 225));
if load_ref('h1') then   pause,end,

%ans = clf_run();
if load_ref('%ans') then   pause,end,

%ans = black([h1;h], 0.01, 100, ['h1';'h']);
if load_ref('%ans') then   pause,end,


xdel_run(winsid());

mclose(%U);
