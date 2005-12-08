getf SCI/util/testexamples.sci
reinit_for_test()
%U=mopen('SCI/tests/graphical_tests/xnumb_data.ref','rb');
%ans = plot2d([-100,500], [-100,600], [-1,-1], '022');
if load_ref('%ans') then   pause,end,

x = 0:100:200;
%ans = xnumb(x, 500 * ones(x), [10,20,35], 1);
if load_ref('%ans') then   pause,end,

xdel_run(winsid());

mclose(%U);
