getf SCI/util/testexamples.sci
reinit_for_test()
%U=mopen('SCI/tests/automatic_tests/sleep_data.ref','rb');
tic;sleep(6000);toc;
if load_ref('%ans') then   pause,end,

xdel_run(winsid());

mclose(%U);
