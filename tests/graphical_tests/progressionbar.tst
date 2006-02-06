getf SCI/util/testexamples.sci
reinit_for_test()
%U=mopen('SCI/tests/graphical_tests/progressionbar_data.ref','rb');
winId = progressionbar('Do something');
realtimeinit(0.3);
for j = 0:0.1:1,
  realtime(3 * j);
  progressionbar(winId);
end,
winclose(winId);
xdel_run(winsid());

mclose(%U);
