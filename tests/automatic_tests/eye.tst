getf SCI/util/testexamples.sci
reinit_for_test()
%U=mopen('SCI/tests/automatic_tests/eye_data.ref','rb');
%ans = eye(2, 3);
if load_ref('%ans') then   pause,end,

A = rand(2, 3);%ans = eye(A);
if load_ref('%ans') then   pause,end,

s = poly(0, 's');A = [s,1;s,s + 1];%ans = eye(A);
if load_ref('%ans') then   pause,end,

A = [1/s,1;s,2];eye(A);
A = ssrand(2, 2, 3);%ans = eye(A);
if load_ref('%ans') then   pause,end,

%ans = [1,2;3,4] + 2 * eye();
if load_ref('%ans') then   pause,end,

xdel_run(winsid());

mclose(%U);
