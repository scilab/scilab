getf SCI/util/testexamples.sci
reinit_for_test()
%U=mopen('SCI/tests/graphical_tests/Sgrayplot_data.ref','rb');
// example #1
x = -10:10;y = -10:10;m = rand(21, 21);
%ans = clf_run();
if load_ref('%ans') then   pause,end,

%ans = xset('colormap', hotcolormap(64));
if load_ref('%ans') then   pause,end,

%ans = Sgrayplot(x, y, m, strf='011', rect=[-20,-20,20,20]);
if load_ref('%ans') then   pause,end,


// example #2
t = -%pi:0.1:%pi;m = sin(t)' * cos(t);
%ans = clf_run();
if load_ref('%ans') then   pause,end,

%ans = xset('colormap', jetcolormap(64));
if load_ref('%ans') then   pause,end,

%ans = colorbar(-1, 1);
if load_ref('%ans') then   pause,end,

%ans = Sgrayplot(t, t, m, strf='041');
if load_ref('%ans') then   pause,end,


// example #3: an animation display cos(t)*sin(x)sin(y).
// (Remark: this animation uses the old mode)
n = 30;
nt = 100;
x = linspace(0, 2 * %pi, n);
y = linspace(0, %pi, n/2);
z = sin(x') * sin(y);
t = linspace(0, 4 * %pi, nt);
xselect();%ans = clf_run();
if load_ref('%ans') then   pause,end,

f = gcf();
f('color_map') = jetcolormap(64);
f('pixmap') = 'on';
%ans = colorbar(-1, 1);
if load_ref('%ans') then   pause,end,

%ans = Sgrayplot(x, y, cos(t(1)) * z, strf='042', zminmax=[-1,1]);
if load_ref('%ans') then   pause,end,

c = gce();
if load_ref('c') then   pause,end,
e = c('children');
if load_ref('e') then   pause,end,

%ans = xtitle('Kaa''s eyes');
if load_ref('%ans') then   pause,end,

for i = 1:nt,
  e.data(eye(), 3) = matrix(cos(t(i)) * z, -1, 1);
  %ans = show_pixmap();
  if load_ref('%ans') then   pause,end,

end,
f('pixmap') = 'off';
xdel_run(winsid());

mclose(%U);
