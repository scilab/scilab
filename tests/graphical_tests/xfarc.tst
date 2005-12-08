getf SCI/util/testexamples.sci
reinit_for_test()
%U=mopen('SCI/tests/graphical_tests/xfarc_data.ref','rb');
// isoview scaling
%ans = plot2d(0, 0, -1, '031', ' ', [-2,-2,2,2]);
if load_ref('%ans') then   pause,end,

%ans = xfarc(-0.5, 0.5, 1, 1, 0, 90 * 64);
if load_ref('%ans') then   pause,end,

%ans = xset('color', 2);
if load_ref('%ans') then   pause,end,

%ans = xfarc(0.5, 0.5, 1, 1, 0, 360 * 64);
if load_ref('%ans') then   pause,end,

xdel_run(winsid());

mclose(%U);
