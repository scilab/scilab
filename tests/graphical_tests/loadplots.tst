getf SCI/util/testexamples.sci
reinit_for_test()
%U=mopen('SCI/tests/graphical_tests/loadplots_data.ref','rb');

%ans = set('old_style', 'on');
if load_ref('%ans') then   pause,end,
// switch to old graphic mode
driver('Rec');xbasc_run();%ans = plot2d([0,1.5,4]);
if load_ref('%ans') then   pause,end,
//make a plot
%ans = xsave(TMPDIR + '/foo.scg');
if load_ref('%ans') then   pause,end,
//save it in a binary file

rec = loadplots(TMPDIR + '/foo.scg');//get the associated data structure
//here rec($) is the data structure associated with the plot2d instruction
%ans = rec($).x;
if load_ref('%ans') then   pause,end,
//the x vector
%ans = rec($).y;
if load_ref('%ans') then   pause,end,
//the y vector

%ans = string(rec);
if load_ref('%ans') then   pause,end,
//the scilab instructions producing the same plot

%ans = mputl(string(rec), TMPDIR + '/foo.sce');
if load_ref('%ans') then   pause,end,
//creates a script file
xbasc_run();%ans = exec(TMPDIR + '/foo.sce', -1);
if load_ref('%ans') then   pause,end,
//execute it to re-create the plot

xbasc_run();
%ans = set('old_style', 'off');
if load_ref('%ans') then   pause,end,
// switch nack to new graphic mode

xdel_run(winsid());

mclose(%U);
