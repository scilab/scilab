getf SCI/util/testexamples.sci
reinit_for_test()
%U=mopen('SCI/tests/automatic_tests/size_data.ref','rb');
[n,m] = size(rand(3, 2));
if load_ref('m') then   pause,end,
if load_ref('n') then   pause,end,

[n,m] = size(['a','b';'c','d']);
if load_ref('m') then   pause,end,
if load_ref('n') then   pause,end,

x = ssrand(3, 2, 4);[ny,nu] = size(x);
if load_ref('nu') then   pause,end,
if load_ref('ny') then   pause,end,

[ny,nu] = size(ss2tf(x));
if load_ref('nu') then   pause,end,
if load_ref('ny') then   pause,end,

[ny,nu,nx] = size(x);
if load_ref('nx') then   pause,end,
if load_ref('nu') then   pause,end,
if load_ref('ny') then   pause,end,

xdel_run(winsid());

mclose(%U);
