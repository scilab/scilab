getf SCI/util/testexamples.sci
reinit_for_test()
%U=mopen('SCI/tests/automatic_tests/iconvert_data.ref','rb');
b = int32([1,-120,127,312]);
if load_ref('b') then   pause,end,

y = iconvert(b, 1);
if load_ref('y') then   pause,end,


xdel_run(winsid());

mclose(%U);
