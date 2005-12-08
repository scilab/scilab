getf SCI/util/testexamples.sci
reinit_for_test()
%U=mopen('SCI/tests/graphical_tests/drawaxis_data.ref','rb');
%ans = plot2d(1:10, 1:10, 1, '020');
if load_ref('%ans') then   pause,end,

// horizontal axis
%ans = drawaxis(x=2:7, y=4, dir='u', tics='v');
if load_ref('%ans') then   pause,end,

// horizontal axis on top of the frame
%ans = drawaxis(x=2:7, dir='u', tics='v');
if load_ref('%ans') then   pause,end,

// horizontal axis at the bottom of the frame
%ans = drawaxis(x=2:7, dir='d', tics='v');
if load_ref('%ans') then   pause,end,


// horizontal axis given by a range
%ans = drawaxis(x=[2,7,3], y=4, dir='d', tics='r');
if load_ref('%ans') then   pause,end,


// vertical axis
%ans = drawaxis(x=4, y=2:7, dir='r', tics='v');
if load_ref('%ans') then   pause,end,

%ans = drawaxis(x=2, y=[2,7,3], dir='l', tics='r');
if load_ref('%ans') then   pause,end,

%ans = drawaxis(y=2:7, dir='r', tics='v');
if load_ref('%ans') then   pause,end,

%ans = drawaxis(y=2:7, dir='l', tics='v');
if load_ref('%ans') then   pause,end,


// horizontal axis with strings displayed at tics positions
drawaxis(x=2:7, y=8, dir='u', tics='v', val='A' + string(1:6));
// vertical axis with strings displayed at tics positions
drawaxis(x=8, y=2:7, dir='r', tics='v', val='B' + string(1:6));

// horizontal axis given with a 'i' range.
drawaxis(x=[2,5,0,3], y=9, dir='u', tics='i');
drawaxis(x=9, y=[2,5,0,3], dir='r', tics='i', sub_int=5);

// horizontal axis again
%ans = drawaxis(x=2:7, y=4, dir='u', tics='v', fontsize=10, textcolor=9, ticscolor=7, seg=0, sub_int=20);
if load_ref('%ans') then   pause,end,

xdel_run(winsid());

mclose(%U);
