getf SCI/util/testexamples.sci
reinit_for_test()
%U=mopen('SCI/tests/automatic_tests/pvm_get_timer_data.ref','rb');
B = rand(100, 100);
A = rand(100, 100);
pvm_set_timer();C = A * B;t = pvm_get_timer();
if load_ref_nocheck('t') then   pause,end,

xdel_run(winsid());

mclose(%U);
