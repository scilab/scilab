getf SCI/util/testexamples.sci
reinit_for_test()
%U=mopen('SCI/tests/graphical_tests/champ_properties_data.ref','rb');


%ans = set('figure_style', 'new');
if load_ref('%ans') then   pause,end,
//create a figure
a = get('current_axes');//get the handle of the newly created axes
a('data_bounds') = [-10,-10;10,10];
%ans = champ(-5:5, -5:5, rand(11, 11), rand(11, 11));
if load_ref('%ans') then   pause,end,


c = a('children');
if load_ref('c') then   pause,end,


c('colored') = 'on';
c('thickness') = 2;
%ans = c('data');
if load_ref('%ans') then   pause,end,
// display the tlist of type "scichampdata"
a('data_bounds') = [-5,-5;5,5];


xdel_run(winsid());

mclose(%U);
