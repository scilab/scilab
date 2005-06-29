getf SCI/util/testexamples.sci
reinit_for_test()
%U=mopen('/usr/local/lib/scilab/tests/Examples/other/iconvert_data.ref','r');
b = int32([1,-120,127,312]);
if load_ref('b') then   pause,end,

y = iconvert(b, 1);
if load_ref('y') then   pause,end,


xdel_run(winsid());

mclose(%U);
