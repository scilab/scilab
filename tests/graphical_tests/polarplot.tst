getf SCI/util/testexamples.sci
reinit_for_test()
%U=mopen('SCI/tests/graphical_tests/polarplot_data.ref','rb');

t = 0:0.01:2 * %pi;
clf_run();%ans = polarplot(sin(7 * t), cos(8 * t));
if load_ref('%ans') then   pause,end,


clf_run();%ans = polarplot([sin(7 * t'),sin(6 * t')], [cos(8 * t'),cos(8 * t')], [1,2]);
if load_ref('%ans') then   pause,end,



xdel_run(winsid());

mclose(%U);
