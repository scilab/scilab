getf SCI/util/testexamples.sci
reinit_for_test()
%U=mopen('SCI/tests/graphical_tests/xsave_data.ref','rb');

//new style
%ans = set('figure_style', 'new');
if load_ref('%ans') then   pause,end,

t = 0:0.01:10;
%ans = subplot(211);
if load_ref('%ans') then   pause,end,
%ans = plot2d(t, sin(t));
if load_ref('%ans') then   pause,end,

%ans = subplot(212);
if load_ref('%ans') then   pause,end,
%ans = plot2d(t, sin(3 * t));
if load_ref('%ans') then   pause,end,

%ans = save(TMPDIR + '/foo.scg', gcf());
if load_ref('%ans') then   pause,end,

%ans = clf_run();
if load_ref('%ans') then   pause,end,

%ans = load(TMPDIR + '/foo.scg');
if load_ref('%ans') then   pause,end,


a = gca();
curve = a.children.children;//handle on the curve
%ans = save(TMPDIR + '/foo.scg', curve);
if load_ref('%ans') then   pause,end,

%ans = delete(curve);
if load_ref('%ans') then   pause,end,

%ans = load(TMPDIR + '/foo.scg');
if load_ref('%ans') then   pause,end,



//old style
%ans = set('figure_style', 'old');
if load_ref('%ans') then   pause,end,

t = 0:0.01:10;
%ans = subplot(211);
if load_ref('%ans') then   pause,end,
%ans = plot2d(t, sin(t));
if load_ref('%ans') then   pause,end,

%ans = subplot(212);
if load_ref('%ans') then   pause,end,
%ans = plot2d(t, sin(3 * t));
if load_ref('%ans') then   pause,end,

%ans = xsave(TMPDIR + '/foo.scg');
if load_ref('%ans') then   pause,end,

%ans = xbasc_run();
if load_ref('%ans') then   pause,end,

%ans = set('figure_style', 'old');
if load_ref('%ans') then   pause,end,

%ans = xload(TMPDIR + '/foo.scg');
if load_ref('%ans') then   pause,end,




xdel_run(winsid());

mclose(%U);
