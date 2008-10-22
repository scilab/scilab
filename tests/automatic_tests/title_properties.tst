getf SCI/util/testexamples.sci
reinit_for_test()
%U=mopen('SCI/tests/automatic_tests/title_properties_data.ref','rb');


%ans = set('figure_style', 'new');
if load_ref('%ans') then   pause,end,
//create a figure
a = get('current_axes');
a('data_bounds') = [-2,-4;2,4];
a('axes_visible') = 'on';
a('box') = 'off';

xtitle(['Titre';'Principal'], 'x', 'y');
t = a('title');
%ans = t('text');
if load_ref('%ans') then   pause,end,
//display text on console
t('font_size') = 4;
t('text') = 'A brand new@title string';
if load_ref('t') then   pause,end,

t('font_style') = 5;
a('x_location') = 'middle';
a('y_location') = 'right';



xdel_run(winsid());

mclose(%U);
