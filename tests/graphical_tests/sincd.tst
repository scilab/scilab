getf SCI/util/testexamples.sci
reinit_for_test()
%U=mopen('SCI/tests/graphical_tests/sincd_data.ref','rb');
%ans = plot(sincd(10, 1));
if load_ref('%ans') then   pause,end,

xdel_run(winsid());

mclose(%U);
