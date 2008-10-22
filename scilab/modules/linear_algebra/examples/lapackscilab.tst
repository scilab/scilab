exec("lapackscilab.sce",-1);
Alfa=2;Beta=3;m=3;n=4;C=ones(m,n);k=2;A=ones(m,k);B=ones(k,n);
C1=dgemm(Alfa,A,B,Beta,C);
if norm(C1-(Alfa*A*B+Beta*C)) > %eps then pause,end
A=[1/2^10,1/2^10;2^10,2^10];
A=rand(3,3);[W,VS,A]=dgeesx(A);
if norm(VS*VS'-eye(3,3))  > 1000*%eps then pause,end
if norm(max(real(dsyev(A)))- max(real(spec(A))) ) > 1000*%eps then pause,end
