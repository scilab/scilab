getf SCI/util/testexamples.sci
reinit_for_test()
%U=mopen('SCI/tests/automatic_tests/name2rgb_data.ref','rb');
rgb = name2rgb('gold');
if load_ref('rgb') then   pause,end,

%ans = rgb2name(rgb);
if load_ref('%ans') then   pause,end,

xdel_run(winsid());

mclose(%U);
