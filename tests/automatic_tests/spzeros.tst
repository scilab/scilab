getf SCI/util/testexamples.sci
reinit_for_test()
%U=mopen('/usr/local/lib/scilab/tests/Examples/other/spzeros_data.ref','r');
%ans = sum(spzeros(1000, 1000));
if load_ref('%ans') then   pause,end,

xdel_run(winsid());

mclose(%U);
