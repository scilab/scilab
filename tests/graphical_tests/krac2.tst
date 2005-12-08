getf SCI/util/testexamples.sci
reinit_for_test()
%U=mopen('SCI/tests/graphical_tests/krac2_data.ref','rb');
h = syslin('c', 352 * poly(-5, 's')/poly([0,0,2000,200,25,1], 's', 'c'));
xbasc_run();%ans = evans(h, 100);
if load_ref('%ans') then   pause,end,

g = krac2(h);
if load_ref('g') then   pause,end,

hf1 = h /. g(1);%ans = roots(denom(hf1));
if load_ref('%ans') then   pause,end,

hf2 = h /. g(2);%ans = roots(denom(hf2));
if load_ref('%ans') then   pause,end,

xdel_run(winsid());

mclose(%U);
