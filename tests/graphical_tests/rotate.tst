getf SCI/util/testexamples.sci
reinit_for_test()
%U=mopen('SCI/tests/graphical_tests/rotate_data.ref','rb');
%ans = xsetech([0,0,1,1], [-1,-1,1,1]);
if load_ref('%ans') then   pause,end,

xy = [0:0.1:10;sin(0:0.1:10)]/10;
for i = 2 * %pi * (0:10)/10,
  xy1 = rotate(xy, i);
  %ans = xpoly(xy1(1, :), xy1(2, :), 'lines');
  if load_ref('%ans') then   pause,end,

end,
xdel_run(winsid());

mclose(%U);
