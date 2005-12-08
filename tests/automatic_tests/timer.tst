getf SCI/util/testexamples.sci
reinit_for_test()
%U=mopen('SCI/tests/automatic_tests/timer_data.ref','rb');
timer();A = rand(100, 100);%ans = timer();
if load_ref_nocheck('%ans') then   pause,end,

xdel_run(winsid());

mclose(%U);
