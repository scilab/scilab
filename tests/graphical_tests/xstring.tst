getf SCI/util/testexamples.sci
reinit_for_test()
%U=mopen('SCI/tests/graphical_tests/xstring_data.ref','rb');
%ans = plot2d([0;1], [0;1], 0);
if load_ref('%ans') then   pause,end,

%ans = xstring(0.5, 0.5, ['Scilab','is';'not','esilaB']);
if load_ref('%ans') then   pause,end,

//Other example
alphabet = ['a','b','c','d','e','f','g','h','i','j','k','l','m','n','o','p','q','r','s','t','u','v','w','x','y','z'];
%ans = xbasc_run();
if load_ref('%ans') then   pause,end,

%ans = plot2d([0;1], [0;2], 0);
if load_ref('%ans') then   pause,end,

%ans = xstring(0.1, 1.8, alphabet);
if load_ref('%ans') then   pause,end,
// alphabet
%ans = xstring(0.1, 1.6, alphabet, 0, 1);
if load_ref('%ans') then   pause,end,
// alphabet in a box
%ans = xstring(0.1, 1.4, alphabet, 20);
if load_ref('%ans') then   pause,end,
// angle
%ans = xset('font', 1, 1);
if load_ref('%ans') then   pause,end,
// use symbol fonts
%ans = xstring(0.1, 0.1, alphabet);
if load_ref('%ans') then   pause,end,

%ans = xset('font', 1, 3);
if load_ref('%ans') then   pause,end,
// change size font
%ans = xstring(0.1, 0.3, alphabet);
if load_ref('%ans') then   pause,end,

xset('font', 1, 24);%ans = xstring(0.1, 0.6, 'a');
if load_ref('%ans') then   pause,end,
//big alpha
%ans = xset('default');
if load_ref('%ans') then   pause,end,

xdel_run(winsid());

mclose(%U);
