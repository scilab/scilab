getf SCI/util/testexamples.sci
reinit_for_test()
%U=mopen('SCI/tests/automatic_tests/parents_data.ref','rb');
%ans = 3^(-1);
if load_ref('%ans') then   pause,end,

x = poly(0, 'x');
//
%ans = (x + 10)/2;
if load_ref('%ans') then   pause,end,

i3 = eye(3, 3);
if load_ref('i3') then   pause,end,

//
a = [1,2,3;4,5,6;7,8,9];
if load_ref('a') then   pause,end,
%ans = a(1, 3);
if load_ref('%ans') then   pause,end,
%ans = a([1,3], :);
if load_ref('%ans') then   pause,end,
%ans = a(:, 3);
if load_ref('%ans') then   pause,end,

a(eye(), 3) = [];
if load_ref('a') then   pause,end,

a(1, $) = 33;
if load_ref('a') then   pause,end,

%ans = a(2, [$,$ - 1]);
if load_ref('%ans') then   pause,end,

a(eye(), $ + 1) = [10;11;12];
if load_ref('a') then   pause,end,

//
w = ssrand(2, 2, 2);%ans = ssprint(w);
if load_ref('%ans') then   pause,end,

%ans = ssprint(w(:, 1));
if load_ref('%ans') then   pause,end,

%ans = ss2tf(w(:, 1));
if load_ref('%ans') then   pause,end,

//
l = list(1, 2, 3, 4);
if load_ref('l') then   pause,end,

[a,b,c,d] = l(:);
if load_ref('d') then   pause,end,
if load_ref('c') then   pause,end,
if load_ref('b') then   pause,end,
if load_ref('a') then   pause,end,

l($ + 1) = 'new';
if load_ref('l') then   pause,end,

//
v = %t([1,1,1,1,1]);
if load_ref('v') then   pause,end,

//
[x,y,z] = (1,2,3);
if load_ref('z') then   pause,end,
if load_ref('y') then   pause,end,
if load_ref('x') then   pause,end,

xdel_run(winsid());

mclose(%U);
