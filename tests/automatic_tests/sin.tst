getf SCI/util/testexamples.sci
reinit_for_test()
%U=mopen('SCI/tests/automatic_tests/sin_data.ref','rb');
%ans = asin(sin([1,0,%i]));
if load_ref('%ans') then   pause,end,

xdel_run(winsid());

mclose(%U);
