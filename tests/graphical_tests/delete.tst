getf SCI/util/testexamples.sci
reinit_for_test()
%U=mopen('SCI/tests/graphical_tests/delete_data.ref','rb');

%ans = set('figure_style', 'new');
if load_ref('%ans') then   pause,end,
// select entity based graphics
subplot(211);
t = 1:10;%ans = plot2d(t, t.^2);
if load_ref('%ans') then   pause,end,

subplot(223);
plot3d();
subplot(224);
plot2d();
xfrect(1, 0, 3, 1);
a = get('current_axes');
if load_ref('a') then   pause,end,

delete();//delete the graphics object newly created
delete(a('children'));//delete all children of the current axes
delete(a);//delete the axes
delete('all');//delete all the graphics objects of the figure

xdel_run(winsid());

mclose(%U);
