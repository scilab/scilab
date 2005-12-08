getf SCI/util/testexamples.sci
reinit_for_test()
%U=mopen('SCI/tests/automatic_tests/imult_data.ref','rb');
z1 = imult(%inf);
if load_ref('z1') then   pause,end,

z2 = %i * %inf;
if load_ref('z2') then   pause,end,

xdel_run(winsid());

mclose(%U);
