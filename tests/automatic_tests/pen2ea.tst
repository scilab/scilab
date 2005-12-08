getf SCI/util/testexamples.sci
reinit_for_test()
%U=mopen('SCI/tests/automatic_tests/pen2ea_data.ref','rb');
E = [1,0];A = [1,2];s = poly(0, 's');
[E,A] = pen2ea(s * E - A);
if load_ref('A') then   pause,end,
if load_ref('E') then   pause,end,

xdel_run(winsid());

mclose(%U);
