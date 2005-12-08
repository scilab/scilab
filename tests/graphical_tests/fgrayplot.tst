getf SCI/util/testexamples.sci
reinit_for_test()
%U=mopen('SCI/tests/graphical_tests/fgrayplot_data.ref','rb');
t = -1:0.1:1;
%ans = deff('[z]=surf(x,y)', 'z=x**2+y**2');
if load_ref('%ans') then   pause,end,

%ans = fgrayplot(t, t, surf, rect=[-2,-2,2,2]);
if load_ref('%ans') then   pause,end,

xdel_run(winsid());

mclose(%U);
