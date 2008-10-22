getf SCI/util/testexamples.sci
reinit_for_test()
%U=mopen('SCI/tests/graphical_tests/graduate_data.ref','rb');
y = (0:0.33:145.78)';
xbasc_run();%ans = plot2d1('enn', 0, y);
if load_ref('%ans') then   pause,end,

[ymn,ymx,np] = graduate(mini(y), maxi(y));
if load_ref('np') then   pause,end,
if load_ref('ymx') then   pause,end,
if load_ref('ymn') then   pause,end,

rect = [1,ymn,prod(size(y)),ymx];
xbasc_run();%ans = plot2d1('enn', 0, y, 1, '011', ' ', rect, [10,3,10,np]);
if load_ref('%ans') then   pause,end,

xdel_run(winsid());

mclose(%U);
