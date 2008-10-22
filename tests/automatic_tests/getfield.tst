getf SCI/util/testexamples.sci
reinit_for_test()
%U=mopen('SCI/tests/automatic_tests/getfield_data.ref','rb');
l = list(1, 'qwerw', %s);
if load_ref('l') then   pause,end,

[a,b] = getfield([3,2], l);
if load_ref('b') then   pause,end,
if load_ref('a') then   pause,end,


a = hypermat([2,2,2], rand(1:2^3));// hypermatrices are coded using mlists
%ans = a(1);
if load_ref('%ans') then   pause,end,
// the a(1,1,1) entry
%ans = getfield(1, a);
if load_ref('%ans') then   pause,end,
// the mlist first field
xdel_run(winsid());

mclose(%U);
