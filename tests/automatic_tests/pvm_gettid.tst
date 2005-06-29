getf SCI/util/testexamples.sci
reinit_for_test()
%U=mopen('SCI/tests/Examples_eng/other/pvm_gettid_data.ref','r');
tid = pvm_gettid('worker', 0);
if load_ref('tid') then   pause,end,

xdel_run(winsid());

mclose(%U);
