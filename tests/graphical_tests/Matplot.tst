getf SCI/util/testexamples.sci
reinit_for_test()
%U=mopen('SCI/tests/graphical_tests/Matplot_data.ref','rb');

%ans = Matplot([1,2,3;4,5,6]);
if load_ref('%ans') then   pause,end,

%ans = clf_run();
if load_ref('%ans') then   pause,end,

// draw the current colormap
%ans = Matplot(1:xget('lastpattern'));
if load_ref('%ans') then   pause,end,


xdel_run(winsid());

mclose(%U);
