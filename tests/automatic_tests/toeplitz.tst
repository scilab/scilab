getf SCI/util/testexamples.sci
reinit_for_test()
%U=mopen('SCI/tests/automatic_tests/toeplitz_data.ref','rb');
A = toeplitz(1:5);
//
T = toeplitz(1:5, 1:2:7);T1 = [1,3,5,7;2,1,3,5;3,2,1,3;4,3,2,1;5,4,3,2];
%ans = T - T1;
if load_ref('%ans') then   pause,end,

//
s = poly(0, 's');
t = toeplitz([s,s + 1,s^2,1 - s]);
t1 = [s,1 + s,s * s,1 - s;1 + s,s,1 + s,s * s;s * s,1 + s,s,1 + s;1 - s,s * s,1 + s,s];
if load_ref('t1') then   pause,end,

%ans = t - t1;
if load_ref('%ans') then   pause,end,

//
t = toeplitz(['1','2','3','4']);
t1 = ['1','2','3','4';'2','1','2','3';'3','2','1','2';'4','3','2','1'];
if load_ref('t1') then   pause,end,

xdel_run(winsid());

mclose(%U);
