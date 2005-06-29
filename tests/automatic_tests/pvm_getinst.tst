getf SCI/util/testexamples.sci
reinit_for_test()
%U=mopen('SCI/tests/Examples_eng/other/pvm_getinst_data.ref','r');
inum = pvm_getinst('worker', pvm_mytid());
if load_ref('inum') then   pause,end,

xdel_run(winsid());

mclose(%U);
