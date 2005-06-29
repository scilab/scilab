getf SCI/util/testexamples.sci
reinit_for_test()
%U=mopen('/usr/local/lib/scilab/tests/Examples/other/frexp_data.ref','r');
[f,e] = frexp([1,%pi,-3,%eps]);
if load_ref('e') then   pause,end,
if load_ref('f') then   pause,end,

xdel_run(winsid());

mclose(%U);
