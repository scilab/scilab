getf SCI/util/testexamples.sci
reinit_for_test()
%U=mopen('SCI/tests/graphical_tests/bessel_data.ref','rb');
//  besselI functions
// ==================
x = linspace(0.01, 10, 5000)';
%ans = xbasc_run();
if load_ref('%ans') then   pause,end,

%ans = subplot(2, 1, 1);
if load_ref('%ans') then   pause,end,

%ans = plot2d(x, besseli(0:4, x), style=2:6);
if load_ref('%ans') then   pause,end,

legend('I' + string(0:4), 2);
%ans = xtitle('Some modified Bessel functions of the first kind');
if load_ref('%ans') then   pause,end,

%ans = subplot(2, 1, 2);
if load_ref('%ans') then   pause,end,

%ans = plot2d(x, besseli(0:4, x, 1), style=2:6);
if load_ref('%ans') then   pause,end,

legend('I' + string(0:4), 1);
%ans = xtitle('Some modified scaled Bessel functions of the first kind');
if load_ref('%ans') then   pause,end,


// besselJ functions
// =================
x = linspace(0, 40, 5000)';
%ans = xbasc_run();
if load_ref('%ans') then   pause,end,

%ans = plot2d(x, besselj(0:4, x), style=2:6, leg='J0@J1@J2@J3@J4');
if load_ref('%ans') then   pause,end,

legend('I' + string(0:4), 1);
%ans = xtitle('Some Bessel functions of the first kind');
if load_ref('%ans') then   pause,end,


// use the fact that J_(1/2)(x) = sqrt(2/(x pi)) sin(x)
// to compare the algorithm of besselj(0.5,x) with a more direct formula
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



// besselK functions
// =================
x = linspace(0.01, 10, 5000)';
%ans = xbasc_run();
if load_ref('%ans') then   pause,end,

%ans = subplot(2, 1, 1);
if load_ref('%ans') then   pause,end,

%ans = plot2d(x, besselk(0:4, x), style=2:6, rect=[0,0,6,10]);
if load_ref('%ans') then   pause,end,

legend('K' + string(0:4), 1);
%ans = xtitle('Some modified Bessel functions of the second kind');
if load_ref('%ans') then   pause,end,

%ans = subplot(2, 1, 2);
if load_ref('%ans') then   pause,end,

%ans = plot2d(x, besselk(0:4, x, 1), style=2:6, rect=[0,0,6,10]);
if load_ref('%ans') then   pause,end,

legend('K' + string(0:4), 1);
%ans = xtitle('Some modified scaled Bessel functions of the second kind');
if load_ref('%ans') then   pause,end,


// besselY functions
// =================
x = linspace(0.1, 40, 5000)';// Y Bessel functions are unbounded  for x -> 0+
%ans = xbasc_run();
if load_ref('%ans') then   pause,end,

%ans = plot2d(x, bessely(0:4, x), style=2:6, rect=[0,-1.5,40,0.6]);
if load_ref('%ans') then   pause,end,

legend('Y' + string(0:4), 4);
%ans = xtitle('Some Bessel functions of the second kind');
if load_ref('%ans') then   pause,end,


// besselH functions
// =================
x = -4:0.025:2;y = -1.5:0.025:1.5;
[X,Y] = ndgrid(x, y);
H = besselh(0, 1, X + %i * Y);
clf_run();f = gcf();
%ans = xset('fpf', ' ');
if load_ref('%ans') then   pause,end,

f('color_map') = jetcolormap(16);
%ans = contour2d(x, y, abs(H), 0.2:0.2:3.2, strf='034', rect=[-4,-1.5,3,1.5]);
if load_ref('%ans') then   pause,end,

%ans = legends(string(0.2:0.2:3.2), 1:16, 'ur');
if load_ref('%ans') then   pause,end,

%ans = xtitle('Level curves of |H1(0,z)|');
if load_ref('%ans') then   pause,end,

xdel_run(winsid());

mclose(%U);
