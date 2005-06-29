getf SCI/util/testexamples.sci
reinit_for_test()
%U=mopen('SCI/tests/Examples_eng/other/nfreq_data.ref','r');
x = [2,8,0,3,7,6,8,7,9,1];
if load_ref('x') then   pause,end,

m = nfreq(x);
if load_ref('m') then   pause,end,

xdel_run(winsid());

mclose(%U);
