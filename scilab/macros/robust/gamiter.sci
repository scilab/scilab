function [ok,sk,stint]=gamiter(P12,P21,gama)
// Copyright INRIA
write(%io(2),gama);
ok=%F;sk=[];stint=%F;
[A,B2,C1,D12]=P12(2:5);
[A,B1,C2,D21]=P21(2:5);
[nx,nx]=size(A);
[nx,nw]=size(B1);[ny,nx]=size(C2);
Q=C1'*C1;R=D12'*D12;S=C1'*D12;
V=B1*B1';L=D21*B1';N=D21*D21';
[K,X,err]=leqr(P12,V/gama/gama);
//[F,X,err]=dleqr(P,size(C2*B2),gama);
//K=F(nw+1:nw+nu,:)
//Abar=A-B2*inv(R)*S';
//Qbar=Q-S*inv(R)*S';
//Abar'*inv((inv(X)+b2*inv(r)*b2'-V/gama/gama))*Abar+Qbar
//-inv(R)*(B2'*inv(inv(X)+B2*inv(R)*B2'-V/gama/gama)*Abar+S')
//Atild=A-L'*inv(N)*C2;
//Vtild=V-L'*inv(N)*L;
[H,Y,err]=leqe(P21,Q/gama/gama);
//[L,Y,err]=dleqe(P,size(C2*B2),gama);
//H=L(:,ny+1:ny+nz)

//Atild*inv(inv(Y)+C2'*inv(N)*C2-Q/gama/gama)*Atild'+Vtild
//-(Atild*inv(inv(Y)+C2'*inv(N)*C2-Q/gama/gama)*C2'+L')*inv(N)
//tests
notok0=or([h==[],k==[]]);
if notok0 then write(%io(2),'fail');return;end
dom=p12(7);
if dom=='d' then
ok1=and(abs(spec(A+B2*K)) < ones(nx,1));
ok2=and(abs(spec(A+H*C2)) < ones(nx,1));
ok3=and(real(spec(gama*gama*eye()-B1'*X*B1)) > zeros(nw,1));
ok4=and(real(spec(gama*gama*eye()-C1*Y*C1')) > zeros(ny,1));
ok5=and(real(spec(inv(X)+B2*inv(R)*B2'-V/gama/gama)) > zeros(nx,1));
ok6=and(real(spec(inv(Y)+C2'*inv(N)*C2-Q/gama/gama)) > zeros(nx,1));
ok7=mini(real(spec(eye()-Y*X/gama/gama))) > 100*%eps;
ok=and([ok3,ok4,ok7]);
if ~ok then write(%io(2),'fail');return;end
E=eye()-Y*X/gama/gama;
W=(A-L'*inv(N)*C2)*inv((inv(X)+B2*inv(R)*B2'-V));
if rcond(E) > 1.d-4 then
          Z=inv(E);
          Ak=A+B2*K*Z+W*(S*K*Z+Q)/gama/gama+H*C2;Bk=H;Ck=-K*Z;
          Sk=syslin('d',Ak,Bk,Ck);
          stint=and(abs(spec(h_cl(P,size(C2*B2),sk))) < ones(2*nx,1))
//Sk1=lqg(P,size(c2*B2));
else
          Ak=A*E+B2*K+W*(S*K+Q*E)/gama/gama+H*C2*E;Bk=H;Ck=-K;
          Sk=des2ss(Ak,Bk,Ck,0*Ck*Bk,E);
end
end
if dom=='c' then
ok1=and(real(spec(A+B2*K)) < zeros(nx,1));
ok2=and(real(spec(A+H*C2)) < zeros(nx,1));
ok3=mini(real(spec(eye()-Y*X/gama/gama))) > 100*%eps;
ok=and([ok1,ok2,ok3]);
if ~ok then write(%io(2),'fail');return;end
Z=inv(eye()-Y*X/gama/gama);;
Ak=A+B2*K*Z+Y*(S*K*Z+Q)/gama/gama+H*C2;Bk=H;Ck=-K*Z;
Sk=syslin('c',Ak,Bk,Ck);
stint=and(real(spec(h_cl(P,size(C2*B2),sk))) < zeros(2*nx,1))
//Sk1=lqg(P,size(c2*B2));
end
write(%io(2),'ok')


