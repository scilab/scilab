getf SCI/util/testexamples.sci
reinit_for_test()
%U=mopen('SCI/tests/graphical_tests/hsv2rgb_data.ref','rb');
t = (0:0.3:2 * %pi)';z = sin(t) * cos(t');
%ans = plot3d1(t, t, z);
if load_ref('%ans') then   pause,end,

f = gcf();f('pixmap') = 'on';
for h = 0:0.1:1,
  hsv = [h * ones(32, 1),linspace(0, 1, 32)',0.7 * ones(32, 1)];
  f('color_map') = hsv2rgb(hsv);
  %ans = show_pixmap();
  if load_ref('%ans') then   pause,end,

  %ans = xpause(10000);
  if load_ref('%ans') then   pause,end,

end,
for v = 0:0.1:1,
  hsv = [ones(32, 1),linspace(0, 1, 32)',v * ones(32, 1)];
  f('color_map') = hsv2rgb(hsv);
  %ans = show_pixmap();
  if load_ref('%ans') then   pause,end,

  %ans = xpause(10000);
  if load_ref('%ans') then   pause,end,

end,
xdel_run(winsid());

mclose(%U);
