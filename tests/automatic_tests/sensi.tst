getf SCI/util/testexamples.sci
reinit_for_test()
%U=mopen('SCI/tests/automatic_tests/sensi_data.ref','rb');
G = ssrand(1, 1, 3);K = ssrand(1, 1, 3);
[Se,Re,Te] = sensi(G, K);
Se1 = inv(eye() + G * K);//Other way to compute
%ans = ss2tf(Se);
if load_ref('%ans') then   pause,end,
//Se seen in transfer form
%ans = ss2tf(Se1);
if load_ref('%ans') then   pause,end,

%ans = ss2tf(Te);
if load_ref('%ans') then   pause,end,

%ans = ss2tf(G * K * Se1);
if load_ref('%ans') then   pause,end,

[Si,Ri,Ti] = sensi(G, K, 'i');
w1 = [ss2tf(Si);ss2tf(Ri);ss2tf(Ti)];
if load_ref('w1') then   pause,end,

w2 = [[ss2tf(inv(eye() + K * G));ss2tf(G * inv(eye() + K * G))];ss2tf(K * G * inv(eye() + K * G))];
%ans = clean(w1 - w2);
if load_ref('%ans') then   pause,end,

xdel_run(winsid());

mclose(%U);
