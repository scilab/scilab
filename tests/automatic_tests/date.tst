getf SCI/util/testexamples.sci
reinit_for_test()
%U=mopen('/usr/local/lib/scilab/tests/Examples/other/date_data.ref','r');
%ans = date();
if load_ref_nocheck('%ans') then   pause,end,

xdel_run(winsid());

mclose(%U);
