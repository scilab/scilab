getf SCI/util/testexamples.sci
reinit_for_test()
%U=mopen('SCI/tests/automatic_tests/date_data.ref','rb');
%ans = date();
if load_ref_nocheck('%ans') then   pause,end,

xdel_run(winsid());

mclose(%U);
