getf SCI/util/testexamples.sci
reinit_for_test()
%U=mopen('SCI/tests/graphical_tests/nf3d_data.ref','rb');
//A sphere...
u = linspace(-%pi/2, %pi/2, 40);
v = linspace(0, 2 * %pi, 20);
x = cos(u)' * cos(v);
y = cos(u)' * sin(v);
z = sin(u)' * ones(v);
//plot3d2(x,y,z) is equivalent to...
[xx,yy,zz] = nf3d(x, y, z);%ans = plot3d(xx, yy, zz);
if load_ref('%ans') then   pause,end,

xdel_run(winsid());

mclose(%U);
