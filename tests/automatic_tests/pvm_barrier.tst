getf SCI/util/testexamples.sci
reinit_for_test()
%U=mopen('SCI/tests/Examples_eng/other/pvm_barrier_data.ref','r');
info = pvm_barrier('worker', pvm_gsize('worker'));
if load_ref('info') then   pause,end,

xdel_run(winsid());

mclose(%U);
