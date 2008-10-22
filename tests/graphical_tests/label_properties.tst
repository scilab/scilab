getf SCI/util/testexamples.sci
reinit_for_test()
%U=mopen('SCI/tests/graphical_tests/label_properties_data.ref','rb');

%ans = set('figure_style', 'new');
if load_ref('%ans') then   pause,end,
//create a figure
a = get('current_axes');
%ans = a('title');
if load_ref('%ans') then   pause,end,

%ans = type(a('title'));
if load_ref('%ans') then   pause,end,

%ans = plot3d();
if load_ref('%ans') then   pause,end,

%ans = a('x_label');
if load_ref('%ans') then   pause,end,

%ans = a('y_label');
if load_ref('%ans') then   pause,end,

%ans = a('z_label');
if load_ref('%ans') then   pause,end,

%ans = xtitle('My title', 'my x axis label', 'Volume');
if load_ref('%ans') then   pause,end,

a.z_label.text = 'Month';
if load_ref('a') then   pause,end,


t = a('title');
t('foreground') = 9;
t('font_size') = 5;
t('font_style') = 5;
t('text') = 'SCILAB';

xlabel = a('x_label');
xlabel('text') = ' Weight';
if load_ref('xlabel') then   pause,end,

xlabel('font_style') = 5;
a.y_label.foreground = 12;
xdel_run(winsid());

mclose(%U);
