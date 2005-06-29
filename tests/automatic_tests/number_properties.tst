getf SCI/util/testexamples.sci
reinit_for_test()
%U=mopen('/usr/local/lib/scilab/tests/Examples/other/number_properties_data.ref','r');
b = number_properties('radix');
if load_ref('b') then   pause,end,

eps = number_properties('eps');
if load_ref('eps') then   pause,end,

xdel_run(winsid());

mclose(%U);
