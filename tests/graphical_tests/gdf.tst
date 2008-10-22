getf SCI/util/testexamples.sci
reinit_for_test()
%U=mopen('SCI/tests/graphical_tests/gdf_data.ref','rb');

%ans = set('old_style', 'off');
if load_ref('%ans') then   pause,end,

f = gdf();
if load_ref('f') then   pause,end,
// get the handle of the model figure
// setting its' properties
f('background') = 7;
f('figure_name') = 'Function gdf()';
f('figure_position') = [-1,100];
f('auto_resize') = 'off';
f('figure_size') = [300,461];
f('axes_size') = [600,400];
%ans = plot2d();
if load_ref('%ans') then   pause,end,
//create a figure
scf(1);
%ans = plot3d();
if load_ref('%ans') then   pause,end,
//create a second figure
set(f, 'default_values', 1);// return to the  default values of figure's model
// see sdf() function
scf(2);
%ans = plot2d();
if load_ref('%ans') then   pause,end,


xdel_run(winsid());

mclose(%U);
