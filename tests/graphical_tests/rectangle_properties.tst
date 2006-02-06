getf SCI/util/testexamples.sci
reinit_for_test()
%U=mopen('SCI/tests/graphical_tests/rectangle_properties_data.ref','rb');
%ans = set('figure_style', 'new');
if load_ref('%ans') then   pause,end,
//create a figure
a = get('current_axes');//get the handle of the newly created axes
a('data_bounds') = [-2,-2;2,2];

%ans = xrect(-1, 1, 2, 2);
if load_ref('%ans') then   pause,end,


r = get('hdl');//get handle on current entity (here the rectangle entity)
%ans = r('type');
if load_ref('%ans') then   pause,end,

%ans = r.parent.type;
if load_ref('%ans') then   pause,end,

r('foreground') = 13;
r('line_style') = 2;
r('fill_mode') = 'on';
r('background') = color('red');
r('clip_box') = [-1,1;1,1];
r.data(eye(), [3,4]) = [1/2,1/2];
r.data(eye(), [1,2]) = [1/2,1/2];
r('clip_state') = 'off';
if load_ref('r') then   pause,end,


xdel_run(winsid());

mclose(%U);
