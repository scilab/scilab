getf SCI/util/testexamples.sci
reinit_for_test()
%U=mopen('SCI/tests/automatic_tests/null_data.ref','rb');
l = list(1, 2, 3);
l(2) = null();
if load_ref('l') then   pause,end,
// get list(1,3)
xdel_run(winsid());

mclose(%U);
