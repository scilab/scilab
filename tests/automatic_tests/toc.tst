getf SCI/util/testexamples.sci
reinit_for_test()
%U=mopen('SCI/tests/Examples_eng/other/toc_data.ref','r');
tic();
realtimeinit(1);
realtime(0);
realtime(10);
toc();
xdel_run(winsid());

mclose(%U);
