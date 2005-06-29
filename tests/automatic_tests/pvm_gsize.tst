getf SCI/util/testexamples.sci
reinit_for_test()
%U=mopen('/usr/local/lib/scilab/tests/Examples/other/pvm_gsize_data.ref','r');
nb_worker = pvm_gsize('worker');
if load_ref('nb_worker') then   pause,end,

xdel_run(winsid());

mclose(%U);
