getf SCI/util/testexamples.sci
reinit_for_test()
%U=mopen('SCI/tests/Examples_eng/other/pvm_get_timer_data.ref','r');
B = rand(100, 100);
A = rand(100, 100);
pvm_set_timer();C = A * B;t = pvm_get_timer();
if load_ref_nocheck('t') then   pause,end,

xdel_run(winsid());

mclose(%U);
