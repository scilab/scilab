getf SCI/util/testexamples.sci
reinit_for_test()
%U=mopen('SCI/tests/graphical_tests/xgetech_data.ref','rb');
// first subwindow
%ans = xsetech([0,0,1,0.5]);
if load_ref('%ans') then   pause,end,

%ans = plot2d();
if load_ref('%ans') then   pause,end,

// then xsetech is used to set the second sub window
%ans = xsetech([0,0.5,1,0.5]);
if load_ref('%ans') then   pause,end,

%ans = grayplot();
if load_ref('%ans') then   pause,end,

// get the graphic scales of first subwindow
%ans = xsetech([0,0,1,0.5]);
if load_ref('%ans') then   pause,end,

[wrect,frect,logflag,arect] = xgetech();
// get the graphic scales of second subwindow
%ans = xsetech([0,0.5,1,0.5]);
if load_ref('%ans') then   pause,end,

[wrect,frect,logflag,arect] = xgetech();
xbasc_run();
%ans = xset('default');
if load_ref('%ans') then   pause,end,

xdel_run(winsid());

mclose(%U);
