getf SCI/util/testexamples.sci
reinit_for_test()
%U=mopen('SCI/tests/graphical_tests/xpoly_data.ref','rb');
x = sin(2 * %pi * (0:5)/5);
y = cos(2 * %pi * (0:5)/5);
%ans = plot2d(0, 0, -1, '010', ' ', [-2,-2,2,2]);
if load_ref('%ans') then   pause,end,

%ans = xset('color', 5);
if load_ref('%ans') then   pause,end,

%ans = xpoly(x, y, 'lines', 1);
if load_ref('%ans') then   pause,end,

%ans = xset('default');
if load_ref('%ans') then   pause,end,

xdel_run(winsid());

mclose(%U);
