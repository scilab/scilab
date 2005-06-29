getf SCI/util/testexamples.sci
reinit_for_test()
%U=mopen('/usr/local/lib/scilab/tests/Examples/other/casc_data.ref','r');
x = [1,2,3;4,5,6;7,8,9;10,11,12];
if load_ref('x') then   pause,end,

cels = casc(x, 'z');
if load_ref('cels') then   pause,end,

xdel_run(winsid());

mclose(%U);
