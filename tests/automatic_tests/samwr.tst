getf SCI/util/testexamples.sci
reinit_for_test()
%U=mopen('/usr/local/lib/scilab/tests/Examples/other/samwr_data.ref','r');
a = [0.33,1.24,2.1,1.03];
if load_ref('a') then   pause,end,

s = samwr(4, 12, a);
if load_ref('s') then   pause,end,

xdel_run(winsid());

mclose(%U);
