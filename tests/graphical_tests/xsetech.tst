getf SCI/util/testexamples.sci
reinit_for_test()
%U=mopen('SCI/tests/graphical_tests/xsetech_data.ref','rb');
// To get a graphical explanation of xsetech parameters enter:
exec('SCI/demos/graphics/xsetechfig.sce');


// Here xsetech is used to split the graphics window in two parts
// first xsetech is used to set the first sub-window
// and the graphics scale
%ans = xsetech([0,0,1,0.5], [-5,-3,5,3]);
if load_ref('%ans') then   pause,end,

// we call plot2d with the "001" option to use the graphics scale
// set by xsetech
%ans = plot2d((1:10)', (1:10)', 1, '001', ' ');
if load_ref('%ans') then   pause,end,

// then xsetech is used to set the second sub-window
%ans = xsetech([0,0.5,1,0.5]);
if load_ref('%ans') then   pause,end,

// the graphics scale is set by xsetech to [0,0,1,1] by default
// and we change it with the use of the rect argument in plot2d
%ans = plot2d((1:10)', (1:10)', 1, '011', ' ', [-6,-6,6,6]);
if load_ref('%ans') then   pause,end,

// Four plots on a single graphics window
%ans = xbasc_run();
if load_ref('%ans') then   pause,end,

%ans = xset('font', 2, 0);
if load_ref('%ans') then   pause,end,

xsetech([0,0,0.5,0.5]);%ans = plot3d();
if load_ref('%ans') then   pause,end,

xsetech([0.5,0,0.5,0.5]);%ans = plot2d();
if load_ref('%ans') then   pause,end,

xsetech([0.5,0.5,0.5,0.5]);%ans = grayplot();
if load_ref('%ans') then   pause,end,

xsetech([0,0.5,0.5,0.5]);%ans = histplot();
if load_ref('%ans') then   pause,end,

// back to default values for the sub-window
%ans = xsetech([0,0,1,1]);
if load_ref('%ans') then   pause,end,

// One plot with changed arect
%ans = xbasc_run();
if load_ref('%ans') then   pause,end,

%ans = xset('default');
if load_ref('%ans') then   pause,end,

%ans = xsetech(arect=[0,0,0,0]);
if load_ref('%ans') then   pause,end,

x = 1:0.1:10;%ans = plot2d(x', sin(x)');
if load_ref('%ans') then   pause,end,

%ans = xbasc_run();
if load_ref('%ans') then   pause,end,

%ans = xsetech(arect=[1/8,1/8,1/16,1/4]);
if load_ref('%ans') then   pause,end,

x = 1:0.1:10;%ans = plot2d(x', sin(x)');
if load_ref('%ans') then   pause,end,

%ans = xbasc_run();
if load_ref('%ans') then   pause,end,

%ans = xset('default');
if load_ref('%ans') then   pause,end,

xdel_run(winsid());

mclose(%U);
