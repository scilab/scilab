getf SCI/util/testexamples.sci
reinit_for_test()
%U=mopen('SCI/tests/graphical_tests/LineSpec_data.ref','rb');
x = 1:0.1:10;// Init.
%ans = plot(x, sin(x), 'r.->');
if load_ref('%ans') then   pause,end,
// plots a dash-dotted line with a right-pointing triangle centered on each points.
clf_run();

// If you specify a marker wihtout a line style, only the marker is drawn
%ans = plot(x, sin(x), 'd');
if load_ref('%ans') then   pause,end,
// plots a dash-dotted line with a right-pointing triangle centered on each points.

x = 1:10;// Init.
// combinations' order does not matter
%ans = plot(x, x .* x, '*cya--');
if load_ref('%ans') then   pause,end,


//multiple plots with different LineSpecs
clf_run();
t = 0:%pi/20:2 * %pi;
%ans = plot(t, sin(t), 'ro-.', t, cos(t), 'cya+', t, abs(sin(t)), '--mo');
if load_ref('%ans') then   pause,end,


xdel_run(winsid());

mclose(%U);
