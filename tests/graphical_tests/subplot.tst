getf SCI/util/testexamples.sci
reinit_for_test()
%U=mopen('SCI/tests/graphical_tests/subplot_data.ref','rb');
%ans = subplot(221);
if load_ref('%ans') then   pause,end,

%ans = plot2d();
if load_ref('%ans') then   pause,end,

%ans = subplot(222);
if load_ref('%ans') then   pause,end,

%ans = plot3d();
if load_ref('%ans') then   pause,end,

%ans = subplot(2, 2, 3);
if load_ref('%ans') then   pause,end,

%ans = param3d();
if load_ref('%ans') then   pause,end,

%ans = subplot(2, 2, 4);
if load_ref('%ans') then   pause,end,

%ans = hist3d();
if load_ref('%ans') then   pause,end,

xdel_run(winsid());

mclose(%U);
