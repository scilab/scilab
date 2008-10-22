getf SCI/util/testexamples.sci
reinit_for_test()
%U=mopen('SCI/tests/automatic_tests/nancumsum_data.ref','rb');
a = [1,2,3;4,5,6];
if load_ref('a') then   pause,end,

s = nancumsum(a);
if load_ref('s') then   pause,end,

s = nancumsum(a, 'r');
if load_ref('s') then   pause,end,

s = nancumsum(a, 'c');
if load_ref('s') then   pause,end,


xdel_run(winsid());

mclose(%U);
