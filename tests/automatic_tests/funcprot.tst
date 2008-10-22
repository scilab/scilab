getf SCI/util/testexamples.sci
reinit_for_test()
%U=mopen('SCI/tests/automatic_tests/funcprot_data.ref','rb');
%ans = funcprot(1);
if load_ref('%ans') then   pause,end,

%ans = deff('[x]=foo(a)', 'x=a');
if load_ref('%ans') then   pause,end,

%ans = deff('[x]=foo(a)', 'x=a+1');
if load_ref('%ans') then   pause,end,

foo = 33;
if load_ref('foo') then   pause,end,

%ans = funcprot(0);
if load_ref('%ans') then   pause,end,

%ans = deff('[x]=foo(a)', 'x=a');
if load_ref('%ans') then   pause,end,

%ans = deff('[x]=foo(a)', 'x=a+1');
if load_ref('%ans') then   pause,end,

foo = 33;
if load_ref('foo') then   pause,end,

xdel_run(winsid());

mclose(%U);
