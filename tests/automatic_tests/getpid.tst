getf SCI/util/testexamples.sci
reinit_for_test()
%U=mopen('/usr/local/lib/scilab/tests/Examples/other/getpid_data.ref','r');
d = 'SD_' + string(getpid()) + '_';
if load_ref_nocheck('d') then   pause,end,

xdel_run(winsid());

mclose(%U);
