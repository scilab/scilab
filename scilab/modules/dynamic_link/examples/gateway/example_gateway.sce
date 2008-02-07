exec(SCI+'/modules/dynamic_link/examples/gateway/builder.sce');
exec(SCI+'/modules/dynamic_link/examples/gateway/loader.sce');

A=ones(2,2);B=ones(2,2);
C=matmul(A,B);
if norm(A*B-matmul(A,B)) > %eps then pause,end

D="test string";
view(A,C,D);