getf SCI/util/testexamples.sci
reinit_for_test()
%U=mopen('SCI/tests/graphical_tests/plot2d2_data.ref','rb');

// plots a step function of value i on the segment [i,i+1]
// the last segment is not drawn
%ans = plot2d2(1:4, 1:4, 1, '111', 'step function', [0,0,5,5]);
if load_ref('%ans') then   pause,end,

// compare the following with plot2d
x = (0:0.1:2 * %pi)';
%ans = clf_run();
if load_ref('%ans') then   pause,end,

%ans = plot2d2(x, [sin(x),sin(2 * x),sin(3 * x)]);
if load_ref('%ans') then   pause,end,

// In New graphics only
%ans = clf_run();
if load_ref('%ans') then   pause,end,

%ans = set('figure_style', 'new');
if load_ref('%ans') then   pause,end,

%ans = plot2d(x, [sin(x),sin(2 * x),sin(3 * x)]);
if load_ref('%ans') then   pause,end,

e = gce();
e.children(1).polyline_style = 2;
e.children(2).polyline_style = 2;
e.children(3).polyline_style = 2;

xdel_run(winsid());

mclose(%U);
