getf SCI/util/testexamples.sci
reinit_for_test()
%U=mopen('SCI/tests/graphical_tests/geom3d_data.ref','rb');
%ans = deff('[z]=surf(x,y)', 'z=sin(x)*cos(y)');
if load_ref('%ans') then   pause,end,

t = %pi * (-10:10)/10;
// 3D plot of the surface
%ans = fplot3d(t, t, surf, 35, 45, 'X@Y@Z');
if load_ref('%ans') then   pause,end,

// now (t,t,sin(t).*cos(t)) is a curve on the surface
// which can be drawn using geom3d and xpoly
[x,y] = geom3d(t, t, sin(t) .* cos(t));
%ans = xpoly(x, y, 'lines');
if load_ref('%ans') then   pause,end,

// adding a comment
[x,y] = geom3d([0,0], [0,0], [5,0]);
%ans = xsegs(x, y);
if load_ref('%ans') then   pause,end,

%ans = xstring(x(1), y(1), 'point (0,0,0)');
if load_ref('%ans') then   pause,end,

xdel_run(winsid());

mclose(%U);
