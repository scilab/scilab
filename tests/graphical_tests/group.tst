getf SCI/util/testexamples.sci
reinit_for_test()
%U=mopen('SCI/tests/graphical_tests/group_data.ref','rb');
z = poly(0, 'z');
h = z/(z - 0.5);
[tg,fr] = group(100, h);
%ans = plot(fr, tg);
if load_ref('%ans') then   pause,end,

xdel_run(winsid());

mclose(%U);
