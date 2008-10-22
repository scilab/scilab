getf SCI/util/testexamples.sci
reinit_for_test()
%U=mopen('SCI/tests/automatic_tests/tic_data.ref','rb');
tic();
realtimeinit(1);
realtime(0);
realtime(10);
toc();
xdel_run(winsid());

mclose(%U);
