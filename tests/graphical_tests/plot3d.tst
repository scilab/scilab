getf SCI/util/testexamples.sci
reinit_for_test()
%U=mopen('SCI/tests/graphical_tests/plot3d_data.ref','rb');
// simple plot using z=f(x,y)
t = (0:0.3:2 * %pi)';
z = sin(t) * cos(t');
%ans = plot3d(t, t, z);
if load_ref('%ans') then   pause,end,

// same plot using facets computed by genfac3d
[xx,yy,zz] = genfac3d(t, t, z);
%ans = clf_run();
if load_ref('%ans') then   pause,end,

%ans = plot3d(xx, yy, zz);
if load_ref('%ans') then   pause,end,

// multiple plots
%ans = clf_run();
if load_ref('%ans') then   pause,end,

%ans = plot3d([xx,xx], [yy,yy], [zz,4 + zz]);
if load_ref('%ans') then   pause,end,

// multiple plots using colors
%ans = clf_run();
if load_ref('%ans') then   pause,end,

%ans = plot3d([xx,xx], [yy,yy], list([zz,zz + 4], [4 * ones(1, 400),5 * ones(1, 400)]));
if load_ref('%ans') then   pause,end,

// simple plot with viewpoint and captions
%ans = clf_run();
if load_ref('%ans') then   pause,end,

%ans = plot3d(1:10, 1:20, 10 * rand(10, 20), alpha=35, theta=45, flag=[2,2,3]);
if load_ref('%ans') then   pause,end,

// plot of a sphere using facets computed by eval3dp
deff('[x,y,z]=sph(alp,tet)', ['x=r*cos(alp).*cos(tet)+orig(1)*ones(tet)';'y=r*cos(alp).*sin(tet)+orig(2)*ones(tet)';'z=r*sin(alp)+orig(3)*ones(tet)']);
r = 1;orig = [0,0,0];
[xx,yy,zz] = eval3dp(sph, linspace(-%pi/2, %pi/2, 40), linspace(0, %pi * 2, 20));
clf_run();%ans = plot3d(xx, yy, zz);
if load_ref('%ans') then   pause,end,

clf_run();
f = gcf();
f('color_map') = hotcolormap(128);
r = 0.3;orig = [1.5,0,0];
[xx1,yy1,zz1] = eval3dp(sph, linspace(-%pi/2, %pi/2, 40), linspace(0, %pi * 2, 20));
cc = (xx + zz + 2) * 32;cc1 = (xx1 - orig(1) + zz1/r + 2) * 32;
clf_run();%ans = plot3d1([xx,xx1], [yy,yy1], list([zz,zz1], [cc,cc1]), theta=70, alpha=80, flag=[5,6,3]);
if load_ref('%ans') then   pause,end,


//Available operations using only New Graphics...
delete(gcf());
t = (0:0.3:2 * %pi)';z = sin(t) * cos(t');
[xx,yy,zz] = genfac3d(t, t, z);
%ans = plot3d([xx,xx], [yy,yy], list([zz,zz + 4], [4 * ones(1, 400),5 * ones(1, 400)]));
if load_ref('%ans') then   pause,end,

e = gce();
f = e('data');
TL = tlist(['3d','x','y','z','color'], f('x'), f('y'), f('z'), 6 * rand(f('z')));// random color matrix
e('data') = TL;
TL2 = tlist(['3d','x','y','z','color'], f('x'), f('y'), f('z'), 4 * rand(1, 800));// random color vector
e('data') = TL2;
TL3 = tlist(['3d','x','y','z','color'], f('x'), f('y'), f('z'), [20 * ones(1, 400),6 * ones(1, 400)]);
e('data') = TL3;
TL4 = tlist(['3d','x','y','z'], f('x'), f('y'), f('z'));// no color
e('data') = TL4;
e('color_flag') = 1;
if load_ref('e') then   pause,end,
// color index proportional to altitude (z coord.)
e('color_flag') = 2;// back to default mode
e('color_flag') = 3;// interpolated shading mode (based on blue default color)
%ans = clf_run();
if load_ref('%ans') then   pause,end,

%ans = plot3d([xx,xx], [yy,yy], list([zz,zz + 4], [4 * ones(1, 400),5 * ones(1, 400)]));
if load_ref('%ans') then   pause,end,

h = gce();//get handle on current entity (here the surface)
a = gca();//get current axes
a('rotation_angles') = [40,70];
a('grid') = [1,1,1];//make grids
a('data_bounds') = [-6,6;6,-1;0,5];
a('axes_visible') = 'off';//axes are hidden
a('axes_bounds') = [0.2,0,1,1];
h('color_flag') = 1;//color according to z
h('color_mode') = -2;//remove the facets boundary by setting color_mode to white color
h('color_flag') = 2;//color according to given colors
h('color_mode') = -1;// put the facets boundary back by setting color_mode to black color
f = gcf();//get the handle of the parent figure
f('color_map') = hotcolormap(512);
c = [1:400,1:400];
TL('color') = [c;c + 1;c + 2;c + 3];
h('data') = TL;
h('color_flag') = 3;// interpolated shading mode

xdel_run(winsid());

mclose(%U);
