getf SCI/util/testexamples.sci
reinit_for_test()
%U=mopen('SCI/tests/graphical_tests/xrpoly_data.ref','rb');
%ans = plot2d(0, 0, -1, '012', ' ', [0,0,10,10]);
if load_ref('%ans') then   pause,end,

%ans = xrpoly([5,5], 5, 5);
if load_ref('%ans') then   pause,end,

xdel_run(winsid());

mclose(%U);
