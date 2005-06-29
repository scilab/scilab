getf SCI/util/testexamples.sci
reinit_for_test()
%U=mopen('/usr/local/lib/scilab/tests/Examples/other/pvm_addhosts_data.ref','r');
info = pvm_addhosts(['isostar','loop']);
if load_ref('info') then   pause,end,

xdel_run(winsid());

mclose(%U);
