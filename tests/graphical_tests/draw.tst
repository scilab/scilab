getf SCI/util/testexamples.sci
reinit_for_test()
%U=mopen('SCI/tests/graphical_tests/draw_data.ref','rb');
%ans = subplot(212);
if load_ref('%ans') then   pause,end,

a = gca();
plot2d;
if load_ref('plot2d') then   pause,end,

drawlater;
if load_ref('drawlater') then   pause,end,


%ans = subplot(211);
if load_ref('%ans') then   pause,end,

plot2d1;
if load_ref('plot2d1') then   pause,end,
/// default drawing mode

e = gce();
%ans = draw(e.children(2));
if load_ref('%ans') then   pause,end,
// draw a single polyline of the second axes

e.children(1).visible = 'off';// We can choose to make a line invisible

%ans = draw(e);
if load_ref('%ans') then   pause,end,
// draw Compound and its children <=> draw all the visible polylines




drawnow;
if load_ref('drawnow') then   pause,end,
/// ...now!

e.children(1).visible = 'on';




xdel_run(winsid());

mclose(%U);
