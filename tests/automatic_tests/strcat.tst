getf SCI/util/testexamples.sci
reinit_for_test()
%U=mopen('SCI/tests/automatic_tests/strcat_data.ref','rb');
%ans = strcat(string(1:10), ',');
if load_ref('%ans') then   pause,end,

xdel_run(winsid());

mclose(%U);
