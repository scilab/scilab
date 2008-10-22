getf SCI/util/testexamples.sci
reinit_for_test()
%U=mopen('SCI/tests/automatic_tests/inttype_data.ref','rb');
x = uint16(1:10);
%ans = inttype(x);
if load_ref('%ans') then   pause,end,

xdel_run(winsid());

mclose(%U);
