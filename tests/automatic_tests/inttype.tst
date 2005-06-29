getf SCI/util/testexamples.sci
reinit_for_test()
%U=mopen('/usr/local/lib/scilab/tests/Examples/other/inttype_data.ref','r');
x = uint16(1:10);
%ans = inttype(x);
if load_ref('%ans') then   pause,end,

xdel_run(winsid());

mclose(%U);
