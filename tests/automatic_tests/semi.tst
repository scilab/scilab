getf SCI/util/testexamples.sci
reinit_for_test()
%U=mopen('SCI/tests/automatic_tests/semi_data.ref','rb');
a = [1,2,3;4,5,6];
a = 1;b = 1;
if load_ref('b') then   pause,end,
c = 2;
if load_ref('c') then   pause,end,

xdel_run(winsid());

mclose(%U);
