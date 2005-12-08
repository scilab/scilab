getf SCI/util/testexamples.sci
reinit_for_test()
%U=mopen('SCI/tests/automatic_tests/sort_data.ref','rb');
[s,p] = sort(rand(1, 10));
//p  is a random permutation of 1:10
A = [1,2,5;3,4,2];
[Asorted,q] = sort(A);%ans = A(q(:)) - Asorted(:);
if load_ref('%ans') then   pause,end,

v = 1:10;
%ans = sort(v);
if load_ref('%ans') then   pause,end,

%ans = sort(v');
if load_ref('%ans') then   pause,end,

%ans = sort(v, 'r');
if load_ref('%ans') then   pause,end,
//Does nothing for row vectors
%ans = sort(v, 'c');
if load_ref('%ans') then   pause,end,

xdel_run(winsid());

mclose(%U);
