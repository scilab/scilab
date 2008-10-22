getf SCI/util/testexamples.sci
reinit_for_test()
%U=mopen('SCI/tests/automatic_tests/ldiv_data.ref','rb');
wss = ssrand(1, 1, 3);[a,b,c,d] = abcd(wss);
wtf = ss2tf(wss);
x1 = ldiv(numer(wtf), denom(wtf), 5);
if load_ref('x1') then   pause,end,

x2 = [c * b;c * a * b;c * (a^2) * b;c * (a^3) * b;c * (a^4) * b];
if load_ref('x2') then   pause,end,

wssbis = markp2ss(x1', 5, 1, 1);
wtfbis = clean(ss2tf(wssbis));
if load_ref('wtfbis') then   pause,end,

x3 = ldiv(numer(wtfbis), denom(wtfbis), 5);
if load_ref('x3') then   pause,end,

xdel_run(winsid());

mclose(%U);
