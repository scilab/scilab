getf SCI/util/testexamples.sci
reinit_for_test()
%U=mopen('SCI/tests/graphical_tests/Matplot_properties_data.ref','rb');

%ans = set('figure_style', 'new');
if load_ref('%ans') then   pause,end,
//create a figure

%ans = Matplot(1:xget('lastpattern'));
if load_ref('%ans') then   pause,end,

e = gce();// get current entity

e('data') = e.data($:-1:1);
if load_ref('e') then   pause,end,
// reverse order


xdel_run(winsid());

mclose(%U);
