getf SCI/util/testexamples.sci
reinit_for_test()
%U=mopen('SCI/tests/automatic_tests/number_properties_data.ref','rb');
b = number_properties('radix');
if load_ref('b') then   pause,end,

eps = number_properties('eps');
if load_ref('eps') then   pause,end,

xdel_run(winsid());

mclose(%U);
