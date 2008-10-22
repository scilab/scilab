getf SCI/util/testexamples.sci
reinit_for_test()
%U=mopen('SCI/tests/automatic_tests/simp_data.ref','rb');
s = poly(0, 's');
[n,d] = simp((s + 1) * (s + 2), (s + 1) * (s - 2));
if load_ref('d') then   pause,end,
if load_ref('n') then   pause,end,


simp_mode(%F);hns = s/s;
if load_ref('hns') then   pause,end,

simp_mode(%T);hns = s/s;
if load_ref('hns') then   pause,end,


xdel_run(winsid());

mclose(%U);
