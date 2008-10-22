getf SCI/util/testexamples.sci
reinit_for_test()
%U=mopen('SCI/tests/graphical_tests/trans_data.ref','rb');
%ans = clf_run();
if load_ref('%ans') then   pause,end,


Hlp = iir(3, 'lp', 'ellip', [0.1,0], [0.08,0.03]);
%ans = subplot(311);
if load_ref('%ans') then   pause,end,

%ans = gainplot(Hlp, 0.001, 0.48);
if load_ref('%ans') then   pause,end,

%ans = legend('original low pass');
if load_ref('%ans') then   pause,end,



Hbp = trans(Hlp, 'bp', [0.01,0.1]);
%ans = subplot(312);
if load_ref('%ans') then   pause,end,

%ans = gainplot(Hbp, 0.001, 0.48);
if load_ref('%ans') then   pause,end,

%ans = legend('band pass');
if load_ref('%ans') then   pause,end,



Hsb = trans(Hlp, 'sb', [0.01,0.1]);
if load_ref('Hsb') then   pause,end,

%ans = subplot(313);
if load_ref('%ans') then   pause,end,

%ans = gainplot(Hsb, 0.001, 0.48);
if load_ref('%ans') then   pause,end,

%ans = legend('stop band');
if load_ref('%ans') then   pause,end,



xdel_run(winsid());

mclose(%U);
