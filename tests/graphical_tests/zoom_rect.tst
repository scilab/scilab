getf SCI/util/testexamples.sci
reinit_for_test()
%U=mopen('SCI/tests/graphical_tests/zoom_rect_data.ref','rb');
%ans = clf_run();
if load_ref('%ans') then   pause,end,

x = 0:0.01:6 * %pi;
%ans = plot2d(x, sin(x^2));
if load_ref('%ans') then   pause,end,

%ans = zoom_rect([16,-1,18,1]);
if load_ref('%ans') then   pause,end,

//more zoom
%ans = zoom_rect([16,0,16.2,1]);
if load_ref('%ans') then   pause,end,

//back to the original
%ans = unzoom();
if load_ref('%ans') then   pause,end,

// zooming using axes_properties
a = gca();
a('zoom_box') = [16,0,16.2,1];
a('zoom_box') = [];

//zooming subplots accordingly
%ans = clf_run();
if load_ref('%ans') then   pause,end,

x = 0:0.01:6 * %pi;
%ans = subplot(211);
if load_ref('%ans') then   pause,end,

%ans = plot2d(x, cos(x));
if load_ref('%ans') then   pause,end,

%ans = subplot(212);
if load_ref('%ans') then   pause,end,

%ans = plot2d(x, cos(2 * x));
if load_ref('%ans') then   pause,end,

rect = [3,-2,7,10];//a rectangle specified in the current axes (last one) coordinates
%ans = zoom_rect(rect);
if load_ref('%ans') then   pause,end,

%ans = unzoom();
if load_ref('%ans') then   pause,end,

//set the global underlying axes as current
f = gcf();%ans = set('current_axes', f.children($));
if load_ref('%ans') then   pause,end,

rect = [0.4,0,0.6,1];
if load_ref('rect') then   pause,end,
//a rectangle specified in ratio of the window size
%ans = zoom_rect(rect);
if load_ref('%ans') then   pause,end,

rect = [0.4,0.2,0.6,0.8];//a rectangle specified in ratio of the window size
%ans = zoom_rect(rect);
if load_ref('%ans') then   pause,end,



xdel_run(winsid());

mclose(%U);
