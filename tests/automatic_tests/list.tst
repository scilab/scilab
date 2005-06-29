getf SCI/util/testexamples.sci
reinit_for_test()
%U=mopen('/usr/local/lib/scilab/tests/Examples/other/list_data.ref','r');
l = list(1, ['a','b']);
if load_ref('l') then   pause,end,

l(0) = 'foo';
if load_ref('l') then   pause,end,

l($ + 1) = 'bar';
if load_ref('l') then   pause,end,

l(2) = 'toto';
if load_ref('l') then   pause,end,

l(3) = rand(1, 2);
if load_ref('l') then   pause,end,

l(3) = null();
if load_ref('l') then   pause,end,

lbis = list('gewurtz', 'caipirina', 'debug');
if load_ref('lbis') then   pause,end,

lter = lstcat(l, lbis);
if load_ref('lter') then   pause,end,

%ans = size(lter) - size(lbis) - size(l);
if load_ref('%ans') then   pause,end,
// must be zero
xdel_run(winsid());

mclose(%U);
