getf SCI/util/testexamples.sci
reinit_for_test()
%U=mopen('SCI/tests/automatic_tests/global_data.ref','rb');
//first: calling environnment and a function share a variable
%ans = global('a');
if load_ref('%ans') then   pause,end,

a = 1;
if load_ref('a') then   pause,end,

%ans = deff('y=f1(x)', 'global a,a=x^2,y=a^2');
if load_ref('%ans') then   pause,end,

%ans = f1(2);
if load_ref('%ans') then   pause,end,

a;
if load_ref('a') then   pause,end,

//second: three functions share variables
%ans = deff('initdata()', 'global A C ;A=10,C=30');
if load_ref('%ans') then   pause,end,

%ans = deff('letsgo()', 'global A C ;disp(A) ;C=70');
if load_ref('%ans') then   pause,end,

%ans = deff('letsgo1()', 'global C ;disp(C)');
if load_ref('%ans') then   pause,end,

%ans = initdata();
if load_ref('%ans') then   pause,end,

%ans = letsgo();
if load_ref('%ans') then   pause,end,

%ans = letsgo1();
if load_ref('%ans') then   pause,end,


xdel_run(winsid());

mclose(%U);
