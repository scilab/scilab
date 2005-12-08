getf SCI/util/testexamples.sci
reinit_for_test()
%U=mopen('SCI/tests/graphical_tests/param3d_properties_data.ref','rb');


%ans = set('figure_style', 'new');
if load_ref('%ans') then   pause,end,
//create a figure
a = get('current_axes');//get the handle of the newly created axes
t = (0:0.1:5 * %pi)';
%ans = param3d1([sin(t),sin(2 * t)], [cos(t),cos(2 * t)], [t/10,sin(t)]);
if load_ref('%ans') then   pause,end,


a('rotation_angles') = [65,75];
a('data_bounds') = [-1,-1,-1;1,1,2];//boundaries given by data_bounds
a('thickness') = 2;
h = a('children');
if load_ref('h') then   pause,end,
//get the handle of the param3d entity: an Compound composed of 2 curves
h.children(1).foreground = 3;
if load_ref('h') then   pause,end,
// first curve
curve2 = h.children(2);
curve2('foreground') = 6;
curve2('mark_style') = 2;


xdel_run(winsid());

mclose(%U);
