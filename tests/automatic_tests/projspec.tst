getf SCI/util/testexamples.sci
reinit_for_test()
%U=mopen('SCI/tests/automatic_tests/projspec_data.ref','rb');
%ans = deff('j=jdrn(n)', 'j=zeros(n,n);for k=1:n-1;j(k,k+1)=1;end');
if load_ref('%ans') then   pause,end,

A = sysdiag(jdrn(3), jdrn(2), rand(2, 2));X = rand(7, 7);
A = X * A * inv(X);
[S,P,D,index] = projspec(A);
index;
if load_ref('index') then   pause,end,
///size of J-block
%ans = trace(P);
if load_ref('%ans') then   pause,end,
//sum of dimensions of J-blocks
%ans = A * S - (eye() - P);
if load_ref('%ans') then   pause,end,

%ans = norm(D^index, 1);
if load_ref('%ans') then   pause,end,

xdel_run(winsid());

mclose(%U);
