getf SCI/util/testexamples.sci
reinit_for_test()
%U=mopen('SCI/tests/graphical_tests/plot2d_old_version_data.ref','rb');
//simple plot
x = (0:0.1:2 * %pi)';
%ans = plot2d(sin(x));
if load_ref('%ans') then   pause,end,

%ans = xbasc_run();
if load_ref('%ans') then   pause,end,

%ans = plot2d(x, sin(x));
if load_ref('%ans') then   pause,end,

//multiple plot
%ans = xbasc_run();
if load_ref('%ans') then   pause,end,

%ans = plot2d(x, [sin(x),sin(2 * x),sin(3 * x)]);
if load_ref('%ans') then   pause,end,

// multiple plot giving the dimensions of the frame
// old syntax and new syntax
%ans = xbasc_run();
if load_ref('%ans') then   pause,end,

%ans = plot2d(x, [sin(x),sin(2 * x),sin(3 * x)], 1:3, '011', '', [0,0,6,0.5]);
if load_ref('%ans') then   pause,end,

%ans = xbasc_run();
if load_ref('%ans') then   pause,end,

%ans = plot2d(x, [sin(x),sin(2 * x),sin(3 * x)], rect=[0,0,6,0.5]);
if load_ref('%ans') then   pause,end,

//multiple plot with captions and given tics // old syntax and new syntax
%ans = xbasc_run();
if load_ref('%ans') then   pause,end,

plot2d(x, [sin(x),sin(2 * x),sin(3 * x)], [1,2,3], '111', 'L1@L2@L3', [0,-2,2 * %pi,2], [2,10,2,10]);
%ans = xbasc_run();
if load_ref('%ans') then   pause,end,

%ans = plot2d(x, [sin(x),sin(2 * x),sin(3 * x)], [1,2,3], leg='L1@L2@L3', nax=[2,10,2,10], rect=[0,-2,2 * %pi,2]);
if load_ref('%ans') then   pause,end,

// isoview
%ans = xbasc_run();
if load_ref('%ans') then   pause,end,

%ans = plot2d(x, sin(x), 1, '041');
if load_ref('%ans') then   pause,end,

// scale
%ans = xbasc_run();
if load_ref('%ans') then   pause,end,

%ans = plot2d(x, sin(x), 1, '061');
if load_ref('%ans') then   pause,end,

// auto scaling with previous plots
%ans = xbasc_run();
if load_ref('%ans') then   pause,end,

%ans = plot2d(x, sin(x), 1);
if load_ref('%ans') then   pause,end,

%ans = plot2d(x, 2 * sin(x), 2);
if load_ref('%ans') then   pause,end,

%ans = plot2d(2 * x, cos(x), 3);
if load_ref('%ans') then   pause,end,

// axis on the right
%ans = xbasc_run();
if load_ref('%ans') then   pause,end,

%ans = plot2d(x, sin(x), 1, '183', 'sin(x)');
if load_ref('%ans') then   pause,end,

// centered axis
%ans = xbasc_run();
if load_ref('%ans') then   pause,end,

%ans = plot2d(x, sin(x), 1, '184', 'sin(x)');
if load_ref('%ans') then   pause,end,

// axis centered at (0,0)
%ans = xbasc_run();
if load_ref('%ans') then   pause,end,

%ans = plot2d(x - 4, sin(x), 1, '185', 'sin(x)');
if load_ref('%ans') then   pause,end,

xdel_run(winsid());

mclose(%U);
