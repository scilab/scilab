getf SCI/util/testexamples.sci
reinit_for_test()
%U=mopen('SCI/tests/automatic_tests/eqiir_data.ref','rb');
[cells,fact,zzeros,zpoles] = eqiir('lp', 'ellip', [2 * %pi/10,4 * %pi/10], 0.02, 0.001);
if load_ref('zpoles') then   pause,end,
if load_ref('zzeros') then   pause,end,
if load_ref('fact') then   pause,end,
if load_ref('cells') then   pause,end,

transfer = fact * poly(zzeros, 'z')/poly(zpoles, 'z');
if load_ref('transfer') then   pause,end,

xdel_run(winsid());

mclose(%U);
