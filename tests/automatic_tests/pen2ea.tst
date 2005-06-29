getf SCI/util/testexamples.sci
reinit_for_test()
%U=mopen('/usr/local/lib/scilab/tests/Examples/other/pen2ea_data.ref','r');
E = [1,0];A = [1,2];s = poly(0, 's');
[E,A] = pen2ea(s * E - A);
if load_ref('A') then   pause,end,
if load_ref('E') then   pause,end,

xdel_run(winsid());

mclose(%U);
