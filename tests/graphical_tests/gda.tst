getf SCI/util/testexamples.sci
reinit_for_test()
%U=mopen('SCI/tests/graphical_tests/gda_data.ref','rb');

a = gda();
if load_ref('a') then   pause,end,
// get the handle of the model axes
// setting its' properties
a('background') = 31;
a('box') = 'off';
a('rotation_angles') = [70,10];
a('foreground') = 2;
a('labels_font_size') = 3;
a('labels_font_color') = 5;
a('sub_tics') = [5,5,3];
a('x_location') = 'top';
%ans = set('figure_style', 'new');
if load_ref('%ans') then   pause,end,

%ans = subplot(211);
if load_ref('%ans') then   pause,end,

%ans = plot2d();
if load_ref('%ans') then   pause,end,
//create an axes entity
%ans = subplot(212);
if load_ref('%ans') then   pause,end,

%ans = plot3d1();
if load_ref('%ans') then   pause,end,
//create a second axes entity
a('grid') = [5,5,5];// setting other model's properties
%ans = clf_run();
if load_ref('%ans') then   pause,end,

t = 0:0.1:5 * %pi;
%ans = plot2d(sin(t), cos(t), t/10);
if load_ref('%ans') then   pause,end,

set(a, 'default_values', 1);// return to the  default values of the model
// see sda() function
%ans = clf_run();
if load_ref('%ans') then   pause,end,

%ans = plot2d(sin(t), cos(t), t/10);
if load_ref('%ans') then   pause,end,


xdel_run(winsid());

mclose(%U);
