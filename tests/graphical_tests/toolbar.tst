getf SCI/util/testexamples.sci
reinit_for_test()
%U=mopen('SCI/tests/graphical_tests/toolbar_data.ref','rb');
%ans = toolbar(-1, 'off');
if load_ref('%ans') then   pause,end,

state = toolbar(-1, 'on');
if load_ref('state') then   pause,end,


plot3d();
h = gcf();
%ans = toolbar(h('figure_id'), 'off');
if load_ref('%ans') then   pause,end,


xdel_run(winsid());

mclose(%U);
