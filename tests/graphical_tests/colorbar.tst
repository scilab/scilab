getf SCI/util/testexamples.sci
reinit_for_test()
%U=mopen('SCI/tests/graphical_tests/colorbar_data.ref','rb');
// example 1
x = linspace(0, 1, 81);
z = cos(2 * %pi * x)' * sin(2 * %pi * x);
zm = min(z);zM = max(z);
%ans = xbasc_run();
if load_ref('%ans') then   pause,end,

%ans = xset('colormap', jetcolormap(64));
if load_ref('%ans') then   pause,end,

%ans = colorbar(zm, zM);
if load_ref('%ans') then   pause,end,

%ans = Sgrayplot(x, x, z);
if load_ref('%ans') then   pause,end,

%ans = xtitle('The function cos(2 pi x)sin(2 pi y)');
if load_ref('%ans') then   pause,end,


// example 2
x = linspace(0, 1, 81);
z = cos(2 * %pi * x)' * sin(2 * %pi * x);
zm = min(z);zM = max(z);
zz = abs(0.5 * cos(2 * %pi * x)' * cos(2 * %pi * x));
zzm = min(zz);zzM = max(zz);
xbasc_run();
%ans = xset('colormap', jetcolormap(64));
if load_ref('%ans') then   pause,end,

new_styl = get('figure_style') == 'new';
if new_styl then   %ans = drawlater();
  if load_ref('%ans') then   pause,end,
end,
%ans = subplot(2, 2, 1);
if load_ref('%ans') then   pause,end,

%ans = colorbar(zm, zM);
if load_ref('%ans') then   pause,end,

%ans = Sgrayplot(x, x, z, strf='031', rect=[0,0,1,1]);
if load_ref('%ans') then   pause,end,

%ans = xtitle('a Sgrayplot with a colorbar');
if load_ref('%ans') then   pause,end,

%ans = subplot(2, 2, 2);
if load_ref('%ans') then   pause,end,

%ans = colorbar(zm, zM);
if load_ref('%ans') then   pause,end,

%ans = plot3d1(x, x, z);
if load_ref('%ans') then   pause,end,

%ans = xtitle('a plot3d1 with a colorbar');
if load_ref('%ans') then   pause,end,

%ans = subplot(2, 2, 3);
if load_ref('%ans') then   pause,end,

%ans = colorbar(zzm, zzM);
if load_ref('%ans') then   pause,end,

%ans = plot3d1(x, x, zz);
if load_ref('%ans') then   pause,end,

%ans = xtitle('a plot3d1 with a colorbar');
if load_ref('%ans') then   pause,end,

%ans = subplot(2, 2, 4);
if load_ref('%ans') then   pause,end,

%ans = colorbar(zzm, zzM);
if load_ref('%ans') then   pause,end,

%ans = Sgrayplot(x, x, zz, strf='031', rect=[0,0,1,1]);
if load_ref('%ans') then   pause,end,

%ans = xtitle('a Sgrayplot with a colorbar');
if load_ref('%ans') then   pause,end,

if new_styl then   %ans = drawnow();
  if load_ref('%ans') then   pause,end,
end,


// example 3
x = linspace(0, 1, 81);
zz = abs(0.5 * cos(2 * %pi * x)' * cos(2 * %pi * x));
zzm = min(zz);zzM = max(zz);
[xf,yf,zf] = genfac3d(x, x, zz);
nb_col = 64;
%ans = xbasc_run();
if load_ref('%ans') then   pause,end,

%ans = xset('colormap', hotcolormap(nb_col));
if load_ref('%ans') then   pause,end,

new_styl = get('figure_style') == 'new';
if new_styl then   %ans = drawlater();
  if load_ref('%ans') then   pause,end,
end,
%ans = colorbar(zzm, zzM);
if load_ref('%ans') then   pause,end,

nbcol = xget('lastpattern');
if load_ref('nbcol') then   pause,end,

zcol = dsearch(zf, linspace(zzm, zzM, nb_col + 1));
%ans = plot3d(xf, yf, list(zf, zcol), flag=[-2,6,4]);
if load_ref('%ans') then   pause,end,

%ans = xtitle('a plot3d with shaded interpolated colors');
if load_ref('%ans') then   pause,end,

if new_styl then   %ans = drawnow();
  if load_ref('%ans') then   pause,end,
end,
%ans = xselect();
if load_ref('%ans') then   pause,end,

xdel_run(winsid());

mclose(%U);
