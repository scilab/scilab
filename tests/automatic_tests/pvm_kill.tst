getf SCI/util/testexamples.sci
reinit_for_test()
%U=mopen('/usr/local/lib/scilab/tests/Examples/other/pvm_kill_data.ref','r');
info = pvm_kill(262153);
if load_ref('info') then   pause,end,

xdel_run(winsid());

mclose(%U);
