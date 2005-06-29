getf SCI/util/testexamples.sci
reinit_for_test()
%U=mopen('SCI/tests/Examples_eng/other/comma_data.ref','r');
a = [1,2,3;4,5,6];
a = 1;
if load_ref('a') then   pause,end,
b = 1;c = 2;
if load_ref('c') then   pause,end,

xdel_run(winsid());

mclose(%U);
