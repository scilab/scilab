getf SCI/util/testexamples.sci
reinit_for_test()
%U=mopen('/usr/local/lib/scilab/tests/Examples/other/pvm_lvgroup_data.ref','r');
info = pvm_lvgroup('worker');
if load_ref('info') then   pause,end,

xdel_run(winsid());

mclose(%U);
