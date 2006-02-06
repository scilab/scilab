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
[x,y] = geom3d(%pi/2, 0, surf(%pi/2, 0));
if load_ref('y') then   pause,end,
if load_ref('x') then   pause,end,


xdel_run(winsid());

mclose(%U);
