getf SCI/util/testexamples.sci
reinit_for_test()
%U=mopen('/usr/local/lib/scilab/tests/Examples/other/savehistory_data.ref','r');
%ans = savehistory(SCI + '/session.scilab');
if load_ref('%ans') then   pause,end,

xdel_run(winsid());

mclose(%U);
