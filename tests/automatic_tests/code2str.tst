getf SCI/util/testexamples.sci
reinit_for_test()
%U=mopen('SCI/tests/automatic_tests/code2str_data.ref','rb');
%ans = code2str([-28,12,18,21,10,11]);
if load_ref('%ans') then   pause,end,

xdel_run(winsid());

mclose(%U);
