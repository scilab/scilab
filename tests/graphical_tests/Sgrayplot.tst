getf SCI/util/testexamples.sci
reinit_for_test()
%U=mopen('SCI/tests/graphical_tests/Sgrayplot_data.ref','rb');
// example #1
x = -10:10;y = -10:10;m = rand(21, 21);
%ans = xbasc_run();
if load_ref('%ans') then   pause,end,

%ans = xset('colormap', hotcolormap(64));
if load_ref('%ans') then   pause,end,

%ans = Sgrayplot(x, y, m, strf='011', rect=[-20,-20,20,20]);
if load_ref('%ans') then   pause,end,


// example #2
t = -%pi:0.1:%pi;m = sin(t)' * cos(t);
%ans = xbasc_run();
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
xselect();%ans = xbasc_run();
if load_ref('%ans') then   pause,end,

fig_styl_init = get('figure_style');
if fig_styl_init == 'new' then   %ans = set('figure_style', 'old');
  if load_ref('%ans') then   pause,end,
end,
%ans = xset('font', 6, 2);
if load_ref('%ans') then   pause,end,

%ans = xset('colormap', jetcolormap(64));
if load_ref('%ans') then   pause,end,

driver('X11');xset('pixmap', 1);
for i = 1:nt,
  %ans = xbasc_run();
  if load_ref('%ans') then   pause,end,

  %ans = colorbar(-1, 1);
  if load_ref('%ans') then   pause,end,

  %ans = Sgrayplot(x, y, cos(t(i)) * z, strf='042', zminmax=[-1,1]);
  if load_ref('%ans') then   pause,end,

  %ans = xtitle('Kaa''s eyes');
  if load_ref('%ans') then   pause,end,

  %ans = xset('wshow');
  if load_ref('%ans') then   pause,end,

end,
xset('pixmap', 0);%ans = driver('Rec');
if load_ref('%ans') then   pause,end,

if fig_styl_init == 'new' then   %ans = set('figure_style', 'new');
  if load_ref('%ans') then   pause,end,
end,
xdel_run(winsid());

mclose(%U);
