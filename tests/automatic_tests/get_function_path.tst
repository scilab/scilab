getf SCI/util/testexamples.sci
reinit_for_test()
%U=mopen('SCI/tests/automatic_tests/get_function_path_data.ref','rb');
%ans = get_function_path('median');
if load_ref_nocheck('%ans') then   pause,end,

xdel_run(winsid());

mclose(%U);
