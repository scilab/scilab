getf SCI/util/testexamples.sci
reinit_for_test()
%U=mopen('SCI/tests/graphical_tests/plot2d_data.ref','rb');

// x initialisation
x = (0:0.1:2 * %pi)';
//simple plot
%ans = plot2d(sin(x));
if load_ref('%ans') then   pause,end,

%ans = clf_run();
if load_ref('%ans') then   pause,end,

%ans = plot2d(x, sin(x));
if load_ref('%ans') then   pause,end,

//multiple plot
%ans = clf_run();
if load_ref('%ans') then   pause,end,

%ans = plot2d(x, [sin(x),sin(2 * x),sin(3 * x)]);
if load_ref('%ans') then   pause,end,

// multiple plot giving the dimensions of the frame
%ans = clf_run();
if load_ref('%ans') then   pause,end,

%ans = plot2d(x, [sin(x),sin(2 * x),sin(3 * x)], rect=[0,0,6,0.5]);
if load_ref('%ans') then   pause,end,

//multiple plot with captions and given tics + style
%ans = clf_run();
if load_ref('%ans') then   pause,end,

%ans = plot2d(x, [sin(x),sin(2 * x),sin(3 * x)], [1,2,3], leg='L1@L2@L3', nax=[2,10,2,10], rect=[0,-2,2 * %pi,2]);
if load_ref('%ans') then   pause,end,

// isoview
%ans = clf_run();
if load_ref('%ans') then   pause,end,

%ans = plot2d(x, sin(x), 1, frameflag=4);
if load_ref('%ans') then   pause,end,

// scale
%ans = clf_run();
if load_ref('%ans') then   pause,end,

%ans = plot2d(x, sin(x), 1, frameflag=6);
if load_ref('%ans') then   pause,end,

// auto scaling with previous plots + style
%ans = clf_run();
if load_ref('%ans') then   pause,end,

%ans = plot2d(x, sin(x), -1);
if load_ref('%ans') then   pause,end,

%ans = plot2d(x, 2 * sin(x), 12);
if load_ref('%ans') then   pause,end,

%ans = plot2d(2 * x, cos(x), 3);
if load_ref('%ans') then   pause,end,

// axis on the right
%ans = clf_run();
if load_ref('%ans') then   pause,end,

%ans = plot2d(x, sin(x), leg='sin(x)');
if load_ref('%ans') then   pause,end,

a = gca();// Handle on axes entity
a('y_location') = 'right';
// axis centered at (0,0)
%ans = clf_run();
if load_ref('%ans') then   pause,end,

%ans = plot2d(x - 4, sin(x), 1, leg='sin(x)');
if load_ref('%ans') then   pause,end,

a = gca();
if load_ref('a') then   pause,end,
// Handle on axes entity
a('x_location') = 'middle';
a('y_location') = 'middle';
// Some operations on entities created by plot2d ...
a = gca();
a('isoview') = 'on';
%ans = a('children');
if load_ref('%ans') then   pause,end,
// list the children of the axes : here it is an Compound child composed of 2 entities
poly1 = a.children.children(2);//store polyline handle into poly1
poly1('foreground') = 4;// another way to change the style...
poly1('thickness') = 3;// ...and the tickness of a curve.
poly1('clip_state') = 'off';
if load_ref('poly1') then   pause,end,
// clipping control
leg = a.children.children(1);// store legend handle into leg
leg('font_style') = 9;
leg('foreground') = 6;
a('isoview') = 'off';

xdel_run(winsid());

mclose(%U);
