getf SCI/util/testexamples.sci
reinit_for_test()
%U=mopen('SCI/tests/automatic_tests/definedfields_data.ref','rb');
l = list(1);l(3) = 5;
if load_ref('l') then   pause,end,

k = definedfields(l);
if load_ref('k') then   pause,end,


t = tlist('x');t(5) = 4;
if load_ref('t') then   pause,end,

%ans = definedfields(t);
if load_ref('%ans') then   pause,end,


m = mlist(['m','a','b']);m('b') = 'sdfgfgd';
if load_ref('m') then   pause,end,

%ans = definedfields(m);
if load_ref('%ans') then   pause,end,

xdel_run(winsid());

mclose(%U);
