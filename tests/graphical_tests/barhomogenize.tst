getf SCI/util/testexamples.sci
reinit_for_test()
%U=mopen('SCI/tests/graphical_tests/barhomogenize_data.ref','rb');

// First example: creation of 1 yellow bar (i.e 1 polyline with polyline_style=6) and 3 bars (i.e 3 polylines with polyline_style=6)
%ans = subplot(2, 3, 1);
if load_ref('%ans') then   pause,end,

%ans = xtitle('ex1: creation of 1 yellow bar and 3 bars ');
if load_ref('%ans') then   pause,end,

x = 1:3;y1 = 1:3;y2 = [4,3,5;6,7,8;9,10,11];
bar(x, y1, 'yellow');bar(x, y2);
// grouped homogenisation of these 4 bars
%ans = subplot(2, 3, 2);
if load_ref('%ans') then   pause,end,

%ans = xtitle('grouped homogenisation');
if load_ref('%ans') then   pause,end,

x = 1:3;y1 = 1:3;y2 = [4,3,5;6,7,8;9,10,11];
bar(x, y1, 'yellow');bar(x, y2);
barhomogenize();
// stacked homogenisation of thes 4 bars
%ans = subplot(2, 3, 3);
if load_ref('%ans') then   pause,end,

%ans = xtitle('stacked homogenisation');
if load_ref('%ans') then   pause,end,

x = 1:3;y1 = 1:3;y2 = [4,3,5;6,7,8;9,10,11];
bar(x, y1, 'yellow');bar(x, y2);
barhomogenize('stacked', 1);

// Second example : creation of 1 red bar (i.e 1 polyline with polyline_style=6) and 2 polylines with type=1 (calling plot function)
%ans = subplot(2, 3, 4);
if load_ref('%ans') then   pause,end,

%ans = xtitle('ex2: creation of 1 bar and 2 polylines');
if load_ref('%ans') then   pause,end,

x = 1:10;y = sin(x)/2;
%ans = bar(x, y, 'red');
if load_ref('%ans') then   pause,end,

x1 = 1:10;y1 = [sin(x);cos(x)];
if load_ref('y1') then   pause,end,

%ans = plot(x1, y1);
if load_ref('%ans') then   pause,end,

// modify the polyline_style type of the second polyline from plot (this polyline becomes a bar)
%ans = subplot(2, 3, 5);
if load_ref('%ans') then   pause,end,

%ans = xtitle('transformation of the second polyline to bar');
if load_ref('%ans') then   pause,end,

x = 1:10;y = sin(x)/2;
%ans = bar(x, y, 'red');
if load_ref('%ans') then   pause,end,

x1 = 1:10;y1 = [sin(x);cos(x)];
if load_ref('y1') then   pause,end,

%ans = plot(x1, y1);
if load_ref('%ans') then   pause,end,

e = gce();e2 = e.children(2);e2('polyline_style') = 6;
// homogenisation of the first bar (from bar function) and second bar (from the modification).
%ans = subplot(2, 3, 6);
if load_ref('%ans') then   pause,end,

%ans = xtitle('grouped homogenisation');
if load_ref('%ans') then   pause,end,

x = 1:10;y = sin(x)/2;
%ans = bar(x, y, 'red');
if load_ref('%ans') then   pause,end,

x1 = 1:10;y1 = [sin(x);cos(x)];
if load_ref('y1') then   pause,end,

%ans = plot(x1, y1);
if load_ref('%ans') then   pause,end,

e = gce();e2 = e.children(2);e2('polyline_style') = 6;
barhomogenize();
// change the style and the width
//barhomogenize('stacked',0.5);
//barhomogenize('stacked',1);
xdel_run(winsid());

mclose(%U);
