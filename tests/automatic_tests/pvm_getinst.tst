getf SCI/util/testexamples.sci
reinit_for_test()
%U=mopen('/usr/local/lib/scilab/tests/Examples/other/pvm_getinst_data.ref','r');
inum = pvm_getinst('worker', pvm_mytid());
if load_ref('inum') then   pause,end,

xdel_run(winsid());

mclose(%U);
