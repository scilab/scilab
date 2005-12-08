getf SCI/util/testexamples.sci
reinit_for_test()
%U=mopen('SCI/tests/graphical_tests/polyline_properties_data.ref','rb');

%ans = set('figure_style', 'new');
if load_ref('%ans') then   pause,end,
//create a figure
a = get('current_axes');
if load_ref('a') then   pause,end,
//get the handle of the newly created axes
a('data_bounds') = [-2,-2;2,2];

%ans = xpoly(sin(2 * %pi * (0:5)/5), cos(2 * %pi * (0:5)/5), 'lines', 0);
if load_ref('%ans') then   pause,end,

p = get('hdl');//get handle on current entity (here the polyline entity)
p('foreground') = 2;
p('thickness') = 3;
p('mark_style') = 9;
d = p('data');d(1, eye()) = [0,0];p('data') = d;
a('rotation_angles') = [0,45];

p('data') = [(-2:0.1:2)',sin((-2:0.1:2) * %pi)'];
if load_ref('p') then   pause,end,

p('mark_mode') = 'off';
p('polyline_style') = 3;
p('line_style') = 4;


xdel_run(winsid());

mclose(%U);
