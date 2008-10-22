getf SCI/util/testexamples.sci
reinit_for_test()
%U=mopen('SCI/tests/graphical_tests/replot_data.ref','rb');
backupstyle = '?';
if load_ref('backupstyle') then   pause,end,

if get('old_style') == 'off' then 
  set('old_style', 'on');
  backupstyle = 'off';
  if load_ref('backupstyle') then   pause,end,

end,

// under old graphics mode
x = (0:0.1:2 * %pi)';
%ans = plot2d(x, sin(x));
if load_ref('%ans') then   pause,end,

%ans = replot([-1,-1,10,2]);
if load_ref('%ans') then   pause,end,


// under new graphics mode
%ans = set('old_style', 'off');
if load_ref('%ans') then   pause,end,

xdel_run(winsid());
%ans = plot();
if load_ref('%ans') then   pause,end,
// plot demo
f = gcf();
%ans = replot([-1,-1,10,2], f.children(1));
if load_ref('%ans') then   pause,end,
// specify axes handle's value
%ans = replot([-3,-2,8,4], f.children(2));
if load_ref('%ans') then   pause,end,

if backupstyle == 'off' then   set('old_style', backupstyle);end,

xdel_run(winsid());

mclose(%U);
