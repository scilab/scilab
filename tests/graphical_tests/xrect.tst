getf SCI/util/testexamples.sci
reinit_for_test()
%U=mopen('SCI/tests/graphical_tests/xrect_data.ref','rb');
%ans = plot2d(0, 0, -1, '010', ' ', [-2,-2,2,2]);
if load_ref('%ans') then   pause,end,

%ans = xset('color', 5);
if load_ref('%ans') then   pause,end,

%ans = xrect(-1, 1, 2, 2);
if load_ref('%ans') then   pause,end,

%ans = xset('default');
if load_ref('%ans') then   pause,end,

xdel_run(winsid());

mclose(%U);
