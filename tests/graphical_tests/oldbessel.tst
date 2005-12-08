getf SCI/util/testexamples.sci
reinit_for_test()
%U=mopen('SCI/tests/graphical_tests/oldbessel_data.ref','rb');
// example #1: display some I Bessel functions
x = linspace(0.01, 10, 5000)';
y = oldbesseli(0:4, x);
ys = oldbesseli(0:4, x, 2);
%ans = xbasc_run();
if load_ref('%ans') then   pause,end,

%ans = subplot(2, 1, 1);
if load_ref('%ans') then   pause,end,

%ans = plot2d(x, y, style=2:6, leg='I0@I1@I2@I3@I4', rect=[0,0,6,10]);
if load_ref('%ans') then   pause,end,

%ans = xtitle('Some modified Bessel functions of the first kind');
if load_ref('%ans') then   pause,end,

%ans = subplot(2, 1, 2);
if load_ref('%ans') then   pause,end,

%ans = plot2d(x, ys, style=2:6, leg='I0s@I1s@I2s@I3s@I4s', rect=[0,0,6,1]);
if load_ref('%ans') then   pause,end,

%ans = xtitle('Some modified scaled Bessel functions of the first kind');
if load_ref('%ans') then   pause,end,


// example #2 : display some J Bessel functions
x = linspace(0, 40, 5000)';
y = besselj(0:4, x);
%ans = xbasc_run();
if load_ref('%ans') then   pause,end,

%ans = plot2d(x, y, style=2:6, leg='J0@J1@J2@J3@J4');
if load_ref('%ans') then   pause,end,

%ans = xtitle('Some Bessel functions of the first kind');
if load_ref('%ans') then   pause,end,


// example #3 : use the fact that J_(1/2)(x) = sqrt(2/(x pi)) sin(x)
//              to compare the algorithm of besselj(0.5,x) with
//              a more direct formula
x = linspace(0.1, 40, 5000)';
y1 = besselj(0.5, x);
y2 = sqrt(2 ./ (%pi * x)) .* sin(x);
er = abs((y1 - y2) ./ y2);
ind = find((er > 0) & (y2 ~= 0));
%ans = xbasc_run();
if load_ref('%ans') then   pause,end,

%ans = subplot(2, 1, 1);
if load_ref('%ans') then   pause,end,

%ans = plot2d(x, y1, style=2);
if load_ref('%ans') then   pause,end,

%ans = xtitle('besselj(0.5,x)');
if load_ref('%ans') then   pause,end,

%ans = subplot(2, 1, 2);
if load_ref('%ans') then   pause,end,

%ans = plot2d(x(ind), er(ind), style=2, logflag='nl');
if load_ref('%ans') then   pause,end,

%ans = xtitle('relative error between 2 formulae for besselj(0.5,x)');
if load_ref('%ans') then   pause,end,


// example #4: display some K Bessel functions
x = linspace(0.01, 10, 5000)';
y = besselk(0:4, x);
ys = besselk(0:4, x, 2);
%ans = xbasc_run();
if load_ref('%ans') then   pause,end,

%ans = subplot(2, 1, 1);
if load_ref('%ans') then   pause,end,

%ans = plot2d(x, y, style=2:6, leg='K0@K1@K2@K3@K4', rect=[0,0,6,10]);
if load_ref('%ans') then   pause,end,

%ans = xtitle('Some modified Bessel functions of the second kind');
if load_ref('%ans') then   pause,end,

%ans = subplot(2, 1, 2);
if load_ref('%ans') then   pause,end,

%ans = plot2d(x, ys, style=2:6, leg='K0s@K1s@K2s@K3s@K4s', rect=[0,0,6,10]);
if load_ref('%ans') then   pause,end,

%ans = xtitle('Some modified scaled Bessel functions of the second kind');
if load_ref('%ans') then   pause,end,


// example #5: plot severals Y Bessel functions
x = linspace(0.1, 40, 5000)';// Y Bessel functions are unbounded  for x -> 0+
y = bessely(0:4, x);
%ans = xbasc_run();
if load_ref('%ans') then   pause,end,

%ans = plot2d(x, y, style=2:6, leg='Y0@Y1@Y2@Y3@Y4', rect=[0,-1.5,40,0.6]);
if load_ref('%ans') then   pause,end,

%ans = xtitle('Some Bessel functions of the second kind');
if load_ref('%ans') then   pause,end,

xdel_run(winsid());

mclose(%U);
