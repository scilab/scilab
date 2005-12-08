getf SCI/util/testexamples.sci
reinit_for_test()
%U=mopen('SCI/tests/graphical_tests/unzoom_data.ref','rb');
%ans = clf_run();
if load_ref('%ans') then   pause,end,

x = 0:0.01:6 * %pi;
%ans = plot2d(x, sin(x^2));
if load_ref('%ans') then   pause,end,

%ans = zoom_rect([16,-1,18,1]);
if load_ref('%ans') then   pause,end,

%ans = unzoom();
if load_ref('%ans') then   pause,end,



//subplots accordingly
%ans = clf_run();
if load_ref('%ans') then   pause,end,

x = 0:0.01:6 * %pi;
%ans = subplot(211);
if load_ref('%ans') then   pause,end,

%ans = plot2d(x, cos(x));
if load_ref('%ans') then   pause,end,

a1 = gca();
%ans = subplot(212);
if load_ref('%ans') then   pause,end,

%ans = plot2d(x, cos(2 * x));
if load_ref('%ans') then   pause,end,

a2 = gca();
rect = [3,-2,7,10];//a rectangle specified in the current axes (last one) coordinates
%ans = zoom_rect(rect);
if load_ref('%ans') then   pause,end,

%ans = unzoom(a1);
if load_ref('%ans') then   pause,end,
//unzoom first plot only
%ans = unzoom(a2);
if load_ref('%ans') then   pause,end,
//unzoom second plot only
%ans = zoom_rect(rect);
if load_ref('%ans') then   pause,end,
//zoom again
%ans = unzoom();
if load_ref('%ans') then   pause,end,
//unzoom all the axes

xdel_run(winsid());

mclose(%U);
