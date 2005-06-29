getf SCI/util/testexamples.sci
reinit_for_test()
%U=mopen('/usr/local/lib/scilab/tests/Examples/other/simp_mode_data.ref','r');
s = poly(0, 's');
mod = simp_mode();
if load_ref('mod') then   pause,end,

simp_mode(%f);hns = s/s;
if load_ref('hns') then   pause,end,

simp_mode(%t);hns = s/s;
if load_ref('hns') then   pause,end,

simp_mode(mod);

xdel_run(winsid());

mclose(%U);
