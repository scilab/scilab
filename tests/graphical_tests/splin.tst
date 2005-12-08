getf SCI/util/testexamples.sci
reinit_for_test()
%U=mopen('SCI/tests/graphical_tests/splin_data.ref','rb');
// example 1
%ans = deff('y=runge(x)', 'y=1 ./(1 + x.^2)');
if load_ref('%ans') then   pause,end,

a = -5;b = 5;n = 11;m = 400;
x = linspace(a, b, n)';
y = runge(x);
d = splin(x, y);
xx = linspace(a, b, m)';
yyi = interp(xx, x, y, d);
yye = runge(xx);
%ans = xbasc_run();
if load_ref('%ans') then   pause,end,

%ans = plot2d(xx, [yyi,yye], style=[2,5], leg='interpolation spline@exact function');
if load_ref('%ans') then   pause,end,

%ans = plot2d(x, y, -9);
if load_ref('%ans') then   pause,end,

%ans = xtitle('interpolation of the Runge function');
if load_ref('%ans') then   pause,end,


// example 2 : show behavior of different splines on random datas
a = 0;b = 1;// interval of interpolation
n = 10;// nb of interpolation  points
m = 800;// discretisation for evaluation
x = linspace(a, b, n)';// abscissae of interpolation points
y = rand(x);// ordinates of interpolation points
xx = linspace(a, b, m)';
yk = interp(xx, x, y, splin(x, y, 'not_a_knot'));
yf = interp(xx, x, y, splin(x, y, 'fast'));
ym = interp(xx, x, y, splin(x, y, 'monotone'));
%ans = xbasc_run();
if load_ref('%ans') then   pause,end,

%ans = plot2d(xx, [yf,ym,yk], style=[5,2,3], strf='121', leg='fast@monotone@not a knot spline');
if load_ref('%ans') then   pause,end,

%ans = plot2d(x, y, -9, strf='000');
if load_ref('%ans') then   pause,end,
// to show interpolation points
%ans = xtitle('Various spline and sub-splines on random datas');
if load_ref('%ans') then   pause,end,

%ans = xselect();
if load_ref('%ans') then   pause,end,

xdel_run(winsid());

mclose(%U);
