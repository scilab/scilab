exec("lapackscilab.sce",-1);
Alfa=2;Beta=3;m=3;n=4;C=ones(m,n);k=2;A=ones(m,k);B=ones(k,n);
C1=dgemm(Alfa,A,B,Beta,C);
if norm(C1-(Alfa*A*B+Beta*C)) > %eps then pause,end
A=[1/2^10,1/2^10;2^10,2^10];
[SCALE, ILOW, IHI]=xxdgebal('S', A);
if norm(SCALE-[0.001;1]) > %eps then pause,end
[W,TAU]=dgeqrf(A);
m=2;V=[];for i=1:2;w(1:i-1)=0;w(i)=1;w(i+1:m)=W(i+1:m,i);V=[V,w];end;
Q=(eye()-TAU(2)*V(:,2)*V(:,2)')*(eye()-TAU(1)*V(:,1)*V(:,1)');
QA=Q*A;
if norm(QA(1,:) - W(1,:))  > %eps then pause,end
A=rand(3,3);[W,VS,A]=dgeesx(A);
if norm(VS*VS'-eye(3,3))  > 1000*%eps then pause,end
if norm(max(real(dsyev(A)))- max(real(spec(A))) ) > 1000*%eps then pause,end
A=rand(3,3);B=ones(3,1);
[B1,A1]=dgels("N", A, ones(3,1));
if norm( A\B -B1 ) > 1000*%eps then pause,end
