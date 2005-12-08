getf SCI/util/testexamples.sci
reinit_for_test()
%U=mopen('SCI/tests/graphical_tests/m_circle_data.ref','rb');
//Example 1 :
s = poly(0, 's');
if load_ref('s') then   pause,end,

h = syslin('c', (s^2 + 2 * 0.9 * 10 * s + 100)/(s^2 + 2 * 0.3 * 10.1 * s + 102.01));
if load_ref('h') then   pause,end,

%ans = nyquist(h, 0.01, 100, '(s^2+2*0.9*10*s+100)/(s^2+2*0.3*10.1*s+102.01)');
if load_ref('%ans') then   pause,end,

m_circle();
//Example 2:
xbasc_run();
h1 = h * syslin('c', (s^2 + 2 * 0.1 * 15.1 * s + 228.01)/(s^2 + 2 * 0.9 * 15 * s + 225));
if load_ref('h1') then   pause,end,

%ans = nyquist([h1;h], 0.01, 100, ['h1';'h']);
if load_ref('%ans') then   pause,end,

m_circle([-8,-6,-4]);
xdel_run(winsid());

mclose(%U);
