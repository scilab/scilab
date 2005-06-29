getf SCI/util/testexamples.sci
reinit_for_test()
%U=mopen('SCI/tests/Examples_eng/other/pvm_set_timer_data.ref','r');
%ans = pvm_set_timer();
if load_ref('%ans') then   pause,end,

xdel_run(winsid());

mclose(%U);
