function [sk,gama]=newhinf(P,r,gamamin,gamamax)
// Hinfinity controller for continuous or discrete plant
// D11 is assumed to be zero.
// gama = best gama in [gamamin,gamamax], sk = controller
// P = augmented plant (syslin list) 
// r=size(P22)=[#ouputs, #inputs]
// Copyright INRIA
gamanew=(gamamin+gamamax)/2;
[A,B1,B2,C1,C2,D11,D12,D21,D22]=smga(P,r);
P12=syslin(dom,A,B2,C1,D12);
P21=syslin(dom,A,B1,C2,D21);
cont=%T;
while cont
[ok,sk,stint]=gamatest(P12,P21,gamanew);
if ok then gama=gamanew;gamamax=gamanew;else gamamin=gamanew;end;
gamaold=gamanew;
gamanew=(gamamin+gamamax)/2;
if abs(gamanew-gamaold) < 0.001 then cont=%F;end
end
[ok,sk,pb]=gamatest(P12,P21,gama);
//closed=lft(P,r,sk);
//if dom=='d' then frq=0:0.01:%pi;end
//if dom=='c' then frq=logspace(-3,3);end
//w=svplot(closed,frq);
//xclear();plot2d(frq',w(1,:)');


function [ok,sk,stint]=gamatest(P12,P21,gama)
// Test if gama is ok. If ok, sk=controller. stint is true (%T) if
// internal stability is achieved.
write(%io(2),gama);
ok=%F;sk=[];stint=%F;
[A,B2,C1,D12]=P12(2:5);
[A,B1,C2,D21]=P21(2:5);
[nx,nx]=size(A);
[nx,nw]=size(B1);[ny,nx]=size(C2);
Q=C1'*C1;R=D12'*D12;S=C1'*D12;
V=B1*B1';L=D21*B1';N=D21*D21';
// Two Riccati
[K,X,errx]=leqr(P12,V/gama/gama);
[H,Y,erry]=leqe(P21,Q/gama/gama);
//tests
notok0=or([h==[],k==[]]);
if notok0 then write(%io(2),'fail');return;end
dom=p12(7);
// ------------------>    Discrete time case (strictly proper)
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
          stint=and(abs(spec(h_cl(P,size(C2*B2),sk))) < ones(2*nx,1))
end
end
// ----------------->       Continuous time case
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


