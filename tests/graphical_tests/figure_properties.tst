getf SCI/util/testexamples.sci
reinit_for_test()
%U=mopen('SCI/tests/graphical_tests/figure_properties_data.ref','rb');
%ans = lines(0);
if load_ref('%ans') then   pause,end,
// disables vertical paging
//Example 1
%ans = set('figure_style', 'new');
if load_ref('%ans') then   pause,end,
//create a figure and set the figure as the current selected one
f = get('current_figure');
if load_ref('f') then   pause,end,
//get the handle of the current figure :
//if none exists, create a figure and return the corresponding handle
%ans = f('figure_position');
if load_ref('%ans') then   pause,end,

f('figure_size') = [200,200];
if load_ref('f') then   pause,end,

f('background') = 2;
if load_ref('f') then   pause,end,

%ans = f('children');
if load_ref('%ans') then   pause,end,
// man can see that an Axes entity already exists
delete(f);
f = gcf();// macro shortcut <=> f=get("current_figure")
f('pixmap') = 'on';
if load_ref('f') then   pause,end,
// set pixmap status to on
%ans = plot2d();
if load_ref('%ans') then   pause,end,
// nothing happens on the screen...
%ans = show_pixmap();
if load_ref('%ans') then   pause,end,
// ...display the pixmap on screen
//Example 2 : default_figure settings
df = get('default_figure');
if load_ref('df') then   pause,end,
// get the default values (shortcut is gdf() )
// Let's change the defaults...
df('color_map') = hotcolormap(128);
if load_ref('df') then   pause,end,

df('background') = 110;
if load_ref('df') then   pause,end,
// set background toa kind of yellow (Note that we are using a color index inside the color_map previously redefined)
scf(122);// creates new figure number 122 with the new default
%ans = plot2d();
if load_ref('%ans') then   pause,end,

scf(214);
t = -%pi:0.3:%pi;
plot3d(t, t, sin(t)' * cos(t), 35, 45, 'X@Y@Z', [15,2,4]);
xdel_run(winsid());

mclose(%U);
