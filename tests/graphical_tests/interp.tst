getf SCI/util/testexamples.sci
reinit_for_test()
%U=mopen('SCI/tests/graphical_tests/interp_data.ref','rb');
// see the examples of splin and lsq_splin

// an example showing C2 and C1 continuity of spline and subspline
a = -8;b = 8;
x = linspace(a, b, 20)';
y = sinc(x);
dk = splin(x, y);// not_a_knot
df = splin(x, y, 'fast');
xx = linspace(a, b, 800)';
[yyk,yy1k,yy2k] = interp(xx, x, y, dk);
[yyf,yy1f,yy2f] = interp(xx, x, y, df);
%ans = xbasc_run();
if load_ref('%ans') then   pause,end,

%ans = subplot(3, 1, 1);
if load_ref('%ans') then   pause,end,

%ans = plot2d(xx, [yyk,yyf]);
if load_ref('%ans') then   pause,end,

%ans = plot2d(x, y, style=-9);
if load_ref('%ans') then   pause,end,

%ans = legends(['not_a_knot spline','fast sub-spline','interpolation points'], [1,2,-9], 'ur', %f);
if load_ref('%ans') then   pause,end,

%ans = xtitle('spline interpolation');
if load_ref('%ans') then   pause,end,

%ans = subplot(3, 1, 2);
if load_ref('%ans') then   pause,end,

%ans = plot2d(xx, [yy1k,yy1f]);
if load_ref('%ans') then   pause,end,

%ans = legends(['not_a_knot spline','fast sub-spline'], [1,2], 'ur', %f);
if load_ref('%ans') then   pause,end,

%ans = xtitle('spline interpolation (derivatives)');
if load_ref('%ans') then   pause,end,

%ans = subplot(3, 1, 3);
if load_ref('%ans') then   pause,end,

%ans = plot2d(xx, [yy2k,yy2f]);
if load_ref('%ans') then   pause,end,

%ans = legends(['not_a_knot spline','fast sub-spline'], [1,2], 'lr', %f);
if load_ref('%ans') then   pause,end,

%ans = xtitle('spline interpolation (second derivatives)');
if load_ref('%ans') then   pause,end,


// here is an example showing the different extrapolation possibilities
x = linspace(0, 1, 11)';
y = cosh(x - 0.5);
d = splin(x, y);
xx = linspace(-0.5, 1.5, 401)';
yy0 = interp(xx, x, y, d, 'C0');
yy1 = interp(xx, x, y, d, 'linear');
yy2 = interp(xx, x, y, d, 'natural');
yy3 = interp(xx, x, y, d, 'periodic');
%ans = xbasc_run();
if load_ref('%ans') then   pause,end,

%ans = plot2d(xx, [yy0,yy1,yy2,yy3], style=2:5, frameflag=2, leg='C0@linear@natural@periodic');
if load_ref('%ans') then   pause,end,

%ans = xtitle(' different way to evaluate a spline outside its domain');
if load_ref('%ans') then   pause,end,

xdel_run(winsid());

mclose(%U);
