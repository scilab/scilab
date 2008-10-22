getf SCI/util/testexamples.sci
reinit_for_test()
%U=mopen('SCI/tests/graphical_tests/fit_dat_data.ref','rb');
%ans = deff('y=FF(x)', 'y=a*(x-b)+c*x.*x');
if load_ref('%ans') then   pause,end,

X = [];Y = [];
a = 34;b = 12;c = 14;for x = 0:0.1:3,  Y = [Y,FF(x) + 100 * (rand() - 0.5)];X = [X,x];end,
Z = [Y;X];
%ans = deff('e=G(p,z)', 'a=p(1),b=p(2),c=p(3),y=z(1),x=z(2),e=y-FF(x)');
if load_ref('%ans') then   pause,end,

[p,err] = fit_dat(G, [3;5;10], Z);
if load_ref('err') then   pause,end,
if load_ref('p') then   pause,end,

%ans = xset('window', 0);
if load_ref('%ans') then   pause,end,

xbasc_run();
%ans = plot2d(X', Y', -1);
if load_ref('%ans') then   pause,end,

%ans = plot2d(X', FF(X)', 5, '002');
if load_ref('%ans') then   pause,end,

a = p(1);
if load_ref('a') then   pause,end,
b = p(2);
if load_ref('b') then   pause,end,
c = p(3);%ans = plot2d(X', FF(X)', 12, '002');
if load_ref('%ans') then   pause,end,


a = 34;b = 12;c = 14;
%ans = deff('s=DG(p,z)', 'y=z(1),x=z(2),s=-[x-p(2),-p(1),x*x]');
if load_ref('%ans') then   pause,end,

[p,err] = fit_dat(G, [3;5;10], Z, DG);
if load_ref('err') then   pause,end,
if load_ref('p') then   pause,end,

%ans = xset('window', 1);
if load_ref('%ans') then   pause,end,

xbasc_run();
%ans = plot2d(X', Y', -1);
if load_ref('%ans') then   pause,end,

%ans = plot2d(X', FF(X)', 5, '002');
if load_ref('%ans') then   pause,end,

a = p(1);
if load_ref('a') then   pause,end,
b = p(2);
if load_ref('b') then   pause,end,
c = p(3);%ans = plot2d(X', FF(X)', 12, '002');
if load_ref('%ans') then   pause,end,

xdel_run(winsid());

mclose(%U);
