getf SCI/util/testexamples.sci
reinit_for_test()
%U=mopen('SCI/tests/graphical_tests/text_properties_data.ref','rb');

%ans = set('figure_style', 'new');
if load_ref('%ans') then   pause,end,
//create a figure
a = get('current_axes');
a('data_bounds') = [0,0;1,1];

%ans = xstring(0.5, 0.6, 'Scilab is not esilaB', 0, 0);
if load_ref('%ans') then   pause,end,


t = get('hdl');
if load_ref('t') then   pause,end,
//get the handle of the newly created object

t('foreground') = 9;
t('font_size') = 5;
t('font_style') = 5;
t('text') = 'SCILAB';
t('font_angle') = 90;

xdel_run(winsid());

mclose(%U);
