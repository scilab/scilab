getf SCI/util/testexamples.sci
reinit_for_test()
%U=mopen('SCI/tests/graphical_tests/xrects_data.ref','rb');
%ans = plot2d([-100,500], [-50,50], [-1,-1], '022');
if load_ref('%ans') then   pause,end,

cols = [-34,-33,-32,-20:5:20,32,33,34];
x = 400 * (0:14)/14;step = 20;
rects = [x;10 * ones(x);step * ones(x);30 * ones(x)];
%ans = xrects(rects, cols);
if load_ref('%ans') then   pause,end,

%ans = xnumb(x, 15 * ones(x), cols);
if load_ref('%ans') then   pause,end,

xdel_run(winsid());

mclose(%U);
