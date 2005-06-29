getf SCI/util/testexamples.sci
reinit_for_test()
%U=mopen('/usr/local/lib/scilab/tests/Examples/other/strcat_data.ref','r');
%ans = strcat(string(1:10), ',');
if load_ref('%ans') then   pause,end,

xdel_run(winsid());

mclose(%U);
