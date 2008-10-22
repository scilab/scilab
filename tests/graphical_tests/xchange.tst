getf SCI/util/testexamples.sci
reinit_for_test()
%U=mopen('SCI/tests/graphical_tests/xchange_data.ref','rb');
t = (0:0.1:2 * %pi)';
%ans = plot2d(t, sin(t));
if load_ref('%ans') then   pause,end,

[x,y,rect] = xchange(1, 1, 'f2i');
if load_ref('rect') then   pause,end,
if load_ref('y') then   pause,end,
if load_ref('x') then   pause,end,

[x,y,rect] = xchange(0, 0, 'i2f');
if load_ref('rect') then   pause,end,
if load_ref('y') then   pause,end,
if load_ref('x') then   pause,end,

xdel_run(winsid());

mclose(%U);
