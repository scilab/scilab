getf SCI/util/testexamples.sci
reinit_for_test()
%U=mopen('SCI/tests/graphical_tests/gce_data.ref','rb');

%ans = set('figure_style', 'new');
if load_ref('%ans') then   pause,end,
//create a figure entity
a = gca();
if load_ref('a') then   pause,end,
//get the handle of the newly created axes
a('data_bounds') = [1,1;10,10];
a('axes_visible') = 'on';
for i = 1:5,
  xfrect(7 - i, 9 - i, 3, 3);
  e = gce();
  e('background') = i;
end,
delete();// delete current entity
delete(gce());// delete current entity
delete(gcf());// delete current figure


xdel_run(winsid());

mclose(%U);
