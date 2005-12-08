getf SCI/util/testexamples.sci
reinit_for_test()
%U=mopen('SCI/tests/graphical_tests/contour2d_data.ref','rb');
%ans = contour2d(1:10, 1:10, rand(10, 10), 5, rect=[0,0,11,11]);
if load_ref('%ans') then   pause,end,

// changing the format of the printing of the levels
%ans = xset('fpf', '%.2f');
if load_ref('%ans') then   pause,end,

%ans = clf_run();
if load_ref('%ans') then   pause,end,

%ans = contour2d(1:10, 1:10, rand(10, 10), 5, rect=[0,0,11,11]);
if load_ref('%ans') then   pause,end,


// now an example with level numbers drawn in a legend
// Caution there are a number of tricks...
x = linspace(0, 4 * %pi, 80);
z = cos(x') * cos(x);
clf_run();f = gcf();
%ans = xset('fpf', ' ');
if load_ref('%ans') then   pause,end,
// trick 1: this implies that the level numbers are not
//          drawn on the level curves
f('color_map') = jetcolormap(7);
// trick 2: to be able to put the legend on the right without
//          interfering with the level curves use rect with a xmax
//          value large enough
%ans = contour2d(x, x, z, -0.75:0.25:0.75, frameflag=3, rect=[0,0,5 * %pi,4 * %pi]);
if load_ref('%ans') then   pause,end,

// trick 3: use legends (note that the more practical legend function
//          will not work as soon as one of the level is formed by 2 curves)
legends(string(-0.75:0.25:0.75), 1:7, 'lr');
%ans = xtitle('Some level curves of the function cos(x)cos(y)');
if load_ref('%ans') then   pause,end,

xdel_run(winsid());

mclose(%U);
