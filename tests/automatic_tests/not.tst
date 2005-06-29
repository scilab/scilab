getf SCI/util/testexamples.sci
reinit_for_test()
%U=mopen('/usr/local/lib/scilab/tests/Examples/other/not_data.ref','r');
%ans = ~[%t,%t,%f];
if load_ref('%ans') then   pause,end,

xdel_run(winsid());

mclose(%U);
