// ven jui 23 09:33:46 CEST 2004

//====================================================
// ../man/fr/arma/arma2p.xml
//====================================================
clear;lines(0);

a=[1,-2.851,2.717,-0.865].*.eye(2,2)
b=[0,1,1,1].*.[1;1];
d=[1,0.7,0.2].*.eye(2,2);
sig=eye(2,2);
ar=armac(a,b,d,2,1,sig)
// extract polynomial matrices from ar representation 
[A,B,D]=arma2p(ar);

xdel(winsid())

//====================================================
// ../man/fr/arma/armac.xml
//====================================================
clear;lines(0);

a=[1,-2.851,2.717,-0.865].*.eye(2,2)
b=[0,1,1,1].*.[1;1];
d=[1,0.7,0.2].*.eye(2,2);
sig=eye(2,2);
ar=armac(a,b,d,2,1,sig)
// extract polynomial matrices from ar representation 
[A,B,D]=arma2p(ar);

xdel(winsid())

//====================================================
// ../man/fr/arma/armax1.xml
//====================================================

//====================================================
// ../man/fr/arma/arma.xml
//====================================================

//====================================================
// ../man/fr/arma/armax.xml
//====================================================
clear;lines(0);

//-Ex1- Arma model : y(t) = 0.2*u(t-1)+0.01*e(t-1)
ny=1,nu=1,sig=0.01;
Arma=armac(1,[0,0.2],[0,1],ny,nu,sig)  //defining the above arma model
u=rand(1,1000,'normal');     //a random input sequence u
y=arsimul(Arma,u); //simulation of a y output sequence associated with u.
Armaest=armax(0,1,y,u);   //Identified model given u and y.
Acoeff=Armaest('a');    //Coefficients of the polynomial A(x)
Bcoeff=Armaest('b')     //Coefficients of the polynomial B(x)
Dcoeff=Armaest('d');    //Coefficients of the polynomial D(x)
[Ax,Bx,Dx]=arma2p(Armaest)   //Results in polynomial form. 

//-Ex2- Arma1: y_t -0.8*y_{t-1} + 0.2*y_{t-2} =  sig*e(t)
ny=1,nu=1;sig=0.001;
// First step: simulation the Arma1 model, for that we define
// Arma2: y_t -0.8*y_{t-1} + 0.2*y_{t-2} = sig*u(t)
// with normal deviates for u(t).  
Arma2=armac([1,-0.8,0.2],sig,0,ny,nu,0);
//Definition of the Arma2 arma model (a model with B=sig and without noise!)
u=rand(1,10000,'normal');  // An input sequence for Arma2
y=arsimul(Arma2,u); // y = output of Arma2 with input u 
//                     can be seen as output of Arma1.
// Second step: identification. We look for an Arma model
// y(t) + a1*y(t-1) + a2 *y(t-2) =  sig*e(t)
Arma1est=armax(2,-1,y,[]);
[A,B,D]=arma2p(Arma1est)

xdel(winsid())

//====================================================
// ../man/fr/arma/arsimul.xml
//====================================================

//====================================================
// ../man/fr/arma/narsimul.xml
//====================================================

//====================================================
// ../man/fr/arma/noisegen.xml
//====================================================
clear;lines(0);

noisegen(0.5,30,1.0);
x=-5:0.01:35;
y=feval(x,Noise);
plot(x,y);

xdel(winsid())

//====================================================
// ../man/fr/arma/odedi.xml
//====================================================

//====================================================
// ../man/fr/arma/prbs_a.xml
//====================================================
clear;lines(0);

u=prbs_a(50,10);
plot2d2("onn",(1:50)',u',1,"151",' ',[0,-1.5,50,1.5]);

xdel(winsid())

//====================================================
// ../man/fr/arma/reglin.xml
//====================================================

//====================================================
// ../man/fr/control/abcd.xml
//====================================================
clear;lines(0);

A=diag([1,2,3]);B=[1;1;1];C=[2,2,2];
sys=syslin('c',A,B,C);
sys("A")
sys("C")
[A1,B1,C1,D1]=abcd(sys);
A1
systf=ss2tf(sys);
[a,b,c,d]=abcd(systf)
spec(a)
c*b-C*B
c*a*b-C*A*B

xdel(winsid())

//====================================================
// ../man/fr/control/abinv.xml
//====================================================
clear;lines(0);

nu=3;ny=4;nx=7;
nrt=2;ngt=3;ng0=3;nvt=5;rk=2;
flag=list('on',nrt,ngt,ng0,nvt,rk);
Sys=ssrand(ny,nu,nx,flag);alfa=-1;beta=-2;
[X,dims,F,U,k,Z]=abinv(Sys,alfa,beta);
[A,B,C,D]=abcd(Sys);dimV=dims(3);dimR=dims(1);
V=X(:,1:dimV);X2=X(:,dimV+1:nx);
X2'*(A+B*F)*V
(C+D*F)*V
X0=X(:,dimR+1:dimV); spec(X0'*(A+B*F)*X0)
trzeros(Sys)
spec(A+B*F)   //nr=2 evals at -1 and noc-dimV=2 evals at -2.
clean(ss2tf(Sys*Z))
// 2nd Example
nx=6;ny=3;nu=2;
A=diag(1:6);A(2,2)=-7;A(5,5)=-9;B=[1,2;0,3;0,4;0,5;0,0;0,0];
C=[zeros(ny,ny),eye(ny,ny)];D=[0,1;0,2;0,3];
sl=syslin('c',A,B,C,D);//sl=ss2ss(sl,rand(6,6))*rand(2,2);
[A,B,C,D]=abcd(sl);  //The matrices of sl.
alfa=-1;beta=-2;
[X,dims,F,U,k,Z]=abinv(sl,alfa,beta);dimVg=dims(2);
clean(X'*(A+B*F)*X)
clean(X'*B*U)
clean((C+D*F)*X)
clean(D*U)
G=(X(:,dimVg+1:$))';
B2=G*B;nd=3;
R=rand(nu,nd);Q2T=-[B2;D]*R;
p=size(G,1);Q2=Q2T(1:p,:);T=Q2T(p+1:$,:);
Q=G\Q2;   //a valid [Q;T] since 
[G*B;D]*R + [G*Q;T]  // is zero
closed=syslin('c',A+B*F,Q+B*R,C+D*F,T+D*R); // closed loop: d-->y
ss2tf(closed)       // Closed loop is zero
spec(closed('A'))   //The plant is not stabilizable!
[ns,nc,W,sl1]=st_ility(sl);
[A,B,C,D]=abcd(sl1);A=A(1:ns,1:ns);B=B(1:ns,:);C=C(:,1:ns);
slnew=syslin('c',A,B,C,D);  //Now stabilizable
//Fnew=stabil(slnew('A'),slnew('B'),-11);
//slnew('A')=slnew('A')+slnew('B')*Fnew;
//slnew('C')=slnew('C')+slnew('D')*Fnew;
[X,dims,F,U,k,Z]=abinv(slnew,alfa,beta);dimVg=dims(2);
[A,B,C,D]=abcd(slnew);
G=(X(:,dimVg+1:$))';
B2=G*B;nd=3;
R=rand(nu,nd);Q2T=-[B2;D]*R;
p=size(G,1);Q2=Q2T(1:p,:);T=Q2T(p+1:$,:);
Q=G\Q2;   //a valid [Q;T] since 
[G*B;D]*R + [G*Q;T]  // is zero
closed=syslin('c',A+B*F,Q+B*R,C+D*F,T+D*R); // closed loop: d-->y
ss2tf(closed)       // Closed loop is zero
spec(closed('A'))

xdel(winsid())

//====================================================
// ../man/fr/control/arhnk.xml
//====================================================
clear;lines(0);

A=diag([-1,-2,-3,-4,-5]);B=rand(5,1);C=rand(1,5);
sl=syslin('c',A,B,C);
slapprox=arhnk(sl,2);
[nk,W]=hankelsv(sl);nk
[nkred,Wred]=hankelsv(slapprox);nkred

xdel(winsid())

//====================================================
// ../man/fr/control/arl2.xml
//====================================================
clear;lines(0);

v=ones(1,20);
xbasc();
plot2d1('enn',0,[v';zeros(80,1)],2,'051',' ',[1,-0.5,100,1.5])

[d,n,e]=arl2(v,poly(1,'z','c'),1)
plot2d1('enn',0,ldiv(n,d,100),2,'000')
[d,n,e]=arl2(v,d,3)
plot2d1('enn',0,ldiv(n,d,100),3,'000')
[d,n,e]=arl2(v,d,8)
plot2d1('enn',0,ldiv(n,d,100),5,'000')

[d,n,e]=arl2(v,poly(1,'z','c'),4,'all')
plot2d1('enn',0,ldiv(n(1),d(1),100),10,'000')

xdel(winsid())

//====================================================
// ../man/fr/control/balreal.xml
//====================================================
clear;lines(0);

A=diag([-1,-2,-3,-4,-5]);B=rand(5,2);C=rand(1,5);
sl=syslin('c',A,B,C);
[slb,U]=balreal(sl);
Wc=clean(ctr_gram(slb))
W0=clean(obs_gram(slb))

xdel(winsid())

//====================================================
// ../man/fr/control/bilin.xml
//====================================================
clear;lines(0);

s=poly(0,'s');z=poly(0,'z');
w=ssrand(1,1,3);
wtf=ss2tf(w);v=[2,3,-1,4];a=v(1);b=v(2);c=v(3);d=v(4);
[horner(wtf,(a*z+b)/(c*z+d)),ss2tf(bilin(w,[a,b,c,d]))]
clean(ss2tf(bilin(bilin(w,[a,b,c,d]),[d,-b,-c,a]))-wtf)

xdel(winsid())

//====================================================
// ../man/fr/control/cainv.xml
//====================================================

//====================================================
// ../man/fr/control/calfrq.xml
//====================================================
clear;lines(0);

s=poly(0,'s')
h=syslin('c',(s^2+2*0.9*10*s+100)/(s^2+2*0.3*10.1*s+102.01))
h1=h*syslin('c',(s^2+2*0.1*15.1*s+228.01)/(s^2+2*0.9*15*s+225)) 
[f1,spl]=calfrq(h1,0.01,1000);
rf=repfreq(h1,f1);
plot2d(real(rf)',imag(rf)')

xdel(winsid())

//====================================================
// ../man/fr/control/canon.xml
//====================================================
clear;lines(0);

A=[1,2,3,4,5;
   1,0,0,0,0;
   0,1,0,0,0;
   6,7,8,9,0;
   0,0,0,1,0];
B=[1,2;
   0,0;
   0,0;
   2,1;
   0,0];
X=rand(5,5);A=X*A*inv(X);B=X*B;    //Controllable pair 
[Ac,Bc,U,ind]=canon(A,B);  //Two indices --> ind=[3.2];
index=1;for k=1:size(ind,'*')-1,index=[index,1+sum(ind(1:k))];end
Acstar=Ac(index,:);Bcstar=Bc(index,:);
s=poly(0,'s');
p1=s^3+2*s^2-5*s+3;p2=(s-5)*(s-3);   
//p1 and p2 are desired closed-loop polynomials with degrees 3,2
c1=coeff(p1);c1=c1($-1:-1:1);c2=coeff(p2);c2=c2($-1:-1:1);
Acstardesired=[-c1,0,0;0,0,0,-c2];  
//Acstardesired(index,:) is companion matrix with char. pol=p1*p2
F=Bcstar\(Acstardesired-Acstar);   //Feedbak gain
Ac+Bc*F         // Companion form 
spec(A+B*F/U)   // F/U is the gain matrix in original basis.

xdel(winsid())

//====================================================
// ../man/fr/control/cls2dls.xml
//====================================================
clear;lines(0);

s=poly(0,'s');z=poly(0,'z');
sl=syslin('c',(s+1)/(s^2-5*s+2));  //Continuous-time system in transfer form
slss=tf2ss(sl);  //Now in state-space form
sl1=cls2dls(slss,0.2);  //sl1= output of cls2dls
sl1t=ss2tf(sl1) // Converts in transfer form
sl2=horner(sl,(2/0.2)*(z-1)/(z+1))   //Compare sl2 and sl1

xdel(winsid())

//====================================================
// ../man/fr/control/colregul.xml
//====================================================

//====================================================
// ../man/fr/control/cont_frm.xml
//====================================================
clear;lines(0);

s=poly(0,'s');NUM=[1+s,s];den=s^2-5*s+1;
sl=cont_frm(NUM,den); 
slss=ss2tf(sl);       //Compare with NUM/den

xdel(winsid())

//====================================================
// ../man/fr/control/cont_mat.xml
//====================================================

//====================================================
// ../man/fr/control/contrss.xml
//====================================================
clear;lines(0);

A=[1,1;0,2];B=[1;0];C=[1,1];sl=syslin('c',A,B,C);  //Non minimal
slc=contrss(sl);
sl1=ss2tf(sl);sl2=ss2tf(slc);      //Compare sl1 and sl2

xdel(winsid())

//====================================================
// ../man/fr/control/contr.xml
//====================================================
clear;lines(0);

W=ssrand(2,3,5,list('co',3));  //cont. subspace has dim 3.
A=W("A");B=W("B");
[n,U]=contr(A,B);n
A1=U'*A*U;
spec(A1(n+1:$,n+1:$))  //uncontrollable modes
spec(A+B*rand(3,5))    

xdel(winsid())

//====================================================
// ../man/fr/control/csim.xml
//====================================================
clear;lines(0);

s=poly(0,'s');rand('seed',0);w=ssrand(1,1,3);w('A')=w('A')-2*eye();
t=0:0.05:5;
//impulse(w) = step (s * w)
xbasc(0);xset("window",0);xselect();
plot2d([t',t'],[(csim('step',t,tf2ss(s)*w))',0*t'])
xbasc(1);xset("window",1);xselect();
plot2d([t',t'],[(csim('impulse',t,w))',0*t'])
//step(w) = impulse (s^-1 * w)
xbasc(3);xset("window",3);xselect();
plot2d([t',t'],[(csim('step',t,w))',0*t'])
xbasc(4);xset("window",4);xselect();
plot2d([t',t'],[(csim('impulse',t,tf2ss(1/s)*w))',0*t'])

//input defined by a time function
deff('u=input(t)','u=abs(sin(t))')
xbasc();plot2d([t',t'],[(csim(input,t,w))',0*t'])


xdel(winsid())

//====================================================
// ../man/fr/control/ctr_gram.xml
//====================================================
clear;lines(0);

A=diag([-1,-2,-3]);B=rand(3,2);
Wc=ctr_gram(A,B)
U=rand(3,3);A1=U*A/U;B1=U*B;
Wc1=ctr_gram(A1,B1)    //Not invariant!

xdel(winsid())

//====================================================
// ../man/fr/control/dbphi.xml
//====================================================

//====================================================
// ../man/fr/control/ddp.xml
//====================================================
clear;lines(0);

rand('seed',0);nx=6;nz=3;nu=2;ny=1;
A=diag(1:6);A(2,2)=-7;A(5,5)=-9;B2=[1,2;0,3;0,4;0,5;0,0;0,0];
C1=[zeros(nz,nz),eye(nz,nz)];D12=[0,1;0,2;0,3];
Sys12=syslin('c',A,B2,C1,D12);
C=[C1;rand(ny,nx)];D2=[D12;rand(ny,size(D12,2))];
Sys=syslin('c',A,B2,C,D2);
[A,B2,C1,D12]=abcd(Sys12);  //The matrices of Sys12.
alfa=-1;beta=-2;flag='ge';
[X,dims,F,U,k,Z]=abinv(Sys12,alfa,beta,flag);
clean(X'*(A+B2*F)*X)
clean(X'*B2*U)
clean((C1+D12*F)*X)
clean(D12*U);
//Calculating an ad-hoc B1,D1
G1=rand(size(B2,2),3);
B1=-B2*G1;
D11=-D12*G1;
D1=[D11;rand(ny,size(B1,2))];

[Closed,F,G]=ddp(Sys,1:nz,B1,D1,'st',alfa,beta);
closed=syslin('c',A+B2*F,B1+B2*G,C1+D12*F,D11+D12*G);
ss2tf(closed)

xdel(winsid())

//====================================================
// ../man/fr/control/des2tf.xml
//====================================================
clear;lines(0);

s=poly(0,'s');
G=[1/(s+1),s;1+s^2,3*s^3];
Descrip=tf2des(G);Tf1=des2tf(Descrip)
Descrip2=tf2des(G,"withD");Tf2=des2tf(Descrip2)
[A,B,C,D,E]=Descrip2(2:6);Tf3=C*inv(s*E-A)*B+D

xdel(winsid())

//====================================================
// ../man/fr/control/dscr.xml
//====================================================
clear;lines(0);

s=poly(0,'s');
Sys=syslin('c',[1,1/(s+1);2*s/(s^2+2),1/s])
ss2tf(dscr(tf2ss(Sys),0.1))

xdel(winsid())

//====================================================
// ../man/fr/control/dsimul.xml
//====================================================
clear;lines(0);

z=poly(0,'z');
h=(1-2*z)/(z^2-0.2*z+1);
sl=tf2ss(h);
u=zeros(1,20);u(1)=1;
x1=dsimul(sl,u)   //Impulse response
u=ones(1,20);
x2=dsimul(sl,u);  //Step response

xdel(winsid())

//====================================================
// ../man/fr/control/dt_ility.xml
//====================================================
clear;lines(0);

A=[2,1,1;0,-2,1;0,0,3];
C=[0,0,1];
X=rand(3,3);A=inv(X)*A*X;C=C*X;
W=syslin('c',A,[],C);
[k,n,U,W1]=dt_ility(W);
W1("A")
W1("C")

xdel(winsid())

//====================================================
// ../man/fr/control/equil1.xml
//====================================================
clear;lines(0);

S1=rand(2,2);S1=S1*S1';
S2=rand(2,2);S2=S2*S2';
S3=rand(2,2);S3=S3*S3';
P=sysdiag(S1,S2,zeros(4,4));
Q=sysdiag(S1,zeros(2,2),S3,zeros(2,2));
X=rand(8,8);
P=X*P*X';Q=inv(X)'*Q*inv(X);
[T,siz]=equil1(P,Q);
P1=clean(T*P*T')
Q1=clean(inv(T)'*Q*inv(T))

xdel(winsid())

//====================================================
// ../man/fr/control/equil.xml
//====================================================
clear;lines(0);

P=rand(4,4);P=P*P';
Q=rand(4,4);Q=Q*Q';
T=equil(P,Q)
clean(T*P*T')
clean(inv(T)'*Q*inv(T))

xdel(winsid())

//====================================================
// ../man/fr/control/feedback.xml
//====================================================
clear;lines(0);

S1=ssrand(2,2,3);S2=ssrand(2,2,2);
W=S1/.S2;
ss2tf(S1/.S2)
//Same operation by LFT:
ss2tf(lft([zeros(2,2),eye(2,2);eye(2,2),-S2],S1))
//Other approach: with constant feedback
BigS=sysdiag(S1,S2); F=[zeros(2,2),eye(2,2);-eye(2,2),zeros(2,2)];
Bigclosed=BigS/.F;
W1=Bigclosed(1:2,1:2);   //W1=W (in state-space).
ss2tf(W1)
//Inverting
ss2tf(S1*inv(eye()+S2*S1))

xdel(winsid())

//====================================================
// ../man/fr/control/flts.xml
//====================================================
clear;lines(0);

sl=syslin('d',1,1,1);u=1:10;
y=flts(u,sl); 
plot2d2("onn",(1:size(u,'c'))',y')
[y1,x1]=flts(u(1:5),sl);y2=flts(u(6:10),sl,x1);
y-[y1,y2]

//With polynomial D:
z=poly(0,'z');
D=1+z+z^2; p =degree(D);
sl=syslin('d',1,1,1,D);
y=flts(u,sl);[y1,x1]=flts(u(1:5),sl);
y2=flts(u(5-p+1:10),sl,x1);  // (update)
y-[y1,y2]

//Delay (transfer form): flts(u,1/z)
// Usual responses
z=poly(0,'z');
h=(1-2*z)/(z^2+0.3*z+1)
u=zeros(1,20);u(1)=1;
imprep=flts(u,tf2ss(h));   //Impulse response
plot2d2("onn",(1:size(u,'c'))',imprep')
u=ones(1,20);
stprep=flts(u,tf2ss(h));   //Step response
plot2d2("onn",(1:size(u,'c'))',stprep')
//
// Other examples
A=[1 2 3;0 2 4;0 0 1];B=[1 0;0 0;0 1];C=eye(3,3);Sys=syslin('d',A,B,C);
H=ss2tf(Sys); u=[1;-1]*(1:10);
//
yh=flts(u,H); ys=flts(u,Sys);
norm(yh-ys,1)    
//hot restart
[ys1,x]=flts(u(:,1:4),Sys);ys2=flts(u(:,5:10),Sys,x);
norm([ys1,ys2]-ys,1)
//
yh1=flts(u(:,1:4),H);yh2=flts(u(:,5:10),H,[u(:,2:4);yh(:,2:4)]);
norm([yh1,yh2]-yh,1)
//with D<>0
D=[-3 8;4 -0.5;2.2 0.9];
Sys=syslin('d',A,B,C,D);
H=ss2tf(Sys); u=[1;-1]*(1:10);
rh=flts(u,H); rs=flts(u,Sys);
norm(rh-rs,1)
//hot restart
[ys1,x]=flts(u(:,1:4),Sys);ys2=flts(u(:,5:10),Sys,x);
norm([ys1,ys2]-rs,1)
//With H:
yh1=flts(u(:,1:4),H);yh2=flts(u(:,5:10),H,[u(:,2:4); yh1(:,2:4)]);
norm([yh1,yh2]-rh)

xdel(winsid())

//====================================================
// ../man/fr/control/frep2tf.xml
//====================================================
clear;lines(0);

s=poly(0,'s');
h=syslin('c',(s-1)/(s^3+5*s+20))
frq=0:0.05:3;repf=repfreq(h,frq);
clean(frep2tf(frq,repf,3))

Sys=ssrand(1,1,10); 
frq=logspace(-3,2,200);
[frq,rep]=repfreq(Sys,frq);  //Frequency response of Sys
[Sys2,err]=frep2tf(frq,rep,10);Sys2=clean(Sys2)//Sys2 obtained from freq. resp of Sys
[frq,rep2]=repfreq(Sys2,frq); //Frequency response of Sys2
xbasc();bode(frq,[rep;rep2])   //Responses of Sys and Sys2
[sort(spec(Sys('A'))),sort(roots(Sys2('den')))] //poles

dom=1/1000; // Sampling time 
z=poly(0,'z');
h=syslin(dom,(z^2+0.5)/(z^3+0.1*z^2-0.5*z+0.08))
frq=(0:0.01:0.5)/dom;repf=repfreq(h,frq);
[Sys2,err]=frep2tf(frq,repf,3,dom);
[frq,rep2]=repfreq(Sys2,frq); //Frequency response of Sys2
xbasc();plot2d1("onn",frq',abs([repf;rep2])');


xdel(winsid())

//====================================================
// ../man/fr/control/freq.xml
//====================================================
clear;lines(0);

s=poly(0,'s');
sys=(s+1)/(s^3-5*s+4)
rep=freq(sys("num"),sys("den"),[0,0.9,1.1,2,3,10,20])
[horner(sys,0),horner(sys,20)]
//
Sys=tf2ss(sys);
[A,B,C,D]=abcd(Sys);
freq(A,B,C,[0,0.9,1.1,2,3,10,20])

xdel(winsid())

//====================================================
// ../man/fr/control/freson.xml
//====================================================
clear;lines(0);

h=syslin('c',-1+%s,(3+2*%s+%s^2)*(50+0.1*%s+%s^2))
fr=freson(h)
bode(h)
g=20*log(abs(repfreq(h,fr)))/log(10)

xdel(winsid())

//====================================================
// ../man/fr/control/gfrancis.xml
//====================================================
clear;lines(0);

Plant=ssrand(1,3,5);
[F,G,H,J]=abcd(Plant);
nw=4;nuu=2;A=rand(nw,nw);
st=maxi(real(spec(A)));A=A-st*eye(A);
B=rand(nw,nuu);C=2*rand(1,nw);D=0*rand(C*B);
Model=syslin('c',A,B,C,D);
[L,M,T]=gfrancis(Plant,Model);
norm(F*T+G*L-T*A,1)
norm(H*T+J*L-C,1)
norm(G*M-T*B,1)
norm(J*M-D,1)

xdel(winsid())

//====================================================
// ../man/fr/control/g_margin.xml
//====================================================
clear;lines(0);

h=syslin('c',-1+%s,3+2*%s+%s^2)
[g,fr]=g_margin(h)
[g,fr]=g_margin(h-10)
nyquist(h-10)

xdel(winsid())

//====================================================
// ../man/fr/control/imrep2ss.xml
//====================================================
clear;lines(0);

s=poly(0,'s');
H=[1/(s+0.5);2/(s-0.4)]   //strictly proper
np=20;w=ldiv(H('num'),H('den'),np);
rep=[w(1:np)';w(np+1:2*np)'];   //The impulse response
H1=ss2tf(imrep2ss(rep))
z=poly(0,'z');
H=(2*z^2-3.4*z+1.5)/(z^2-1.6*z+0.8)     //Proper transfer function
u=zeros(1,20);u(1)=1;
rep=rtitr(H('num'),H('den'),u);   //Impulse rep. 
//   <=> rep=ldiv(H('num'),H('den'),20)
w=z*imrep2ss(rep)   //Realization with shifted impulse response 
// i.e strictly proper to proper
H2=ss2tf(w);

xdel(winsid())

//====================================================
// ../man/fr/control/invsyslin.xml
//====================================================

//====================================================
// ../man/fr/control/kpure.xml
//====================================================
clear;lines(0);

s=poly(0,'s');
h=syslin('c',(s-1)/(1+5*s+s^2+s^3))
xbasc();evans(h)
g=kpure(h)
hf=h/.g(1)
roots(denom(hf))

xdel(winsid())

//====================================================
// ../man/fr/control/krac2.xml
//====================================================
clear;lines(0);

h=syslin('c',352*poly(-5,'s')/poly([0,0,2000,200,25,1],'s','c'));
xbasc();evans(h,100)
g=krac2(h)
hf1=h/.g(1);roots(denom(hf1))
hf2=h/.g(2);roots(denom(hf2))

xdel(winsid())

//====================================================
// ../man/fr/control/linmeq.xml
//====================================================
clear;lines(0);

//(1a)
n=40;m=30;
A=rand(n,n);C=rand(n,m);B=rand(m,m);
X = linmeq(1,A,B,C);
norm(A*X+X*B-C,1)
//(1b)
flag=[1,0,0]
X = linmeq(1,A,B,C,flag);
norm(A*X*B+X-C,1)
//(2a)
A=rand(n,n);C=rand(A);C=C+C';
X = linmeq(2,A,C);
norm(A'*X + X*A -C,1)
//(2b)
X = linmeq(2,A,C,[1 0]);
norm(A'*X*A -X-C,1)
//(3a)
A=rand(n,n);
A=A-(max(real(spec(A)))+1)*eye(); //shift eigenvalues
C=rand(A);
X=linmeq(3,A,C);
norm(A'*X'*X+X'*X*A +C'*C,1)
//(3b)
A = [-0.02, 0.02,-0.10, 0.02,-0.03, 0.12;
      0.02, 0.14, 0.12,-0.10,-0.02,-0.14;     
     -0.10, 0.12, 0.05, 0.03,-0.04,-0.04;     
      0.02,-0.10, 0.03,-0.06, 0.08, 0.11;      
     -0.03,-0.02,-0.04, 0.08, 0.14,-0.07;   
      0.12,-0.14,-0.04, 0.11,-0.07, 0.04]    

C=rand(A);
X=linmeq(3,A,C,[1 0]);
norm(A'*X'*X*A - X'*X +C'*C,1)

xdel(winsid())

//====================================================
// ../man/fr/control/lin.xml
//====================================================
clear;lines(0);

deff('[y,xdot]=sim(x,u)','xdot=[u*sin(x);-u*x^2];y=xdot(1)+xdot(2)')
sl=lin(sim,1,2);

xdel(winsid())

//====================================================
// ../man/fr/control/lqe.xml
//====================================================

//====================================================
// ../man/fr/control/lqg2stan.xml
//====================================================
clear;lines(0);

ny=2;nu=3;nx=4;
P22=ssrand(ny,nu,nx);
bigQ=rand(nx+nu,nx+nu);bigQ=bigQ*bigQ';
bigR=rand(nx+ny,nx+ny);bigR=bigR*bigR';
[P,r]=lqg2stan(P22,bigQ,bigR);K=lqg(P,r);  //K=LQG-controller
spec(h_cl(P,r,K))      //Closed loop should be stable
//Same as Cl=P22/.K; spec(Cl('A'))
s=poly(0,'s')
lqg2stan(1/(s+2),eye(2,2),eye(2,2))

xdel(winsid())

//====================================================
// ../man/fr/control/lqg.xml
//====================================================

//====================================================
// ../man/fr/control/lqr.xml
//====================================================
clear;lines(0);

A=rand(2,2);B=rand(2,1);   //two states, one input
Q=diag([2,5]);R=2;     //Usual notations x'Qx + u'Ru
Big=sysdiag(Q,R);    //Now we calculate C1 and D12
[w,wp]=fullrf(Big);C1=wp(:,1:2);D12=wp(:,3:$);   //[C1,D12]'*[C1,D12]=Big
P=syslin('c',A,B,C1,D12);    //The plant (continuous-time)
[K,X]=lqr(P)
spec(A+B*K)    //check stability
norm(A'*X+X*A-X*B*inv(R)*B'*X+Q,1)  //Riccati check
P=syslin('d',A,B,C1,D12);    // Discrete time plant
[K,X]=lqr(P)     
spec(A+B*K)   //check stability
norm(A'*X*A-(A'*X*B)*pinv(B'*X*B+R)*(B'*X*A)+Q-X,1) //Riccati check

xdel(winsid())

//====================================================
// ../man/fr/control/ltitr.xml
//====================================================
clear;lines(0);

A=eye(2,2);B=[1;1];
x0=[-1;-2];
u=[1,2,3,4,5];
x=ltitr(A,B,u,x0)
x1=A*x0+B*u(1)
x2=A*x1+B*u(2)
x3=A*x2+B*u(3) //....

xdel(winsid())

//====================================================
// ../man/fr/control/markp2ss.xml
//====================================================
clear;lines(0);

W=ssrand(2,3,4);   //random system with 2 outputs and 3 inputs
[a,b,c,d]=abcd(W);
markpar=[c*b,c*a*b,c*a^2*b,c*a^3*b,c*a^4*b];
S=markp2ss(markpar,5,2,3);
[A,B,C,D]=abcd(S);
Markpar=[C*B,C*A*B,C*A^2*B,C*A^3*B,C*A^4*B];
norm(markpar-Markpar,1)
//Caution... c*a^5*b is not C*A^5*B !

xdel(winsid())

//====================================================
// ../man/fr/control/minreal.xml
//====================================================
clear;lines(0);

A=[-eye(2,2),rand(2,2);zeros(2,2),-2*eye(2,2)];
B=[rand(2,2);zeros(2,2)];C=rand(2,4);
sl=syslin('c',A,B,C);
slb=minreal(sl);
ss2tf(sl)
ss2tf(slb)
ctr_gram(sl)
clean(ctr_gram(slb))
clean(obs_gram(slb))

xdel(winsid())

//====================================================
// ../man/fr/control/minss.xml
//====================================================
clear;lines(0);

sl=syslin('c',[1 0;0 2],[1;0],[2 1]);
ssprint(sl);
ssprint(minss(sl))

xdel(winsid())

//====================================================
// ../man/fr/control/obscont.xml
//====================================================
clear;lines(0);

ny=2;nu=3;nx=4;P=ssrand(ny,nu,nx);[A,B,C,D]=abcd(P);
Kc=-ppol(A,B,[-1,-1,-1,-1]);  //Controller gain
Kf=-ppol(A',C',[-2,-2,-2,-2]);Kf=Kf';    //Observer gain
cl=P/.(-obscont(P,Kc,Kf));spec(cl('A'))   //closed loop system
[J,r]=obscont(P,Kc,Kf);
Q=ssrand(nu,ny,3);Q('A')=Q('A')-(maxi(real(spec(Q('A'))))+0.5)*eye(Q('A')) 
//Q is a stable parameter
K=lft(J,r,Q);
spec(h_cl(P,K))  // closed-loop A matrix (should be stable);

xdel(winsid())

//====================================================
// ../man/fr/control/observer.xml
//====================================================
clear;lines(0);

nx=5;nu=1;ny=1;un=3;us=2;Sys=ssrand(ny,nu,nx,list('dt',us,us,un));
//nx=5 states, nu=1 input, ny=1 output, 
//un=3 unobservable states, us=2 of them unstable.
[Obs,U,m]=observer(Sys);  //Stable observer (default)
W=U';H=W(m+1:nx,:);[A,B,C,D]=abcd(Sys);  //H*U=[0,eye(no,no)];
Sys2=ss2tf(syslin('c',A,B,H))  //Transfer u-->z
Idu=eye(nu,nu);Sys3=ss2tf(H*U(:,m+1:$)*Obs*[Idu;Sys])  
//Transfer u-->[u;y=Sys*u]-->Obs-->xhat-->HUxhat=zhat  i.e. u-->output of Obs
//this transfer must equal Sys2, the u-->z transfer  (H2=eye).

xdel(winsid())

//====================================================
// ../man/fr/control/obs_gram.xml
//====================================================
clear;lines(0);

A=-diag(1:3);C=rand(2,3);
Go=obs_gram(A,C,'c');     // <=> w=syslin('c',A,[],C); Go=obs_gram(w);
norm(Go*A+A'*Go+C'*C,1)
norm(lyap(A,-C'*C,'c')-Go,1)
A=A/4; Go=obs_gram(A,C,'d');    //discrete time case
norm(lyap(A,-C'*C,'d')-Go,1)

xdel(winsid())

//====================================================
// ../man/fr/control/obsv_mat.xml
//====================================================

//====================================================
// ../man/fr/control/obsvss.xml
//====================================================

//====================================================
// ../man/fr/control/pfss.xml
//====================================================
clear;lines(0);

W=ssrand(1,1,6);
elts=pfss(W); 
W1=0;for k=1:size(elts), W1=W1+ss2tf(elts(k));end
clean(ss2tf(W)-W1)

xdel(winsid())

//====================================================
// ../man/fr/control/phasemag.xml
//====================================================
clear;lines(0);

s=poly(0,'s');
h=syslin('c',1/((s+5)*(s+10)*(100+6*s+s*s)*(s+.3)));
[frq,rf]=repfreq(h,0.1,20,0.005);
xbasc(0);
plot2d(frq',phasemag(rf,'c')');
xbasc(1);
plot2d(frq',phasemag(rf,'m')');

xdel(winsid())

//====================================================
// ../man/fr/control/p_margin.xml
//====================================================
clear;lines(0);

h=syslin('c',-1+%s,3+2*%s+%s^2)
[p,fr]=p_margin(h)  
[p,fr]=p_margin(h+0.7)  
nyquist(h+0.7)
t=(0:0.1:2*%pi)';plot2d(sin(t),cos(t),-3,'000')

xdel(winsid())

//====================================================
// ../man/fr/control/ppol.xml
//====================================================
clear;lines(0);

A=rand(3,3);B=rand(3,2);
F=ppol(A,B,[-1,-2,-3]);
spec(A-B*F)

xdel(winsid())

//====================================================
// ../man/fr/control/projsl.xml
//====================================================
clear;lines(0);

rand('seed',0);sl=ssrand(2,2,5);[A,B,C,D]=abcd(sl);poles=spec(A)
[Q,M]=pbig(A,0,'c');  //keeping unstable poles
slred=projsl(sl,Q,M);spec(slred('A'))
sl('D')=rand(2,2);  //making proper system
trzeros(sl)  //zeros of sl
wi=inv(sl);  //wi=inverse in state-space
[q,m]=psmall(wi('A'),2,'d');  //keeping small zeros (poles of wi) i.e. abs(z)<2
slred2=projsl(sl,q,m);
trzeros(slred2)  //zeros of slred2 = small zeros of sl
//  Example keeping second order modes
A=diag([-1,-2,-3]);
sl=syslin('c',A,rand(3,2),rand(2,3));[nk2,W]=hankelsv(sl)
[Q,M]=pbig(W,nk2(2)-%eps,'c');    //keeping 2 eigenvalues of W
slr=projsl(sl,Q,M);  //reduced model
hankelsv(slr)

xdel(winsid())

//====================================================
// ../man/fr/control/repfreq.xml
//====================================================
clear;lines(0);

A=diag([-1,-2]);B=[1;1];C=[1,1];
Sys=syslin('c',A,B,C);
frq=0:0.02:5;w=frq*2*%pi; //frq=frequencies in Hz ;w=frequencies in rad/sec;
[frq1,rep] =repfreq(Sys,frq);
[db,phi]=dbphi(rep);
Systf=ss2tf(Sys)    //Transfer function of Sys
x=horner(Systf,w(2)*sqrt(-1))    // x is Systf(s) evaluated at s = i w(2)
rep=20*log(abs(x))/log(10)   //magnitude of x in dB
db(2)    // same as rep
ang=atan(imag(x),real(x));   //in rad.
ang=ang*180/%pi              //in degrees
phi(2)
repf=repfreq(Sys,frq);
repf(2)-x

xdel(winsid())

//====================================================
// ../man/fr/control/riccsl.xml
//====================================================

//====================================================
// ../man/fr/control/ricc.xml
//====================================================
clear;lines(0);

//Standard formulas to compute Riccati solutions
A=rand(3,3);B=rand(3,2);C=rand(3,3);C=C*C';R=rand(2,2);R=R*R'+eye();
B=B*inv(R)*B';
X=ricc(A,B,C,'cont');
norm(A'*X+X*A-X*B*X+C,1)
H=[A -B;-C -A'];
[T,d]=schur(eye(H),H,'cont');T=T(:,1:d);
X1=T(4:6,:)/T(1:3,:);
norm(X1-X,1)
[T,d]=schur(H,'cont');T=T(:,1:d);
X2=T(4:6,:)/T(1:3,:);
norm(X2-X,1)
//       Discrete time case
F=A;B=rand(3,2);G1=B;G2=R;G=G1/G2*G1';H=C;
X=ricc(F,G,H,'disc');
norm(F'*X*F-(F'*X*G1/(G2+G1'*X*G1))*(G1'*X*F)+H-X)
H1=[eye(3,3) G;zeros(3,3) F'];
H2=[F zeros(3,3);-H eye(3,3)];
[T,d]=schur(H2,H1,'disc');T=T(:,1:d);X1=T(4:6,:)/T(1:3,:);
norm(X1-X,1)
Fi=inv(F);
Hami=[Fi Fi*G;H*Fi F'+H*Fi*G];
[T,d]=schur(Hami,'d');T=T(:,1:d);
Fit=inv(F');
Ham=[F+G*Fit*H -G*Fit;-Fit*H Fit];
[T,d]=schur(Ham,'d');T=T(:,1:d);X2=T(4:6,:)/T(1:3,:);
norm(X2-X,1)

xdel(winsid())

//====================================================
// ../man/fr/control/rowregul.xml
//====================================================
clear;lines(0);

s=%s;
w=[1/s,0;s/(s^3+2),2/s];
Sl=tf2ss(w);
[Stmp,Ws]=rowregul(Sl,-1,-2);
Stmp('D')     // D matrix of Stmp
clean(ss2tf(Stmp))

xdel(winsid())

//====================================================
// ../man/fr/control/rtitr.xml
//====================================================
clear;lines(0);

z=poly(0,'z');
Num=1+z;Den=1+z;u=[1,2,3,4,5];
rtitr(Num,Den,u)-u
//Other examples
//siso
//causal
n1=1;d1=poly([1 1],'z','coeff');       // y(j)=-y(j-1)+u(j-1)
r1=[0 1 0 1 0 1 0 1 0 1 0];
r=rtitr(n1,d1,ones(1,10));norm(r1-r,1)
//hot restart
r=rtitr(n1,d1,ones(1,9),1,0);norm(r1(2:11)-r)
//non causal
n2=poly([1 1 1],'z','coeff');d2=d1;    // y(j)=-y(j-1)+u(j-1)+u(j)+u(j+1)
r2=[2 1 2 1 2 1 2 1 2];
r=rtitr(n2,d2,ones(1,10));norm(r-r2,1)
//hot restart
r=rtitr(n2,d2,ones(1,9),1,2);norm(r2(2:9)-r,1)
//
//MIMO example
//causal
d1=d1*diag([1 0.5]);n1=[1 3 1;2 4 1];r1=[5;14]*r1;
r=rtitr(n1,d1,ones(3,10));norm(r1-r,1)
//
r=rtitr(n1,d1,ones(3,9),[1;1;1],[0;0]);
norm(r1(:,2:11)-r,1)
//polynomial n1  (same ex.)
n1(1,1)=poly(1,'z','c');r=rtitr(n1,d1,ones(3,10));norm(r1-r,1)
//
r=rtitr(n1,d1,ones(3,9),[1;1;1],[0;0]);
norm(r1(:,2:11)-r,1)
//non causal
d2=d1;n2=n2*n1;r2=[5;14]*r2;
r=rtitr(n2,d2,ones(3,10));norm(r2-r)
//
r=rtitr(n2,d2,ones(3,9),[1;1;1],[10;28]);
norm(r2(:,2:9)-r,1)
//
//  State-space or transfer
a = [0.21 , 0.63 , 0.56 , 0.23 , 0.31
     0.76 , 0.85 , 0.66 , 0.23 , 0.93
     0 , 0.69 , 0.73 , 0.22 , 0.21
     0.33 , 0.88 , 0.2 , 0.88 , 0.31
     0.67 , 0.07 , 0.54 , 0.65 , 0.36];
b = [0.29 , 0.5 , 0.92
     0.57 , 0.44 , 0.04
     0.48 , 0.27 , 0.48
     0.33 , 0.63 , 0.26
     0.59 , 0.41 , 0.41];
c = [0.28 , 0.78 , 0.11 , 0.15 , 0.84
     0.13 , 0.21 , 0.69 , 0.7 , 0.41];
d = [0.41 , 0.11 , 0.56
     0.88 , 0.2 , 0.59];
s=syslin('d',a,b,c,d);
h=ss2tf(s);num=h('num');den=h('den');den=den(1,1)*eye(2,2);
u=1;u(3,10)=0;r3=flts(u,s);
r=rtitr(num,den,u);norm(r3-r,1)

xdel(winsid())

//====================================================
// ../man/fr/control/sm2des.xml
//====================================================

//====================================================
// ../man/fr/control/sm2ss.xml
//====================================================

//====================================================
// ../man/fr/control/specfact.xml
//====================================================
clear;lines(0);

A=diag([-1,-2]);B=[1;1];C=[1,1];D=1;s=poly(0,'s');
W1=syslin('c',A,B,C,D);
phi=gtild(W1,'c')+W1;
phis=clean(ss2tf(phi))
clean(phis-horner(phis,-s)');   //check this is 0...
[A,B,C,D]=abcd(W1);
[W0,L]=specfact(A,B,C,D);
W=syslin('c',A,B,L,W0)
Ws=ss2tf(W);
horner(Ws,-s)*Ws

xdel(winsid())

//====================================================
// ../man/fr/control/ss2des.xml
//====================================================
clear;lines(0);

s=poly(0,'s');
G=[1/(s+1),s;1+s^2,3*s^3];Sl=tf2ss(G);
S=ss2des(Sl)
S1=ss2des(Sl,"withD")
Des=des2ss(S);Des(5)=clean(Des(5))
Des1=des2ss(S1)

xdel(winsid())

//====================================================
// ../man/fr/control/ss2ss.xml
//====================================================
clear;lines(0);

Sl=ssrand(2,2,5); trzeros(Sl)       // zeros are invariant:
Sl1=ss2ss(Sl,rand(5,5),rand(2,5),rand(5,2)); 
trzeros(Sl1), trzeros(rand(2,2)*Sl1*rand(2,2))
// output injection [ A + GC, (B+GD,-G)]
//                  [   C   , (D   , 0)]
p=1,m=2,n=2; sys=ssrand(p,m,n);

// feedback (m,n)  first and then output injection.

F1=rand(m,n);
G=rand(n,p);
[sys1,right,left]=ss2ss(sys,rand(n,n),F1,G,2);

// Sl1 equiv left*sysdiag(sys*right,eye(p,p)))

res=clean(ss2tf(sys1) - ss2tf(left*sysdiag(sys*right,eye(p,p))))

// output injection then feedback (m+p,n) 
F2=rand(p,n); F=[F1;F2];
[sys2,right,left]=ss2ss(sys,rand(n,n),F,G,1);

// Sl1 equiv left*sysdiag(sys,eye(p,p))*right 

res=clean(ss2tf(sys2)-ss2tf(left*sysdiag(sys,eye(p,p))*right))

// when F2= 0; sys1 and sys2 are the same 
F2=0*rand(p,n);F=[F1;F2];
[sys2,right,left]=ss2ss(sys,rand(n,n),F,G,1);

res=clean(ss2tf(sys2)-ss2tf(sys1))

xdel(winsid())

//====================================================
// ../man/fr/control/ss2tf.xml
//====================================================
clear;lines(0);

s=poly(0,'s');
h=[1,1/s;1/(s^2+1),s/(s^2-2)]
sl=tf2ss(h);
h=clean(ss2tf(sl))
[Ds,NUM,chi]=ss2tf(sl)

xdel(winsid())

//====================================================
// ../man/fr/control/stabil.xml
//====================================================
clear;lines(0);

// Gain:
Sys=ssrand(0,2,5,list('st',2,3,3));
A=Sys('A');B=Sys('B');F=stabil(A,B);
spec(A) //2 controllable modes 2 unstable uncontrollable modes
//and one stable uncontrollable mode
spec(A+B*F) //the two controllable modes are set to -1.
// Compensator:
Sys=ssrand(3,2,5,list('st',2,3,3)); //3 outputs, 2 inputs, 5 states
//2 controllables modes, 3 controllable or stabilizable modes.
K=stabil(Sys,-2,-3);  //Compensator for Sys.
spec(Sys('A'))
spec(h_cl(Sys,K))   //K Stabilizes what can be stabilized.

xdel(winsid())

//====================================================
// ../man/fr/control/st_ility.xml
//====================================================
clear;lines(0);

A=diag([0.9,-2,3]);B=[0;0;1];Sl=syslin('c',A,B,[]);
[ns,nc,U]=st_ility(Sl);
U'*A*U
U'*B
[ns,nc,U]=st_ility(syslin('d',A,B,[]));
U'*A*U
U'*B

xdel(winsid())

//====================================================
// ../man/fr/control/svplot.xml
//====================================================
clear;lines(0);

x=logspace(-3,3);
y=svplot(ssrand(2,2,4),x);
xbasc();plot2d1("oln",x',20*log(y')/log(10));
xgrid(12)
xtitle("Singular values plot","(Rd/sec)", "Db");

xdel(winsid())

//====================================================
// ../man/fr/control/sysfact.xml
//====================================================
clear;lines(0);

//Kalman filter
Sys=ssrand(3,2,4);Sys('D')=rand(3,2);
S=sysfact(Sys,lqr(Sys),'post');
ww=minss(Sys*S);
ss2tf(gtild(ww)*ww),Sys('D')'*Sys('D')
//Kernel
Sys=ssrand(2,3,4);
[X,d,F,U,k,Z]=abinv(Sys);
ss2tf(Sys*Z)
ss2tf(Sys*sysfact(Sys,F,'post')*U)

xdel(winsid())

//====================================================
// ../man/fr/control/syssize.xml
//====================================================

//====================================================
// ../man/fr/control/tf2ss.xml
//====================================================
clear;lines(0);

s=poly(0,'s');
H=[2/s,(s+1)/(s^2-5)];
Sys=tf2ss(H)
clean(ss2tf(Sys))

xdel(winsid())

//====================================================
// ../man/fr/control/time_id.xml
//====================================================
clear;lines(0);

z=poly(0,'z');
h=(1-2*z)/(z^2-0.5*z+5)
rep=[0;ldiv(h('num'),h('den'),20)]; //impulse response
H=time_id(2,'impuls',rep)
//  Same example with flts and u
u=zeros(1,20);u(1)=1;
rep=flts(u,tf2ss(h));        //impulse response
H=time_id(2,u,rep)
//  step response
u=ones(1,20);
rep=flts(u,tf2ss(h));     //step response.
H=time_id(2,'step',rep)
H=time_id(3,u,rep)    //with u as input and too high order required

xdel(winsid())

//====================================================
// ../man/fr/control/trzeros.xml
//====================================================
clear;lines(0);

W1=ssrand(2,2,5);trzeros(W1)    //call trzeros
roots(det(systmat(W1)))         //roots of det(system matrix)
s=poly(0,'s');W=[1/(s+1);1/(s-2)];W2=(s-3)*W*W';[nt,dt,rk]=trzeros(W2);
St=systmat(tf2ss(W2));[Q,Z,Qd,Zd,numbeps,numbeta]=kroneck(St);
St1=Q*St*Z;rowf=(Qd(1)+Qd(2)+1):(Qd(1)+Qd(2)+Qd(3));
colf=(Zd(1)+Zd(2)+1):(Zd(1)+Zd(2)+Zd(3));
roots(St1(rowf,colf)), nt./dt     //By Kronecker form

xdel(winsid())

//====================================================
// ../man/fr/control/ui_observer.xml
//====================================================
clear;lines(0);

A=diag([3,-3,7,4,-4,8]);
B=[eye(3,3);zeros(3,3)];
C=[0,0,1,2,3,4;0,0,0,0,0,1];
D=[1,2,3;0,0,0];
rand('seed',0);w=ss2ss(syslin('c',A,B,C,D),rand(6,6));
[A,B,C,D]=abcd(w);
B=[B,matrix(1:18,6,3)];D=[D,matrix(-(1:6),2,3)];
reject=1:3;
Sys=syslin('c',A,B,C,D);
N1=[-2,-3];C1=-N1*C;D1=-N1*D;
nw=length(reject);nu=size(Sys('B'),2)-nw;
ny=size(Sys('C'),1);nz=size(C1,1);
[UIobs,J,N]=ui_observer(Sys,reject,C1,D1);

W=[zeros(nu,nw),eye(nu,nu);Sys];UIobsW=UIobs*W;   
//(w,u) --> z=UIobs*[0,I;Sys](w,u)
clean(ss2tf(UIobsW));
wu_to_z=syslin('c',A,B,C1,D1);clean(ss2tf(wu_to_z));
clean(ss2tf(wu_to_z)-ss2tf(UIobsW),1.d-7)
/////2nd example//////
nx=2;ny=3;nwu=2;Sys=ssrand(ny,nwu,nx);
C1=rand(1,nx);D1=[0,1];
UIobs=ui_observer(Sys,1,C1,D1);

xdel(winsid())

//====================================================
// ../man/fr/control/unobs.xml
//====================================================
clear;lines(0);

A=diag([1,2,3]);C=[1,0,0];
unobs(A,C)

xdel(winsid())

//====================================================
// ../man/fr/control/zeropen.xml
//====================================================

//====================================================
// ../man/fr/dcd/cdfbet.xml
//====================================================

//====================================================
// ../man/fr/dcd/cdfbin.xml
//====================================================

//====================================================
// ../man/fr/dcd/cdfchi.xml
//====================================================

//====================================================
// ../man/fr/dcd/cdfchn.xml
//====================================================

//====================================================
// ../man/fr/dcd/cdffnc.xml
//====================================================

//====================================================
// ../man/fr/dcd/cdff.xml
//====================================================

//====================================================
// ../man/fr/dcd/cdfgam.xml
//====================================================

//====================================================
// ../man/fr/dcd/cdfnbn.xml
//====================================================

//====================================================
// ../man/fr/dcd/cdfnor.xml
//====================================================

//====================================================
// ../man/fr/dcd/cdfpoi.xml
//====================================================

//====================================================
// ../man/fr/dcd/cdft.xml
//====================================================

//====================================================
// ../man/fr/dcd/grand.xml
//====================================================

//====================================================
// ../man/fr/elementary/abs.xml
//====================================================
clear;lines(0);
abs([1,%i,-1,-%i,1+%i])
xdel(winsid())

//====================================================
// ../man/fr/elementary/acoshm.xml
//====================================================
clear;lines(0);
A=[1,2;3,4];
coshm(acoshm(A))
A(1,1)=A(1,1)+%i;
coshm(acoshm(A))
xdel(winsid())

//====================================================
// ../man/fr/elementary/acosh.xml
//====================================================
clear;lines(0);
x=[0,1,%i];
cosh(acosh(x))
xdel(winsid())

//====================================================
// ../man/fr/elementary/acosm.xml
//====================================================
clear;lines(0);
A=[1,2;3,4];
cosm(acosm(A))
xdel(winsid())

//====================================================
// ../man/fr/elementary/acos.xml
//====================================================
clear;lines(0);
x=[1,%i,-1,-%i]
cos(acos(x))
xdel(winsid())

//====================================================
// ../man/fr/elementary/addf.xml
//====================================================
clear;lines(0);
addf('0','1')
addf('1','a')
addf('1','2')
'a'+'b'
xdel(winsid())

//====================================================
// ../man/fr/elementary/adj2sp.xml
//====================================================
clear;lines(0);

A = sprand(100,50,.05);
[xadj,adjncy,anz]= sp2adj(A);
[n,m]=size(A);
p = adj2sp(xadj,adjncy,anz,[n,m]);
A-p,

xdel(winsid())

//====================================================
// ../man/fr/elementary/amell.xml
//====================================================

//====================================================
// ../man/fr/elementary/and.xml
//====================================================

//====================================================
// ../man/fr/elementary/asinhm.xml
//====================================================
clear;lines(0);
A=[1,2;2,3]
sinhm(asinhm(A))
xdel(winsid())

//====================================================
// ../man/fr/elementary/asinh.xml
//====================================================
clear;lines(0);
A=[1,2;2,3]
sinh(asinh(A))
xdel(winsid())

//====================================================
// ../man/fr/elementary/asinm.xml
//====================================================
clear;lines(0);
A=[1,2;3,4]
sinm(asinm(A))
asinm(A)+%i*logm(%i*A+sqrtm(eye()-A*A))
xdel(winsid())

//====================================================
// ../man/fr/elementary/asin.xml
//====================================================
clear;lines(0);
A=[1,2;3,4]
sin(asin(A))
xdel(winsid())

//====================================================
// ../man/fr/elementary/atanhm.xml
//====================================================
clear;lines(0);
A=[1,2;3,4];
tanhm(atanhm(A))
xdel(winsid())

//====================================================
// ../man/fr/elementary/atanh.xml
//====================================================
clear;lines(0);

// example 1
x=[0,%i,-%i]
tanh(atanh(x))

// example 2
x = [-%inf -3 -2 -1 0 1 2 3 %inf]
ieee(2)
atanh(tanh(x))

// example 3 (see Remark)
ieee(2)
atanh([1 2])
atanh([1 0.5])

xdel(winsid())

//====================================================
// ../man/fr/elementary/atanm.xml
//====================================================
clear;lines(0);
tanm(atanm([1,2;3,4]))
xdel(winsid())

//====================================================
// ../man/fr/elementary/atan.xml
//====================================================
clear;lines(0);

// examples with the second form
x=[1,%i,-1,%i]
phasex=atan(imag(x),real(x))
atan(0,-1)
atan(-%eps,-1)

// branch cuts
atan(-%eps + 2*%i)
atan(+%eps + 2*%i)
atan(-%eps - 2*%i)
atan(+%eps - 2*%i)

// values at the branching points
ieee(2)
atan(%i)
atan(-%i)

xdel(winsid())

//====================================================
// ../man/fr/elementary/besseli.xml
//====================================================
clear;lines(0);
// visualisation de quelques fonctions de Bessel du premier type 
x = linspace(0.01,10,5000)';
y = besseli(0:4,x);
ys = besseli(0:4,x,2);
xbasc()
subplot(2,1,1)
   plot2d(x,y, style=2:6, leg="I0@I1@I2@I3@I4", rect=[0,0,6,10])
   xtitle("Quelques fonctions de Bessel du premier type")
subplot(2,1,2)
   plot2d(x,ys, style=2:6, leg="I0s@I1s@I2s@I3s@I4s", rect=[0,0,6,1])
   xtitle("Quelques fonctions de Bessel du premier type normalisées")

xdel(winsid())

//====================================================
// ../man/fr/elementary/besselj.xml
//====================================================
clear;lines(0);
// exemple #1 : Visualisation de quelques fonctions de Bessel
x = linspace(0,40,5000)';
y = besselj(0:4,x);
xbasc()
plot2d(x,y, style=2:6, leg="J0@J1@J2@J3@J4")
xtitle("Quelques fonctions de Bessel du premier type")

// exemple #2 : utilise la relation J_(1/2)(x) = sqrt(2/(x pi)) sin(x)
//              pour comparer besselj(0.5,x) avec une formule plus directe
x = linspace(0.1,40,5000)';
y1 = besselj(0.5, x);
y2 = sqrt(2 ./(%pi*x)).*sin(x);
er = abs((y1-y2)./y2);
ind = find(er > 0 & y2 ~= 0);
xbasc()
subplot(2,1,1)
   plot2d(x,y1,style=2)
   xtitle("besselj(0.5,x)")
subplot(2,1,2)
   plot2d(x(ind), er(ind), style=2, logflag="nl")
   xtitle("Erreur relative entre la formule 2 et besselj(0.5,x)") 
xdel(winsid())

//====================================================
// ../man/fr/elementary/besselk.xml
//====================================================
clear;lines(0);
// exemple : visualisation de quelques fonctions K de Bessel
x = linspace(0.01,10,5000)';
y = besselk(0:4,x);
ys = besselk(0:4,x,2);
xbasc()
subplot(2,1,1)
   plot2d(x,y, style=2:6, leg="K0@K1@K2@K3@K4", rect=[0,0,6,10])
   xtitle("Quelques fonctions de Bessel du second type")
subplot(2,1,2)
   plot2d(x,ys, style=2:6, leg="K0s@K1s@K2s@K3s@K4s", rect=[0,0,6,10])
   xtitle("Quelques fonctions normalisées de Bessel du second type")

xdel(winsid())

//====================================================
// ../man/fr/elementary/bessely.xml
//====================================================
clear;lines(0);
// exemple : Visualisation de quelques fonctions Y de Bessel.
x = linspace(0.1,40,5000)'; // les fonctions Y de  Bessel ne sont pas
                            //bornées pour x -> 0+
y = bessely(0:4,x);
xbasc()
plot2d(x,y, style=2:6, leg="Y0@Y1@Y2@Y3@Y4", rect=[0,-1.5,40,0.6])
xtitle("Quelques fonctions Y de Bessel")
xdel(winsid())

//====================================================
// ../man/fr/elementary/beta.xml
//====================================================
clear;lines(0);
// example 1 :
beta(5,2) - beta(2,5)   // symetry (must be exactly 0)
beta(0.5,0.5)           // exact value is pi

// example 2 : an error study based on the relation  B(1,x) = 1/x
// (computing 1/x must lead to only a relative error of eps_m, so
//  it may be used near as a reference to evaluate the error in B(1,x))  
x = logspace(-8,8,20000)';
e = beta(ones(x),x) - (1)./x;
er = abs(e) .* x;
ind = find(er ~= 0);
eps = ones(x(ind))*number_properties("eps");
xbasc()
plot2d(x(ind),[er(ind) eps 2*eps],style=[1 2 3],logflag="ll",leg="er@eps_m@2 eps_m")
xtitle("approximate relative error in computing beta(1,x)")
xselect()

// example 3 : plotting the beta function 
t = linspace(0.2,10,60);
X = t'*ones(t); Y = ones(t')*t;
Z = beta(X,Y);
xbasc()
plot3d(t, t, Z, flag=[2 4 4], leg="x@y@z", alpha=75, theta=30)
xtitle("The beta function on [0.2,10]x[0.2,10]")
xselect()
xdel(winsid())

//====================================================
// ../man/fr/elementary/binomial.xml
//====================================================
clear;lines(0);

// first example
n=10;p=0.3; xbasc(); plot2d3(0:n,binomial(p,n));

// second example 
n=50;p=0.4;
mea=n*p; sigma=sqrt(n*p*(1-p));
x=( (0:n)-mea )/sigma;
xbasc()
plot2d(x, sigma*binomial(p,n));
deff('y=Gauss(x)','y=1/sqrt(2*%pi)*exp(-(x.^2)/2)')
plot2d(x, Gauss(x), style=2);

// by binomial formula (Caution if big n)
function pr=binomial2(p,n)
x=poly(0,'x');pr=coeff((1-p+x)^n).*horner(x^(0:n),p);
endfunction
p=1/3;n=5;
binomial(p,n)-binomial2(p,n)

// by Gamma function: gamma(n+1)=n! (Caution if big n)
p=1/3;n=5;
Cnks=gamma(n+1)./(gamma(1:n+1).*gamma(n+1:-1:1));
x=poly(0,'x');
pr=Cnks.*horner(x.^(0:n).*(1-x)^(n:-1:0),p);
pr-binomial(p,n)

xdel(winsid())

//====================================================
// ../man/fr/elementary/bloc2exp.xml
//====================================================

//====================================================
// ../man/fr/elementary/bloc2ss.xml
//====================================================

//====================================================
// ../man/fr/elementary/bsplin3val.xml
//====================================================
clear;lines(0);
deff("v=f(x,y,z)","v=cos(x).*sin(y).*cos(z)");
deff("v=fx(x,y,z)","v=-sin(x).*sin(y).*cos(z)");
deff("v=fxy(x,y,z)","v=-sin(x).*cos(y).*cos(z)");
deff("v=fxyz(x,y,z)","v=sin(x).*cos(y).*sin(z)");
deff("v=fxxyz(x,y,z)","v=cos(x).*cos(y).*sin(z)");
n = 20;  // n x n x n  interpolation points
x = linspace(0,2*%pi,n); y=x; z=x; // interpolation grid
[X,Y,Z] = ndgrid(x,y,z); V = f(X,Y,Z);
tl = splin3d(x,y,z,V,[5 5 5]);

// compute f and some derivates on a point
// and compare with the spline interpolant 
xp = grand(1,1,"unf",0,2*%pi); 
yp = grand(1,1,"unf",0,2*%pi); 
zp = grand(1,1,"unf",0,2*%pi); 

f_e = f(xp,yp,zp)
f_i = bsplin3val(xp,yp,zp,tl,[0 0 0])

fx_e = fx(xp,yp,zp)
fx_i = bsplin3val(xp,yp,zp,tl,[1 0 0])

fxy_e = fxy(xp,yp,zp)
fxy_i = bsplin3val(xp,yp,zp,tl,[1 1 0])

fxyz_e = fxyz(xp,yp,zp)
fxyz_i = bsplin3val(xp,yp,zp,tl,[1 1 1])

fxxyz_e = fxxyz(xp,yp,zp)
fxxyz_i = bsplin3val(xp,yp,zp,tl,[2 1 1])

xdel(winsid())

//====================================================
// ../man/fr/elementary/calerf.xml
//====================================================
clear;lines(0);
deff('y=f(t)','y=exp(-t^2)');
calerf(1,0)
2/sqrt(%pi)*intg(0,1,f)
xdel(winsid())

//====================================================
// ../man/fr/elementary/ceil.xml
//====================================================
clear;lines(0);
ceil([1.9 -2.5])-[2,-2]
ceil(-%inf)
x=rand()*10^20;ceil(x)-x
xdel(winsid())

//====================================================
// ../man/fr/elementary/cmb_lin.xml
//====================================================

//====================================================
// ../man/fr/elementary/conj.xml
//====================================================
clear;lines(0);
x=[1+%i,-%i;%i,2*%i];
conj(x)
x'-conj(x)  // x' est la transposée conjuguée de x
xdel(winsid())

//====================================================
// ../man/fr/elementary/coshm.xml
//====================================================
clear;lines(0);
A=[1,2;2,4]
acoshm(coshm(A))
xdel(winsid())

//====================================================
// ../man/fr/elementary/cosh.xml
//====================================================
clear;lines(0);
x=[0,1,%i]
acosh(cosh(x))
xdel(winsid())

//====================================================
// ../man/fr/elementary/cosm.xml
//====================================================
clear;lines(0);
A=[1,2;3,4]
cosm(A)-0.5*(expm(%i*A)+expm(-%i*A))
xdel(winsid())

//====================================================
// ../man/fr/elementary/cos.xml
//====================================================
clear;lines(0);
x=[0,1,%i]
acos(cos(x))
xdel(winsid())

//====================================================
// ../man/fr/elementary/cotg.xml
//====================================================
clear;lines(0);
x=[1,%i];
cotg(x)-cos(x)./sin(x)
xdel(winsid())

//====================================================
// ../man/fr/elementary/cothm.xml
//====================================================
clear;lines(0);
A=[1,2;3,4];
cothm(A)
xdel(winsid())

//====================================================
// ../man/fr/elementary/coth.xml
//====================================================
clear;lines(0);
x=[1,2*%i]
t=exp(x);
(t-ones(x)./t).\(t+ones(x)./t)
coth(x)
xdel(winsid())

//====================================================
// ../man/fr/elementary/cshep2d.xml
//====================================================
clear;lines(0);
// interpolation of cos(x)cos(y) with randomly choosen interpolation points
n = 150; // nb of interpolation points
xy = grand(n,2,"unf",0,2*%pi);
z = cos(xy(:,1)).*cos(xy(:,2));
xyz = [xy z];
tl_coef = cshep2d(xyz);

// evaluation on a grid
m = 30;
xx = linspace(0,2*%pi,m);
[X,Y] = ndgrid(xx,xx);
Z = eval_cshep2d(X,Y, tl_coef);
xbasc()
plot3d(xx,xx,Z,flag=[2 6 4])
param3d1(xy(:,1),xy(:,2),list(z,-9), flag=[0 0])
xtitle("Cubic Shepard Interpolation of cos(x)cos(y) with randomly choosen interpolation points")
legends("interpolation points",-9,1)
xselect()
xdel(winsid())

//====================================================
// ../man/fr/elementary/cumprod.xml
//====================================================
clear;lines(0);
A=[1,2;3,4];
cumprod(A)
cumprod(A,'r')
cumprod(A,'c')
rand('seed',0);
a=rand(3,4);
[m,n]=size(a);
w=zeros(a);
w(1,:)=a(1,:);
for k=2:m;w(k,:)=w(k-1,:).*a(k,:);end;w-cumprod(a,'r')
xdel(winsid())

//====================================================
// ../man/fr/elementary/cumsum.xml
//====================================================
clear;lines(0);
A=[1,2;3,4];
cumsum(A)
cumsum(A,'r')
cumsum(A,'c')
a=rand(3,4)+%i;
[m,n]=size(a);
w=zeros(a);
w(1,:)=a(1,:);
for k=2:m;w(k,:)=w(k-1,:)+a(k,:);end;w-cumsum(a,'r')
xdel(winsid())

//====================================================
// ../man/fr/elementary/delip.xml
//====================================================
clear;lines(0);
ck=0.5;
delip([1,2],ck)
deff('y=f(t)','y=1/sqrt((1-t^2)*(1-ck^2*t^2))')
intg(0,1,f)    // OK puisque la solution est réelle !
xdel(winsid())

//====================================================
// ../man/fr/elementary/diag.xml
//====================================================
clear;lines(0);
diag([1,2])

A=[1,2;3,4];
diag(A)  // diagonale principale
diag(A,1) 

diag(sparse(1:10))  // matrice diagonale creuse

// Construction d'une matrice tridiagonale de taille 2*m+1
m=5;diag(-m:m) +  diag(ones(2*m,1),1) +diag(ones(2*m,1),-1)
xdel(winsid())

//====================================================
// ../man/fr/elementary/diff.xml
//====================================================
clear;lines(0);

v=(1:8)^3;
diff(v)
diff(v,3)

A=[(1:8)^2
   (1:8)^3
   (1:8)^4];

diff(A,3,2)

//approximate differentiation
step=0.001
t=0:step:10;
y=sin(t);
dy=diff(sin(t))/step; //approximate differentiation of sine function
norm(dy-cos(t(1:$-1)),%inf)

xdel(winsid())

//====================================================
// ../man/fr/elementary/dlgamma.xml
//====================================================
clear;lines(0);
dlgamma(0.5)

xdel(winsid())

//====================================================
// ../man/fr/elementary/double.xml
//====================================================
clear;lines(0);
x=int8([0 12 140])
double(x)
xdel(winsid())

//====================================================
// ../man/fr/elementary/dsearch.xml
//====================================================
clear;lines(0);

// example #1 (elementary stat for U(0,1))
m = 50000 ; n = 10;
X = grand(m,1,"def");
val = linspace(0,1,n+1)';
[ind, occ] = dsearch(X, val);
xbasc() ; plot2d2(val, [occ/m;0])  // no normalisation : y must be near 1/n


// example #2 (elementary stat for B(N,p))
N = 8 ; p = 0.5; m = 50000;
X = grand(m,1,"bin",N,p); val = (0:N)';
[ind, occ] = dsearch(X, val, "d");
Pexp = occ/m; Pexa = binomial(p,N); 
xbasc() ; hm = 1.1*max(max(Pexa),max(Pexp));
plot2d3([val val+0.1], [Pexa' Pexp],[1 2],"111",  ...
        "Pexact@Pexp", [-1 0 N+1 hm],[0 N+2 0 6])
xtitle(  "binomial law B("+string(N)+","+string(p)+") :" ...
        +" exact probability versus experimental ones")


// example #3 (piecewise Hermite polynomial)
x = [0 ; 0.2 ; 0.35 ; 0.5 ; 0.65 ; 0.8 ;  1];
y = [0 ; 0.1 ;-0.1  ; 0   ; 0.4  ;-0.1 ;  0];
d = [1 ; 0   ; 0    ; 1   ; 0    ; 0   ; -1];
X = linspace(0, 1, 200)';
ind = dsearch(X, x);
// define Hermite base functions
deff("y=Ll(t,k,x)","y=(t-x(k+1))./(x(k)-x(k+1))")   // Lagrange left on Ik
deff("y=Lr(t,k,x)","y=(t-x(k))./(x(k+1)-x(k))")     // Lagrange right on Ik
deff("y=Hl(t,k,x)","y=(1-2*(t-x(k))./(x(k)-x(k+1))).*Ll(t,k,x).^2")
deff("y=Hr(t,k,x)","y=(1-2*(t-x(k+1))./(x(k+1)-x(k))).*Lr(t,k,x).^2")
deff("y=Kl(t,k,x)","y=(t-x(k)).*Ll(t,k,x).^2")
deff("y=Kr(t,k,x)","y=(t-x(k+1)).*Lr(t,k,x).^2")
// plot the curve
Y = y(ind).*Hl(X,ind) + y(ind+1).*Hr(X,ind) + d(ind).*Kl(X,ind) + d(ind+1).*Kr(X,ind);
xbasc(); plot2d(X,Y,2) ; plot2d(x,y,-9,"000") 
xtitle("an Hermite piecewise polynomial")
// NOTE : you can verify by adding these ones : 
// YY = interp(X,x,y,d); plot2d(X,YY,3,"000")
  
xdel(winsid())

//====================================================
// ../man/fr/elementary/erfc.xml
//====================================================
clear;lines(0);
erf([0.5,0.2])+erfc([0.5,0.2])
xdel(winsid())

//====================================================
// ../man/fr/elementary/erfcx.xml
//====================================================

//====================================================
// ../man/fr/elementary/erf.xml
//====================================================
clear;lines(0);
deff('y=f(t)','y=exp(-t^2)');
erf(0.5)-2/sqrt(%pi)*intg(0,0.5,f)
xdel(winsid())

//====================================================
// ../man/fr/elementary/eval_cshep2d.xml
//====================================================
clear;lines(0);
// see example section of cshep2d

// this example shows the behavior far from the interpolation points ...
deff("z=f(x,y)","z = 1+ 50*(x.*(1-x).*y.*(1-y)).^2")
x = linspace(0,1,10);
[X,Y] = ndgrid(x,x);
X = X(:); Y = Y(:); Z = f(X,Y);
S = cshep2d([X Y Z]);
// evaluation inside and outside the square [0,1]x[0,1]
m = 40;
xx = linspace(-1.5,0.5,m);
[xp,yp] = ndgrid(xx,xx);
zp = eval_cshep2d(xp,yp,S);
// compute facet (to draw one color for extrapolation region
// and another one for the interpolation region)
[xf,yf,zf] = genfac3d(xx,xx,zp);
color = 2*ones(1,size(zf,2));
// indices corresponding to facet in the interpolation region
ind=find( mean(xf,"r")>0 & mean(xf,"r")<1 & mean(yf,"r")>0 & mean(yf,"r")<1 );
color(ind)=3;
xbasc();
plot3d(xf,yf,list(zf,color), flag=[2 6 4])
legends(["extrapolation region","interpolation region"],[2 3],1)
xselect()
xdel(winsid())

//====================================================
// ../man/fr/elementary/eval.xml
//====================================================
clear;lines(0);
a=1; b=2; Z=['a','sin(b)'] ; eval(Z)  // renvoie la matrice [1,0.909];
xdel(winsid())

//====================================================
// ../man/fr/elementary/eye.xml
//====================================================
clear;lines(0);
eye(2,3)
A=rand(2,3);eye(A)
s=poly(0,'s');A=[s,1;s,s+1];eye(A)
A=[1/s,1;s,2];eye(A);
A=ssrand(2,2,3);eye(A)
[1 2;3 4]+2*eye()
xdel(winsid())

//====================================================
// ../man/fr/elementary/fix.xml
//====================================================

//====================================================
// ../man/fr/elementary/floor.xml
//====================================================
clear;lines(0);
floor([1.9 -2.5])-[1,-3]
floor(-%inf)
x=rand()*10^20;floor(x)-x
xdel(winsid())

//====================================================
// ../man/fr/elementary/frexp.xml
//====================================================
clear;lines(0);
[f,e]=frexp([1,%pi,-3,%eps])
xdel(winsid())

//====================================================
// ../man/fr/elementary/full.xml
//====================================================
clear;lines(0);

sp=sparse([1,2;5,4;3,1],[1,2,3]);
A=full(sp)

xdel(winsid())

//====================================================
// ../man/fr/elementary/gammaln.xml
//====================================================
clear;lines(0);
gammaln(0.5)

xdel(winsid())

//====================================================
// ../man/fr/elementary/gamma.xml
//====================================================
clear;lines(0);

// simple examples
gamma(0.5)
gamma(6)-prod(1:5)

// the graph of the Gamma function on [a,b]
a = -3; b = 5;
x = linspace(a,b,40000)';
y = gamma(x);
xbasc()
c=xget("color")
xset("color",2)
plot2d(x, y, style=0, axesflag=5, rect=[a, -10, b, 10])
xset("color",c)
xtitle("The gamma function on ["+string(a)+","+string(b)+"]")
xselect()

xdel(winsid())

//====================================================
// ../man/fr/elementary/gsort.xml
//====================================================
clear;lines(0);
alr=[1,2,2;
     1,2,1;
     1,1,2;
     1,1,1];
[alr1,k]=gsort(alr,'lr','i')
[alr1,k]=gsort(alr,'lc','i')
xdel(winsid())

//====================================================
// ../man/fr/elementary/imag.xml
//====================================================

//====================================================
// ../man/fr/elementary/imult.xml
//====================================================
clear;lines(0);

z1 = imult(%inf)
z2 = %i * %inf

xdel(winsid())

//====================================================
// ../man/fr/elementary/ind2sub.xml
//====================================================
clear;lines(0);

   ind2sub([2,3,2],1:12)
   [i,j,k]=ind2sub([2,3,2],1:12)
 
xdel(winsid())

//====================================================
// ../man/fr/elementary/int8.xml
//====================================================
clear;lines(0);
int8([1 -120 127 312])
uint8([1 -120 127 312])

x=int32(-200:100:400)
int8(x)
xdel(winsid())

//====================================================
// ../man/fr/elementary/integrate.xml
//====================================================
clear;lines(0);
integrate('sin(x)','x',0,%pi)
integrate(['if x==0 then 1,';
           'else sin(x)/x,end'],'x',0,%pi)
xdel(winsid())

//====================================================
// ../man/fr/elementary/interp2d.xml
//====================================================
clear;lines(0);
// see the examples of splin2d

// this example shows some different extrapolation features
// interpolation of cos(x)cos(y)
n = 7;  // a n x n interpolation grid
x = linspace(0,2*%pi,n); y = x;
z = cos(x')*cos(y);
C = splin2d(x, y, z, "periodic");

// now evaluate on a bigger domain than [0,2pi]x [0,2pi]
m = 80; // discretisation parameter of the evaluation grid
xx = linspace(-0.5*%pi,2.5*%pi,m); yy = xx;
[XX,YY] = ndgrid(xx,yy);
zz1 = interp2d(XX,YY, x, y, C, "C0");
zz2 = interp2d(XX,YY, x, y, C, "by_zero");
zz3 = interp2d(XX,YY, x, y, C, "periodic");
zz4 = interp2d(XX,YY, x, y, C, "natural");
xbasc()
subplot(2,2,1)
  plot3d(xx, yy, zz1, flag=[2 6 4])
  xtitle("extrapolation with the C0 outmode")
subplot(2,2,2)
  plot3d(xx, yy, zz2, flag=[2 6 4])
  xtitle("extrapolation with the by_zero outmode")
subplot(2,2,3)
  plot3d(xx, yy, zz3, flag=[2 6 4])
  xtitle("extrapolation with the periodic outmode")
subplot(2,2,4)
  plot3d(xx, yy, zz4, flag=[2 6 4])
  xtitle("extrapolation with the natural outmode")
xselect()
xdel(winsid())

//====================================================
// ../man/fr/elementary/interp3d.xml
//====================================================
clear;lines(0);
// see the examples of the splin3d help page
xdel(winsid())

//====================================================
// ../man/fr/elementary/interpln.xml
//====================================================
clear;lines(0);
x=[1 10 20 30 40];
y=[1 30 -10 20 40];
plot2d(x',y',[-3],"011"," ",[-10,-40,50,50]);
yi=interpln([x;y],-4:45);
plot2d((-4:45)',yi',[3],"000");
xdel(winsid())

//====================================================
// ../man/fr/elementary/interp.xml
//====================================================
clear;lines(0);
// see the examples of splin and lsq_splin

// an example showing C2 and C1 continuity of spline and subspline
a = -8; b = 8;
x = linspace(a,b,20)';
y = sinc(x);
dk = splin(x,y);  // not_a_knot
df = splin(x,y, "fast");
xx = linspace(a,b,800)';
[yyk, yy1k, yy2k] = interp(xx, x, y, dk); 
[yyf, yy1f, yy2f] = interp(xx, x, y, df); 
xbasc()
subplot(3,1,1)
plot2d(xx, [yyk yyf])
plot2d(x, y, style=-9)
legends(["not_a_knot spline","fast sub-spline","interpolation points"],...
        [1 2 -9], "ur",%f)
xtitle("spline interpolation")
subplot(3,1,2)
plot2d(xx, [yy1k yy1f])
legends(["not_a_knot spline","fast sub-spline"], [1 2], "ur",%f)
xtitle("spline interpolation (derivatives)")
subplot(3,1,3)
plot2d(xx, [yy2k yy2f])
legends(["not_a_knot spline","fast sub-spline"], [1 2], "lr",%f)
xtitle("spline interpolation (second derivatives)")

// here is an example showing the different extrapolation possibilities
x = linspace(0,1,11)';
y = cosh(x-0.5);
d = splin(x,y);
xx = linspace(-0.5,1.5,401)';
yy0 = interp(xx,x,y,d,"C0");
yy1 = interp(xx,x,y,d,"linear");
yy2 = interp(xx,x,y,d,"natural");
yy3 = interp(xx,x,y,d,"periodic");  
xbasc()
plot2d(xx,[yy0 yy1 yy2 yy3],style=2:5,frameflag=2,leg="C0@linear@natural@periodic")
xtitle(" different way to evaluate a spline outside its domain")
xdel(winsid())

//====================================================
// ../man/fr/elementary/intersect.xml
//====================================================
clear;lines(0);
A=round(5*rand(10,1));
B=round(5*rand(7,1));

intersect(A,B)
[N,ka,kb]=intersect(A,B)

intersect('a'+string(A),'a'+string(B))

xdel(winsid())

//====================================================
// ../man/fr/elementary/intsplin.xml
//====================================================
clear;lines(0);
t=0:0.1:%pi
intsplin(t,sin(t))
xdel(winsid())

//====================================================
// ../man/fr/elementary/inttrap.xml
//====================================================
clear;lines(0);
t=0:0.1:%pi
inttrap(t,sin(t))
xdel(winsid())

//====================================================
// ../man/fr/elementary/int.xml
//====================================================

//====================================================
// ../man/fr/elementary/isdef.xml
//====================================================
clear;lines(0);
A=1;
isdef('A')
clear A
isdef('A')
xdel(winsid())

//====================================================
// ../man/fr/elementary/isempty.xml
//====================================================
clear;lines(0);

a=1
isempty(a(2:$))
isempty(find(rand(1:10)==5))

xdel(winsid())

//====================================================
// ../man/fr/elementary/isequal.xml
//====================================================
clear;lines(0);

a=[1 2]
isequal(a,[1 2])
isequal(a,1)
  
xdel(winsid())

//====================================================
// ../man/fr/elementary/isinf.xml
//====================================================
clear;lines(0);
isinf([1 0.01 -%inf %inf])
xdel(winsid())

//====================================================
// ../man/fr/elementary/isnan.xml
//====================================================
clear;lines(0);
isnan([1 0.01 -%nan %inf-%inf])
xdel(winsid())

//====================================================
// ../man/fr/elementary/isreal.xml
//====================================================
clear;lines(0);
isreal([1 2])
isreal(1+0*%i)
isreal(1+0*%i,0)
isreal(1+%s)
isreal(sprand(3,3,0.1))
xdel(winsid())

//====================================================
// ../man/fr/elementary/kron.xml
//====================================================
clear;lines(0);
A=[1,2;3,4];
kron(A,A)
A.*.A
sparse(A).*.sparse(A)
A(1,1)=%i;
kron(A,A)
xdel(winsid())

//====================================================
// ../man/fr/elementary/ldivf.xml
//====================================================
clear;lines(0);
ldivf('1','1')
ldivf('a','0')
ldivf('a','x')
ldivf('2','4')
xdel(winsid())

//====================================================
// ../man/fr/elementary/legendre.xml
//====================================================
clear;lines(0);
// example 1 : plot of the 6 first Legendre polynomials on (-1,1)
l = nearfloat("pred",1);
x = linspace(-l,l,200)';
y = legendre(0:5, 0,  x);
xbasc()
plot2d(x,y', leg="p0@p1@p2@p3@p4@p5@p6")
xtitle("the 6 th first Legendre polynomials")

// example 2 : plot of the associated Legendre functions of degree 5 
l = nearfloat("pred",1);
x = linspace(-l,l,200)';
y = legendre(5, 0:5, x, "norm");
xbasc()
plot2d(x,y', leg="p5,0@p5,1@p5,2@p5,3@p5,4@p5,5")
xtitle("the (normalised) associated Legendre functions of degree 5")

// example 3 : define then plot a spherical harmonic
// 3-1 : define the function Ylm
function [y] = Y(l,m,theta,phi)
   // theta may be a scalar or a row vector
   // phi may be a scalar or a column vector
   if m >= 0 then
      y = (-1)^m/(sqrt(2*%pi))*exp(%i*m*phi)*legendre(l, m, cos(theta), "norm")
   else
      y = 1/(sqrt(2*%pi))*exp(%i*m*phi)*legendre(l, -m, cos(theta), "norm")
   end      
endfunction

// 3.2 : define another useful function
function [x,y,z] = sph2cart(theta,phi,r)
   // theta row vector      1 x nt
   // phi   column vector  np x 1
   // r     scalar or np x nt matrix (r(i,j) the length at phi(i) theta(j)) 
   x = r.*(cos(phi)*sin(theta));
   y = r.*(sin(phi)*sin(theta));
   z = r.*(ones(phi)*cos(theta));
endfunction

// 3-3 plot Y31(theta,phi)
l = 3; m = 1;
theta = linspace(0.1,%pi-0.1,60);
phi = linspace(0,2*%pi,120)';
f = Y(l,m,theta,phi);
[x1,y1,z1] = sph2cart(theta,phi,abs(f));       [xf1,yf1,zf1] = nf3d(x1,y1,z1);
[x2,y2,z2] = sph2cart(theta,phi,abs(real(f))); [xf2,yf2,zf2] = nf3d(x2,y2,z2);
[x3,y3,z3] = sph2cart(theta,phi,abs(imag(f))); [xf3,yf3,zf3] = nf3d(x3,y3,z3);

xbasc()
subplot(1,3,1)
plot3d(xf1,yf1,zf1,flag=[2 4 4]); xtitle("|Y31(theta,phi)|")
subplot(1,3,2)
plot3d(xf2,yf2,zf2,flag=[2 4 4]); xtitle("|Real(Y31(theta,phi))|")
subplot(1,3,3)
plot3d(xf3,yf3,zf3,flag=[2 4 4]); xtitle("|Imag(Y31(theta,phi))|")
xdel(winsid())

//====================================================
// ../man/fr/elementary/lex_sort.xml
//====================================================
clear;lines(0);
M=round(2*rand(20,3));

lex_sort(M)
lex_sort(M,'unique')
[N,k]=lex_sort(M,[1 3],'unique')

xdel(winsid())

//====================================================
// ../man/fr/elementary/linear_interpn.xml
//====================================================
clear;lines(0);
// example 1 : 1d linear interpolation
x = linspace(0,2*%pi,11);
y = sin(x);
xx = linspace(-2*%pi,4*%pi,400)';
yy = linear_interpn(xx, x, y, "periodic");
xbasc()
plot2d(xx,yy,style=2)
plot2d(x,y,style=-9, strf="000")
xtitle("linear interpolation of sin(x) with 11 interpolation points")

// example 2 : bilinear interpolation
n = 8;
x = linspace(0,2*%pi,n); y = x;
z = 2*sin(x')*sin(y);
xx = linspace(0,2*%pi, 40);
[xp,yp] = ndgrid(xx,xx);
zp = linear_interpn(xp,yp, x, y, z);
xbasc()
plot3d(xx, xx, zp, flag=[2 6 4])
[xg,yg] = ndgrid(x,x);
param3d1(xg,yg, list(z,-9*ones(1,n)), flag=[0 0])
xtitle("Bilinear interpolation of 2sin(x)sin(y)")
legends("interpolation points",-9,1)
xselect()

// example 3 : bilinear interpolation and experimentation
//             with all the outmode features
nx = 20; ny = 30;
x = linspace(0,1,nx);
y = linspace(0,2, ny);
[X,Y] = ndgrid(x,y);
z = 0.4*cos(2*%pi*X).*cos(%pi*Y);
nxp = 60 ; nyp = 120;
xp = linspace(-0.5,1.5, nxp);
yp = linspace(-0.5,2.5, nyp);
[XP,YP] = ndgrid(xp,yp);
zp1 = linear_interpn(XP, YP, x, y, z, "natural");
zp2 = linear_interpn(XP, YP, x, y, z, "periodic");
zp3 = linear_interpn(XP, YP, x, y, z, "C0");
zp4 = linear_interpn(XP, YP, x, y, z, "by_zero");
zp5 = linear_interpn(XP, YP, x, y, z, "by_nan");
xbasc()
subplot(2,3,1)
   plot3d(x, y, z, leg="x@y@z", flag = [2 4 4])
   xtitle("initial function 0.4 cos(2 pi x) cos(pi y)")
subplot(2,3,2)
   plot3d(xp, yp, zp1, leg="x@y@z", flag = [2 4 4])
   xtitle("Natural")
subplot(2,3,3)
   plot3d(xp, yp, zp2, leg="x@y@z", flag = [2 4 4])
   xtitle("Periodic")
subplot(2,3,4)
   plot3d(xp, yp, zp3, leg="x@y@z", flag = [2 4 4])
   xtitle("C0")
subplot(2,3,5)
   plot3d(xp, yp, zp4, leg="x@y@z", flag = [2 4 4])
   xtitle("by_zero")
subplot(2,3,6)
   plot3d(xp, yp, zp5, leg="x@y@z", flag = [2 4 4])
   xtitle("by_nan")
xselect()


// example 4 : trilinear interpolation (see splin3d help
//             page which have the same example with
//             tricubic spline interpolation)
getf("SCI/demos/interp/interp_demo.sci") 
func =  "v=(x-0.5).^2 + (y-0.5).^3 + (z-0.5).^2";
deff("v=f(x,y,z)",func);
n = 5; 
x = linspace(0,1,n); y=x; z=x;
[X,Y,Z] = ndgrid(x,y,z);
V = f(X,Y,Z);
// compute (and display) the linear interpolant on some slices
m = 41;
dir = ["z="  "z="  "z="  "x="  "y="];
val = [ 0.1   0.5   0.9   0.5   0.5];
ebox = [0 1 0 1 0 1];

XF=[]; YF=[]; ZF=[]; VF=[];
for i = 1:length(val)
   [Xm,Xp,Ym,Yp,Zm,Zp] = slice_parallelepiped(dir(i), val(i), ebox, m, m, m);
   Vm = linear_interpn(Xm,Ym,Zm, x, y, z, V);
   [xf,yf,zf,vf] = nf3dq(Xm,Ym,Zm,Vm,1);
   XF = [XF xf]; YF = [YF yf]; ZF = [ZF zf]; VF = [VF vf]; 
   Vp =  linear_interpn(Xp,Yp,Zp, x, y, z, V);
   [xf,yf,zf,vf] = nf3dq(Xp,Yp,Zp,Vp,1);
   XF = [XF xf]; YF = [YF yf]; ZF = [ZF zf]; VF = [VF vf]; 
end
nb_col = 128;
vmin = min(VF); vmax = max(VF);
color = dsearch(VF,linspace(vmin,vmax,nb_col+1));
xset("colormap",jetcolormap(nb_col));
xbasc()
xset("hidden3d",xget("background"))
colorbar(vmin,vmax)
plot3d(XF, YF, list(ZF,color), flag=[-1 6 4])
xtitle("tri-linear interpolation of "+func)
xselect()
xdel(winsid())

//====================================================
// ../man/fr/elementary/linspace.xml
//====================================================
clear;lines(0);
linspace(1,2,10)
xdel(winsid())

//====================================================
// ../man/fr/elementary/log10.xml
//====================================================
clear;lines(0);
10.^(log10([1,%i,-1,-%i]))
xdel(winsid())

//====================================================
// ../man/fr/elementary/log1p.xml
//====================================================
clear;lines(0);

format("e",24)
log(1.001)
log1p(0.001)
log(1 + 1.e-7)
log1p(1.e-7)
log(1 + 1.e-20)
log1p(1.e-20)
format("v") //reset default format
  
xdel(winsid())

//====================================================
// ../man/fr/elementary/log2.xml
//====================================================
clear;lines(0);
2.^(log2([1,%i,-1,-%i]))
xdel(winsid())

//====================================================
// ../man/fr/elementary/logm.xml
//====================================================
clear;lines(0);
A=[1,2;3,4];
logm(A)
expm(logm(A))
A1=A*A';
logm(A1)
expm(logm(A1))
A1(1,1)=%i;
expm(logm(A1))
xdel(winsid())

//====================================================
// ../man/fr/elementary/logspace.xml
//====================================================
clear;lines(0);
logspace(1,2,10)
xdel(winsid())

//====================================================
// ../man/fr/elementary/log.xml
//====================================================
clear;lines(0);
exp(log([1,%i,-1,-%i]))
xdel(winsid())

//====================================================
// ../man/fr/elementary/lsq_splin.xml
//====================================================
clear;lines(0);
// this is an artifical example where the datas xd and yd
// are build from a perturbed sin function
a = 0; b = 2*%pi;
sigma = 0.1;  // standard deviation of the gaussian noise
m = 200;       // number of experimental points
xd = linspace(a,b,m)';
yd = sin(xd) + grand(xd,"nor",0,sigma);

n = 6; // number of breakpoints
x = linspace(a,b,n)';

// compute the spline
[y, d] = lsq_splin(xd, yd, x);  // use equal weights

// plotting
ye = sin(xd);
ys = interp(xd, x, y, d);
xbasc()
plot2d(xd,[ye yd ys],style=[2 -2 3], ...
       leg="exact function@experimental measures (gaussian perturbation)@fitted spline")
xtitle("a least square spline")
xselect()
xdel(winsid())

//====================================================
// ../man/fr/elementary/lstsize.xml
//====================================================
clear;lines(0);
lstsize(list(1,'aqsdf'))
x=ssrand(3,2,4);
[ny,nu]=size(x)
lstsize(x)
xdel(winsid())

//====================================================
// ../man/fr/elementary/maxi.xml
//====================================================
clear;lines(0);
[m,n]=maxi([1,3,1])
[m,n]=maxi([3,1,1],[1,3,1],[1,1,3])
[m,n]=maxi([3,-2,1],1)
[m,n]=maxi(list([3,1,1],[1,3,1],[1,1,3]))
[m,n]=maxi(list(1,3,1))
xdel(winsid())

//====================================================
// ../man/fr/elementary/max.xml
//====================================================
clear;lines(0);
[m,n]=max([1,3,1])
[m,n]=max([3,1,1],[1,3,1],[1,1,3])
[m,n]=max([3,-2,1],1)
[m,n]=max(list([3,1,1],[1,3,1],[1,1,3]))
[m,n]=max(list(1,3,1))
xdel(winsid())

//====================================================
// ../man/fr/elementary/mini.xml
//====================================================
clear;lines(0);
[m,n]=mini([1,3,1])
[m,n]=mini([3,1,1],[1,3,1],[1,1,3])
[m,n]=mini(list([3,1,1],[1,3,1],[1,1,3]))
[m,n]=mini(list(1,3,1))
xdel(winsid())

//====================================================
// ../man/fr/elementary/minus.xml
//====================================================
clear;lines(0);
[1,2]-1
[]-2

%s-2
1/%s-2
"cat"+"enate"
xdel(winsid())

//====================================================
// ../man/fr/elementary/min.xml
//====================================================
clear;lines(0);
[m,n]=min([1,3,1])
[m,n]=min([3,1,1],[1,3,1],[1,1,3])
[m,n]=min(list([3,1,1],[1,3,1],[1,1,3]))
[m,n]=min(list(1,3,1))
xdel(winsid())

//====================================================
// ../man/fr/elementary/modulo.xml
//====================================================
clear;lines(0);
n=[1,2,10,15];m=[2,2,3,5];
modulo(n,m)

modulo(-3,9)
pmodulo(-3,9)
xdel(winsid())

//====================================================
// ../man/fr/elementary/mps2linpro.xml
//====================================================

//====================================================
// ../man/fr/elementary/mtlb_sparse.xml
//====================================================
clear;lines(0);

X=sparse(rand(2,2)); Y=mtlb_sparse(X);
Y, full(Y), [ij,v,mn]=spget(Y)

xdel(winsid())

//====================================================
// ../man/fr/elementary/mulf.xml
//====================================================
clear;lines(0);
mulf('1','a')
mulf('0','a')
'a'+'b'   // attention ...
xdel(winsid())

//====================================================
// ../man/fr/elementary/ndgrid.xml
//====================================================
clear;lines(0);
// create a simple 2d grid
nx = 40; ny = 40;
x = linspace(-1,1,nx);
y = linspace(-1,1,ny);
[X,Y] = ndgrid(x,y);
// compute a function on the grid and plot it
//deff("z=f(x,y)","z=128*x.^2 .*(1-x).^2 .*y.^2 .*(1-y).^2");
deff("z=f(x,y)","z=x.^2 + y.^3")
Z = f(X,Y);
xbasc()
plot3d(x,y,Z, flag=[2 6 4]); xselect()

// create a simple 3d grid
nx = 10; ny = 6; nz = 4;
x = linspace(0,2,nx);
y = linspace(0,1,ny);
z = linspace(0,0.5,nz);
[X,Y,Z] = ndgrid(x,y,z);
// try to display this 3d grid ...
XF=[]; YF=[]; ZF=[];
for k=1:nz
   [xf,yf,zf] = nf3d(X(:,:,k),Y(:,:,k),Z(:,:,k));
   XF = [XF xf]; YF = [YF yf]; ZF = [ZF zf];
end
for j=1:ny
   [xf,yf,zf] = nf3d(matrix(X(:,j,:),[nx,nz]),...
                     matrix(Y(:,j,:),[nx,nz]),...
                     matrix(Z(:,j,:),[nx,nz]));
   XF = [XF xf]; YF = [YF yf]; ZF = [ZF zf];
end
xbasc()
plot3d(XF,YF,ZF, flag=[0 6 3], leg="X@Y@Z")
xtitle("A 3d grid !"); xselect()
xdel(winsid())

//====================================================
// ../man/fr/elementary/ndims.xml
//====================================================
clear;lines(0);

A=rand(2,3);
ndims(A)

A=rand(2,3,2);
size(A),ndims(A)

H=[1/%s,1/(%s+1)]
ndims(H)

xdel(winsid())

//====================================================
// ../man/fr/elementary/nearfloat.xml
//====================================================
clear;lines(0);

format("e",22)
nearfloat("succ",1) - 1
1 - nearfloat("pred",1)
format("v") //reset default format
  
xdel(winsid())

//====================================================
// ../man/fr/elementary/nextpow2.xml
//====================================================
clear;lines(0);

nextpow2(127)
nextpow2(128)
nextpow2(0:10)

xdel(winsid())

//====================================================
// ../man/fr/elementary/nnz.xml
//====================================================
clear;lines(0);
sp=sparse([1,2;4,5;3,10],[1,2,3]);
nnz(sp)
a=[1 0 0 0 2];
nnz(a)
xdel(winsid())

//====================================================
// ../man/fr/elementary/norm.xml
//====================================================
clear;lines(0);
A=[1,2,3];
norm(A,1)
norm(A,'inf')
A=[1,2;3,4]
max(svd(A))-norm(A)

A=sparse([1 0 0 33 -1])
norm(A)

xdel(winsid())

//====================================================
// ../man/fr/elementary/not.xml
//====================================================
clear;lines(0);
~[%t %t %f]
xdel(winsid())

//====================================================
// ../man/fr/elementary/number_properties.xml
//====================================================
clear;lines(0);

b = number_properties("radix")
eps = number_properties("eps")
  
xdel(winsid())

//====================================================
// ../man/fr/elementary/ones.xml
//====================================================
clear;lines(0);
ones(3)
ones(3,3)
ones(2,3,2)
xdel(winsid())

//====================================================
// ../man/fr/elementary/or.xml
//====================================================
clear;lines(0);
or([%t %t %f])
[%t %t %f]|[%f %t %t]
[%t %t %f]|%f
xdel(winsid())

//====================================================
// ../man/fr/elementary/pen2ea.xml
//====================================================
clear;lines(0);
E=[1,0];A=[1,2];s=poly(0,'s');
[E,A]=pen2ea(s*E-A)
xdel(winsid())

//====================================================
// ../man/fr/elementary/pertrans.xml
//====================================================
clear;lines(0);

A=[1,2;3,4]
pertrans(A)

xdel(winsid())

//====================================================
// ../man/fr/elementary/prod.xml
//====================================================
clear;lines(0);
A=[1,2;0,100];
prod(A)
prod(A,'c')
prod(A,'r')
xdel(winsid())

//====================================================
// ../man/fr/elementary/rand.xml
//====================================================
clear;lines(0);
x=rand(10,10,'uniform')
rand('normal')
rand('info')
y=rand(x,'normal');
x=rand(2,2,2)
xdel(winsid())

//====================================================
// ../man/fr/elementary/rat.xml
//====================================================
clear;lines(0);
[n,d]=rat(%pi)
[n,d]=rat(%pi,1.d-12)
n/d-%pi
xdel(winsid())

//====================================================
// ../man/fr/elementary/rdivf.xml
//====================================================
clear;lines(0);
ldivf('c','d')
ldivf('1','2')
ldivf('a','0')
xdel(winsid())

//====================================================
// ../man/fr/elementary/real.xml
//====================================================

//====================================================
// ../man/fr/elementary/round.xml
//====================================================
clear;lines(0);
round([1.9 -2.5])-[2,-3]
round(1.6+2.1*%i)-(2+2*%i)
round(-%inf)
x=rand()*10^20;round(x)-x
xdel(winsid())

//====================================================
// ../man/fr/elementary/setdiff.xml
//====================================================
clear;lines(0);

 a = [223;111;2;4;2;2];
 b = [2;3;21;223;123;22];
 setdiff(a,b)
 [v,k]=setdiff(string(a),string(b))

xdel(winsid())

//====================================================
// ../man/fr/elementary/signm.xml
//====================================================
clear;lines(0);
A=rand(4,4);B=A+A';X=signm(B);spec(B),spec(X)

xdel(winsid())

//====================================================
// ../man/fr/elementary/sign.xml
//====================================================
clear;lines(0);
sign(rand(2,3))
sign(1+%i)
xdel(winsid())

//====================================================
// ../man/fr/elementary/sinc.xml
//====================================================
clear;lines(0);

x=linspace(-10,10,3000);
plot2d(x,sinc(x))

xdel(winsid())

//====================================================
// ../man/fr/elementary/sinhm.xml
//====================================================
clear;lines(0);
A=[1,2;2,4]
asinhm(sinhm(A))
A(1,1)=%i;sinhm(A)-(expm(A)-expm(-A))/2   // cas complexe
xdel(winsid())

//====================================================
// ../man/fr/elementary/sinh.xml
//====================================================
clear;lines(0);
asinh(sinh([0,1,%i]))
xdel(winsid())

//====================================================
// ../man/fr/elementary/sinm.xml
//====================================================
clear;lines(0);
A=[1,2;2,4];
sinm(A)+0.5*%i*(expm(%i*A)-expm(-%i*A))
xdel(winsid())

//====================================================
// ../man/fr/elementary/sin.xml
//====================================================
clear;lines(0);
asin(sin([1,0,%i]))
xdel(winsid())

//====================================================
// ../man/fr/elementary/size.xml
//====================================================
clear;lines(0);
[n,m]=size(rand(3,2))
[n,m]=size(['a','b';'c','d'])
x=ssrand(3,2,4);[ny,nu]=size(x)
[ny,nu]=size(ss2tf(x))
[ny,nu,nx]=size(x)
xdel(winsid())

//====================================================
// ../man/fr/elementary/smooth.xml
//====================================================
clear;lines(0);
x=[1 10 20 30 40];
y=[1 30 -10 20 40];
plot2d(x',y',[3],"011"," ",[-10,-40,50,50]);
yi=smooth([x;y],0.1);
plot2d(yi(1,:)',yi(2,:)',[1],"000");
xdel(winsid())

//====================================================
// ../man/fr/elementary/solve.xml
//====================================================
clear;lines(0);
A=['1','a';'0','2'];   //  matrice triangulaire supérieure 
b=['x';'y'];
w=solve(A,b)
a=1;x=2;y=5;
evstr(w)
inv([1,1;0,2])*[2;5]
xdel(winsid())

//====================================================
// ../man/fr/elementary/sort.xml
//====================================================
clear;lines(0);
[s,p]=sort(rand(1,10));
// p est une permutation aléatoire de 1:10
A=[1,2,5;3,4,2];
[A_trie,q]=sort(A);A(q(:))-A_trie(:)
v=1:10;
sort(v)
sort(v')
sort(v,'r')  // ne fait rien pour les vecteurs lignes
sort(v,'c')
xdel(winsid())

//====================================================
// ../man/fr/elementary/sp2adj.xml
//====================================================
clear;lines(0);

A = sprand(100,50,.05);
[xadj,adjncy,anz]= sp2adj(A);
[n,m]=size(A);
p = adj2sp(xadj,adjncy,anz,[n,m]);
A-p,

xdel(winsid())

//====================================================
// ../man/fr/elementary/sparse.xml
//====================================================
clear;lines(0);

sp=sparse([1,2;4,5;3,10],[1,2,3])
size(sp)
x=rand(2,2);abs(x)-full(abs(sparse(x)))

xdel(winsid())

//====================================================
// ../man/fr/elementary/spcompack.xml
//====================================================
clear;lines(0);

// A is the sparse matrix:
A=[1,0,0,0,0,0,0;
   0,1,0,0,0,0,0;
   0,0,1,0,0,0,0;
   0,0,1,1,0,0,0;
   0,0,1,1,1,0,0;
   0,0,1,1,0,1,0;
   0,0,1,1,0,1,1];
A=sparse(A);
//For this matrix, the standard adjacency representation is given by:
xadj=[1,2,3,8,12,13,15,16];
adjncy=[1, 2, 3,4,5,6,7, 4,5,6,7, 5, 6,7, 7];
//(see sp2adj).
// increments in vector xadj give the number of non zero entries in each column
// ie there is 2-1=1 entry in the column 1
//    there is 3-2=1 entry in the column 2
//    there are 8-3=5 entries in the column 3
//              12-8=4                      4
//etc
//The row index of these entries is given by the adjncy vector
// for instance, 
// adjncy (3:7)=adjncy(xadj(3):xadj(4)-1)=[3,4,5,6,7] 
// says that the 5=xadj(4)-xadj(3) entries in column 3 have row
// indices 3,4,5,6,7.
//In the compact representation, the repeated sequences in adjncy
//are eliminated.
//Here in adjncy the sequences 4,5,6,7  and 7 are eliminated.
//The standard structure (xadj,adjncy) takes the compressed form (lindx,xlindx)
lindx=[1, 2, 3,4,5,6,7, 5, 6,7];
xlindx=[1,2,3,8,9,11];
//(Columns 4 and 7 of A are eliminated).
//A can be reconstructed from (xadj,xlindx,lindx).
[xadj,adjncy,anz]= sp2adj(A);
adjncy -  spcompack(xadj,xlindx,lindx)

xdel(winsid())

//====================================================
// ../man/fr/elementary/speye.xml
//====================================================
clear;lines(0);
eye(3,3)-full(speye(3,3))
xdel(winsid())

//====================================================
// ../man/fr/elementary/spget.xml
//====================================================
clear;lines(0);

sp=sparse([1,2;4,5;3,10],[1,2,3])
[ij,v,mn]=spget(sp);

xdel(winsid())

//====================================================
// ../man/fr/elementary/splin2d.xml
//====================================================
clear;lines(0);
// example 1 : interpolation of cos(x)cos(y)
n = 7;  // a regular grid with n x n interpolation points
        // will be used
x = linspace(0,2*%pi,n); y = x;
z = cos(x')*cos(y);
C = splin2d(x, y, z, "periodic");
m = 50; // discretisation parameter of the evaluation grid
xx = linspace(0,2*%pi,m); yy = xx;
[XX,YY] = ndgrid(xx,yy);
zz = interp2d(XX,YY, x, y, C);
emax = max(abs(zz - cos(xx')*cos(yy)));
xbasc()
plot3d(xx, yy, zz, flag=[2 4 4])
[X,Y] = ndgrid(x,y);
param3d1(X,Y,list(z,-9*ones(1,n)), flag=[0 0])
str = msprintf(" with %d x %d interpolation points. ermax = %g",n,n,emax) 
xtitle("spline interpolation of cos(x)cos(y)"+str)

// example 2 : different interpolation functions on random datas
n = 6;
x = linspace(0,1,n); y = x;
z = rand(n,n);
np = 50;
xp = linspace(0,1,np); yp = xp;
[XP, YP] = ndgrid(xp,yp);
ZP1 = interp2d(XP, YP, x, y, splin2d(x, y, z, "not_a_knot"));
ZP2 = linear_interpn(XP, YP, x, y, z);
ZP3 = interp2d(XP, YP, x, y, splin2d(x, y, z, "natural"));
ZP4 = interp2d(XP, YP, x, y, splin2d(x, y, z, "monotone"));
xset("colormap", jetcolormap(64))
xbasc()
subplot(2,2,1)
   plot3d1(xp, yp, ZP1, flag=[2 2 4])
   xtitle("not_a_knot")
subplot(2,2,2)
   plot3d1(xp, yp, ZP2, flag=[2 2 4])
   xtitle("bilinear interpolation")
subplot(2,2,3)
   plot3d1(xp, yp, ZP3, flag=[2 2 4])
   xtitle("natural")
subplot(2,2,4)
   plot3d1(xp, yp, ZP4, flag=[2 2 4])
   xtitle("monotone")
xselect()

// example 3 : not_a_knot spline and monotone sub-spline
//             on a step function
a = 0; b = 1; c = 0.25; d = 0.75;
// create interpolation grid
n = 11;
x = linspace(a,b,n);
ind = find(c <= x & x <= d); 
z = zeros(n,n); z(ind,ind) = 1;  // a step inside a square
// create evaluation grid
np = 220;
xp = linspace(a,b, np);
[XP, YP] = ndgrid(xp, xp);
zp1 = interp2d(XP, YP, x, x, splin2d(x,x,z));
zp2 = interp2d(XP, YP, x, x, splin2d(x,x,z,"monotone"));
// plot
xbasc()
xset("colormap",jetcolormap(128))
subplot(1,2,1)
   plot3d1(xp, xp, zp1, flag=[-2 6 4])
   xtitle("spline (not_a_knot)")
subplot(1,2,2)
   plot3d1(xp, xp, zp2, flag=[-2 6 4])
   xtitle("subspline (monotone)")
xdel(winsid())

//====================================================
// ../man/fr/elementary/splin3d.xml
//====================================================
clear;lines(0);
// example 1
func =  "v=cos(2*%pi*x).*sin(2*%pi*y).*cos(2*%pi*z)";
deff("v=f(x,y,z)",func);
n = 10;  // n x n x n  interpolation points
x = linspace(0,1,n); y=x; z=x; // interpolation grid
[X,Y,Z] = ndgrid(x,y,z);
V = f(X,Y,Z);
tl = splin3d(x,y,z,V,[5 5 5]);
m = 10000;
// compute an approximated error
xp = grand(m,1,"def"); yp = grand(m,1,"def"); zp = grand(m,1,"def");
vp_exact = f(xp,yp,zp);
vp_interp = interp3d(xp,yp,zp, tl);
er = max(abs(vp_exact - vp_interp))
// now retry with n=20 and see the error


// example 2 (see linear_interpn help page which have the
//            same example with trilinear interpolation)
getf("SCI/demos/interp/interp_demo.sci") 
func =  "v=(x-0.5).^2 + (y-0.5).^3 + (z-0.5).^2";
deff("v=f(x,y,z)",func);
n = 5; 
x = linspace(0,1,n); y=x; z=x;
[X,Y,Z] = ndgrid(x,y,z);
V = f(X,Y,Z);
tl = splin3d(x,y,z,V);
// compute (and display) the 3d spline interpolant on some slices
m = 41;
dir = ["z="  "z="  "z="  "x="  "y="];
val = [ 0.1   0.5   0.9   0.5   0.5];
ebox = [0 1 0 1 0 1];
XF=[]; YF=[]; ZF=[]; VF=[];
for i = 1:length(val)
   [Xm,Xp,Ym,Yp,Zm,Zp] = slice_parallelepiped(dir(i), val(i), ebox, m, m, m);
   Vm = interp3d(Xm,Ym,Zm, tl);
   [xf,yf,zf,vf] = nf3dq(Xm,Ym,Zm,Vm,1);
   XF = [XF xf]; YF = [YF yf]; ZF = [ZF zf]; VF = [VF vf]; 
   Vp = interp3d(Xp,Yp,Zp, tl);
   [xf,yf,zf,vf] = nf3dq(Xp,Yp,Zp,Vp,1);
   XF = [XF xf]; YF = [YF yf]; ZF = [ZF zf]; VF = [VF vf]; 
end
nb_col = 128;
vmin = min(VF); vmax = max(VF);
color = dsearch(VF,linspace(vmin,vmax,nb_col+1));
xset("colormap",jetcolormap(nb_col));
xbasc(); xset("hidden3d",xget("background"));
colorbar(vmin,vmax)
plot3d(XF, YF, list(ZF,color), flag=[-1 6 4])
xtitle("3d spline interpolation of "+func)
xselect()
xdel(winsid())

//====================================================
// ../man/fr/elementary/splin.xml
//====================================================
clear;lines(0);
// example 1
deff("y=runge(x)","y=1 ./(1 + x.^2)")
a = -5; b = 5; n = 11; m = 400;
x = linspace(a, b, n)';
y = runge(x);
d = splin(x, y);
xx = linspace(a, b, m)';
yyi = interp(xx, x, y, d);
yye = runge(xx);
xbasc()
plot2d(xx, [yyi yye], style=[2 5], leg="interpolation spline@exact function")
plot2d(x, y, -9)
xtitle("interpolation of the Runge function")

// example 2 : show behavior of different splines on random datas
a = 0; b = 1;        // interval of interpolation
n = 10;              // nb of interpolation  points
m = 800;             // discretisation for evaluation
x = linspace(a,b,n)'; // abscissae of interpolation points
y = rand(x);          // ordinates of interpolation points
xx = linspace(a,b,m)';
yk = interp(xx, x, y, splin(x,y,"not_a_knot"));
yf = interp(xx, x, y, splin(x,y,"fast"));
ym = interp(xx, x, y, splin(x,y,"monotone"));
xbasc()
plot2d(xx, [yf ym yk], style=[5 2 3], strf="121", ...
       leg="fast@monotone@not a knot spline")
plot2d(x,y,-9, strf="000")  // to show interpolation points
xtitle("Various spline and sub-splines on random datas")
xselect()
xdel(winsid())

//====================================================
// ../man/fr/elementary/spones.xml
//====================================================
clear;lines(0);
A=sprand(10,12,0.1);
sp=spones(A)
B = A~=0
bool2s(B)
xdel(winsid())

//====================================================
// ../man/fr/elementary/sprand.xml
//====================================================
clear;lines(0);
W=sprand(100,1000,0.001);
xdel(winsid())

//====================================================
// ../man/fr/elementary/spzeros.xml
//====================================================
clear;lines(0);
sum(spzeros(1000,1000))
xdel(winsid())

//====================================================
// ../man/fr/elementary/sqrtm.xml
//====================================================
clear;lines(0);
x=[0 1;2 4]
w=sqrtm(x); 
norm(w*w-x)
x(1,2)=%i;
w=sqrtm(x);norm(w*w-x,1)
xdel(winsid())

//====================================================
// ../man/fr/elementary/sqrt.xml
//====================================================
clear;lines(0);
sqrt([2,4])
sqrt(-1)
xdel(winsid())

//====================================================
// ../man/fr/elementary/squarewave.xml
//====================================================
clear;lines(0);
t=(0:0.1:5*%pi)';
plot2d1('onn',t,[2*sin(t),1.5*squarewave(t),squarewave(t,10)])
xdel(winsid())

//====================================================
// ../man/fr/elementary/ssprint.xml
//====================================================
clear;lines(0);

 a=[1 1;0 1];b=[0 1;1 0];c=[1,1];d=[3,2];
 ssprint(syslin('c',a,b,c,d))
 ssprint(syslin('d',a,b,c,d))

xdel(winsid())

//====================================================
// ../man/fr/elementary/ssrand.xml
//====================================================
clear;lines(0);

//flag=list('st',dim_cont_subs,dim_stab_subs,dim_stab0)  
//dim_cont_subs<=dim_stab_subs<=dim_stab0  
//pair (A,B) U-similar to:
//    [*,*,*,*;     [*;    
//    [0,s,*,*;     [0;
//A=  [0,0,i,*;   B=[0;
//    [0,0,0,u]     [0]
//     
// (A11,B1) controllable  s=stable matrix i=neutral matrix u=unstable matrix
[Sl,U]=ssrand(2,3,8,list('st',2,5,5));
w=ss2ss(Sl,inv(U)); //undo the random change of basis => form as above
[n,nc,u,sl]=st_ility(Sl);n,nc

xdel(winsid())

//====================================================
// ../man/fr/elementary/sub2ind.xml
//====================================================
clear;lines(0);

i=[1 2 1 1 2 1 1];
j=[1 2 3 1 2 3 3];
k=[1 2 1 2 1 2 1];
sub2ind([2,3,2],i,j,k)

sub2ind([2,3,2],[i',j',k'])
xdel(winsid())

//====================================================
// ../man/fr/elementary/subf.xml
//====================================================
clear;lines(0);
subf('0','a')
subf('2','1')
subf('a','0')
xdel(winsid())

//====================================================
// ../man/fr/elementary/sum.xml
//====================================================
clear;lines(0);

A=[1,2;3,4];
trace(A)-sum(diag(A))
sum(A,'c')-A*ones(2,1)
sum(A+%i)
A=sparse(A);sum(A,'c')-A*ones(2,1)
s=poly(0,'s');
M=[s,%i+s;s^2,1];
sum(M),sum(M,2)

xdel(winsid())

//====================================================
// ../man/fr/elementary/sysconv.xml
//====================================================
clear;lines(0);

s1=ssrand(1,1,2);
s2=ss2tf(s1);
[s1,s2]=sysconv(s1,s2);

xdel(winsid())

//====================================================
// ../man/fr/elementary/sysdiag.xml
//====================================================
clear;lines(0);

 s=poly(0,'s')
 sysdiag(rand(2,2),1/(s+1),[1/(s-1);1/((s-2)*(s-3))])
 sysdiag(tf2ss(1/s),1/(s+1),[1/(s-1);1/((s-2)*(s-3))])

 s=poly(0,'s')
 sysdiag(rand(2,2),1/(s+1),[1/(s-1);1/((s-2)*(s-3))])
 sysdiag(tf2ss(1/s),1/(s+1),[1/(s-1);1/((s-2)*(s-3))])

xdel(winsid())

//====================================================
// ../man/fr/elementary/syslin.xml
//====================================================
clear;lines(0);
A=[0,1;0,0];B=[1;1];C=[1,1];
S1=syslin('c',A,B,C)   // Définition du système
S1.A    // La matrice A
S1.X0, S1.dt // affichage de X0 et du domaine temporel
s=poly(0,'s');
D=s;
S2=syslin('c',A,B,C,D)
H1=(1+2*s)/s^2, S1bis=syslin('c',H1)
H2=(1+2*s+s^3)/s^2, S2bis=syslin('c',H2)
S1+S2
[S1,S2]
ss2tf(S1)-S1bis
S1bis+S2bis
S1*S2bis
size(S1)
xdel(winsid())

//====================================================
// ../man/fr/elementary/tanhm.xml
//====================================================
clear;lines(0);
A=[1,2;3,4];
tanhm(A)
xdel(winsid())

//====================================================
// ../man/fr/elementary/tanh.xml
//====================================================
clear;lines(0);
x=[1,%i,-1,-%i]
tanh(x)
sinh(x)./cosh(x)
xdel(winsid())

//====================================================
// ../man/fr/elementary/tanm.xml
//====================================================
clear;lines(0);
A=[1,2;3,4];
tanm(A)
xdel(winsid())

//====================================================
// ../man/fr/elementary/tan.xml
//====================================================
clear;lines(0);
x=[1,%i,-1,-%i]
tan(x)
sin(x)./cos(x)
xdel(winsid())

//====================================================
// ../man/fr/elementary/toeplitz.xml
//====================================================
clear;lines(0);
A=toeplitz(1:5);
//
T=toeplitz(1:5,1:2:7);T1=[1 3 5 7;2 1 3 5;3 2 1 3;4 3 2 1;5 4 3 2];
T-T1
//
s=poly(0,'s');
t=toeplitz([s,s+1,s^2,1-s]);
t1=[s,1+s,s*s,1-s;1+s,s,1+s,s*s;s*s,1+s,s,1+s;1-s,s*s,1+s,s]
t-t1
//
t=toeplitz(['1','2','3','4']);
t1=['1','2','3','4';'2','1','2','3';'3','2','1','2';'4','3','2','1']
xdel(winsid())

//====================================================
// ../man/fr/elementary/trfmod.xml
//====================================================

//====================================================
// ../man/fr/elementary/trianfml.xml
//====================================================
clear;lines(0);
A=['1','2';'a','b']
W=trianfml([A,string(eye(2,2))])
U=W(:,3:4)
a=5;b=6;
A=evstr(A)
U=evstr(U)
U*A
evstr(W(:,1:2))
xdel(winsid())

//====================================================
// ../man/fr/elementary/tril.xml
//====================================================
clear;lines(0);
s=poly(0,'s');
tril([s,s;s,1])
tril([1/s,1/s;1/s,1])
xdel(winsid())

//====================================================
// ../man/fr/elementary/trisolve.xml
//====================================================
clear;lines(0);
A=['x','y';'0','z'];b=['0';'1'];
w=trisolve(A,b)
x=5;y=2;z=4;
evstr(w)
inv(evstr(A))*evstr(b)
xdel(winsid())

//====================================================
// ../man/fr/elementary/triu.xml
//====================================================
clear;lines(0);
s=poly(0,'s');
triu([s,s;s,1])
triu([1/s,1/s;1/s,1])
xdel(winsid())

//====================================================
// ../man/fr/elementary/typeof.xml
//====================================================
clear;lines(0);

typeof(1)
typeof(poly(0,'x'))

typeof(1/poly(0,'x'))
typeof(%t)

w=sprand(100,100,0.001);
typeof(w)
typeof(w==w)

deff('y=f(x)','y=2*x');
typeof(f)

L=tlist(['V','a','b'],18,'Scilab');
typeof(L)

xdel(winsid())

//====================================================
// ../man/fr/elementary/union.xml
//====================================================
clear;lines(0);
A=round(5*rand(10,1));
B=round(5*rand(7,1));

union(A,B)
[N,ka,kb]=union(A,B)

union('a'+string(A),'b'+string(B))

xdel(winsid())

//====================================================
// ../man/fr/elementary/unique.xml
//====================================================
clear;lines(0);
M=round(2*rand(20,1));

unique(M)
[N,k]=unique(M)

unique(string(M))
[N,k]=unique(string(M))

xdel(winsid())

//====================================================
// ../man/fr/elementary/vectorfind.xml
//====================================================
clear;lines(0);

      alr=[1,2,2;
	   1,2,1;
	   1,1,2;
	   1,1,1;
	   1,2,1];
      ind = vectorfind(alr,[1,2,1],'r')
      ind = vectorfind(string(alr),string([1,2,1]),'r')
 
xdel(winsid())

//====================================================
// ../man/fr/elementary/zeros.xml
//====================================================
clear;lines(0);
zeros(3)
zeros(3,3)
zeros(2,3,2)
xdel(winsid())

//====================================================
// ../man/fr/fileio/diary.xml
//====================================================

//====================================================
// ../man/fr/fileio/dir.xml
//====================================================
clear;lines(0);
    
    dir
    dir SCI/macros/util/*.sci
    x=dir('SCI/macros/util/f*.sci')
    dt=getdate(x.date);
    mprintf("%s: %d-%d-%d %d:%d:%d\n",x.name,dt(:,[3 2 1 7:9]))
   
xdel(winsid())

//====================================================
// ../man/fr/fileio/dispfiles.xml
//====================================================
clear;lines(0);

dispfiles()

xdel(winsid())

//====================================================
// ../man/fr/fileio/disp.xml
//====================================================
clear;lines(0);

disp([1 2],3)
deff('[]=%t_p(l)','disp(l(3),l(2))')
disp(tlist('t',1,2))

xdel(winsid())

//====================================================
// ../man/fr/fileio/fileinfo.xml
//====================================================
clear;lines(0);

w=fileinfo(SCI+'/scilab.star')
getdate(w(6))
  
xdel(winsid())

//====================================================
// ../man/fr/fileio/fileparts.xml
//====================================================
clear;lines(0);

   [path,fname,extension]=fileparts('SCI/scilab.star')
   fileparts('SCI/scilab.star','extension')
 
xdel(winsid())

//====================================================
// ../man/fr/fileio/file.xml
//====================================================
clear;lines(0);

u=file('open',TMPDIR+'/foo','unknown')
for k=1:4
  a=rand(1,4)
  write(u,a)
end
file('rewind',u)
x=read(u,2,4)
file('close',u)
//
u1=file('open',TMPDIR+'/foo','unknown')
u2=mopen(TMPDIR+'/foo1','wb')
[units,typs,nams]=file()
file('close',u1);
mclose(u2);

xdel(winsid())

//====================================================
// ../man/fr/fileio/fprintfMat.xml
//====================================================
clear;lines(0);

n=50;
a=rand(n,n,'u');
fprintfMat(TMPDIR+'/Mat',a,'%5.2f');
a1=fscanfMat(TMPDIR+'/Mat');

xdel(winsid())

//====================================================
// ../man/fr/fileio/fprintf.xml
//====================================================
clear;lines(0);

u=file('open','results','unknown') //open the result file
t=0:0.1:2*%pi;
for tk=t
 fprintf(u,'time = %6.3f value = %6.3f',tk,sin(tk)) // write a line
end
file('close',u) //close the result file

xdel(winsid())

//====================================================
// ../man/fr/fileio/fscanfMat.xml
//====================================================
clear;lines(0);

fd=mopen(TMPDIR+'/Mat','w');
mfprintf(fd,'Some text.....\n');
mfprintf(fd,'Some text again\n');
a=rand(6,6);
for i=1:6 ,
 for j=1:6, mfprintf(fd,'%5.2f ',a(i,j));end;
 mfprintf(fd,'\n'); 
end
mclose(fd);
a1=fscanfMat(TMPDIR+'/Mat')

xdel(winsid())

//====================================================
// ../man/fr/fileio/fscanf.xml
//====================================================

//====================================================
// ../man/fr/fileio/getio.xml
//====================================================

//====================================================
// ../man/fr/fileio/input.xml
//====================================================
clear;lines(0);

//x=input("How many iterations?")
//x=input("What is your name?","string")

xdel(winsid())

//====================================================
// ../man/fr/fileio/isdir.xml
//====================================================
clear;lines(0);

isdir(TMPDIR)
isdir SCI/scilab.star

xdel(winsid())

//====================================================
// ../man/fr/fileio/lines.xml
//====================================================

//====================================================
// ../man/fr/fileio/loadmatfile.xml
//====================================================

//====================================================
// ../man/fr/fileio/load.xml
//====================================================
clear;lines(0);

a=eye(2,2);b=ones(a);
save('vals.dat',a,b);
clear a
clear b
load('vals.dat','a','b');

xdel(winsid())

//====================================================
// ../man/fr/fileio/ls.xml
//====================================================
clear;lines(0);
    
    ls
    ls SCI/macros/util/*.sci
    x=ls('SCI/macros/util/f*.sci')
   
xdel(winsid())

//====================================================
// ../man/fr/fileio/manedit.xml
//====================================================
clear;lines(0);

//manedit('lqg')

xdel(winsid())

//====================================================
// ../man/fr/fileio/matfile2sci.xml
//====================================================

//====================================================
// ../man/fr/fileio/mclearerr.xml
//====================================================

//====================================================
// ../man/fr/fileio/mclose.xml
//====================================================

//====================================================
// ../man/fr/fileio/mdelete.xml
//====================================================

//====================================================
// ../man/fr/fileio/meof.xml
//====================================================

//====================================================
// ../man/fr/fileio/merror.xml
//====================================================

//====================================================
// ../man/fr/fileio/mfscanf.xml
//====================================================
clear;lines(0);

s='1 1.3'
[n,a,b]=msscanf(s,"%i %e")
msscanf(s,"%i %e")

msscanf(" 12\n",'%c%c%c%c') //scan characters

msscanf('0xabc','%x') //scan with hexadecimal format

msscanf('012345abczoo','%[0-9abc]%s')  //[] notation

//create a file with data
u=mopen(TMPDIR+'/foo','w');
t=(0:0.1:%pi)';mfprintf(u,"%6.3f %6.3f\n",t,sin(t))
mclose(u);

//read the file line by line
u=mopen(TMPDIR+'/foo','r');
[n,a,b]=mfscanf(u,'%e %e')
l=mfscanf(u,'%e %e')
mclose(u);

//use niter
[n,Names,Ages]=msscanf(-1,["Alain 19";"Pierre 15";"Tom 12"],'%s %d')
D=msscanf(-1,["Alain 19";"Pierre 15";"Tom 12"],'%s %d')
typeof(D)
Names=D(:,1) //strings
Age=D(:,2)  //numerical values


u=mopen(TMPDIR+'/foo','w');
mfprintf(u,"%s\n",["Alain 19";"Pierre 15";"Tom 12"])
mclose(u);

u=mopen(TMPDIR+'/foo','r');
[n,Names,Ages]=mfscanf(2,u,'%s %d')
mclose(u);



xdel(winsid())

//====================================================
// ../man/fr/fileio/mgetl.xml
//====================================================
clear;lines(0);

mgetl('SCI/scilab.star',5)

mgetl SCI/macros/elem/erf.sci

fd=mopen('SCI/scilab.star','r')
mgetl(fd,10)
mclose(fd)

xdel(winsid())

//====================================================
// ../man/fr/fileio/mgetstr.xml
//====================================================

//====================================================
// ../man/fr/fileio/mget.xml
//====================================================
clear;lines(0);

file1 = 'test1.bin';
file2 = 'test2.bin';
fd1=mopen(file1,'wb');
fd2=mopen(file2,'wb');
mput(1996,'ull',fd1);
mput(1996,'ull',fd2);
mclose(fd1);
mclose(fd2);

fd1=mopen(file1,'rb');
if 1996<>mget(1,'ull',fd1) ;write(%io(2),'Bug');end;
fd2=mopen(file2,'rb');
if 1996<>mget(1,'ull',fd2) ;write(%io(2),'Bug');end;
mclose(fd1);
mclose(fd2);

xdel(winsid())

//====================================================
// ../man/fr/fileio/mopen.xml
//====================================================

//====================================================
// ../man/fr/fileio/mprintf.xml
//====================================================
clear;lines(0);

mprintf('At iteration %i, Result is:\nalpha=%f',33,0.535)

msprintf('%5.3f %5.3f',123,0.732)
msprintf('%5.3f\n%5.3f',123,0.732)

A=rand(5,2);
// vectorized forms: the format directive needs 
// two operand, each column of A is used as an operand. 
// and the mprintf function is applied on each row of A 
mprintf('%5.3f\t%5.3f\n',A)

colors=['red';'green';'blue';'pink';'black'];
RGB=[1 0 0;0 1 0;0 0 1;1 0.75 0.75;0 0 0];
mprintf('%d\t%s\t%f\t%f\t%f\n',(1:5)',colors,RGB)


xdel(winsid())

//====================================================
// ../man/fr/fileio/mputl.xml
//====================================================

//====================================================
// ../man/fr/fileio/mputstr.xml
//====================================================

//====================================================
// ../man/fr/fileio/mput.xml
//====================================================
clear;lines(0);

  filen = 'test.bin';
  mopen(filen,'wb');
  mput(1996,'l');mput(1996,'i');mput(1996,'s');mput(98,'c');
// force little-endian 
  mput(1996,'ll');mput(1996,'il');mput(1996,'sl');mput(98,'cl');
// force big-endian 
  mput(1996,'lb');mput(1996,'ib');mput(1996,'sb');mput(98,'cb');
//
  mclose();
  mopen(filen,'rb');
  if 1996<>mget(1,'l') then pause,end
  if 1996<>mget(1,'i') then pause,end
  if 1996<>mget(1,'s') then pause,end
  if   98<>mget(1,'c') then pause,end
  // force little-endian
  if 1996<>mget(1,'ll') then pause,end
  if 1996<>mget(1,'il') then pause,end
  if 1996<>mget(1,'sl') then pause,end
  if   98<>mget(1,'cl') then pause,end
  // force big-endian 
  if 1996<>mget(1,'lb') then pause,end
  if 1996<>mget(1,'ib') then pause,end
  if 1996<>mget(1,'sb') then pause,end
  if   98<>mget(1,'cb') then pause,end
  //
  mclose();

xdel(winsid())

//====================================================
// ../man/fr/fileio/mseek.xml
//====================================================
clear;lines(0);

file3='test3.bin'
fd1= mopen(file3,'wb');
for i=1:10, mput(i,'d'); end 
mseek(0);
mput(678,'d');
mseek(0,fd1,'end');
mput(932,'d');
mclose(fd1)
fd1= mopen(file3,'rb');
res=mget(11,'d')
res1=[1:11]; res1(1)=678;res1($)=932;
if res1<>res ;write(%io(2),'Bug');end;
mseek(0,fd1,'set');
// trying to read more than stored data 
res1=mget(100,'d',fd1);
if res1<>res ;write(%io(2),'Bug');end;
meof(fd1)
mclearerr(fd1)
mclose(fd1);

xdel(winsid())

//====================================================
// ../man/fr/fileio/mtell.xml
//====================================================

//====================================================
// ../man/fr/fileio/newest.xml
//====================================================
clear;lines(0);

newest('SCI/macros/xdess/bode.sci','SCI/macros/xdess/bode.bin')
newest('SCI/macros/xdess/bode.'+['sci','bin'])

xdel(winsid())

//====================================================
// ../man/fr/fileio/oldload.xml
//====================================================
clear;lines(0);

a=eye(2,2);b=ones(a);
oldsave(TMPDIR+'/vals.dat',a,b);
clear a
clear b
oldload(TMPDIR+'/vals.dat','a','b');

xdel(winsid())

//====================================================
// ../man/fr/fileio/oldsave.xml
//====================================================
clear;lines(0);

a=eye(2,2);b=ones(a);
oldsave('TMPDIR/val.dat',a,b);
clear a
clear b
oldload('TMPDIR/val.dat','a','b');


xdel(winsid())

//====================================================
// ../man/fr/fileio/printf_conversion.xml
//====================================================

//====================================================
// ../man/fr/fileio/printf.xml
//====================================================
clear;lines(0);

printf('Result is:\nalpha=%f",0.535)

xdel(winsid())

//====================================================
// ../man/fr/fileio/print.xml
//====================================================
clear;lines(0);

a=rand(3,3);p=poly([1,2,3],'s');l=list(1,'asdf',[1 2 3]);
print(%io(2),a,p,l)
write(%io(2),a)

xdel(winsid())

//====================================================
// ../man/fr/fileio/read4b.xml
//====================================================

//====================================================
// ../man/fr/fileio/readb.xml
//====================================================

//====================================================
// ../man/fr/fileio/readc_.xml
//====================================================

//====================================================
// ../man/fr/fileio/readmps.xml
//====================================================
clear;lines(0);


//Let the LP problem:
//objective:
//   min     XONE + 4 YTWO + 9 ZTHREE
//constraints:
//  LIM1:    XONE +   YTWO            < = 5
//  LIM2:    XONE +            ZTHREE > = 10
// MYEQN:         -   YTWO  +  ZTHREE   = 7
//Bounds
//  0 < = XONE < = 4
// -1 < = YTWO < = 1

//Generate MPS file 
txt=['NAME          TESTPROB'
     'ROWS'
     ' N  COST'
     ' L  LIM1'
     ' G  LIM2'
     ' E  MYEQN'
     'COLUMNS'
     '    XONE      COST                 1   LIM1                 1'
     '    XONE      LIM2                 1'
     '    YTWO      COST                 4   LIM1                 1'
     '    YTWO      MYEQN               -1'
     '    ZTHREE    COST                 9   LIM2                 1'
     '    ZTHREE    MYEQN                1'
     'RHS'
     '    RHS1      LIM1                 5   LIM2                10'
     '    RHS1      MYEQN                7'
     'BOUNDS'
     ' UP BND1      XONE                 4'
     ' LO BND1      YTWO                -1'
     ' UP BND1      YTWO                 1'
     'ENDATA'];
mputl(txt,TMPDIR+'/test.mps')
//Read the MPS file
P=readmps(TMPDIR+'/test.mps',[0 10^30])
//Convert it to linpro format
LP=mps2linpro(P)
//Solve it with linpro
[x,lagr,f]=linpro(LP(2:$))

xdel(winsid())

//====================================================
// ../man/fr/fileio/read.xml
//====================================================
clear;lines(0);

if MSDOS then unix('del foo');
else unix('rm -f foo'); end
A=rand(3,5); write('foo',A);
B=read('foo',3,5)
B=read('foo',-1,5)
read(%io(1),1,1,'(a)')  // waits for user's input

xdel(winsid())

//====================================================
// ../man/fr/fileio/save.xml
//====================================================
clear;lines(0);

a=eye(2,2);b=ones(a);
save('val.dat',a,b);
clear a
clear b
load('val.dat','a','b');

// sequential save into a file
fd=mopen('TMPDIR/foo','wb')
for k=1:4, x=k^2;save(fd,x,k),end
mclose(fd)
fd=mopen('TMPDIR/foo','rb')
for i=1:4, load(fd,'x','k');x,k,end
mclose(fd)

// appending variables to an old save file
fd=mopen('TMPDIR/foo','r+')
mseek(0,fd,'end') 
lst=list(1,2,3)
save(fd,lst)
mclose(fd)


xdel(winsid())

//====================================================
// ../man/fr/fileio/scanf_conversion.xml
//====================================================

//====================================================
// ../man/fr/fileio/scanf.xml
//====================================================

//====================================================
// ../man/fr/fileio/sprintf.xml
//====================================================
clear;lines(0);

fahr=120
sprintf('%3d Fahrenheit = %6.1f Celsius',fahr,(5/9)*(fahr-32)) 


xdel(winsid())

//====================================================
// ../man/fr/fileio/sscanf.xml
//====================================================

//====================================================
// ../man/fr/fileio/startup.xml
//====================================================

//====================================================
// ../man/fr/fileio/tk_getdir.xml
//====================================================
clear;lines(0);

tk_getdir()
tk_getdir('SCI/macros/xdess')
tk_getdir(title='Choose a directory name ')

xdel(winsid())

//====================================================
// ../man/fr/fileio/tk_getfile.xml
//====================================================
clear;lines(0);

tk_getfile()
tk_getfile('*.sci','SCI/macros/xdess')
tk_getfile(title='Choose a file name ')

xdel(winsid())

//====================================================
// ../man/fr/fileio/tk_savefile.xml
//====================================================
clear;lines(0);

tk_savefile()
tk_savefile('*.sci','SCI/macros/xdess')
tk_savefile(title='Choose a file name ')

xdel(winsid())

//====================================================
// ../man/fr/fileio/warning.xml
//====================================================

//====================================================
// ../man/fr/fileio/writb.xml
//====================================================

//====================================================
// ../man/fr/fileio/write4b.xml
//====================================================

//====================================================
// ../man/fr/fileio/write.xml
//====================================================
clear;lines(0);

if MSDOS then unix('del asave');
else unix('rm -f asave'); end
A=rand(5,3); write('asave',A); A=read('asave',5,3);
write(%io(2),A,'('' | '',3(f10.3,'' | ''))')
write(%io(2),string(1:10))
write(%io(2),strcat(string(1:10),','))
write(%io(2),1:10,'(10(i2,3x))')

if MSDOS then unix('del foo');
else unix('rm -f foo'); end
write('foo',A)

xdel(winsid())

//====================================================
// ../man/fr/fileio/xgetfile.xml
//====================================================
clear;lines(0);

xgetfile()
xgetfile('*.sci','SCI/macros/xdess')
xgetfile(title='Choose a file name ')

xdel(winsid())

//====================================================
// ../man/fr/functions/addinter.xml
//====================================================

//====================================================
// ../man/fr/functions/argn.xml
//====================================================

//====================================================
// ../man/fr/functions/clearfun.xml
//====================================================

//====================================================
// ../man/fr/functions/comp.xml
//====================================================

//====================================================
// ../man/fr/functions/deff.xml
//====================================================
clear;lines(0);

deff('[x]=myplus(y,z)','x=y+z')
//
deff('[x]=mymacro(y,z)',['a=3*y+1'; 'x=a*z+y'])

xdel(winsid())

//====================================================
// ../man/fr/functions/delbpt.xml
//====================================================
clear;lines(0);

setbpt('foo',1),setbpt('foo',10),delbpt('foo',10),dispbpt()
delbpt('foo',1)

xdel(winsid())

//====================================================
// ../man/fr/functions/dispbpt.xml
//====================================================

//====================================================
// ../man/fr/functions/edit.xml
//====================================================
clear;lines(0);

//newedit=edit('edit')  //opens editor with text of this function
//myfunction=edit('myfunction')  //opens editor for a new function

xdel(winsid())

//====================================================
// ../man/fr/functions/funcprot.xml
//====================================================
clear;lines(0);

funcprot(1)
deff('[x]=foo(a)','x=a')
deff('[x]=foo(a)','x=a+1')
foo=33
funcprot(0)
deff('[x]=foo(a)','x=a')
deff('[x]=foo(a)','x=a+1')
foo=33

xdel(winsid())

//====================================================
// ../man/fr/functions/functions.xml
//====================================================
clear;lines(0);
// définition en ligne
function [x,y]=myfct(a,b)
x=a+b
y=a-b
endfunction

[x,y]=myfct(3,2)

// autre type de définition en ligne
deff('[x,y]=myfct(a,b)',['x=a+b';
                         'y=a-b'])
// définition dans un fichier texte (voir exec)
exec SCI/macros/elem/asin.sci;

// définition dans un fichier texte (voir getf)
getf SCI/macros/elem/asin.sci;

xdel(winsid())

//====================================================
// ../man/fr/functions/function.xml
//====================================================
clear;lines(0);
// définition en ligne
function [x,y]=myfct(a,b)
x=a+b
y=a-b
endfunction

[x,y]=myfct(3,2)

// définition sur une seule ligne
function y=sq(x),y=x^2,endfunction

sq(3)

// définitions imbriquées
function y=foo(x)
a=sin(x)
function y=sq(x), y=x^2,endfunction
y=sq(a)+1
endfunction

foo(%pi/3)


// définition dans un script
exec SCI/macros/elem/asin.sci;


xdel(winsid())

//====================================================
// ../man/fr/functions/genlib.xml
//====================================================

//====================================================
// ../man/fr/functions/getd.xml
//====================================================
clear;lines(0);
getd('SCI/macros/auto')
xdel(winsid())

//====================================================
// ../man/fr/functions/get_function_path.xml
//====================================================
clear;lines(0);

get_function_path('median')

xdel(winsid())

//====================================================
// ../man/fr/functions/getf.xml
//====================================================
clear;lines(0);
getf('SCI/macros/xdess/plot.sci')

getf SCI/macros/xdess/plot.sci
xdel(winsid())

//====================================================
// ../man/fr/functions/library.xml
//====================================================
clear;lines(0);

// elemlib is a predefined library
elemlib //displays the contents of the library
A=rand(3,3);
cosm(A) //loads cosm and executes it

who // now cosm is a variable

elemlib.sinm //loads sinm from the library

elemlib.cosm(A) //reloads cosm and executes it

xdel(winsid())

//====================================================
// ../man/fr/functions/lib.xml
//====================================================
clear;lines(0);

//define some variables
function z = myplus(x, y), z = x + y,endfunction
function z = yourplus(x, y), x = x - y,endfunction
A=1:10;

//create the *.bin files in libdir
libdir=TMPDIR
save(libdir + '/myplus.bin', myplus);
save(libdir + '/yourplus.bin', yourplus);
save(libdir + '/A.bin', A);

//create the name file
mputl(['myplus';'yourplus';'A'],TMPDIR+'/names');

//build the library containing myplus and yourplus
xlib = lib(libdir+'/')

//erase the variables
clear myplus yourplus A

//Automatic loading and execution
myplus(1,2)

A

xdel(winsid())

//====================================================
// ../man/fr/functions/macr2lst.xml
//====================================================

//====================================================
// ../man/fr/functions/macr2tree.xml
//====================================================
clear;lines(0);

tree=macr2tree(help);
txt=tree2code(tree,%T);
write(%io(2),txt,'(a)')

xdel(winsid())

//====================================================
// ../man/fr/functions/macrovar.xml
//====================================================
clear;lines(0);

deff('y=f(x1,x2)','loc=1;y=a*x1+x2-loc')
vars=macrovar(f)

xdel(winsid())

//====================================================
// ../man/fr/functions/macro.xml
//====================================================

//====================================================
// ../man/fr/functions/newfun.xml
//====================================================

//====================================================
// ../man/fr/functions/plotprofile.xml
//====================================================
clear;lines(0);

//define function and prepare it for profiling
deff('x=foo(n)',['if n==0 then'
                 '  x=[]'
                 'else'
                 '  x=0'
                 '  for k=1:n'
                 '    s=svd(rand(n+10,n+10))'
                 '    x=x+s(1)'
                 '  end'
                 'end'],'p')
//call the function
foo(30)
//get execution profiles
plotprofile(foo) // click on Exit to exit

xdel(winsid())

//====================================================
// ../man/fr/functions/profile.xml
//====================================================
clear;lines(0);

//define function and prepare it for profiling
deff('x=foo(n)',['if n==0 then'
                 '  x=[]'
                 'else'
                 '  x=0'
                 '  for k=1:n'
                 '    s=svd(rand(n+10,n+10))'
                 '    x=x+s(1)'
                 '  end'
                 'end'],'p')
//call the function
foo(10)
//get execution profiles
profile(foo)
//call the function
foo(20)
profile(foo) //execution profiles are cumulated

xdel(winsid())

//====================================================
// ../man/fr/functions/setbpt.xml
//====================================================

//====================================================
// ../man/fr/functions/showprofile.xml
//====================================================
clear;lines(0);

//define function and prepare it for profiling
deff('x=foo(n)',['if n==0 then'
                 '  x=[]'
                 'else'
                 '  x=0'
                 '  for k=1:n'
                 '    s=svd(rand(n+10,n+10))'
                 '    x=x+s(1)'
                 '  end'
                 'end'],'p')
//call the function
foo(30)
//get execution profiles
showprofile(foo)

xdel(winsid())

//====================================================
// ../man/fr/functions/varargin.xml
//====================================================
clear;lines(0);

deff('exampl(a,varargin)',['[lhs,rhs]=argn(0)'
                          'if rhs>=1 then disp(varargin),end'])
exampl(1)
exampl()
exampl(1,2,3)
l=list('a',%s,%t);
exampl(1,l(2:3))

xdel(winsid())

//====================================================
// ../man/fr/functions/varargout.xml
//====================================================
clear;lines(0);

deff('varargout=exampl()','varargout=list(1,2,3,4)')

x=exampl()
[x,y]=exampl()
[x,y,z]=exampl()

xdel(winsid())

//====================================================
// ../man/fr/graphics/addcolor.xml
//====================================================

//====================================================
// ../man/fr/graphics/Compound_properties.xml
//====================================================
clear;lines(0);
   


xdel(winsid())

//====================================================
// ../man/fr/graphics/alufunctions.xml
//====================================================

//====================================================
// ../man/fr/graphics/arc_properties.xml
//====================================================
clear;lines(0);
   set("figure_style","new") //create a figure
   a=get("current_axes");//get the handle of the newly created axes
   a.data_bounds=[-2,-2;2,2];

   xarc(-1.5,1.5,3,3,0,360*64)

   arc=get("hdl"); //get handle on current entity (here the arc entity)
   arc.fill_mode="on";
   arc.foreground=5;
   arc.data(:,[3 6])=[2 270*64];  
   xfarc(-.5,1,.4,.6,0,360*64);
   arc.visible="off";

xdel(winsid())

//====================================================
// ../man/fr/graphics/axes_properties.xml
//====================================================
clear;lines(0);
   set("figure_style","new") //create a figure
   a=get("current_axes")//get the handle of the newly created axes
   a.axes_visible="on"; // makes the axes visible
   a.font_size=3; //set the tics label font size
   a.x_location="top"; //set the x axis position
   a.data_bounds=[-100,-2,-1;100,2,1]; //set the boundary values for the x, y and z coordinates.
   a.sub_tics=[5,0];
   a.labels_font_color=5;
   a.grid=[2,2];
   a.box="off";
   // Example with 3D axes
   clf(); //clear the graphics window
   x=0.1:0.1:2*%pi;plot2d(x-.3,sin(x)*7+.2);
   a=gca(); // get the handle of the current axes
   a.grid=[1 -1 -1]; //make x-grid
   a.rotation_angles=[70 250]; //turn the axes with giving angles
   a.grid=[1 6 -1]; //make y-grid
   a.view="2d"; //return te the 2d view
   a.box="off"; 
   a.labels_font_color=5;
   a.children.children.thickness=4;
   a.children.children.polyline_style=3;
   a.view="3d"; //return te the 3d view
   a.children.children.thickness=1;
   a.children.children.foreground=2;
   a.grid=[1 6 3]; //make z-grid
   a.parent.background=4;
   a.background=7;
   plot2d(cos(x)+1,3*sin(x)-3);
   plot2d(cos(x)+7,3*sin(x)+3);
   a.children(2).children.polyline_style=2;
   a.children(1).children.polyline_style=4;
   a.children(1).children.foreground=5;
   a.children(2).children.foreground=14;
   a.parent.figure_size= [1200,800];
   a.box="on";
   a.labels_font_size=4;
   a.parent.background=8;
   a.parent.figure_size= [400,200];
   a.rotation_angles=[0 260];
   delete(a.children(2)); 
   delete(); // delete current object
   a.labels_font_size=1;
   a.auto_clear= "on";
   x=0:0.1:2.5*%pi;plot2d(10*cos(x),sin(x));
   a=gca(); 
   a.rotation_angles=[45 45];
   a.data_bounds=[-20,-3,-2;20 3 ,2];
   xrect([-4 0.5 8 1]);
   a.isoview="on"; // isoview mode
   xrect([-2 0.25 4 0.5]);
   a.children(1).fill_mode="on";
   a.axes_visibles="off";
   a.children(1).data=[-2 0.25 -1 4 0.5];
   a.children(2).data=[-4 0.5 1 8 1];
   x=2*%pi*(0:7)/8;
   xv=[.2*sin(x);.9*sin(x)];yv=[.2*cos(x);.9*cos(x)];
   xsegs(10*xv,yv,1:8)
   s=a.children(1);
   s.arrow_size=1;
   s.segs_color=5;
   a.data_bounds //the boundary values for the x,y and z coordinates
   a.view="2d";
   a.data_bounds=[-10,-1; 10,1]; // set the boundary values for the two-dimensional views
   // Example on axes model
   da=gda() // get the handle on axes model to view and edit the fields
   // title by default
   da.title.text="My Default@Title"
   da.title.foreground = 12;
   da.title.font_size = 4;
   // x labels default
   da.x_label.text="x";
   da.x_label.font_style = 8;
   da.x_label.font_size = 2;
   da.x_label.foreground = 5;
   da.x_location = "middle";
   // y labels default
   da.y_label.text="y";
   da.y_label.font_style = 3;
   da.y_label.font_size = 5;
   da.y_label.foreground = 3;
   da.y_location = "right";
   da.thickness = 2;
   da.foreground = 7;
   // the plot
   x=(0:0.1:2*%pi)';
   plot2d(x,[sin(x),sin(2*x),sin(3*x)],style=[1,2,3],rect=[0,-2,2*%pi,2]);

xdel(winsid())

//====================================================
// ../man/fr/graphics/axis_properties.xml
//====================================================
clear;lines(0);


  set("figure_style","new") //create a figure
   a=get("current_axes");//get the handle of the newly created axes
   a.data_bounds=[-1,-1;10,10];

  drawaxis(x=2:7,y=4,dir='u');
  a1=a.children(1)
  a1.xtics_coord=[1 4 5  8 10];
  a1.tics_color=2;
  a1.labels_font_size=3;
  a1.tics_direction="bottom";
  a1.tics_labels= [" February" "May"  "june" "August"  "October"];
  
  drawaxis(x=1.2:1:10,y=5,dir='u',textcolor=13);
  a2=get("hdl")
  a2.sub_tics=0;
  a2.tics_segments="off";
  a2.ytics_coord=4;
  
  drawaxis(x=-1,y=0:1:7,dir='r',fontsize=10,textcolor=5,ticscolor=6,sub_int=10)
  a3=get("hdl");
  a3.tics_labels= 'B'  +string(0:7);
  a3.tics_direction="left";

   

xdel(winsid())

//====================================================
// ../man/fr/graphics/black.xml
//====================================================
clear;lines(0);

s=poly(0,'s')
h=syslin('c',(s^2+2*0.9*10*s+100)/(s^2+2*0.3*10.1*s+102.01))
chart();
sstr='(s^2+2*0.9*10*s+100)/(s^2+2*0.3*10.1*s+102.01)';
black(h,0.01,100,sstr);
h1=h*syslin('c',(s^2+2*0.1*15.1*s+228.01)/(s^2+2*0.9*15*s+225))
clf()
black([h1;h],0.01,100,['h1';'h'])
 
xdel(winsid())

//====================================================
// ../man/fr/graphics/bode.xml
//====================================================
clear;lines(0);

s=poly(0,'s')
h=syslin('c',(s^2+2*0.9*10*s+100)/(s^2+2*0.3*10.1*s+102.01))
title='(s^2+2*0.9*10*s+100)/(s^2+2*0.3*10.1*s+102.01)';
bode(h,0.01,100,title);
h1=h*syslin('c',(s^2+2*0.1*15.1*s+228.01)/(s^2+2*0.9*15*s+225))
clf()
bode([h1;h],0.01,100,['h1';'h'])
 
xdel(winsid())

//====================================================
// ../man/fr/graphics/champ1.xml
//====================================================
clear;lines(0);

champ1(-5:5,-5:5,rand(11,11),rand(11,11),rect=[-10,-10,10,10],arfact=2)
 
xdel(winsid())

//====================================================
// ../man/fr/graphics/champ_properties.xml
//====================================================
clear;lines(0);

  
  set("figure_style","new") //create a figure
   a=get("current_axes");//get the handle of the newly created axes
   a.data_bounds=[-10,-10;10,10];
   champ(-5:5,-5:5,rand(11,11),rand(11,11))

   c=a.children

   c.colored="on";
   c.thickness=2;
   a.data_bounds=[-5,-5;5,5];


xdel(winsid())

//====================================================
// ../man/fr/graphics/champ.xml
//====================================================
clear;lines(0);

// using rect as plot boundaries 
champ(-5:5,-5:5,rand(11,11),rand(11,11),rect=[-10,-10,10,10],arfact=2)
// using (x,y) to get boundaries 
clf()
champ(-5:5,-5:5,rand(11,11),rand(11,11),2,[-10,-10,10,10],"021")
 
xdel(winsid())

//====================================================
// ../man/fr/graphics/chart.xml
//====================================================
clear;lines(0);
s=poly(0,'s')
h=syslin('c',(s^2+2*0.9*10*s+100)/(s^2+2*0.3*10.1*s+102.01))
black(h,0.01,100,'(s^2+2*0.9*10*s+100)/(s^2+2*0.3*10.1*s+102.01)')
chart(list(1,0,2,3));

// Un autre exemple :

xbasc()
h1=h*syslin('c',(s^2+2*0.1*15.1*s+228.01)/(s^2+2*0.9*15*s+225))
black([h1;h],0.01,100,['h1';'h'])
chart([-8 -6 -4],[80 120],list(1,0));
xdel(winsid())

//====================================================
// ../man/fr/graphics/clear_pixmap.xml
//====================================================

//====================================================
// ../man/fr/graphics/clf.xml
//====================================================
clear;lines(0);
   f4=scf(4); //creates figure with id==4 and make it the current one
   f.color_map = jetcolormap(64);
   f.figure_size = [400, 200];
   plot2d()
   clf(f,'reset')
   f0=scf(0); //creates figure with id==0 and make it the current one
   f0.color_map=hotcolormap(128);
   plot3d1();
   clf() // equivalent to clf(gcf(),'clear')
   plot3d1(); // color_map unchanged
   clf(gcf(),'reset')
   plot3d1(); // color_map changed (back to the default one with 32 colors)
xdel(winsid())

//====================================================
// ../man/fr/graphics/colorbar.xml
//====================================================
clear;lines(0);
// example 1
x = linspace(0,1,81);
z = cos(2*%pi*x)'*sin(2*%pi*x);
zm = min(z); zM = max(z);
xbasc()
xset("colormap",jetcolormap(64))
colorbar(zm,zM)
Sgrayplot(x,x,z)
xtitle("The function cos(2 pi x)sin(2 pi y)")

// example 2 
x = linspace(0,1,81);
z = cos(2*%pi*x)'*sin(2*%pi*x);
zm = min(z); zM = max(z);
zz = abs(0.5*cos(2*%pi*x)'*cos(2*%pi*x));
zzm = min(zz); zzM = max(zz);
xbasc();
xset("colormap",jetcolormap(64))
subplot(2,2,1)
   colorbar(zm,zM)
   Sgrayplot(x,x,z, strf="031", rect=[0 0 1 1])
   xtitle("a Sgrayplot with a colorbar")
subplot(2,2,2)
   colorbar(zm,zM)
   plot3d1(x,x,z)
   xtitle("a plot3d1 with a colorbar")
subplot(2,2,3)
   colorbar(zzm,zzM)
   plot3d1(x,x,zz)
   xtitle("a plot3d1 with a colorbar")
subplot(2,2,4)
   colorbar(zzm,zzM)
   Sgrayplot(x,x,zz, strf="031", rect=[0 0 1 1])
   xtitle("a Sgrayplot with a colorbar")

// example 3
x = linspace(0,1,81);
zz = abs(0.5*cos(2*%pi*x)'*cos(2*%pi*x));
zzm = min(zz); zzM = max(zz);
[xf,yf,zf]=genfac3d(x,x,zz);
nb_col = 64;
xbasc()
xset("colormap",hotcolormap(nb_col))
colorbar(zzm,zzM)
nbcol = xget("lastpattern")
zcol = dsearch(zf, linspace(zzm, zzM, nb_col+1));
plot3d(xf, yf, list(zf, zcol), flag = [-2 6 4])
xtitle("a plot3d with shaded interpolated colors")
xselect()
xdel(winsid())

//====================================================
// ../man/fr/graphics/color_list.xml
//====================================================

//====================================================
// ../man/fr/graphics/colormap.xml
//====================================================
clear;lines(0);

n=64;
r=linspace(0,1,n)';
g=linspace(1,0,n)';
b=ones(r);
cmap=[r g b];
f=gcf(); f.color_map=cmap;
plot3d1()
f.color_map=get(sdf(),"color_map");

xdel(winsid())

//====================================================
// ../man/fr/graphics/color.xml
//====================================================
clear;lines(0);

x=linspace(-2*%pi,2*%pi,100)';
// using existing colors
plot2d(x,[sin(x),cos(x)],style=[color("red"),color("green")]);
// new colors: there are added to the colormap
e=gce(); p1=e.children(1); p2=e.children(2);
p1.foreground=color("purple"); p2.foreground=color("navy blue");
// using RGV values
p1.foreground=color(255,128,128);

xdel(winsid())

//====================================================
// ../man/fr/graphics/contour2di.xml
//====================================================
clear;lines(0);
[xc,yc]=contour2di(1:10,1:10,rand(10,10),5);
k=1;n=yc(k);c=1;
while k+yc(k)<size(xc,'*')
  n=yc(k);
  plot2d(xc(k+(1:n)),yc(k+(1:n)),c)
  c=c+1;
  k=k+n+1;
end

xdel(winsid())

//====================================================
// ../man/fr/graphics/contour2d.xml
//====================================================
clear;lines(0);

contour2d(1:10,1:10,rand(10,10),5,rect=[0,0,11,11])
// changing the format of the printing of the levels
xset("fpf","%.2f")
clf()
contour2d(1:10,1:10,rand(10,10),5,rect=[0,0,11,11])
 
xdel(winsid())

//====================================================
// ../man/fr/graphics/contourf.xml
//====================================================
clear;lines(0);
contourf(1:10,1:10,rand(10,10),5,1:5,"011"," ",[0,0,11,11])
xdel(winsid())

//====================================================
// ../man/fr/graphics/contour.xml
//====================================================
clear;lines(0);

t=%pi*[-10:10]/10;
deff("[z]=surf(x,y)","z=sin(x)*cos(y)"); z=feval(t,t,surf);
rect=[-%pi,%pi,-%pi,%pi,-1,1];
contour(t,t,z,10,35,45," ",[0,1,0],rect)
// changing the format of the printing of the levels
xset("fpf","%.2f")
xbasc()
contour(t,t,z,10,flag=[0,1,0],ebox=rect)

xdel(winsid())

//====================================================
// ../man/fr/graphics/copy.xml
//====================================================
clear;lines(0);




xdel(winsid())

//====================================================
// ../man/fr/graphics/delete.xml
//====================================================
clear;lines(0);


  set("figure_style","new") // select entity based graphics  
  subplot(211);
  t=1:10;plot2d(t,t.^2),
  subplot(223);
  plot3d();
  subplot(224);
  plot2d();
  xfrect(1,0,3,1);
  a=get("current_axes") 
  delete(); //delete the graphics object newly created
  delete(a.children); //delete all children of the current axes
  delete(a); //delete the axes
  delete("all"); //delete all the graphics objects of the figure


xdel(winsid())

//====================================================
// ../man/fr/graphics/dragrect.xml
//====================================================
clear;lines(0);
xsetech(frect=[0,0,100,100])
r=dragrect([10;10;30;10])
xrect(r)
xdel(winsid())

//====================================================
// ../man/fr/graphics/drawaxis.xml
//====================================================
clear;lines(0);
plot2d(1:10,1:10,1,"020")
// axe horizontal
drawaxis(x=2:7,y=4,dir='u',tics='v')
// axe horizontal en haut du cadre
drawaxis(x=2:7,dir='u',tics='v') 
// axe horizontal en bas du cadre
drawaxis(x=2:7,dir='d',tics='v') 

// axe horizontal donné par une plage de variation 
drawaxis(x=[2,7,3],y=4,dir='d',tics='r') 

// axe vertical
drawaxis(x=4,y=2:7,dir='r',tics='v')
drawaxis(x=2,y=[2,7,3],dir='l',tics='r')
drawaxis(y=2:7,dir='r',tics='v')
drawaxis(y=2:7,dir='l',tics='v')

// axe horizontal avec chaînes de caractères au dessus des graduations
drawaxis(x=2:7,y=8,dir='u',tics='v',val='A'+string(1:6));
// axe vertical avec chaînes de caractères au dessus des graduations
drawaxis(x=8,y=2:7,dir='r',tics='v',val='B'+string(1:6));

// axe horizontal avec 'i' 
drawaxis(x=[2,5,0,3],y=9,dir='u',tics='i');
drawaxis(x=9,y=[2,5,0,3],dir='r',tics='i',sub_int=5);

// encore un axe horizontal
drawaxis(x=2:7,y=4,dir='u',tics='v',fontsize=10,textcolor=9,ticscolor=7,seg=0,sub_int=20) 
xdel(winsid())

//====================================================
// ../man/fr/graphics/drawlater.xml
//====================================================
clear;lines(0);

  set("figure_style","new") // select entity based graphics
  drawlater(); 
  xfarc(.25,.55,.1,.15,0,64*360);
  xfarc(.55,.55,.1,.15,0,64*360);
  xfrect(.3,.8,.3,.2); 
  xfrect(.2,.7,.5,.2);  
  xfrect(.32,.78,.1,.1);
  xfrect(.44,.78,.14,.1);
  xfrect(-.2,.4,1.5,.8);
  xstring(0.33,.9,"A Scilab Car");    
  a=get("current_axes");
  a.children(1).font_size=4;
  a.children(1).font_style=4;  
  a.children(1).foreground=5;
  a.children(3).foreground=8;
  a.children(4).foreground=8; 
  a.children(5).foreground=17;
  a.children(6).foreground=17; 
  a.children(7).foreground=25;
  a.children(8).foreground=25;
  xclick();drawnow();

xdel(winsid())

//====================================================
// ../man/fr/graphics/drawnow.xml
//====================================================
clear;lines(0);


  set("figure_style","new") // select entity based graphics
  f=get("current_figure") // handle of the current figure
  
  drawlater();
  subplot(221);
  t=1:10;plot2d(t,t.^2)
  subplot(222);
  x=0:1:7;plot2d(x,cos(x),2) 
  subplot(234);
  plot2d(t,cos(t),3);
  subplot(235);
  plot2d(x,sin(2*x),5); 
  subplot(236);
  plot2d(t,tan(2*t));  
 
  draw(f.children([3 4]));
  drawnow();


xdel(winsid())

//====================================================
// ../man/fr/graphics/draw.xml
//====================================================
clear;lines(0);




xdel(winsid())

//====================================================
// ../man/fr/graphics/driver.xml
//====================================================

//====================================================
// ../man/fr/graphics/edit_curv.xml
//====================================================

//====================================================
// ../man/fr/graphics/errbar.xml
//====================================================
clear;lines(0);
t=[0:0.1:2*%pi]';
y=[sin(t) cos(t)]; x=[t t];
plot2d(x,y)
errbar(x,y,0.05*ones(x),0.03*ones(x))
xdel(winsid())

//====================================================
// ../man/fr/graphics/eval3dp.xml
//====================================================
clear;lines(0);
p1=linspace(0,2*%pi,10);
p2=linspace(0,2*%pi,10);
deff("[x,y,z]=scp(p1,p2)",["x=p1.*sin(p1).*cos(p2)";..
                            "y=p1.*cos(p1).*cos(p2)";..
                            "z=p1.*sin(p2)"])
[Xf,Yf,Zf]=eval3dp(scp,p1,p2);
plot3d(Xf,Yf,Zf)
xdel(winsid())

//====================================================
// ../man/fr/graphics/eval3d.xml
//====================================================
clear;lines(0);
  x=-5:5;y=x;
  deff('[z]=f(x,y)',['z= x.*y']);
  z=eval3d(f,x,y);
  plot3d(x,y,z);
// 
  deff('[z]=f(x,y)',['z= x*y']);
  z=feval(x,y,f);
  plot3d(x,y,z);
xdel(winsid())

//====================================================
// ../man/fr/graphics/evans.xml
//====================================================
clear;lines(0);

H=syslin('c',352*poly(-5,'s')/poly([0,0,2000,200,25,1],'s','c'));
evans(H,100)
P=3.0548543 - 8.8491842*%i;    //P=selected point
k=-1/real(horner(H,P));
Ns=H('num');Ds=H('den');
roots(Ds+k*Ns)     //contains P as particular root
// Another one
clf();s=poly(0,'s');n=1+s;
d=real(poly([-1 -2 -%i %i],'s'));
evans(n,d,100);
//
clf();n=real(poly([0.1-%i 0.1+%i,-10],'s'));
evans(n,d,80);
 
xdel(winsid())

//====================================================
// ../man/fr/graphics/fac3d.xml
//====================================================

//====================================================
// ../man/fr/graphics/fchamp.xml
//====================================================
clear;lines(0);

deff("[xdot] = derpol(t,x)",..
        ["xd1 = x(2)";..
         "xd2 = -x(1) + (1 - x(1)**2)*x(2)";..
         "xdot = [ xd1 ; xd2 ]"])
xf= -1:0.1:1;
yf= -1:0.1:1;
fchamp(derpol,0,xf,yf)
clf()
fchamp(derpol,0,xf,yf,1,[-2,-2,2,2],"011")
 
xdel(winsid())

//====================================================
// ../man/fr/graphics/fcontour2d.xml
//====================================================
clear;lines(0);

deff('z=surf(x,y)','z=x^4-y^4')
x=-3:0.1:3;
y=x;
fcontour2d(x,y,surf,10);

xdel(winsid())

//====================================================
// ../man/fr/graphics/fcontour.xml
//====================================================
clear;lines(0);

deff("[z]=surf(x,y)","z=sin(x)*cos(y)");
t=%pi*[-10:10]/10;

fcontour(t,t,surf,10)

xbasc();fcontour(t,t,surf,10,ebox=[-4 4 -4 4 -1 1],zlev=-1,flag=[0 1 4])
 
xdel(winsid())

//====================================================
// ../man/fr/graphics/fec_properties.xml
//====================================================
clear;lines(0);


   set("figure_style","new"); //create a figure

   x=-10:10; y=-10:10;m =rand(21,21);
   Sgrayplot(x,y,m);
   a=get("current_axes");
   f=a.children.children(2)
   f.data(:,3)=(1:size(f.data,1))';
   a.parent.color_map=hotcolormap(64);


xdel(winsid())

//====================================================
// ../man/fr/graphics/fec.xml
//====================================================

//====================================================
// ../man/fr/graphics/fgrayplot.xml
//====================================================
clear;lines(0);
t=-1:0.1:1;
deff("[z]=surf(x,y)","z=x**2+y**2")
fgrayplot(t,t,surf,rect=[-2,-2,2,2])
xdel(winsid())

//====================================================
// ../man/fr/graphics/figure_properties.xml
//====================================================
clear;lines(0);
   lines(0) // disables vertical paging 
  //Example 1
   set("figure_style","new") //create a figure and set the figure as the current selected one
   f=get("current_figure") //get the handle of the current figure : 
                                           //if none exists, create a figure and return the corresponding handle
   f.figure_position
   f.figure_size=[200,200]
   f.background=2
   f.children  // man can see that an Axes entity already exists
   delete(f);
   f=gcf(); // macro shortcut <=> f=get("current_figure")
   f.pixmap = "on" // set pixmap status to on
   plot2d() // nothing happens on the screen...
   show_pixmap() // ...display the pixmap on screen
   //Example 2 : default_figure settings
   df=get("default_figure") // get the default values (shortcut is gdf() )
   // Let's change the defaults...
   df.color_map=hotcolormap(128)
   df.background= 110 // set background toa kind of yellow (Note that we are using a color index inside the color_map previously redefined)
   scf(122); // creates new figure number 122 with the new default
   plot2d()
   scf(214);
   t=-%pi:0.3:%pi;
   plot3d(t,t,sin(t)'*cos(t),35,45,'X@Y@Z',[15,2,4]);
xdel(winsid())

//====================================================
// ../man/fr/graphics/fplot2d.xml
//====================================================
clear;lines(0);

deff("[y]=f(x)","y=sin(x)+cos(x)")
x=[0:0.1:10]*%pi/10;
fplot2d(x,f)
clf();
fplot2d(1:10,'parab')
 
xdel(winsid())

//====================================================
// ../man/fr/graphics/fplot3d1.xml
//====================================================
clear;lines(0);

deff('z=f(x,y)','z=x^4-y^4')
x=-3:0.2:3 ;y=x ;
clf() ;fplot3d1(x,y,f,alpha=5,theta=31)  
 
xdel(winsid())

//====================================================
// ../man/fr/graphics/fplot3d.xml
//====================================================
clear;lines(0);

deff('z=f(x,y)','z=x^4-y^4')
x=-3:0.2:3 ;y=x ;
clf() ;fplot3d(x,y,f,alpha=5,theta=31)  
 
xdel(winsid())

//====================================================
// ../man/fr/graphics/gainplot.xml
//====================================================
clear;lines(0);

s=poly(0,'s')
h=syslin('c',(s^2+2*0.9*10*s+100)/(s^2+2*0.3*10.1*s+102.01))
gainplot(h,0.01,100,'(s^2+2*0.9*10*s+100)/(s^2+2*0.3*10.1*s+102.01)')
clf()
h1=h*syslin('c',(s^2+2*0.1*15.1*s+228.01)/(s^2+2*0.9*15*s+225))
gainplot([h1;h],0.01,100,['h1';'h'])
 
xdel(winsid())

//====================================================
// ../man/fr/graphics/gca.xml
//====================================================
clear;lines(0);
   
    set("figure_style","new") //create a figure
    subplot(211)
    a=gca() //get the current axes
    a.box="off";
    t=-%pi:0.3:%pi;plot3d(t,t,sin(t)'*cos(t),80,50,'X@Y@Z',[5,2,4]);
    subplot(212)
    plot2d(); //simple plot
    a=gca() //get the current axes
    a.box="off";
    a.x_location="middle";
    a.parent.background=4;
    delete(gca()) // delete the current axes    
    xdel(0) //delete a graphics window 

xdel(winsid())

//====================================================
// ../man/fr/graphics/gce.xml
//====================================================
clear;lines(0);
   
    set("figure_style","new") //create a figure entity
    a=gca() //get the handle of the newly created axes
    a.data_bounds=[1,10;1,10];
    for i=1:5
      xfrect(7-i,9-i,3,3);
      e=gce();
      e.foreground=i;
     end
    delete(); // delete current entity
    delete(gce()); // delete current entity
    delete(gca()); // delete current axes
     

xdel(winsid())

//====================================================
// ../man/fr/graphics/gcf.xml
//====================================================
clear;lines(0);
    set("figure_style","new") //create a figure
    f=gcf()  
    f.figure_size= [610,469]/2;
    f.figure_name= "Foo";

    f=figure(); // create a figure
    h=uicontrol(f,'style','listbox','position', [10 10 150 160]);// create a listbox
    set(h, 'string', "item 1|item 2|item3");// fill the list
    set(h, 'value', [1 3]); // select item 1 and 3 in the list
    gcf()

    scf(0); //make graphic window 0 the current figure
    gcf()  // return the graphic handle of the current figure

    figure(f) //make GUI  window f the current figure
    gcf()

xdel(winsid())

//====================================================
// ../man/fr/graphics/gda.xml
//====================================================
clear;lines(0);
     
    a=gda() // get the handle of the model axes 
      // setting its' properties
    a.background=31;
    a.box="off";
    a.rotation_angles=[70 10];
    a.tics_color=2;
    a.labels_font_size=3;
    a.labels_font_color=5;
    a.sub_tics=[5 5 3];
    a.x_location="top";
    set("figure_style","new")
    subplot(211)
    plot2d() //create an axes entity
    subplot(212)
    plot3d1() //create a second axes entity
    a.grids=[5 5 5]; // setting other model's properties
    clf()
    t=0:0.1:5*%pi; 
    plot2d(sin(t),cos(t),t/10) 
    set(a,"default_values",1); // return to the  default values of the model
                               // see sda() function
    clf()
    plot2d(sin(t),cos(t),t/10)

xdel(winsid())

//====================================================
// ../man/fr/graphics/gdf.xml
//====================================================
clear;lines(0);
   
   set old_style off
   f=gdf() // get the handle of the model figure 
   // setting its' properties
   f.background=7;
   f.figure_name="Function gdf()";
   f.figure_position=[-1 100];
   f.auto_resize="off";
   f.figure_size=[300 461];
   f.axes_size=[600 400];
   plot2d() //create a figure
   scf(1);
   plot3d() //create a second figure
   set(f,"default_values",1); // return to the  default values of figure's model
                              // see sdf() function
   scf(2);
   plot2d() 

xdel(winsid())

//====================================================
// ../man/fr/graphics/genfac3d.xml
//====================================================
clear;lines(0);
t=[0:0.3:2*%pi]'; z=sin(t)*cos(t');
[xx,yy,zz]=genfac3d(t,t,z);
plot3d(xx,yy,zz)
xdel(winsid())

//====================================================
// ../man/fr/graphics/geom3d.xml
//====================================================
clear;lines(0);
deff("[z]=surf(x,y)","z=sin(x)*cos(y)")
t=%pi*(-10:10)/10;
// dessin 3D de la surface
fplot3d(t,t,surf,35,45,"X@Y@Z")
// maintenant (t,t,sin(t).*cos(t)) est une courbe sur la surface
// pouvant être dessinée avec geom3d et xpoly
[x,y]=geom3d(t,t,sin(t).*cos(t));
xpoly(x,y,"lines")
// ajout d'un commentaire 
[x,y]=geom3d([0,0],[0,0],[5,0]);
xsegs(x,y)
xstring(x(1),y(1),"point (0,0,0)")
xdel(winsid())

//====================================================
// ../man/fr/graphics/getcolor.xml
//====================================================

//====================================================
// ../man/fr/graphics/getfont.xml
//====================================================
clear;lines(0);

[fId,fSize]=getfont();
xset("font",fId,fSize)
plot2d(0,0,rect=[0 0 10 10],axesflag=0)
xstring(5,5,"string")
  
xdel(winsid())

//====================================================
// ../man/fr/graphics/getlinestyle.xml
//====================================================
clear;lines(0);
x=0:0.1:10;
plot2d(x,sin(x))
e=gce(); // store the Compound containing the plot
e.children(1).line_style = getlinestyle();

xdel(winsid())

//====================================================
// ../man/fr/graphics/getmark.xml
//====================================================
clear;lines(0);
set("figure_style","new")
[mark,mrkSize]=getmark();
plot2d(x,sin(x),style=-mark);
clf();
plot2d(x,sin(x))
e=gce(); // store the Compound containing the plot
[mark,mrkSize]=getmark();
e.children(1).mark_style = mark;

xdel(winsid())

//====================================================
// ../man/fr/graphics/getsymbol.xml
//====================================================

//====================================================
// ../man/fr/graphics/get.xml
//====================================================
clear;lines(0);
  // for graphics entities
    clf()
    get("old_style") // check the state of the graphics' style
    set("figure_style","new") //create a figure
    get("figure_style") // check the style of the graphics' figure

    // simple graphics objects  
    subplot(121);
    x=[-.2:0.1:2*%pi]';
    plot2d(x-2,x.^2);
    subplot(122);
    xrect(.2,.7,.5,.2);     
    xrect(.3,.8,.3,.2);
    xfarc(.25,.55,.1,.15,0,64*360);
    xfarc(.55,.55,.1,.15,0,64*360);
    xstring(0.2,.9,"Example <<A CAR>>");
 
    h=get("hdl") //get the object newly created
    h.font_size=3;
 
    f=get("current_figure") //get the current figure 
    f.figure_size
    f.figure_size=[700 500];
    f.children
    f.children(2).type
    f.children(2).children
    f.children(2).children.children.thickness=4; 
 
    a=get("current_axes") //get the current axes
    a.children.type
    a.children.foreground //get the foreground color of a set of graphics objects
    a.children.foreground=9;

  // for  User Interface objects
   h=uicontrol('string', 'Button'); // Opens a window with a  button.
   p=get(h,'position'); // get the geometric aspect of the button
   disp('Button width: ' + string(p(3))); // print the width of the button
   close(); // close figure

 
xdel(winsid())

//====================================================
// ../man/fr/graphics/glue.xml
//====================================================
clear;lines(0);




xdel(winsid())

//====================================================
// ../man/fr/graphics/graduate.xml
//====================================================
clear;lines(0);
  y=(0:0.33:145.78)';
  xbasc();plot2d1('enn',0,y)
  [ymn,ymx,np]=graduate(mini(y),maxi(y))
  rect=[1,ymn,prod(size(y)),ymx];
  xbasc();plot2d1('enn',0,y,1,'011',' ',rect,[10,3,10,np])
xdel(winsid())

//====================================================
// ../man/fr/graphics/graphics_entities.xml
//====================================================
clear;lines(0);


  //Play this example line per line

  set("figure_style","new") //create a figure in entity mode

  //get the handle on the Figure entity and display its properties
  f=get("current_figure") 
  a=f.children // the handle on the Axes child
  x=(1:10)';  plot2d(x,[x.^2 x.^1.5])
  e=a.children //Compound of 2 polylines

  p1=e.children(1) //the last drawn polyline properties
  p1.foreground=5;  // change the polyline color
  e.children.thickness=5; // change the thickness of the two polylines

  delete(e.children(2))

  move(e.children,[0,30]) //translate the polyline

  a.axes_bounds=[0 0 0.5 0.5]; 

  subplot(222) //create a new Axes entity
  plot(1:10);
  a1=f.children(1); //get its handle
  copy(e.children,a1); //copy the polyline of the first plot in the new Axes
  a1.data_bounds=[1 0;10 100]; //change the Axes bounds  

  set("current_figure",10) //create a new figure with figure_id=10
  plot3d() //the drawing are sent to figure 10
  set("current_figure",f) //make the previous figure the current one
  plot2d(x,x^3) the drawing are send to the initial figure


xdel(winsid())

//====================================================
// ../man/fr/graphics/Graphics.xml
//====================================================

//====================================================
// ../man/fr/graphics/graycolormap.xml
//====================================================
clear;lines(0);

xset("colormap",graycolormap(32))
plot3d1() 

xdel(winsid())

//====================================================
// ../man/fr/graphics/grayplot_properties.xml
//====================================================
clear;lines(0);

   set("figure_style","new") //create a figure


   m=5;n=5;
   M=round(32*rand(m,n));
   grayplot(1:m,1:n,M)

   a=get("current_axes");
   a.data_bounds=  [-1,-1;7,7]
   h=a.children

   h.data_mapping="direct";
   
   // A 2D ploting of a matrix using colors 
   xbasc()
   a=get("current_axes");
   a.data_bounds=  [0,0;4,4];
  
   b=5*ones(11,11); b(2:10,2:10)=4; b(5:7,5:7)=2;
   Matplot1(b,[1,1,3,3])  ;
  
   h=a.children
   for i=1:7
    xclick(); // click the mouse to sets Matplot data
    h.data=h.data+4;
   end
   


xdel(winsid())

//====================================================
// ../man/fr/graphics/grayplot.xml
//====================================================
clear;lines(0);

x=-10:10; y=-10:10;m =rand(21,21);
grayplot(x,y,m,rect=[-20,-20,20,20])
t=-%pi:0.1:%pi; m=sin(t)'*cos(t);
clf()
grayplot(t,t,m)
 
xdel(winsid())

//====================================================
// ../man/fr/graphics/graypolarplot.xml
//====================================================
clear;lines(0);


  rho=1:0.1:4;theta=(0:0.02:1)*2*%pi;
  z=30+round(theta'*(1+rho^2));
  f=gcf();
  f.color_map= hotcolormap(128);
  clf();graypolarplot(theta,rho,z)
 
xdel(winsid())

//====================================================
// ../man/fr/graphics/gr_menu.xml
//====================================================

//====================================================
// ../man/fr/graphics/hist3d.xml
//====================================================

//====================================================
// ../man/fr/graphics/histplot.xml
//====================================================
clear;lines(0);

histplot()

d=rand(1,10000,'normal');
clf();histplot(20,d)

clf();histplot(20,d,normalization=%f)
clf();histplot(20,d,leg='rand(1,10000,''normal'')',style=5)

 
xdel(winsid())

//====================================================
// ../man/fr/graphics/hotcolormap.xml
//====================================================
clear;lines(0);

xset("colormap",hotcolormap(32))
plot3d1() 

xdel(winsid())

//====================================================
// ../man/fr/graphics/isoview.xml
//====================================================
clear;lines(0);

t=[0:0.1:2*%pi]';
plot2d(sin(t),cos(t))
xbasc()
isoview(-1,1,-1,1)
plot2d(sin(t),cos(t),1,"001")
xset("default")

plot2d(sin(t),cos(t),frameflag=4)

xdel(winsid())

//====================================================
// ../man/fr/graphics/jetcolormap.xml
//====================================================
clear;lines(0);

xset("colormap",jetcolormap(64))
plot3d1() 

xdel(winsid())

//====================================================
// ../man/fr/graphics/label_properties.xml
//====================================================
clear;lines(0);

   set("figure_style","new") //create a figure
   a=get("current_axes");
   a.title
   type(a.title)
   plot3d()
   a.x_label
   a.y_label
   a.z_label
   xtitle("My title","my x axis label", "Volume") 
   a.z_label.text="Month"

   t=a.title;
   t.foreground=9;
   t.font_size=5;
   t.font_style=5;
   t.text="SCILAB";
   
   xlabel=a.x_label;
   xlabel.text=" Weight"
   xlabel.font_style= 5;
   a.y_label.foreground = 12;
xdel(winsid())

//====================================================
// ../man/fr/graphics/legend_properties.xml
//====================================================
clear;lines(0);
   

  set("figure_style","new") //create a figure
   plot2d();
   a=get("current_axes");
   l=a.children.children(1)
   l.text="sin(x)@sin(2*x)@sin(3*x)";
   l.visible="off";
    


xdel(winsid())

//====================================================
// ../man/fr/graphics/legends.xml
//====================================================
clear;lines(0);
t=0:0.1:2*%pi;
plot2d(t,[cos(t'),cos(2*t'),cos(3*t')],[-1,2 3]);  
legends(['cos(t)';'cos(2*t)';'cos(3*t)'],[-1,2 3],4)
xset("line style",2);plot2d(t,cos(t),style=5);
xset("line style",4);plot2d(t,sin(t),style=3);
legends(["sin(t)";"cos(t)"],[[5;2],[3;4]])

xdel(winsid())

//====================================================
// ../man/fr/graphics/legend.xml
//====================================================
clear;lines(0);

set figure_style new
t=0:0.1:2*%pi;
a=gca();a.data_bounds=[t(1) -1.8;t($) 1.8];

plot2d(t,[cos(t'),cos(2*t'),cos(3*t')],[-1,2 3]);  
e=gce();
e.children(1).thickness=3;
e.children(2).line_style=4;

hl=legend(['cos(t)';'cos(2*t)';'cos(3*t)'],a=1);

xdel(winsid())

//====================================================
// ../man/fr/graphics/loadplots.xml
//====================================================
clear;lines(0);


driver('Rec');xbasc();plot2d([0 1.5 4]) //make a plot
xsave(TMPDIR+'/foo.scg') //save it in a binary file

rec=loadplots(TMPDIR+'/foo.scg'); //get the associated data structure
//here rec(9) is the data structure associated with the plot2d instruction
rec(9).x //the x vector
rec(9).y //the y vector

string(rec) //the scilab instructions producing the same plot

mputl(string(rec),TMPDIR+'/foo.sce') //creates a script file
xbasc();exec(TMPDIR+'/foo.sce',-1) //execute it to re-create the plot


xdel(winsid())

//====================================================
// ../man/fr/graphics/locate.xml
//====================================================

//====================================================
// ../man/fr/graphics/Matplot1.xml
//====================================================
clear;lines(0);
//--- premier exemple
//  on fixe l'échelle courante 
xsetech(frect=[0,0,10,10])
xrect(0,10,10,10)
a=5*ones(11,11); a(2:10,2:10)=4; a(5:7,5:7)=2;
// première matrice dans le rectangle [1,1,3,3]
Matplot1(a,[1,1,3,3])
a=ones(10,10); a= 3*tril(a)+ 2*a; 
// deuxième matrice dans le rectangle [5,6,7,8]
Matplot1(a,[5,6,7,8])
xset('default')
xbasc()
//--- deuxième exemple 
xsetech(frect=[0,0,10,10])
xrect(0,10,10,10)
n=100;
xset('pixmap',1)
driver('X11');
for k=-n:n,
  a=ones(n,n);
  a= 3*tril(a,k)+ 2*a;
  a= a + a';
  k1= 3*(k+100)/200;
  Matplot1(a,[k1,2,k1+7,9])
  xset('wshow')
  xset('wwpc')
end
xset('pixmap',0)
xset('default')
xbasc()
xdel(winsid())

//====================================================
// ../man/fr/graphics/Matplot.xml
//====================================================
clear;lines(0);

Matplot([1 2 3;4 5 6])
// draw the current colormap 
Matplot((1:xget("lastpattern")))

xdel(winsid())

//====================================================
// ../man/fr/graphics/m_circle.xml
//====================================================
clear;lines(0);

//Example 1 :
  s=poly(0,'s')
  h=syslin('c',(s^2+2*0.9*10*s+100)/(s^2+2*0.3*10.1*s+102.01))
  nyquist(h,0.01,100,'(s^2+2*0.9*10*s+100)/(s^2+2*0.3*10.1*s+102.01)')
  m_circle();
//Example 2:
  xbasc();
  h1=h*syslin('c',(s^2+2*0.1*15.1*s+228.01)/(s^2+2*0.9*15*s+225))
  nyquist([h1;h],0.01,100,['h1';'h'])
  m_circle([-8 -6 -4]);

xdel(winsid())

//====================================================
// ../man/fr/graphics/milk_drop.xml
//====================================================
clear;lines(0);
x=-2:0.1:2; y=x;
z=eval3d(milk_drop,x,y);
plot3d(x,y,z)
xdel(winsid())

//====================================================
// ../man/fr/graphics/move.xml
//====================================================
clear;lines(0);
xdel(winsid())

//====================================================
// ../man/fr/graphics/name2rgb.xml
//====================================================
clear;lines(0);

rgb=name2rgb("gold")
rgb2name(rgb)

xdel(winsid())

//====================================================
// ../man/fr/graphics/nf3d.xml
//====================================================
clear;lines(0);
// une sphère...
u = linspace(-%pi/2,%pi/2,40);
v = linspace(0,2*%pi,20);
x= cos(u)'*cos(v);
y= cos(u)'*sin(v);
z= sin(u)'*ones(v);
// plot3d2(x,y,z) est équivalent à ...
[xx,yy,zz]=nf3d(x,y,z); plot3d(xx,yy,zz)
xdel(winsid())

//====================================================
// ../man/fr/graphics/nyquist.xml
//====================================================
clear;lines(0);

clf();
s=poly(0,'s');
h=syslin('c',(s^2+2*0.9*10*s+100)/(s^2+2*0.3*10.1*s+102.01));
comm='(s^2+2*0.9*10*s+100)/(s^2+2*0.3*10.1*s+102.01)';
nyquist(h,0.01,100,comm);
h1=h*syslin('c',(s^2+2*0.1*15.1*s+228.01)/(s^2+2*0.9*15*s+225))
clf();
nyquist([h1;h],0.01,100,['h1';'h'])
clf();nyquist([h1;h])
 
xdel(winsid())

//====================================================
// ../man/fr/graphics/param3d1.xml
//====================================================
clear;lines(0);

t=[0:0.1:5*%pi]';
param3d1([sin(t),sin(2*t)],[cos(t),cos(2*t)],..
  list([t/10,sin(t)],[3,2]),35,45,"X@Y@Z",[2,3])

 set("figure_style","new") //create a figure
 a=get("current_axes");//get the handle of the newly created axes
 t=[0:0.1:5*%pi]';
 param3d1([sin(t),sin(2*t)],[cos(t),cos(2*t)],[t/10,sin(t)])
 a.rotation_angles=[65,75]; 
 a.data_bounds=[-1,-1,-1;1,1,2]; //boundaries given by data_bounds
 a.thickness = 2;
 h=a.children //get the handle of the param3d entity: an Compound composed of 2 curves
 h.children(1).foreground = 3 // first curve
 curve2 = h.children(2);
 curve2.foreground = 6;
 curve2.mark_style = 2;

xdel(winsid())

//====================================================
// ../man/fr/graphics/param3d_properties.xml
//====================================================
clear;lines(0);
   

   set("figure_style","new") //create a figure
   a=get("current_axes");//get the handle of the newly created axes
   t=[0:0.1:5*%pi]';
   param3d1([sin(t),sin(2*t)],[cos(t),cos(2*t)],[t/10,sin(t)])
  
   a.rotation_angles=[65,75]; 
   a.data_bounds=[-1,-1,-1;1,1,2]; //boundaries given by data_bounds
   a.thickness = 2;
   h=a.children //get the handle of the param3d entity: an Compound composed of 2 curves
   h.children(1).foreground = 3 // first curve
   curve2 = h.children(2);
   curve2.foreground = 6;
   curve2.mark_style = 2;

 
xdel(winsid())

//====================================================
// ../man/fr/graphics/param3d.xml
//====================================================
clear;lines(0);

t=0:0.1:5*%pi;
param3d(sin(t),cos(t),t/10,35,45,"X@Y@Z",[2,3])
 
xdel(winsid())

//====================================================
// ../man/fr/graphics/paramfplot2d.xml
//====================================================
clear;lines(0);
deff('y=f(x,t)','y=t*sin(x)')
x=linspace(0,2*%pi,50);theta=0:0.05:1;
paramfplot2d(f,x,theta);
xdel(winsid())

//====================================================
// ../man/fr/graphics/plot2d1.xml
//====================================================
clear;lines(0);

// multiple plot without giving x
x=[0:0.1:2*%pi]';
plot2d1("enn",1,[sin(x) sin(2*x) sin(3*x)])
// multiple plot using only one x
clf()
plot2d1("onn",x,[sin(x) sin(2*x) sin(3*x)])
// logarithmic plot
x=[0.1:0.1:3]'; clf()
plot2d1("oll",x,[exp(x) exp(x^2) exp(x^3)])
 
xdel(winsid())

//====================================================
// ../man/fr/graphics/plot2d2.xml
//====================================================
clear;lines(0);

// plots a step function of value i on the segment [i,i+1]
// the last segment is not drawn
plot2d2([1:4],[1:4],1,"111","step function",[0,0,5,5])
// compare the following with plot2d
x=[0:0.1:2*%pi]';
clf()
plot2d2(x,[sin(x) sin(2*x) sin(3*x)])
  // In New graphics only
clf()
set("figure_style","new")
plot2d(x,[sin(x) sin(2*x) sin(3*x)])
e=gce();
e.children(1).polyline_style=2;
e.children(2).polyline_style=2;
e.children(3).polyline_style=2;

xdel(winsid())

//====================================================
// ../man/fr/graphics/plot2d3.xml
//====================================================
clear;lines(0);

// compare the following with plot2d1
x=[0:0.1:2*%pi]';
plot2d3(x,[sin(x) sin(2*x) sin(3*x)])
// In New graphics only
clf()
set("figure_style","new")
plot2d(x,[sin(x) sin(2*x) sin(3*x)])
e=gce();
e.children(1).polyline_style=3;
e.children(2).polyline_style=3;
xdel(winsid())

//====================================================
// ../man/fr/graphics/plot2d4.xml
//====================================================
clear;lines(0);

// compare the following with plot2d1
x=[0:0.1:2*%pi]';
plot2d4(x,[sin(x) sin(2*x) sin(3*x)])
 // In New graphics only
clf()
set("figure_style","new")
plot2d(x,[sin(x) sin(2*x) sin(3*x)])
e=gce();
e.children(1).polyline_style=4;
e.children(2).polyline_style=4;
e.children(3).polyline_style=4;
xdel(winsid())

//====================================================
// ../man/fr/graphics/plot2d_old_version.xml
//====================================================
clear;lines(0);

//simple plot 
x=[0:0.1:2*%pi]';
plot2d(sin(x))
xbasc()
plot2d(x,sin(x))
//multiple plot
xbasc()
plot2d(x,[sin(x) sin(2*x) sin(3*x)])
// multiple plot giving the dimensions of the frame 
// old syntax and new syntax
xbasc()
plot2d(x,[sin(x) sin(2*x) sin(3*x)],1:3,"011","",[0,0,6,0.5])
xbasc()
plot2d(x,[sin(x) sin(2*x) sin(3*x)],rect=[0,0,6,0.5])
//multiple plot with captions and given tics // old syntax and new syntax
xbasc()
plot2d(x,[sin(x) sin(2*x) sin(3*x)],..
  [1,2,3],"111","L1@L2@L3",[0,-2,2*%pi,2],[2,10,2,10]) xbasc()
plot2d(x,[sin(x) sin(2*x) sin(3*x)],..
  [1,2,3],leg="L1@L2@L3",nax=[2,10,2,10],rect=[0,-2,2*%pi,2])
// isoview
xbasc()
plot2d(x,sin(x),1,"041")
// scale
xbasc()
plot2d(x,sin(x),1,"061")
// auto scaling with previous plots
xbasc()
plot2d(x,sin(x),1)
plot2d(x,2*sin(x),2) plot2d(2*x,cos(x),3)
// axis on the right xbasc()
plot2d(x,sin(x),1,"183","sin(x)")
// centered axis xbasc()
plot2d(x,sin(x),1,"184","sin(x)")
// axis centered at (0,0)
xbasc()
plot2d(x-4,sin(x),1,"185","sin(x)")

xdel(winsid())

//====================================================
// ../man/fr/graphics/plot2d.xml
//====================================================
clear;lines(0);

// x initialisation 
x=[0:0.1:2*%pi]';
//simple plot
plot2d(sin(x))
clf()
plot2d(x,sin(x))
//multiple plot
clf()
plot2d(x,[sin(x) sin(2*x) sin(3*x)])
// multiple plot giving the dimensions of the frame
clf()
plot2d(x,[sin(x) sin(2*x) sin(3*x)],rect=[0,0,6,0.5])
//multiple plot with captions and given tics + style
clf()
plot2d(x,[sin(x) sin(2*x) sin(3*x)],..
  [1,2,3],leg="L1@L2@L3",nax=[2,10,2,10],rect=[0,-2,2*%pi,2])
// isoview
clf()
plot2d(x,sin(x),1,frameflag= 4) 
// scale 
clf()
plot2d(x,sin(x),1,frameflag= 6) 
// auto scaling with previous plots + style
clf()
plot2d(x,sin(x),-1)
plot2d(x,2*sin(x),12) 
plot2d(2*x,cos(x),3) 
// axis on the right 
clf()
plot2d(x,sin(x),leg="sin(x)") 
a=gca(); // Handle on axes entity 
a.y_location ="right"; 
// axis centered at (0,0) 
clf()
plot2d(x-4,sin(x),1,leg="sin(x)") 
a=gca() // Handle on axes entity
a.x_location = "middle"; 
a.y_location = "middle"; 
// Some operations on entities created by plot2d ...
a=gca();
a.isoview='on'; 
a.children // list the children of the axes : here it is an Compound child composed of 2 entities 
poly1= a.children.children(2); //store polyline handle into poly1 
poly1.foreground = 4; // another way to change the style...
poly1.thickness = 3;  // ...and the tickness of a curve.
poly1.clip_state='off' // clipping control
leg= a.children.children(1); // store legend handle into leg
leg.font_style = 9; 
leg.foreground = 6;
a.isoview='off'; 
 
xdel(winsid())

//====================================================
// ../man/fr/graphics/plot3d1.xml
//====================================================
clear;lines(0);
t=[0:0.3:2*%pi]'; z=sin(t)*cos(t'); 
plot3d1(t,t,z) 
// same plot using facets computed by genfac3d 
[xx,yy,zz]=genfac3d(t,t,z); 
clf(); 
plot3d1(xx,yy,zz) 
// multiple plots 
clf(); 
plot3d1([xx xx],[yy yy],[zz 4+zz]) 
// simple plot with viewpoint and captions 
clf() ;
plot3d1(1:10,1:20,10*rand(10,20),35,45,"X@Y@Z",[2,2,3]) 
// same plot without grid 
clf() 
plot3d1(1:10,1:20,10*rand(10,20),35,45,"X@Y@Z",[-2,2,3]) 
// plot of a sphere using facets computed by eval3dp 
deff("[x,y,z]=sph(alp,tet)",["x=r*cos(alp).*cos(tet)+orig(1)*ones(tet)";.. 
"y=r*cos(alp).*sin(tet)+orig(2)*ones(tet)";.. 
"z=r*sin(alp)+orig(3)*ones(tet)"]); 
r=1; orig=[0 0 0]; 
[xx,yy,zz]=eval3dp(sph,linspace(-%pi/2,%pi/2,40),linspace(0,%pi*2,20)); 
clf() 

plot3d(xx,yy,zz)
e=gce();
e.color_flag=1;
scf(2);
plot3d1(xx,yy,zz) // the 2 graphics are similar

xdel(winsid())

//====================================================
// ../man/fr/graphics/plot3d2.xml
//====================================================
clear;lines(0);

u = linspace(-%pi/2,%pi/2,40);
v = linspace(0,2*%pi,20);
X = cos(u)'*cos(v);
Y = cos(u)'*sin(v);
Z = sin(u)'*ones(v);
plot3d2(X,Y,Z);
 // New Graphic mode only 
 e=gce();
e.color_mode=4; // change color
f=e.data;
TL = tlist(["3d" "x" "y" "z" "color"],f.x,f.y,f.z,10*(f.z)+1);
e.data=TL;
xdel(winsid())

//====================================================
// ../man/fr/graphics/plot3d3.xml
//====================================================
clear;lines(0);

u = linspace(-%pi/2,%pi/2,40);
v = linspace(0,2*%pi,20);
X = cos(u)'*cos(v);
Y = cos(u)'*sin(v);
Z = sin(u)'*ones(v);
plot3d3(X,Y,Z);
 // New Graphic mode only  
e=gce(); // get the current entity
e.visible='off';
e.visible='on'; // back to the mesh view


xdel(winsid())

//====================================================
// ../man/fr/graphics/plot3d_old_version.xml
//====================================================
clear;lines(0);

// simple plot using z=f(x,y)
t=[0:0.3:2*%pi]'; z=sin(t)*cos(t');
plot3d(t,t,z)
// same plot using facets computed by genfac3d
[xx,yy,zz]=genfac3d(t,t,z);
xbasc()
plot3d(xx,yy,zz)
// multiple plots
xbasc()
plot3d([xx xx],[yy yy],[zz 4+zz])
// multiple plots using colors
xbasc()
plot3d([xx xx],[yy yy],list([zz zz+4],[4*ones(1,400) 5*ones(1,400)]))
// simple plot with viewpoint and captions
xbasc()
plot3d(1:10,1:20,10*rand(10,20),35,45,"X@Y@Z",[2,2,3])
// plot of a sphere using facets computed by eval3dp
deff("[x,y,z]=sph(alp,tet)",["x=r*cos(alp).*cos(tet)+orig(1)*ones(tet)";..
  "y=r*cos(alp).*sin(tet)+orig(2)*ones(tet)";..
  "z=r*sin(alp)+orig(3)*ones(tet)"]);
r=1; orig=[0 0 0];
[xx,yy,zz]=eval3dp(sph,linspace(-%pi/2,%pi/2,40),linspace(0,%pi*2,20));
xbasc();plot3d(xx,yy,zz)

xbasc();xset('colormap',hotcolormap(128));
r=0.3;orig=[1.5 0 0];
[xx1,yy1,zz1]=eval3dp(sph,linspace(-%pi/2,%pi/2,40),linspace(0,%pi*2,20));
cc=(xx+zz+2)*32;cc1=(xx1-orig(1)+zz1/r+2)*32;   
xbasc();plot3d1([xx xx1],[yy yy1],list([zz,zz1],[cc cc1]),70,80)

xbasc();plot3d1([xx xx1],[yy yy1],list([zz,zz1],[cc cc1]),theta=70,alpha=80,flag=[5,6,3])
 
xdel(winsid())

//====================================================
// ../man/fr/graphics/plot3d.xml
//====================================================
clear;lines(0);
// simple plot using z=f(x,y) 
t=[0:0.3:2*%pi]';
z=sin(t)*cos(t');
plot3d(t,t,z) 
// same plot using facets computed by genfac3d
[xx,yy,zz]=genfac3d(t,t,z); 
clf() 
plot3d(xx,yy,zz)
// multiple plots 
clf()
plot3d([xx xx],[yy yy],[zz 4+zz]) 
// multiple plots using colors 
clf()
plot3d([xx xx],[yy yy],list([zz zz+4],[4*ones(1,400) 5*ones(1,400)])) 
// simple plot with viewpoint and captions 
clf() 
plot3d(1:10,1:20,10*rand(10,20),alpha=35,theta=45,flag=[2,2,3]) 
// plot of a sphere using facets computed by eval3dp 
deff("[x,y,z]=sph(alp,tet)",["x=r*cos(alp).*cos(tet)+orig(1)*ones(tet)";.. 
 "y=r*cos(alp).*sin(tet)+orig(2)*ones(tet)";.. 
 "z=r*sin(alp)+orig(3)*ones(tet)"]); 
r=1; orig=[0 0 0]; 
[xx,yy,zz]=eval3dp(sph,linspace(-%pi/2,%pi/2,40),linspace(0,%pi*2,20)); 
clf();plot3d(xx,yy,zz) 
clf();
f=gcf();
f.color_map = hotcolormap(128); 
r=0.3;orig=[1.5 0 0]; 
[xx1,yy1,zz1]=eval3dp(sph,linspace(-%pi/2,%pi/2,40),linspace(0,%pi*2,20)); 
cc=(xx+zz+2)*32;cc1=(xx1-orig(1)+zz1/r+2)*32; 
clf();plot3d1([xx xx1],[yy yy1],list([zz,zz1],[cc cc1]),theta=70,alpha=80,flag=[5,6,3])

//Available operations using only New Graphics...
delete(gcf());
t=[0:0.3:2*%pi]'; z=sin(t)*cos(t');
[xx,yy,zz]=genfac3d(t,t,z);
plot3d([xx xx],[yy yy],list([zz zz+4],[4*ones(1,400) 5*ones(1,400)]))
e=gce();
f=e.data;
TL = tlist(["3d" "x" "y" "z" "color"],f.x,f.y,f.z,6*rand(f.z)); // random color matrix
e.data = TL;
TL2 = tlist(["3d" "x" "y" "z" "color"],f.x,f.y,f.z,4*rand(1,800)); // random color vector
e.data = TL2;
TL3 = tlist(["3d" "x" "y" "z" "color"],f.x,f.y,f.z,[20*ones(1,400) 6*ones(1,400)]);
e.data = TL3;
TL4 = tlist(["3d" "x" "y" "z"],f.x,f.y,f.z); // no color
e.data = TL4;
e.color_flag=1 // color index proportional to altitude (z coord.)
e.color_flag=2; // back to default mode
e.color_flag= 3; // interpolated shading mode (based on blue default color)
clf()
plot3d([xx xx],[yy yy],list([zz zz+4],[4*ones(1,400) 5*ones(1,400)]))
h=gce(); //get handle on current entity (here the surface)
a=gca(); //get current axes
a.rotation_angles=[40,70];
a.grid=[1 1 1]; //make grids
a.data_bounds=[-6,6;6,-1;0,5];
a.axes_visible="off"; //axes are hidden
a.axes_bounds=[.2 0 1 1];
h.color_flag=1; //color according to z
h.color_mode=-2;  //remove the facets boundary by setting color_mode to white color
h.color_flag=2; //color according to given colors
h.color_mode = -1; // put the facets boundary back by setting color_mode to black color
f=gcf();//get the handle of the parent figure    
f.color_map=hotcolormap(512);
c=[1:400,1:400];
TL.color = [c;c+1;c+2;c+3];
h.data = TL;
h.color_flag=3; // interpolated shading mode

xdel(winsid())

//====================================================
// ../man/fr/graphics/plotframe.xml
//====================================================
clear;lines(0);
    x=[-0.3:0.8:27.3]';
    y=rand(x);
    rect=[min(x),min(y),max(x),max(y)];
    tics=[4,10,2,5];    // 4 x-intervalles et 2 y-intervalles
    plotframe(rect,tics,[%f,%f],["My plot","x","y"],[0,0,0.5,0.5])
    plot2d(x,y,2,"000")
    plotframe(rect,tics,[%t,%f],["My plot avec grille","x","y"],[0.5,0,0.5,0.5])
    plot2d(x,y,3,"000")
    plotframe(rect,tics,[%t,%t],..
    ["plot avec grille et bornes automatiques","x","y"],[0,0.5,0.5,0.5])
    plot2d(x,y,4,"000")
    plotframe(rect,tics,[%f,%t],..
    ["plot sans grille mais avec bornes automatiques ","x","y"],..
    [0.5,0.5,0.5,0.5])
    plot2d(x,y,5,"000")
    xset("default")
xdel(winsid())

//====================================================
// ../man/fr/graphics/plot.xml
//====================================================
clear;lines(0);

x=0:0.1:2*%pi;
// simple plot
plot(sin(x))
// using captions
xbasc()
plot(x,sin(x),"sin","time","plot of sinus")
// plot 2 functions
xbasc()
plot([sin(x);cos(x)])

xdel(winsid())

//====================================================
// ../man/fr/graphics/plzr.xml
//====================================================
clear;lines(0);

s=poly(0,'s');
n=[1+s   2+3*s+4*s^2        5; 0        1-s             s];
d=[1+3*s   5-s^3           s+1;1+s     1+s+s^2      3*s-1];
h=syslin('c',n./d); 
plzr(h);

xdel(winsid())

//====================================================
// ../man/fr/graphics/polarplot.xml
//====================================================
clear;lines(0);

t= 0:.01:2*%pi;
clf();polarplot(sin(7*t),cos(8*t))

clf();polarplot([sin(7*t') sin(6*t')],[cos(8*t') cos(8*t')],[1,2])

 
xdel(winsid())

//====================================================
// ../man/fr/graphics/polyline_properties.xml
//====================================================
clear;lines(0);

   set("figure_style","new") //create a figure
   a=get("current_axes")//get the handle of the newly created axes
   a.data_bounds=[-2,2;-2,2];

   xpoly(sin(2*%pi*(0:5)/5),cos(2*%pi*(0:5)/5),"lines",0)
   p=get("hdl"); //get handle on current entity (here the polyline entity)
   p.foreground=2;
   p.thickness=3;
   p.mark_style=9;
   d=p.data;d(1,:)=[0 0];p.data=d;
   a.rotation_angles=[0 45];
   
   p.data=[(-2:0.1:2)' sin((-2:0.1:2)*%pi)']
   p.mark_mode="off";
   p.polyline_style=3;
   p.line_style=4;


xdel(winsid())

//====================================================
// ../man/fr/graphics/printing.xml
//====================================================

//====================================================
// ../man/fr/graphics/rectangle_properties.xml
//====================================================
clear;lines(0);
   set("figure_style","new") //create a figure
   a=get("current_axes");//get the handle of the newly created axes
   a.data_bounds=[-2,2;-2,2];

   xrect(-1,1,2,2)

   r=get("hdl");//get handle on current entity (here the rectangle entity)
   r.type
   r.parent.type
   r.foreground=13;
   r.line_style=2;
   r.fill_mode="on";
   r.clip_box=[-1 1;1 1];
   r.data(:,[3 4])=[1/2 1/2];
   r.data(:,[1 2])=[1/2 1/2];
   r.clip_state="off"
      
xdel(winsid())

//====================================================
// ../man/fr/graphics/replot.xml
//====================================================
clear;lines(0);
x=[0:0.1:2*%pi]';
plot2d(x,sin(x))
replot([-1,-1,10,2])
xdel(winsid())

//====================================================
// ../man/fr/graphics/rgb2name.xml
//====================================================
clear;lines(0);

rgb2name(255,128,128)
rgb2name([255 215 0])
// get color #10 of the current colormap and find its name
cmap=get(gcf(),"color_map");
rgb2name(cmap(10,:)*255)

xdel(winsid())

//====================================================
// ../man/fr/graphics/rotate.xml
//====================================================
clear;lines(0);
xsetech([0,0,1,1],[-1,-1,1,1])
xy=[(0:0.1:10);sin(0:0.1:10)]/10;
for i=2*%pi*(0:10)/10,
  [xy1]=rotate(xy,i);
  xpoly(xy1(1,:),xy1(2,:),"lines")
end
xdel(winsid())

//====================================================
// ../man/fr/graphics/rubberbox.xml
//====================================================
clear;lines(0);

xsetech(frect=[0,0,100,100])
[x,y]=xclick();r=rubberbox([x;y;30;10])
xrect(r)
r=rubberbox()

xdel(winsid())

//====================================================
// ../man/fr/graphics/scaling.xml
//====================================================

//====================================================
// ../man/fr/graphics/scf.xml
//====================================================
clear;lines(0);
   f4=scf(4); //creates figure with id==4 and make it the current one
   f0=scf(0); //creates figure with id==0 and make it the current one
   plot2d() //draw in current figure (id=0)
   scf(f4); // set first created figure as current one
   plot3d() //draw in current figure (id=4)
xdel(winsid())

//====================================================
// ../man/fr/graphics/sd2sci.xml
//====================================================

//====================================================
// ../man/fr/graphics/sda.xml
//====================================================
clear;lines(0);
   
  x=[0:0.1:2*%pi]';
  set old_style off
  f=get("default_figure"); // get the handle of the model figure 
  a=get("default_axes"); // get the handle of the model axes 
    // setting its' properties
  f.figure_size=[1200 900];
  f.figure_position=[0 0]; 
  a.background=4; 
  a.box="off";
  a. tics_color=5;
  a.labels_font_color=25;
  a.labels_font_size=4;
  a.sub_tics=[7 3];
  a.x_location="middle";
  a.y_location="middle";
  a.tight_limits="on";
  a.thickness=2;
  a.grid=[-1 24];
  subplot(221);
  plot2d(x-2,sin(x))
  subplot(222);
  plot2d(x-6,[2*cos(x)+.7 2*cos(x)+.9 cos(2*x) .2+sin(3*x)],[-1,-2,-3 -4])
  sda() // return to the  default values of the axes' model
  subplot(223);
  plot2d(x-2,sin(x))
  subplot(224);
  plot2d(x-6,[2*cos(x)+.7 2*cos(x)+.9 cos(2*x) .2+sin(3*x)],[-1,-2,-3 -4])
  xdel(0)
  plot2d(x-2,sin(x))
  

xdel(winsid())

//====================================================
// ../man/fr/graphics/sdf.xml
//====================================================
clear;lines(0);
   
   x=[0:0.1:2*%pi]';
   set old_style off
   f=get("default_figure"); // get the handle of the model figure 
   a=get("default_axes"); // get the handle of the model axes 
     // setting its' properties
   f.background=4;
   f.auto_resize="off";
   f.figure_size=[400 300];
   f.axes_size=[600 400];
   f.figure_position=[0 -1];
   a.x_location="top";
   a.y_location="left";
   for (i=1:6)
     xset("window",i) // create a figure with the identifier i
     plot2d(x,[sin(x) cos(x)],[i -i])
     xclick();
     if i == 4, sdf(); end // return to the  default values of the figure's model
    end
    
xdel(winsid())

//====================================================
// ../man/fr/graphics/secto3d.xml
//====================================================

//====================================================
// ../man/fr/graphics/segs_properties.xml
//====================================================
clear;lines(0);

  set("figure_style","new") //create a figure
   a=get("current_axes");//get the handle of the newly created axes
   a.data_bounds=[-10,-10;10,10];
   x=2*%pi*(0:7)/8;
   xv=[2*sin(x);9*sin(x)];
   yv=[2*cos(x);9*cos(x)];
   xsegs(xv,yv,1:8)

   s=a.children
   s.arrow_size=1;
    s.segs_color=15:22;
    for j=1:2
      for i=1:8
        h=s.data(i*2,j);
        s.data(i*2,j)=s.data(i*2-1,j);
        s.data(i*2-1,j)=  h;
       end
    end

   s.segs_color=5; //set all the colors to 5

   s.clip_box=[-4,4,8,8];
   a.thickness=4;
   xrect(s.clip_box);

xdel(winsid())

//====================================================
// ../man/fr/graphics/set.xml
//====================================================
clear;lines(0);


   clf()
   set("figure_style","new") //create a figure
   set("auto_clear","off") ;
   // Exemple of a Plot 2D
   x=[-.2:0.1:2*%pi]';
   plot2d(x-.3,[sin(x-1) cos(2*x)],[1 2] );
   a=get("current_axes");
   p1=a.children.children(1);
   p2=a.children.children(2);
   // set the named properties to the specified values on the objects
   set(p2,"foreground",13);
   set(p2,"polyline_style",2);
   set(a,'tight_limits',"on");
   set(a,"box","off");
   set(a,"sub_tics",[ 7 0 ]);
   set(a,"y_location","middle")
   set(p2,'thickness',2);
   set(p1,'mark_mode',"on");
   set(p1,'mark_style',3);
   plot2d(x-2,x.^2/20);
   p3= a.children(1).children;
   set([a p1 p2 p3],"foreground",5)

 
xdel(winsid())

//====================================================
// ../man/fr/graphics/Sfgrayplot.xml
//====================================================
clear;lines(0);
t=-1:0.1:1;
deff("[z]=surf(x,y)","z=x**2+y**2")
Sfgrayplot(t,t,surf,"111",[-2,-2,2,2])
xdel(winsid())

//====================================================
// ../man/fr/graphics/Sgrayplot.xml
//====================================================
clear;lines(0);

x=-10:10; y=-10:10;m =rand(21,21);
Sgrayplot(x,y,m,"111",[-20,-20,20,20])
t=-%pi:0.1:%pi; m=sin(t)'*cos(t);
clf()
Sgrayplot(t,t,m)
 
xdel(winsid())

//====================================================
// ../man/fr/graphics/sgrid.xml
//====================================================
clear;lines(0);

H=syslin('c',352*poly(-5,'s')/poly([0,0,2000,200,25,1],'s','c'));
evans(H,100)
sgrid()
sgrid(0.6,2,7)

xdel(winsid())

//====================================================
// ../man/fr/graphics/show_pixmap.xml
//====================================================
clear;lines(0);
   set figure_style new
   f=gcf();f.pixmap='on'; //set the pixmap mode
   a=gca();a.data_bounds=[0 0; 10 10];
   //construct two rectangles
   xrects([0;10;1;1],5);r1=gce();r1=r1.children;
   xrects([0;1;1;1],13);r2=gce();r2=r2.children;
   //animation loop
   for k=1:1000
      //draw the rectangles in the pixmap buffer
      move(r1,[0.01,-0.01]);move(r2,[0.01,0.01]) 
      //show the pixmap buffer
      show_pixmap()
   end
xdel(winsid())

//====================================================
// ../man/fr/graphics/square.xml
//====================================================
clear;lines(0);
t=[0:0.1:2*%pi]';
plot2d(sin(t),cos(t))
xbasc()
square(-1,-1,1,1)
plot2d(sin(t),cos(t))
xset("default")
xdel(winsid())

//====================================================
// ../man/fr/graphics/subplot.xml
//====================================================
clear;lines(0);
subplot(221)
plot2d()
subplot(222)
plot3d()
subplot(2,2,3)
param3d()
subplot(2,2,4)
hist3d()
xdel(winsid())

//====================================================
// ../man/fr/graphics/surface_properties.xml
//====================================================
clear;lines(0);
//create a figure 
t=[0:0.3:2*%pi]'; z=sin(t)*cos(t');
[xx,yy,zz]=genfac3d(t,t,z); 
plot3d([xx xx],[yy yy],list([zz zz+4],[4*ones(1,400) 5*ones(1,400)])) 
h=get("hdl") //get handle on current entity (here the surface) 
a=gca(); //get current axes
a.rotation_angles=[40,70]; 
a.grid=[1 1 1]; 
//make grids 
a.data_bounds=[-6,6;6,-1;0,5]; 
a.axes_visible="off"; 
//axes are hidden a.axes_bounds=[.2 0 1 1]; 
f=get("current_figure");
//get the handle of the parent figure 
f.color_map=hotcolormap(64); 
//change the figure colormap 
h.color_flag=1; 
//color according to z 
h.color_mode=-2; 
//remove the facets boundary 
h.color_flag=2; 
//color according to given colors 
h.data.color=[1+modulo(1:400,64),1+modulo(1:400,64)];
//shaded
h.color_flag=3; 

scf(2); // creates second window
plot3d([xx xx],[yy yy],list([zz zz+4],[4*ones(1,400) 5*ones(1,400)]))
e=gce();
f=e.data;
TL = tlist(["3d" "x" "y" "z" "color"],f.x,f.y,f.z,6*rand(f.z)); // random color matrix
e.data = TL;
TL2 = tlist(["3d" "x" "y" "z" "color"],f.x,f.y,f.z,4*rand(1,800)); // random color vector
e.data = TL2;
TL3 = tlist(["3d" "x" "y" "z" "color"],f.x,f.y,f.z,[20*ones(1,400) 6*ones(1,400)]);
e.data = TL3;
TL4 = tlist(["3d" "x" "y" "z"],f.x,f.y,f.z); // no color specified
e.data = TL4;
e.color_flag=1 // color index proportional to altitude (z coord.)
e.color_flag=2; // back to default mode
e.color_flag= 3; // interpolated shading mode (based on blue default color because field data.color is not filled)



xdel(winsid())

//====================================================
// ../man/fr/graphics/text_properties.xml
//====================================================
clear;lines(0);

  set("figure_style","new") //create a figure
   a=get("current_axes");
   a.data_bounds=[0,0;1,1];

   xstring(0.5,0.6,"Scilab is not esilaB",0,0)

   t=get("hdl")   //get the handle of the newly created object

   t.foreground=9;
   t.font_size=5;
   t.font_style=5;
   t.text="SCILAB";
   t.font_angle=90;

xdel(winsid())

//====================================================
// ../man/fr/graphics/titlepage.xml
//====================================================

//====================================================
// ../man/fr/graphics/title_properties.xml
//====================================================
clear;lines(0);
   

   set("figure_style","new") //create a figure
   a=get("current_axes"); 
   a.data_bounds=[-2,-4;2,4];
   a.axes_visible="on"; 
   a.box="off"; 

   xtitle(['Titre';'Principal'],'x','y');
   t=a.title;
   t.text //display text on console
   t.font_size=4;
   t.text="A brand new@title string"  
   t.font_style=5;
   a.x_location="middle"; 
   a.y_location="right";
  

 
xdel(winsid())

//====================================================
// ../man/fr/graphics/twinkle.xml
//====================================================
clear;lines(0);

x=linspace(-2*%pi,2*%pi,100)';
plot2d(x,[sin(x),cos(x)]);
e=gce(); p1=e.children(1); p2=e.children(2);
// cos plot twinkle
twinkle(p1)
// sin plot twinkle 10 times
twinkle(p2,10)
// axes twinkle
twinkle(gca())

xdel(winsid())

//====================================================
// ../man/fr/graphics/unglue.xml
//====================================================

//====================================================
// ../man/fr/graphics/winsid.xml
//====================================================

//====================================================
// ../man/fr/graphics/xarcs.xml
//====================================================
clear;lines(0);
plot2d(0,0,-1,"031"," ",[-1,-1,1,1])
arcs=[-1.0 0.0 0.5; // x du point en haut à gauche
       1.0 0.0 0.5; // y du point en haut à gauche
       0.5 1.0 0.5; // largeur
       0.5 0.5 1.0; // hauteur
       0.0 0.0 0.0; // angle 1
       180*64 360*64 90*64]; // angle 2
xarcs(arcs,[1,2,3])
xdel(winsid())

//====================================================
// ../man/fr/graphics/xarc.xml
//====================================================
clear;lines(0);
// échelle isométrique 
plot2d(0,0,-1,"031"," ",[-2,-2,2,2])
xset("color",3)
xarc(-1,1,2,2,0,90*64)
xarc(-1.5,1.5,3,3,0,360*64)
xdel(winsid())

//====================================================
// ../man/fr/graphics/xarrows.xml
//====================================================
clear;lines(0);
x=2*%pi*(0:9)/8;
x1=[sin(x);9*sin(x)];
y1=[cos(x);9*cos(x)];
plot2d([-10,10],[-10,10],[-1,-1],"022")
xset("clipgrf")
xarrows(x1,y1,1,1:10)
xset("clipoff")
xdel(winsid())

//====================================================
// ../man/fr/graphics/xaxis.xml
//====================================================
clear;lines(0);
x=[-%pi:0.1:%pi]';
// dessin avec axe
plot2d(x,sin(x),1,"010"," ",[-4 -1 4 1])
// l'axe des x
xpoly([-4 4],[0 0],"lines")
xaxis(0,[2 2],[2 0.1 3],[-4 0])
xstring(-4.1,-0.25,"-4"); xstring(-0.2,-0.1,"0"); xstring(4,-0.25,"4")
//  l'axe des y
xpoly([0 0],[-1 1],"lines")
xaxis(90,[2 2],[0.5 0.025 3],[0 1])
xstring(-0.5,-1.05,"-1"); xstring(-0.35,0.95,"1")
xdel(winsid())

//====================================================
// ../man/fr/graphics/xbasc.xml
//====================================================

//====================================================
// ../man/fr/graphics/xbasimp.xml
//====================================================

//====================================================
// ../man/fr/graphics/xbasr.xml
//====================================================

//====================================================
// ../man/fr/graphics/xchange.xml
//====================================================
clear;lines(0);
t=[0:0.1:2*%pi]';
plot2d(t,sin(t))
[x,y,rect]=xchange(1,1,"f2i")
[x,y,rect]=xchange(0,0,"i2f")
xdel(winsid())

//====================================================
// ../man/fr/graphics/xclear.xml
//====================================================

//====================================================
// ../man/fr/graphics/xclea.xml
//====================================================
clear;lines(0);
x=[0:0.1:2*%pi]';
plot2d(x,sin(x))
xclea(1,1,1,1)
xdel(winsid())

//====================================================
// ../man/fr/graphics/xclick.xml
//====================================================

//====================================================
// ../man/fr/graphics/xclip.xml
//====================================================
clear;lines(0);
x=0:0.2:2*%pi;
x1=[sin(x);100*sin(x)];
y1=[cos(x);100*cos(x)];
y1=y1+20*ones(y1);
// No clip 
plot2d([-100,500],[-100,600],[-1,-1],"022")
xsegs(10*x1+200*ones(x1),10*y1+200*ones(y1))
// clipping défini par un rectangle 
xbasc(); plot2d([-100,500],[-100,600],[-1,-1],"022")
xrect(150,460,100,150)
xclip(150,460,100,150)
xsegs(10*x1+200*ones(x1),10*y1+200*ones(y1))
// rectangle de clipping = cadre du dessin
xbasc(); plot2d([-100,500],[-100,600],[-1,-1],"022")
xclip("clipgrf")
xsegs(10*x1+200*ones(x1),10*y1+200*ones(y1));
// fin du clipping
xclip()
xdel(winsid())

//====================================================
// ../man/fr/graphics/xdel.xml
//====================================================

//====================================================
// ../man/fr/graphics/xend.xml
//====================================================
clear;lines(0);
driver("Pos")
xinit("foo.ps")
plot2d()
xend()
driver("X11")
xdel(winsid())

//====================================================
// ../man/fr/graphics/xfarcs.xml
//====================================================
clear;lines(0);
plot2d(0,0,-1,"031"," ",[-1,-1,1,1])
arcs=[-1.0 0.0 0.5; // abscisse du coin superieur gauche
       1.0 0.0 0.5; // ordonnée du coin superieur gauche
       0.5 1.0 0.5; // longueur
       0.5 0.5 1.0; // hauteur
       0.0 0.0 0.0; // angle 1
       180*64 360*64 90*64]; // angle 2
xfarcs(arcs,[1,2,3])
xdel(winsid())

//====================================================
// ../man/fr/graphics/xfarc.xml
//====================================================
clear;lines(0);
// échelle isométrique
plot2d(0,0,-1,"031"," ",[-2,-2,2,2])
xfarc(-0.5,0.5,1,1,0,90*64)
xset("color",2)
xfarc(0.5,0.5,1,1,0,360*64)
xdel(winsid())

//====================================================
// ../man/fr/graphics/xfpolys.xml
//====================================================
clear;lines(0);
    plot2d(0,0,[-1],"012"," ",[0,-10,210,40])
    x1=[0,10,20,30,20,10,0]';
    y1=[15,30,30,15,0,0,15]';
    xpols=[x1 x1 x1 x1]; xpols=xpols+[0,60,120,180].*.ones(x1);
    ypols=[y1 y1 y1 y1];
    // choix de la couleur
    xset("color",5)
    xfpolys(xpols,ypols,[-1,0,1,2])
    xset("default")
xdel(winsid())

//====================================================
// ../man/fr/graphics/xfpoly.xml
//====================================================
clear;lines(0);
x=sin(2*%pi*(0:5)/5);
y=cos(2*%pi*(0:5)/5);
plot2d(0,0,-1,"010"," ",[-2,-2,2,2])
xset("color",5)
xfpoly(x,y)
xset("default")
xdel(winsid())

//====================================================
// ../man/fr/graphics/xfrect.xml
//====================================================
clear;lines(0);
plot2d(0,0,-1,"010"," ",[-2,-2,2,2])
xset("color",5)
xfrect(-1,1,2,2)
xset("default")
xdel(winsid())

//====================================================
// ../man/fr/graphics/xgetech.xml
//====================================================
clear;lines(0);
// première sous-fenêtre
xsetech([0,0,1.0,0.5])
plot2d()
// deuxième sous-fenêtre activée 
xsetech([0,0.5,1.0,0.5])
grayplot()
// recupération de l'échelle 
xsetech([0,0,1.0,0.5])
[wrect,frect,logflag,arect]=xgetech();
// recupération de l'échelle 
xsetech([0,0.5,1.0,0.5])
[wrect,frect,logflag,arect]=xgetech();
xbasc();
xdel(winsid())

//====================================================
// ../man/fr/graphics/xgetmouse.xml
//====================================================
clear;lines(0);
    xselect(); xbasc(); xsetech([0 0 1 1],[0 0 100 100])
    xset("alufunction",6)
    xtitle(" On dessine un rectangle ")
    [b,x0,y0]=xclick(); rep=[x0,y0,-1]; x=x0; y=y0;
    xrect(x0,y0,x-x0,y-y0)
    while rep(3)==-1 then
    rep=xgetmouse(0)
    xrect(x0,y0,x-x0,y0-y)
    x=rep(1); y=rep(2);
    xrect(x0,y0,x-x0,y0-y)
    end
    xset("alufunction",3)
xdel(winsid())

//====================================================
// ../man/fr/graphics/xget.xml
//====================================================

//====================================================
// ../man/fr/graphics/xgraduate.xml
//====================================================
clear;lines(0);
  [x1,xa,np1,np2,kMinr,kMaxr,ar]=xgraduate(-0.3,0.2)
xdel(winsid())

//====================================================
// ../man/fr/graphics/xgrid.xml
//====================================================
clear;lines(0);
x=[0:0.1:2*%pi]';
plot2d(sin(x))
xgrid(2)
xdel(winsid())

//====================================================
// ../man/fr/graphics/xinfo.xml
//====================================================

//====================================================
// ../man/fr/graphics/xinit.xml
//====================================================
clear;lines(0);
driver("Pos")
xinit("foo.ps")
plot2d()
xend()
driver("X11")
xdel(winsid())

//====================================================
// ../man/fr/graphics/xlfont.xml
//====================================================
clear;lines(0);

// Caution : this example may not work if your system have not
//           the schoolbook bold font 
if MSDOS then
   xlfont("Century Schoolbook Bold",10)
else
   xlfont("-adobe-new century schoolbook-bold-r-normal-*-%s-*-75-75-*-*-iso8859-1",10)
end
xbasc()
xset("font", 6, 2)  // use helvetica at 12 pts
plot2d()
xset("font", 10, 4) // use Schoolbook bold at 18 pts
xtitle("plot2d demo","x","y")

xdel(winsid())

//====================================================
// ../man/fr/graphics/xload.xml
//====================================================

//====================================================
// ../man/fr/graphics/xname.xml
//====================================================

//====================================================
// ../man/fr/graphics/xnumb.xml
//====================================================
clear;lines(0);
plot2d([-100,500],[-100,600],[-1,-1],"022")
x=0:100:200;
xnumb(x,500*ones(x),[10,20,35],1)
xdel(winsid())

//====================================================
// ../man/fr/graphics/xpause.xml
//====================================================

//====================================================
// ../man/fr/graphics/xpolys.xml
//====================================================
clear;lines(0);
    plot2d(0,0,-1,"012"," ",[0,0,1,1])
    rand("uniform")
    xset("color",3)
    xpolys(rand(3,5),rand(3,5),[-1,-2,0,1,2])
    xset("default")
xdel(winsid())

//====================================================
// ../man/fr/graphics/xpoly.xml
//====================================================
clear;lines(0);
x=sin(2*%pi*(0:5)/5);
y=cos(2*%pi*(0:5)/5);
plot2d(0,0,-1,"010"," ",[-2,-2,2,2])
xset("color",5)
xpoly(x,y,"lines",1)
xset("default")
xdel(winsid())

//====================================================
// ../man/fr/graphics/xrects.xml
//====================================================
clear;lines(0);
plot2d([-100,500],[-50,50],[-1,-1],"022")
cols=[-34,-33,-32,-20:5:20,32,33,34];
x=400*(0:14)/14; step=20;
rects=[x;10*ones(x);step*ones(x);30*ones(x)];
xrects(rects,cols)
xnumb(x,15*ones(x),cols)
xdel(winsid())

//====================================================
// ../man/fr/graphics/xrect.xml
//====================================================
clear;lines(0);
plot2d(0,0,-1,"010"," ",[-2,-2,2,2])
xset("color",5)
xrect(-1,1,2,2)
xset("default")
xdel(winsid())

//====================================================
// ../man/fr/graphics/xrpoly.xml
//====================================================
clear;lines(0);
plot2d(0,0,-1,"012"," ",[0,0,10,10])
xrpoly([5,5],5,5)
xdel(winsid())

//====================================================
// ../man/fr/graphics/xs2fig.xml
//====================================================

//====================================================
// ../man/fr/graphics/xs2gif.xml
//====================================================

//====================================================
// ../man/fr/graphics/xs2ppm.xml
//====================================================

//====================================================
// ../man/fr/graphics/xs2ps.xml
//====================================================

//====================================================
// ../man/fr/graphics/xsave.xml
//====================================================

//====================================================
// ../man/fr/graphics/xsegs.xml
//====================================================
clear;lines(0);
x=2*%pi*(0:9)/8;
xv=[sin(x);9*sin(x)];
yv=[cos(x);9*cos(x)];
plot2d([-10,10],[-10,10],[-1,-1],"022")
xsegs(xv,yv,1:10)
xdel(winsid())

//====================================================
// ../man/fr/graphics/xselect.xml
//====================================================

//====================================================
// ../man/fr/graphics/xsetech.xml
//====================================================
clear;lines(0);
// pour avoir une explication des paramètres de xsetech() taper :
exec('SCI/demos/graphics/xsetechfig.sce');


// On coupe la fenêtre en deux 
// D'abord on choisit la première sous-fenêtre 
// et l'échelle
xsetech([0,0,1.0,0.5],[-5,-3,5,3])
// on appelle plot2d avec l'option "001" pour utiliser l'échelle  
// choisie par xsetech
plot2d([1:10]',[1:10]',1,"001"," ")
// on choisit la deuxième sous-fenêtre 
xsetech([0,0.5,1.0,0.5])
// l'échelle est [0,0,1,1] par défaut 
// on la change avec l'argument rect de plot2d 
plot2d([1:10]',[1:10]',1,"011"," ",[-6,-6,6,6])
// 4 dessins sur une seule fenêtre
xbasc()
xset("font",2,0)
xsetech([0,0,0.5,0.5]); plot3d()
xsetech([0.5,0,0.5,0.5]); plot2d()
xsetech([0.5,0.5,0.5,0.5]); grayplot()
xsetech([0,0.5,0.5,0.5]); histplot()
// retour aux valeurs par défaut 
xsetech([0,0,1,1])
// Un dessin avec arect change
xbasc()
xset("default")
xsetech(arect=[0,0,0,0]) 
x=1:0.1:10;plot2d(x',sin(x)')
xbasc()
xsetech(arect=[1/8,1/8,1/16,1/4])
x=1:0.1:10;plot2d(x',sin(x)')
xbasc()
xset("default")
xdel(winsid())

//====================================================
// ../man/fr/graphics/xsetm.xml
//====================================================

//====================================================
// ../man/fr/graphics/xset.xml
//====================================================

//====================================================
// ../man/fr/graphics/xstringb.xml
//====================================================
clear;lines(0);
str=["Scilab" "n''est ";"pas" "Matlab"];
plot2d(0,0,[-1,1],"010"," ",[0,0,1,1]);
r=[0,0,1,0.5];
xstringb(r(1),r(2),str,r(3),r(4),"fill");
xrect(r(1),r(2)+r(4),r(3),r(4))
r=[r(1),r(2)+r(4)+0.01,r(3),r(4)/2];
xrect(r(1),r(2)+r(4),r(3),r(4))
xstringb(r(1),r(2),str,r(3),r(4),"fill");
r=[r(1),r(2)+r(4)+0.01,r(3),r(4)/2];
xrect(r(1),r(2)+r(4),r(3),r(4))
xstringb(r(1),r(2),str,r(3),r(4),"fill");
xdel(winsid())

//====================================================
// ../man/fr/graphics/xstringl.xml
//====================================================
clear;lines(0);
plot2d([0;1],[0;1],0)
str=["Scilab" "n''est ";"pas" "Matlab"];
r=xstringl(0.5,0.5,str)
xrects([r(1) r(2)+r(4) r(3) r(4)]')
xstring(r(1),r(2),str)
xdel(winsid())

//====================================================
// ../man/fr/graphics/xstring.xml
//====================================================
clear;lines(0);
plot2d([0;1],[0;1],0)
xstring(0.5,0.5,["Scilab" "n''est ";"pas" "Matlab"])
// autre exemple
alphabet=["a" "b" "c" "d" "e" "f" "g" ..
          "h" "i" "j" "k" "l" "m" "n" ..
          "o" "p" "q" "r" "s" "t" "u" ..
          "v" "w" "x" "y" "z"];
xbasc()
plot2d([0;1],[0;2],0)
xstring(0.1,1.8,alphabet)     // alphabet
xstring(0.1,1.6,alphabet,0,1) // alphabet dans une boîte
xstring(0.1,1.4,alphabet,20)  // angle
xset("font",1,1)              // police "symbol"
xstring(0.1,0.1,alphabet)
xset("font",1,3)              // changement de la taille de police
xstring(0.1,0.3,alphabet)
xset("font",1,24); xstring(0.1,0.6,"a") // un grand alpha
xset("default")
xdel(winsid())

//====================================================
// ../man/fr/graphics/xtape.xml
//====================================================

//====================================================
// ../man/fr/graphics/xtitle.xml
//====================================================

//====================================================
// ../man/fr/graphics/zgrid.xml
//====================================================

//====================================================
// ../man/fr/gui/addmenu.xml
//====================================================
clear;lines(0);

addmenu('foo')
foo='disp(''hello'')'

addmenu('Hello',['Franck';'Peter'])
Hello=['disp(''hello Franck'')';'disp(''hello Peter'')']

addmenu(0,'Hello',['Franck';'Peter'])
Hello_0=['disp(''hello Franck'')';'disp(''hello Peter'')']

addmenu('Bye',list(0,'French_Bye'))
French_Bye='disp(''Au revoir'')'

//C defined Callback
// creating Callback code
code=[ '#include ""machine.h""'
 'void foo(char *name,int *win,int *entry)'
 '{'
 '  if (*win==-1) '
 '    sciprint(""menu %s(%i) in Scilab window selected\r\n"",name,*entry+1);'
 '  else'
 '    sciprint(""menu %s(%i) in window %i selected\r\n"",name,*entry+1,*win);'
 '}'];
//creating foo.c file
dir=getcwd(); chdir(TMPDIR)
mputl(code,TMPDIR+'/foo.c');
//reating Makefile
ilib_for_link('foo','foo.o',[],'c');
exec('loader.sce');
chdir(dir);
//add menu
addmenu('foo',['a','b','c'],list(1,'foo'))

xdel(winsid())

//====================================================
// ../man/fr/gui/browsevar.xml
//====================================================
clear;lines(0);
browsevar();]]>
xdel(winsid())

//====================================================
// ../man/fr/gui/buttondialog.xml
//====================================================
clear;lines(0);
answ=buttondialog("This is an example","ok");
answ=buttondialog("Do the work?","yes|no|maybe","question");
]]>
xdel(winsid())

//====================================================
// ../man/fr/gui/config.xml
//====================================================

//====================================================
// ../man/fr/gui/delmenu.xml
//====================================================
clear;lines(0);

addmenu('foo')
delmenu('foo')

xdel(winsid())

//====================================================
// ../man/fr/gui/demoplay.xml
//====================================================
clear;lines(0);
demoplay();]]>
xdel(winsid())

//====================================================
// ../man/fr/gui/editvar.xml
//====================================================
clear;lines(0);
a=rand(10,10);
editvar a;
b=['hello';'good bye'];
editvar b;]]>
xdel(winsid())

//====================================================
// ../man/fr/gui/getvalue.xml
//====================================================
clear;lines(0);
    labels=["module";"frequence";"phase    "];
    [ok,mag,freq,ph]=getvalue("signal sinusoidal",labels,...
    list("vec",1,"vec",1,"vec",1),["0.85";"10^2";"%pi/3"])
xdel(winsid())

//====================================================
// ../man/fr/gui/halt.xml
//====================================================

//====================================================
// ../man/fr/gui/havewindow.xml
//====================================================

//====================================================
// ../man/fr/gui/keyboard.xml
//====================================================

//====================================================
// ../man/fr/gui/progressionbar.xml
//====================================================
clear;lines(0);
winId=progressionbar('Do something');
realtimeinit(0.3);
for j=0:0.1:1,
    realtime(3*j);
    progressionbar(winId);
end
winclose(winId);]]>
xdel(winsid())

//====================================================
// ../man/fr/gui/seteventhandler.xml
//====================================================
clear;lines(0);

   function my_eventhandler(win,x,y,ibut)
       if ibut==-1 then 
         [x,y]=xchange(x,y,'i2f')
         xinfo('Mouse position is ('+string(x)+','+string(y)+')')
       end
   endfunction
   plot2d()
   seteventhandler('my_eventhandler')
   //now move the mouse over the graphic window/
   seteventhandler('') //suppress the event handler

xdel(winsid())

//====================================================
// ../man/fr/gui/setmenu.xml
//====================================================
clear;lines(0);

addmenu('foo')   //New button made in main scilab window
unsetmenu('foo')   //button foo cannot be activated (grey string)
setmenu('foo')     //button foo can be activated (black string)

xdel(winsid())

//====================================================
// ../man/fr/gui/unsetmenu.xml
//====================================================
clear;lines(0);

//addmenu('foo')
//unsetmenu('foo')
//unsetmenu('File',2)

xdel(winsid())

//====================================================
// ../man/fr/gui/waitbar.xml
//====================================================
clear;lines(0);
winId=waitbar('This is an example');
realtimeinit(0.3);
for j=0:0.1:1,
    realtime(3*j);
    waitbar(j,winId);
end
winclose(winId);]]>
xdel(winsid())

//====================================================
// ../man/fr/gui/winclose.xml
//====================================================
clear;lines(0);
//CREATE SOME WINDOWS
win1=waitbar('This is an example');
win2=waitbar('HELLO!');
halt();
winclose([win1,win2]);]]>
xdel(winsid())

//====================================================
// ../man/fr/gui/winlist.xml
//====================================================

//====================================================
// ../man/fr/gui/x_choices.xml
//====================================================
clear;lines(0);
l1=list('choix 1',1,['article c1','article c2','article c3']);
l2=list('choix 2',2,['article d1','article d2','article d3']);
l3=list('choix 3',3,['article e1','article e2']);
rep=x_choices('Menu d''articles',list(l1,l2,l3));
xdel(winsid())

//====================================================
// ../man/fr/gui/x_choose.xml
//====================================================
clear;lines(0);
n=x_choose(['item1';'item2';'item3'],['ceci est un commentaire';'pour la boite de dialogue'])
n=x_choose(['item1';'item2';'item3'],['ceci est un commentaire'],'Terminer')
xdel(winsid())

//====================================================
// ../man/fr/gui/x_dialog.xml
//====================================================
clear;lines(0);
gain=evstr(x_dialog('valeur du gain ?','0.235'))
x_dialog(['Methode';'entrer la periode d''echantillonnage'],'1')
m=evstr(x_dialog('entrer une matrice 3x3',['[0 0 0';'0 0 0';'0 0 0]']))
xdel(winsid())

//====================================================
// ../man/fr/gui/x_matrix.xml
//====================================================
clear;lines(0);

//m=evstr(x_matrix('enter a  3x3 matrix ',rand(3,3)))

xdel(winsid())

//====================================================
// ../man/fr/gui/x_mdialog.xml
//====================================================
clear;lines(0);
 txt=['module';'frequence';'phase    '];
 sig=x_mdialog('choisir le signal sinusoidal',txt,['1';'10';'0'])
 mag=evstr(sig(1))
 frq=evstr(sig(2))
 ph=evstr(sig(3))

 rep=x_mdialog(['Simulation d''un systeme';'avec un regulateur PI'],...
                      ['gain P';'gain I'],[' ';' '])


 n=5;m=4;mat=rand(n,m);
 row='row';labelv=row(ones(1,n))+string(1:n)
 col='col';labelh=col(ones(1,m))+string(1:m)
 new=evstr(x_mdialog('Matrice a editer',labelv,labelh,string(mat)))

xdel(winsid())

//====================================================
// ../man/fr/gui/x_message_modeless.xml
//====================================================
clear;lines(0);
 x_message_modeless(['Ceci est un message non modal'
                     'Scilab peut continuer a travailler'
                     ' '
                     'Cliquer sur ""Ok"" pour fermer le message'])
 x_message_modeless('Voici un deuxieme message')
xdel(winsid())

//====================================================
// ../man/fr/gui/x_message.xml
//====================================================
clear;lines(0);
 gain=0.235;x_message('La valeur du gain est :'+string(gain))
 x_message(['matrice singuliere';'utiliser les moindres carres'])

 r=x_message(['Votre probleme est mal conditionne';
             'continuer ?'],['Oui','Non'])
xdel(winsid())

//====================================================
// ../man/fr/linear/aff2ab.xml
//====================================================
clear;lines(0);

// Lyapunov equation solver (one unknown variable, one constraint)
deff('Y=lyapunov(X,D)','[A,Q]=D(:);Xm=X(:); Y=list(A''*Xm+Xm*A-Q)')
A=rand(3,3);Q=rand(3,3);Q=Q+Q';D=list(A,Q);dimX=[3,3];
[Aly,bly]=aff2ab(lyapunov,dimX,D);
[Xl,kerA]=linsolve(Aly,bly); Xv=vec2list(Xl,dimX); lyapunov(Xv,D)
Xm=Xv(:); A'*Xm+Xm*A-Q

// Lyapunov equation solver with redundant constraint X=X'
// (one variable, two constraints) D is global variable
deff('Y=ly2(X,D)','[A,Q]=D(:);Xm=X(:); Y=list(A''*Xm+Xm*A-Q,Xm''-Xm)')
A=rand(3,3);Q=rand(3,3);Q=Q+Q';D=list(A,Q);dimX=[3,3];
[Aly,bly]=aff2ab(ly2,dimX,D);
[Xl,kerA]=linsolve(Aly,bly); Xv=vec2list(Xl,dimX); ly2(Xv,D)

// Francis equations
// Find matrices X1 and X2 such that:
// A1*X1 - X1*A2 + B*X2 -A3 = 0
// D1*X1 -D2 = 0 
deff('Y=bruce(X,D)','[A1,A2,A3,B,D1,D2]=D(:),...
[X1,X2]=X(:);Y=list(A1*X1-X1*A2+B*X2-A3,D1*X1-D2)')
A1=[-4,10;-1,2];A3=[1;2];B=[0;1];A2=1;D1=[0,1];D2=1;
D=list(A1,A2,A3,B,D1,D2);
[n1,m1]=size(A1);[n2,m2]=size(A2);[n3,m3]=size(B);
dimX=[[m1,n2];[m3,m2]];
[Af,bf]=aff2ab(bruce,dimX,D);
[Xf,KerAf]=linsolve(Af,bf);Xsol=vec2list(Xf,dimX)
bruce(Xsol,D)

// Find all X which commute with A
deff('y=f(X,D)','y=list(D(:)*X(:)-X(:)*D(:))')
A=rand(3,3);dimX=[3,3];[Af,bf]=aff2ab(f,dimX,list(A));
[Xf,KerAf]=linsolve(Af,bf);[p,q]=size(KerAf);
Xsol=vec2list(Xf+KerAf*rand(q,1),dimX);
C=Xsol(:); A*C-C*A

xdel(winsid())

//====================================================
// ../man/fr/linear/balanc.xml
//====================================================
clear;lines(0);

A=[1/2^10,1/2^10;2^10,2^10];
[Ab,X]=balanc(A);
norm(A(1,:))/norm(A(2,:))
norm(Ab(1,:))/norm(Ab(2,:))

xdel(winsid())

//====================================================
// ../man/fr/linear/bdiag.xml
//====================================================
clear;lines(0);
// Cas réel: blocs 1x1 et 2x2
a=rand(5,5);[ab,x,bs]=bdiag(a);ab
// Cas complexe : blocs complexes 1x1
[ab,x,bs]=bdiag(a+%i*0);ab
xdel(winsid())

//====================================================
// ../man/fr/linear/chfact.xml
//====================================================

//====================================================
// ../man/fr/linear/chol.xml
//====================================================
clear;lines(0);
    W=rand(5,5)+%i*rand(5,5);
    X=W*W';
    R=chol(X);
    norm(R'*R-X)
xdel(winsid())

//====================================================
// ../man/fr/linear/chsolve.xml
//====================================================
clear;lines(0);
A=sprand(20,20,0.1);
A=A*A'+eye();  
spcho=chfact(A);
sol=(1:20)';rhs=A*sol;
spcho=chfact(A);
chsolve(spcho,rhs)
xdel(winsid())

//====================================================
// ../man/fr/linear/classmarkov.xml
//====================================================
clear;lines(0);

//P has two recurrent classes (with 2 and 1 states) 2 transient states
P=genmarkov([2,1],2,'perm')
[perm,rec,tr,indsRec,indsT]=classmarkov(P);
P(perm,perm)

xdel(winsid())

//====================================================
// ../man/fr/linear/coff.xml
//====================================================
clear;lines(0);
M=[1,2;0,3];
[N,d]=coff(M)
N/d
inv(%s*eye()-M)
xdel(winsid())

//====================================================
// ../man/fr/linear/colcomp.xml
//====================================================
clear;lines(0);
A=rand(5,2)*rand(2,5);
[X,r]=colcomp(A);
norm(A*X(:,1:$-r),1)
xdel(winsid())

//====================================================
// ../man/fr/linear/companion.xml
//====================================================
clear;lines(0);
s=poly(0,'s');
p=poly([1,2,3,4,1],'s','c')
det(s*eye()-companion(p))
roots(p)
spec(companion(p))
xdel(winsid())

//====================================================
// ../man/fr/linear/cond.xml
//====================================================
clear;lines(0);
A=testmatrix('hilb',6);
cond(A)
xdel(winsid())

//====================================================
// ../man/fr/linear/det.xml
//====================================================
clear;lines(0);
    x=poly(0,'x');
    det([x,1+x;2-x,x^2])
    w=ssrand(2,2,4);roots(det(systmat(w))),trzeros(w)   // zéros du système linéaire
    A=rand(3,3);
    det(A), prod(spec(A))
xdel(winsid())

//====================================================
// ../man/fr/linear/eigenmarkov.xml
//====================================================
clear;lines(0);

//P has two recurrent classes (with 2 and 1 states) 2 transient states
P=genmarkov([2,1],2) 
[M,Q]=eigenmarkov(P);
P*Q-Q
Q*M-P^20

xdel(winsid())

//====================================================
// ../man/fr/linear/ereduc.xml
//====================================================
clear;lines(0);

X=[1 2 3;4 5 6]
[E,Q,Z ,stair ,rk]=ereduc(X,1.d-15)

xdel(winsid())

//====================================================
// ../man/fr/linear/expm.xml
//====================================================
clear;lines(0);
X=[1 2;3 4]
expm(X)
logm(expm(X))    
xdel(winsid())

//====================================================
// ../man/fr/linear/exp.xml
//====================================================
clear;lines(0);
x=[1,2,3+%i];
log(exp(x))  // élément par élément
2^x
exp(x*log(2))

xdel(winsid())

//====================================================
// ../man/fr/linear/fstair.xml
//====================================================

//====================================================
// ../man/fr/linear/fullrfk.xml
//====================================================
clear;lines(0);
A=rand(5,2)*rand(2,5);[Bk,Ck]=fullrfk(A,3);
norm(Bk*Ck-A^3,1)
xdel(winsid())

//====================================================
// ../man/fr/linear/fullrf.xml
//====================================================
clear;lines(0);
A=rand(5,2)*rand(2,5);
[Q,M]=fullrf(A);
norm(Q*M-A,1)
[X,d]=rowcomp(A);Y=X';
svd([A,Y(:,1:d),Q])        // Im(Q) = Im(A) = Im(Y(:,1:2))
xdel(winsid())

//====================================================
// ../man/fr/linear/genmarkov.xml
//====================================================
clear;lines(0);

//P has two recurrent classes (with 2 and 1 states) 2 transient states
P=genmarkov([2,1],2,'perm')
[perm,rec,tr,indsRec,indsT]=classmarkov(P);
P(perm,perm)

xdel(winsid())

//====================================================
// ../man/fr/linear/givens.xml
//====================================================
clear;lines(0);
A=[3,4;5,6];
U=givens(A(:,1));
U*A
xdel(winsid())

//====================================================
// ../man/fr/linear/glever.xml
//====================================================
clear;lines(0);
    s=%s;F=[-1,s,0,0;0,-1,0,0;0,0,s-2,0;0,0,0,s-1];
    [Bfs,Bis,chis]=glever(F)
    inv(F)-((Bfs/chis) - Bis)
xdel(winsid())

//====================================================
// ../man/fr/linear/gschur.xml
//====================================================

//====================================================
// ../man/fr/linear/gspec.xml
//====================================================

//====================================================
// ../man/fr/linear/hess.xml
//====================================================
clear;lines(0);
    A=rand(3,3);[U,H]=hess(A);
    and( abs(U*H*U'-A)<1.d-10 )
xdel(winsid())

//====================================================
// ../man/fr/linear/householder.xml
//====================================================

//====================================================
// ../man/fr/linear/im_inv.xml
//====================================================
clear;lines(0);

A=[rand(2,5);[zeros(3,4),rand(3,1)]];B=[[1,1;1,1];zeros(3,2)];
W=rand(5,5);A=W*A;B=W*B;
[X,dim]=im_inv(A,B)
svd([A*X(:,1:dim),B])   //vectors A*X(:,1:dim) belong to range(B)
[X,dim,Y]=im_inv(A,B);[Y*A*X,Y*B]

xdel(winsid())

//====================================================
// ../man/fr/linear/inv.xml
//====================================================
clear;lines(0);
    A=rand(3,3);inv(A)*A
    //
    x=poly(0,'x');
    A=[x,1,x;x^2,2,1+x;1,2,3];inv(A)*A
    //
    A=[1/x,2;2+x,2/(1+x)]
    inv(A)*A
    //
    A=ssrand(2,2,3);
    W=inv(A)*A
    clean(ss2tf(W))
xdel(winsid())

//====================================================
// ../man/fr/linear/kernel.xml
//====================================================
clear;lines(0);
A=rand(3,1)*rand(1,3);
A*kernel(A)
A=sparse(A);
clean(A*kernel(A))
xdel(winsid())

//====================================================
// ../man/fr/linear/kroneck.xml
//====================================================
clear;lines(0);

F=randpencil([1,1,2],[2,3],[-1,3,1],[0,3]);
Q=rand(17,17);Z=rand(18,18);F=Q*F*Z;
//random pencil with eps1=1,eps2=1,eps3=1; 2 J-blocks @ infty 
//with dimensions 2 and 3
//3 finite eigenvalues at -1,3,1 and eta1=0,eta2=3
[Q,Z,Qd,Zd,numbeps,numbeta]=kroneck(F);
[Qd(1),Zd(1)]    //eps. part is sum(epsi) x (sum(epsi) + number of epsi) 
[Qd(2),Zd(2)]    //infinity part
[Qd(3),Zd(3)]    //finite part
[Qd(4),Zd(4)]    //eta part is (sum(etai) + number(eta1)) x sum(etai)
numbeps
numbeta

xdel(winsid())

//====================================================
// ../man/fr/linear/linsolve.xml
//====================================================
clear;lines(0);
A=rand(5,3)*rand(3,8);
b=A*ones(8,1);[x,kerA]=linsolve(A,b);A*x+b   // b compatible
b=ones(5,1);[x,kerA]=linsolve(A,b);A*x+b   // b incompatible
A=rand(5,5);[x,kerA]=linsolve(A,b), -inv(A)*b  // x est unique
xdel(winsid())

//====================================================
// ../man/fr/linear/lsq.xml
//====================================================
clear;lines(0);

//Build the data
x=(1:10)';

y1=3*x+4.5+3*rand(x,'normal');
y2=1.8*x+0.5+2*rand(x,'normal');
plot2d(x,[y1,y2],[-2,-3])
//Find the linear regression 
A=[x,ones(x)];B=[y1,y2];
X=lsq(A,B);

y1e=X(1,1)*x+X(2,1);
y2e=X(1,2)*x+X(2,2);
plot2d(x,[y1e,y2e],[2,3])

//Difference between lsq(A,b) and A\b
A=rand(4,2)*rand(2,3);//a rank 2 matrix
b=rand(4,1);
X1=lsq(A,b)
X2=A\b
[A*X1-b, A*X2-b] //the residuals are the same
  
xdel(winsid())

//====================================================
// ../man/fr/linear/ludel.xml
//====================================================

//====================================================
// ../man/fr/linear/lufact.xml
//====================================================
clear;lines(0);
a=rand(5,5);b=rand(5,1);A=sparse(a);
[h,rk]=lufact(A);
x=lusolve(h,b);a*x-b
ludel(h)
xdel(winsid())

//====================================================
// ../man/fr/linear/luget.xml
//====================================================
clear;lines(0);
a=rand(5,2)*rand(2,5);A=sparse(a);
[hand,rk]=lufact(A);[P,L,U,Q]=luget(hand);
full(L), P*L*U*Q-A
clean(P*L*U*Q-A)
ludel(hand)
xdel(winsid())

//====================================================
// ../man/fr/linear/lusolve.xml
//====================================================
clear;lines(0);
    non_zeros=[1,2,3,4];rows_cols=[1,1;2,2;3,3;4,4];
    sp=sparse(rows_cols,non_zeros);
    [h,rk]=lufact(sp);x=lusolve(h,[1;1;1;1]);ludel(h)
    rk,sp*x


    non_zeros=[1,2,3,4];rows_cols=[1,1;2,2;3,3;4,4];
    sp=sparse(rows_cols,non_zeros);
    x=lusolve(sp,-ones(4,1));
    sp*x

xdel(winsid())

//====================================================
// ../man/fr/linear/lu.xml
//====================================================
clear;lines(0);
    a=rand(4,4);
    [l,u]=lu(a)
    norm(l*u-a)


    [h,rk]=lufact(sparse(a))  // lufact fonctionne avec des matrices creuses 
    [P,L,U,Q]=luget(h);
    ludel(h)
    P=full(P);L=full(L);U=full(U);Q=full(Q); 
    norm(P*L*U*Q-a) // P,Q sont des matrices de permutation
xdel(winsid())

//====================================================
// ../man/fr/linear/lyap.xml
//====================================================
clear;lines(0);
    A=rand(4,4);C=rand(A);C=C+C';
    X=lyap(A,C,'c');
    A'*X + X*A -C
    X=lyap(A,C,'d');
    A'*X*A - X -C
xdel(winsid())

//====================================================
// ../man/fr/linear/nlev.xml
//====================================================
clear;lines(0);
    A=rand(3,3);x=poly(0,'x');
    [NUM,den]=nlev(A,'x')
    clean(den-poly(A,'x'))
    clean(NUM/den-inv(x*eye()-A))
xdel(winsid())

//====================================================
// ../man/fr/linear/orth.xml
//====================================================
clear;lines(0);
A=rand(5,3)*rand(3,4);
[X,dim]=rowcomp(A);X=X';
svd([orth(A),X(:,1:dim)])
xdel(winsid())

//====================================================
// ../man/fr/linear/pbig.xml
//====================================================
clear;lines(0);
    A=diag([1,2,3]);X=rand(A);A=inv(X)*A*X;
    [Q,M]=pbig(A,1.5,'d');
    spec(M*A*Q)
    [Q1,M1]=fullrf(eye()-Q*M);
    spec(M1*A*Q1)
xdel(winsid())

//====================================================
// ../man/fr/linear/pencan.xml
//====================================================
clear;lines(0);

F=randpencil([],[1,2],[1,2,3],[]);
F=rand(6,6)*F*rand(6,6);
[Q,M,i1]=pencan(F);
W=clean(M*F*Q)
roots(det(W(1:i1,1:i1)))
det(W($-2:$,$-2:$))

xdel(winsid())

//====================================================
// ../man/fr/linear/penlaur.xml
//====================================================
clear;lines(0);

F=randpencil([],[1,2],[1,2,3],[]);
F=rand(6,6)*F*rand(6,6);[E,A]=pen2ea(F);
[Si,Pi,Di]=penlaur(F);
[Bfs,Bis,chis]=glever(F);
norm(coeff(Bis,1)-Di,1)

xdel(winsid())

//====================================================
// ../man/fr/linear/pinv.xml
//====================================================
clear;lines(0);
    A=rand(5,2)*rand(2,4);
    norm(A*pinv(A)*A-A,1)
xdel(winsid())

//====================================================
// ../man/fr/linear/polar.xml
//====================================================
clear;lines(0);
A=rand(5,5);
[Ro,Theta]=polar(A);
norm(A-Ro*expm(%i*Theta),1)
xdel(winsid())

//====================================================
// ../man/fr/linear/projspec.xml
//====================================================
clear;lines(0);

deff('j=jdrn(n)','j=zeros(n,n);for k=1:n-1;j(k,k+1)=1;end')
A=sysdiag(jdrn(3),jdrn(2),rand(2,2));X=rand(7,7);
A=X*A*inv(X);
[S,P,D,index]=projspec(A);
index   //size of J-block
trace(P)  //sum of dimensions of J-blocks
A*S-(eye()-P)
norm(D^index,1)

xdel(winsid())

//====================================================
// ../man/fr/linear/proj.xml
//====================================================
clear;lines(0);
X1=rand(5,2);X2=rand(5,3);
P=proj(X1,X2);
norm(P^2-P,1)
trace(P)    // il s'agit de dim(X2)
[Q,M]=fullrf(P);
svd([Q,X2])   // Im(Q) = Im(X2)
xdel(winsid())

//====================================================
// ../man/fr/linear/psmall.xml
//====================================================
clear;lines(0);

A=diag([1,2,3]);X=rand(A);A=inv(X)*A*X;
[Q,M]=psmall(A,2.5,'d');
spec(M*A*Q)
[Q1,M1]=fullrf(eye()-Q*M);
spec(M1*A*Q1)

xdel(winsid())

//====================================================
// ../man/fr/linear/qr.xml
//====================================================
clear;lines(0);
    // QR factorization, generic case
    // X is tall (full rank)
    X=rand(5,2);[Q,R]=qr(X); [Q'*X R]
    //X is fat (full rank)
    X=rand(2,3);[Q,R]=qr(X); [Q'*X R]
    //Column 4 of X is a linear combination of columns 1 and 2:
    X=rand(8,5);X(:,4)=X(:,1)+X(:,2); [Q,R]=qr(X); R, R(:,4)
    //X has rank 2, rows 3 to $ of R are zero:
    X=rand(8,2)*rand(2,5);[Q,R]=qr(X); R
    //Evaluating the rank rk: column pivoting ==> rk first
    //diagonal entries of R are non zero :
    A=rand(5,2)*rand(2,5);
    [Q,R,rk,E] = qr(A,1.d-10);
    norm(Q'*A-R)
    svd([A,Q(:,1:rk)])    //span(A) =span(Q(:,1:rk))
xdel(winsid())

//====================================================
// ../man/fr/linear/quaskro.xml
//====================================================

//====================================================
// ../man/fr/linear/randpencil.xml
//====================================================
clear;lines(0);

F=randpencil([0,1],[2],[-1,0,1],[3]);
[Q,Z,Qd,Zd,numbeps,numbeta]=kroneck(F);
Qd, Zd
s=poly(0,'s');
F=randpencil([],[1,2],s^3-2,[]); //regular pencil
det(F)

xdel(winsid())

//====================================================
// ../man/fr/linear/range.xml
//====================================================

//====================================================
// ../man/fr/linear/rankqr.xml
//====================================================
clear;lines(0);

A=rand(5,3)*rand(3,7);
[Q,R,JPVT,RANK,SVAL]=rankqr(A,%eps)

xdel(winsid())

//====================================================
// ../man/fr/linear/rank.xml
//====================================================
clear;lines(0);
rank([1.d-80,0;0,1.d-80])
rank([1,0;0,1.d-80])
xdel(winsid())

//====================================================
// ../man/fr/linear/rcond.xml
//====================================================
clear;lines(0);
A=diag([1:10]);
rcond(A)
A(1,1)=0.000001;
rcond(A)
xdel(winsid())

//====================================================
// ../man/fr/linear/rowcomp.xml
//====================================================
clear;lines(0);
    A=rand(5,2)*rand(2,4);          // 4 vecteurs colonne dont 2 indépendants
    [X,dim]=rowcomp(A);Xp=X';
    svd([Xp(:,1:dim),A])            // Im(A) = Im(Xp(:,1:dim)
    x=A*rand(4,1);                  // x appartient à Im(A)
    y=X*x  
    norm(y(dim+1:$))/norm(y(1:dim)) // la norme est petite
xdel(winsid())

//====================================================
// ../man/fr/linear/rowshuff.xml
//====================================================
clear;lines(0);

F=randpencil([],[2],[1,2,3],[]);
F=rand(5,5)*F*rand(5,5);   // 5 x 5 regular pencil with 3 evals at 1,2,3
[Ws,F1]=rowshuff(F,-1);
[E1,A1]=pen2ea(F1);
svd(E1)           //E1 non singular
roots(det(Ws))
clean(inv(F)-inv(F1)*Ws,1.d-7)

xdel(winsid())

//====================================================
// ../man/fr/linear/rref.xml
//====================================================
clear;lines(0);

A=[1 2;3 4;5 6];
X=rref([A,eye(3,3)]);
R=X(:,1:2)
L=X(:,3:5);L*A

xdel(winsid())

//====================================================
// ../man/fr/linear/schur.xml
//====================================================
clear;lines(0);

//SCHUR FORM OF A MATRIX
//----------------------
A=diag([-0.9,-2,2,0.9]);X=rand(A);A=inv(X)*A*X;
[U,T]=schur(A);T

[U,dim,T]=schur(A,'c');
T(1:dim,1:dim)      //stable cont. eigenvalues

function t=mytest(Ev),t=abs(Ev)<0.95,endfunction
[U,dim,T]=schur(A,mytest);
T(1:dim,1:dim)  

// The same function in C (a Compiler is required)
C=['int mytest(double *EvR, double *EvI) {' //the C code
   'if (*EvR * *EvR + *EvI * *EvI < 0.9025) return 1;'
   'else return 0; }';]
mputl(C,TMPDIR+'/mytest.c')


//build and link
lp=ilib_for_link('mytest','mytest.o',[],'c',TMPDIR+'/Makefile');
link(lp,'mytest','c'); 

//run it
[U,dim,T]=schur(A,'mytest');
//SCHUR FORM OF A PENCIL
//----------------------
F=[-1,%s, 0,   1;
    0,-1,5-%s, 0;
    0, 0,2+%s, 0;
    1, 0, 0, -2+%s];
A=coeff(F,0);E=coeff(F,1);
[As,Es,Q,Z]=schur(A,E);
Q'*F*Z //It is As+%s*Es


[As,Es,Z,dim] = schur(A,E,'c')
function t=mytest(Alpha,Beta),t=real(Alpha)<0,endfunction
[As,Es,Z,dim] = schur(A,E,mytest)

//the same function in Fortran (a Compiler is required)
ftn=['integer function mytestf(ar,ai,b)' //the fortran code
      'double precision ar,ai,b'
      'mytestf=0'
      'if(ar.lt.0.0d0) mytestf=1'
      'end']
mputl('      '+ftn,TMPDIR+'/mytestf.f')

//build and link
lp=ilib_for_link('mytestf','mytestf.o',[],'F',TMPDIR+'/Makefile');
link(lp,'mytestf','f'); 

//run it

[As,Es,Z,dim] = schur(A,E,'mytestf')

xdel(winsid())

//====================================================
// ../man/fr/linear/spaninter.xml
//====================================================
clear;lines(0);

A=rand(5,3)*rand(3,4);     // A is 5 x 4, rank=3
B=[A(:,2),rand(5,1)]*rand(2,2);
[X,dim]=spaninter(A,B);
X1=X(:,1:dim);    //The intersection
svd(A),svd([X1,A])   // X1 in span(A)
svd(B),svd([B,X1])   // X1 in span(B)

xdel(winsid())

//====================================================
// ../man/fr/linear/spanplus.xml
//====================================================
clear;lines(0);

A=rand(6,2)*rand(2,5);      // rank(A)=2
B=[A(:,1),rand(6,2)]*rand(3,3);   //two additional independent vectors
[X,dim,dimA]=spanplus(A,B);
dimA
dim

xdel(winsid())

//====================================================
// ../man/fr/linear/spantwo.xml
//====================================================
clear;lines(0);

A=[1,0,0,4;
   5,6,7,8;
   0,0,11,12;
   0,0,0,16];
B=[1,2,0,0]';C=[4,0,0,1]; 
Sl=ss2ss(syslin('c',A,B,C),rand(A));
[no,X]=contr(Sl('A'),Sl('B'));CO=X(:,1:no);  //Controllable part
[uo,Y]=unobs(Sl('A'),Sl('C'));UO=Y(:,1:uo);  //Unobservable part
[Xp,dimc,dimu,dim]=spantwo(CO,UO);    //Kalman decomposition
Slcan=ss2ss(Sl,inv(Xp));

xdel(winsid())

//====================================================
// ../man/fr/linear/spchol.xml
//====================================================
clear;lines(0);
X=[
3.,  0.,  0.,  2.,  0.,  0.,  2.,  0.,  2.,  0.,  0. ;
0.,  5.,  4.,  0.,  0.,  0.,  0.,  0.,  0.,  0.,  0. ;
0.,  4.,  5.,  0.,  0.,  0.,  0.,  0.,  0.,  0.,  0. ;
2.,  0.,  0.,  3.,  0.,  0.,  2.,  0.,  2.,  0.,  0. ;
0.,  0.,  0.,  0. , 5.,  0.,  0.,  0.,  0.,  0.,  4. ;
0.,  0.,  0.,  0.,  0.,  4.,  0.,  3.,  0.,  3.,  0. ;
2.,  0.,  0.,  2.,  0.,  0.,  3.,  0.,  2.,  0.,  0. ;
0.,  0.,  0.,  0.,  0.,  3.,  0.,  4.,  0.,  3.,  0. ;
2.,  0.,  0.,  2.,  0.,  0.,  2.,  0.,  3.,  0.,  0. ;
0.,  0.,  0.,  0.,  0.,  3.,  0.,  3.,  0.,  4.,  0. ;
0.,  0.,  0.,  0.,  4.,  0.,  0.,  0.,  0.,  0.,  5.];
X=sparse(X);[R,P] = spchol(X);
max(P*R*R'*P'-X)
xdel(winsid())

//====================================================
// ../man/fr/linear/spec.xml
//====================================================
clear;lines(0);
    // MATRIX EIGENVALUES
    A=diag([1,2,3]);X=rand(3,3);A=inv(X)*A*X;
    spec(A)
    //
    x=poly(0,'x');
    pol=det(x*eye()-A)
    roots(pol)
    //
    [S,X]=bdiag(A);
    clean(inv(X)*A*X)

    // PENCIL EIGENVALUES
    A=rand(3,3);
    [al,be,Z] = spec(A,eye(A));al./be
    clean(inv(Z)*A*Z)  //displaying the eigenvalues (generic matrix)
    A=A+%i*rand(A);E=rand(A);
    roots(det(%s*E-A))   //complex case

xdel(winsid())

//====================================================
// ../man/fr/linear/sqroot.xml
//====================================================
clear;lines(0);
X=rand(5,2)*rand(2,5);X=X*X';
W=sqroot(X)
norm(W*W'-X,1)
//
X=rand(5,2)+%i*rand(5,2);X=X*X';
W=sqroot(X)
norm(W*W'-X,1)
xdel(winsid())

//====================================================
// ../man/fr/linear/sva.xml
//====================================================
clear;lines(0);
A=rand(5,4)*rand(4,5);
[U,s,V]=sva(A,2);
B=U*s*V';
svd(A)
svd(B)
clean(svd(A-B))
xdel(winsid())

//====================================================
// ../man/fr/linear/svd.xml
//====================================================
clear;lines(0);
    X=rand(4,2)*rand(2,4)
    svd(X)
    sqrt(spec(X*X'))
xdel(winsid())

//====================================================
// ../man/fr/linear/sylv.xml
//====================================================
clear;lines(0);

A=rand(4,4);C=rand(4,3);B=rand(3,3);
X = sylv(A,B,C,'c');
norm(A*X+X*B-C)
X=sylv(A,B,C,'d') 
norm(A*X*B-X-C)

xdel(winsid())

//====================================================
// ../man/fr/linear/trace.xml
//====================================================
clear;lines(0);
A=rand(3,3);
trace(A)-sum(spec(A))
xdel(winsid())

//====================================================
// ../man/fr/metanet/add_edge.xml
//====================================================
clear;lines(0);
ta=[1 1 2 2 2 3 4 5 5 7 8 8 9 10 10 10 11 12 13 13 13 14 15 16 16 17 17];
he=[2 10 3 5 7 4 2 4 6 8 6 9 7 7 11 15 12 13 9 10 14 11 16 1 17 14 15];
g=make_graph('foo',1,17,ta,he);
g('node_x')=[283 163 63 57 164 164 273 271 339 384 504 513 439 623 631 757 642];
g('node_y')=[59 133 223 318 227 319 221 324 432 141 209 319 428 443 187 151 301];
show_graph(g);
g=add_edge(1,7,g);
g('edge_color')=[ones(ta) 11];
show_graph(g);
xdel(winsid())

//====================================================
// ../man/fr/metanet/add_node.xml
//====================================================
clear;lines(0);
ta=[1 1 2 2 2 3 4 5 5 7 8 8 9 10 10 10 11 12 13 13 13 14 15 16 16 17 17];
he=[2 10 3 5 7 4 2 4 6 8 6 9 7 7 11 15 12 13 9 10 14 11 16 1 17 14 15];
g=make_graph('foo',1,17,ta,he);
g('node_x')=[283 163 63 57 164 164 273 271 339 384 504 513 439 623 631 757 642];
g('node_y')=[59 133 223 318 227 319 221 324 432 141 209 319 428 443 187 151 301];
show_graph(g);
n=g('node_number');
g1=add_node(g,[270 140]);
g1('node_color')=[ones(1,n) 11];
show_graph(g1);
xdel(winsid())

//====================================================
// ../man/fr/metanet/adj_lists.xml
//====================================================
clear;lines(0);
ta=[2 3 3 5 3 4 4 5 8];
he=[1 2 4 2 6 6 7 7 4];
g=make_graph('foo',1,8,ta,he);
g('node_x')=[129 200 283 281 128 366 122 333];
g('node_y')=[61 125 129 189 173 135 236 249];
show_graph(g);
[lp,la,ls]=adj_lists(g)
[lp,la,ls]=adj_lists(1,g('node_number'),ta,he)
xdel(winsid())

//====================================================
// ../man/fr/metanet/arc_graph.xml
//====================================================
clear;lines(0);
ta=[1 1 2 4 4 5 6 7 2 3 5 1];
he=[2 6 3 6 7 8 8 8 4 7 3 5];
g=make_graph('foo',1,8,ta,he);
g('node_x')=[281  284  360  185  405  182  118  45];
g('node_y')=[262  179  130  154  368  248  64  309];
show_graph(g);
g1=arc_graph(g);
show_graph(g1,'new');
xdel(winsid())

//====================================================
// ../man/fr/metanet/arc_number.xml
//====================================================

//====================================================
// ../man/fr/metanet/articul.xml
//====================================================
clear;lines(0);
ta=[2  1 3 2 2 4 4 5 6 7 8 8 9 10 10 10 10 11 12 13 14 15 16 17 17];
he=[1 10 2 5 7 3 2 4 5 8 6 9 7  7 11 13 15 12 13 14 11 16 17 14 15];
g=make_graph('foo',1,17,ta,he);
g('node_x')=[283 163 63 57 164 164 273 271 339 384 504 513 439 623 631 757 642];
g('node_y')=[59 133 223 318 227 319 221 324 432 141 209 319 428 443 187 151 301];
g('node_diam')=[1:(g('node_number'))]+20;
show_graph(g);
nart = articul(g)
show_nodes(nart);
xdel(winsid())

//====================================================
// ../man/fr/metanet/bandwr.xml
//====================================================
clear;lines(0);
ta=[2  1 3 2 2 4 4 5 6 7 8 8 9 10 10 10 10 11 12 13 13 14 15 16 16 17 17];
he=[1 10 2 5 7 3 2 4 5 8 6 9 7 7 11 13 15 12 13  9 14 11 16 1 17 14 15];
g=make_graph('foo',0,17,ta,he);
g('node_x')=[283 163 63 57 164 164 273 271 339 384 504 513 439 623 631 757 642];
g('node_y')=[59 133 223 318 227 319 221 324 432 141 209 319 428 443 187 151 301];
// LE GRAPHE
show_graph(g);
a=graph_2_mat(g,'node-node');
ww=tril(a)'+eye();
ww1=full(ww);
xset('window',1)
hist3d((ww1+tril(ww1',-1)+tril(ww1,-1)'),52,85); 
// RÉDUCTION DE LARGEUR DE BANDE POUR LA MATRICE
[iperm,mrepi,prof,ierr]=bandwr(ww);
max(prof(2:$)-prof(1:($-1)))
// GRAPHE AVEC LA NOUVELLE NUMÉROTATION
g2=g;g2('node_name')=string(iperm);
show_graph(g2,'new')
// NOUVELLE MATRICE
n=g('node_number');
yy=ww1(mrepi,mrepi);
xset('window',3)
hist3d((yy+tril(yy',-1)+tril(yy,-1)'),52,85); 
// ON COMMENCE AVEC LA MÊME MATRICE
[ij,v,mn]=spget(ww);
g1=make_graph('foo',0,n,ij(:,1)',ij(:,2)');
g1('node_x')=g('node_x');g1('node_y')=g('node_y');
// GRAPHE
//show_graph(g1,'rep');
[lp,la,ls] = adj_lists(1,n,g1('tail'),g1('head'));
[iperm,mrepi,prof,ierr]=bandwr(lp,ls,n,0);
g2=g;g2('node_name')=string(iperm);
show_graph(g2,'new');
xdel(winsid())

//====================================================
// ../man/fr/metanet/best_match.xml
//====================================================
clear;lines(0);
ta=[27 27 3 12 11 12 27 26 26 25 25 24 23 23 21 22 21 20 19 18 18];
ta=[ta  16 15 15 14 12 9 10 6 9 17 8 17 10 20 11 23 23 12 18 28]; 
he=[ 1  2 2  4  5 11 13  1 25 22 24 22 22 19 13 13 14 16 16  9 16];
he=[he  10 10 11 12  2 6  5 5 7  8 7  9  6 11  4 18 13  3 28 17];
n=28;
g=make_graph('foo',0,n,ta,he);
xx=[46 120 207 286 366 453 543 544 473 387 300 206 136 250 346 408];
g('node_x')=[xx 527 443 306 326 196 139 264  55  58  46 118 513];
yy=[36  34  37  40  38  40  35 102 102  98  93  96 167 172 101 179];
g('node_y')=[yy 198 252 183 148 172 256 259 258 167 109 104 253];
show_graph(g);
[card,match] = best_match(g);
sp=sparse([ta' he'],[1:size(ta,2)]',[n,n]);
sp1=sparse([[1:n]' match'],ones(1,size(match,2))',[n,n]);
[ij,v,mn]=spget(sp.*sp1);
show_arcs(v');
//
// AVEC UN GRAPHE PLUS GRAND
g=load_graph(SCI+'/demos/metanet/mesh1000');
g('directed')=0;
ta=g('tail');he=g('head');n=node_number(g);
show_graph(g,'new',[3000,1000]);
[card,match] = best_match(g);
sp=sparse([ta' he'],[1:size(ta,2)]',[n,n]);
sp1=sparse([[1:n]' match'],ones(1,size(match,2))',[n,n]);
[ij,v,mn]=spget(sp.*sp1);
show_arcs(v');
xdel(winsid())

//====================================================
// ../man/fr/metanet/chain_struct.xml
//====================================================
clear;lines(0);
ta=[1 1 2 3 5 4 6 7 7 3 3 8 8 5];
he=[2 3 5 4 6 6 7 4 3 2 8 1 7 4];
g=make_graph('foo',1,8,ta,he);
g('node_x')=[116 231 192 323 354 454 305 155];
g('node_y')=[118 116 212 219 117 185 334 316];
show_graph(g);
[fe,che,fn,chn] = chain_struct(g)
xdel(winsid())

//====================================================
// ../man/fr/metanet/check_graph.xml
//====================================================

//====================================================
// ../man/fr/metanet/circuit.xml
//====================================================
clear;lines(0);
// graphe avec circuit
ta=[1 1 2 3 5 4 6 7 7 3 3 8 8 5];
he=[2 3 5 4 6 6 7 4 3 2 8 1 7 4];
g=make_graph('foo',1,8,ta,he);
g('node_x')=[116 231 192 323 354 454 305 155];
g('node_y')=[ 118 116 212 219 117 185 334 316];
show_graph(g);
p=circuit(g)
show_arcs(p)
// graphe sans circuit
g=make_graph('foo',1,4,[1 2 2 3],[2 3 4 4]);
[p,r]=circuit(g)
xdel(winsid())

//====================================================
// ../man/fr/metanet/connex.xml
//====================================================
clear;lines(0);
ta=[1 1 2 2 2 3 4 4 5 6 7 7 7 8 9 10 12 12 13 13 14 15];
he=[2 6 3 4 5 1 3 5 1 7 5 8 9 5 8 11 10 11 11 15 13 14];
g=make_graph('foo',1,15,ta,he);
g('node_x')=[197 191 106 194 296 305 305 418 422 432 552 550 549 416 548];
g('node_y')=[76 181 276 278 276 83 174 281 177 86 175 90 290 397 399];
show_graph(g);
[nc,ncomp]=connex(g)
g('node_color')=10+ncomp; 
g('node_diam')=10+10*ncomp;
x_message('Affichage des composantes connexes du graphe');
show_graph(g);
xdel(winsid())

//====================================================
// ../man/fr/metanet/con_nodes.xml
//====================================================
clear;lines(0);
ta=[1 1 2 2 2 3 4 4 5 7 7 9 10 12 12 13 13 14 15];
he=[2 6 3 4 5 1 3 5 1 8 9 8 11 10 11 11 15 13 14];
g=make_graph('foo',1,15,ta,he);
g('node_x')=[197 191 106 194 296 305 305 418 422 432  552 550 549 416 548];
g('node_y')=[76 181 276 278 276 83 174 281 177 86 175 90 290 397 399];
show_graph(g);
con_nodes(2,g)
x_message('Affichage des sommets de la composante connexe #2');
n=g('node_number');
nodecolor=0*ones(1,n);
nodecolor(1,con_nodes(2,g))=11*ones(con_nodes(2,g));
g('node_color')=nodecolor;
nodediam=20.*ones(1,n);
nodediam(1,con_nodes(2,g))=30*ones(con_nodes(2,g));
g('node_diam')=nodediam;
show_graph(g);
xdel(winsid())

//====================================================
// ../man/fr/metanet/contract_edge.xml
//====================================================
clear;lines(0);
ta=[1 1 2 2 2 3 4 5 5 7 8 8 9 10 10 10 10 10 11 12 13 13 13 14 15 16 16 17 17];
he=[2 10 3 5 7 4 2 4 6 8 6 9 7 7 11 13 13 15 12 13 9 10 14 11 16 1 17 14 15];
g=make_graph('foo',1,17,ta,he);
g('node_x')=[283 163 63 57 164 164 273 271 339 384 504 513 439 623 631 757 642];
g('node_y')=[59 133 223 318 227 319 221 324 432 141 209 319 428 443 187 151 301];
show_graph(g);
g1=contract_edge(10,13,g);
show_graph(g1,'new');
xdel(winsid())

//====================================================
// ../man/fr/metanet/convex_hull.xml
//====================================================
clear;lines(0);
ta=[27 27 3 12 11 12 27 26 26 25 25 24 23 23 21 22 21 20 19 18 18];
ta=[ta  16 15 15 14 12 9 10 6 9 17 8 17 10 20 11 23 23 12 18 28]; 
he=[ 1  2 2  4  5 11 13  1 25 22 24 22 22 19 13 13 14 16 16  9 16];
he=[he  10 10 11 12  2 6  5 5 7  8 7  9  6 11  4 18 13  3 28 17];
g=make_graph('foo',0,28,ta,he);
xx=[46 120 207 286 366 453 543 544 473 387 300 206 136 250 346 408];
g('node_x')=[xx 527 443 306 326 196 139 264  55  58  46 118 513];
yy=[36  34  37  40  38  40  35 102 102  98  93  96 167 172 101 179];
g('node_y')=[yy 198 252 183 148 172 256 259 258 167 109 104 253];
show_graph(g);
xy=[g('node_x');g('node_y')];
[nhull,ind] = convex_hull(xy)
show_nodes(ind);
xdel(winsid())

//====================================================
// ../man/fr/metanet/cycle_basis.xml
//====================================================
clear;lines(0);
ta=[1 1 2 2 2 3 4 5 5 7 8 8 9 10 10 10 10 10 11 12 13 13 13 14 15 16 16 17 17];
he=[2 10 3 5 7 4 2 4 6 8 6 9 7 7 11 13 13 15 12 13 9 10 14 11 16 1 17 14 15];
gt=make_graph('foo',1,17,ta,he);
gt('node_x')=[283 163 63 57 164 164 273 271 339 384 504 513 439 623 631 757 642];
gt('node_y')=[59 133 223 318 227 319 221 324 432 141 209 319 428 443 187 151 301];
gt('edge_color')=modulo([1:(edge_number(gt))],15)+1;
gt('node_diam')=[1:(gt('node_number'))]+20;
show_graph(gt);
g=graph_simp(gt);
g('edge_color')=modulo([1:(edge_number(g))],15)+1;
g('node_diam')=gt('node_diam');
g('default_edge_hi_width')=12;
show_graph(g);
spc=cycle_basis(g);
for kk=1:(size(spc,1)),
  aaa=spc(kk,:);aaa=full(aaa);aaa(aaa==0)=[];
  show_arcs(aaa);
end;
xdel(winsid())

//====================================================
// ../man/fr/metanet/delete_arcs.xml
//====================================================
clear;lines(0);
ta=[1 1 2 2 2 3 4 5 5 7 8 8 9 10 10 10 10 10 11 12 13 13 13 14 15 16 16 17 17];
he=[2 10 3 5 7 4 2 4 6 8 6 9 7 7 11 13 13 15 12 13 9 10 14 11 16 1 17 14 15];
g=make_graph('foo',1,17,ta,he);
g('node_x')=[283 163 63 57 164 164 273 271 339 384 504 513 439 623 631 757 642];
g('node_y')=[59 133 223 318 227 319 221 324 432 141 209 319 428 443 187 151 301];
show_graph(g);
ij=[13 10;8 6;5 4;4 2];
gt=delete_arcs(ij,g);
show_graph(gt,'new');
g('directed')=0;
gt=delete_arcs(ij,g);
show_graph(gt,'new');
xdel(winsid())

//====================================================
// ../man/fr/metanet/delete_nodes.xml
//====================================================
clear;lines(0);
ta=[1 1 2 2 2 3 4 5 5 7 8 8 9 10 10 10 10 10 11 12 13 13 13 14 15 16 16 17 17];
he=[2 10 3 5 7 4 2 4 6 8 6 9 7 7 11 13 13 15 12 13 9 10 14 11 16 1 17 14 15];
g=make_graph('foo',1,17,ta,he);
g('node_x')=[283 163 63 57 164 164 273 271 339 384 504 513 439 623 631 757 642];
g('node_y')=[59 133 223 318 227 319 221 324 432 141 209 319 428 443 187 151 301];
show_graph(g);
v=[10 13 4];
gt=delete_nodes(v,g);
show_graph(gt,'new');
xdel(winsid())

//====================================================
// ../man/fr/metanet/edge_number.xml
//====================================================

//====================================================
// ../man/fr/metanet/edit_graph_menus.xml
//====================================================

//====================================================
// ../man/fr/metanet/edit_graph.xml
//====================================================
clear;lines(0);

 ta=[1 1 2 2 2 3 4 5 5 7 8 8 9 10 10 10 11 12 13 13 13 14 15 16 16 17 17];
 he=[2 10 3 5 7 4 2 4 6 8 6 9 7 7 11 15 12 13 9 10 14 11 16 1 17 14 15];
 g=make_graph('foo',1,17,ta,he);
 g('node_x')=[283 163 63 57 164 164 273 271 339 384 504 513 439 623 631 757 642];
 g('node_y')=[59 133 223 318 227 319 221 324 432 141 209 319 428 443 187 151 301];
 edit_graph(g)


xdel(winsid())

//====================================================
// ../man/fr/metanet/find_path.xml
//====================================================
clear;lines(0);
ta=[1 1 2 2 2 3 4 5 5 7 8 8 9 10 10 10 11 12 13 13 13 14 15 16 16 17 17];
he=[2 10 3 5 7 4 2 4 6 8 6 9 7 7 11 15 12 13 9 10 14 11 16 1 17 14 15];
g=make_graph('foo',1,17,ta,he);
g('node_x')=[283 163 63 57 164 164 273 271 339 384 504 513 439 623 631 757 642];
g('node_y')=[59 133 223 318 227 319 221 324 432 141 209 319 428 443 187 151 301];
show_graph(g);
p=find_path(1,14,g);
edgecolor=1*ones(ta); edgecolor(p)=11*ones(p); g('edge_color')=edgecolor;
show_graph(g); show_arcs(p);
xdel(winsid())

//====================================================
// ../man/fr/metanet/gen_net.xml
//====================================================
clear;lines(0);
v=[1,10,2,1,0,10,100,100,0,100,50,50];
g=gen_net('foo',1,v);
show_graph(g)
// génération interactive
g=gen_net();
show_graph(g)
xdel(winsid())

//====================================================
// ../man/fr/metanet/girth.xml
//====================================================
clear;lines(0);
ta=[1 6 2 4 7 5 6 8 4 3 5 1];
he=[2 1 3 6 4 8 8 7 2 7 3 5];
g=make_graph('foo',1,8,ta,he);
g('node_x')=[285  284  335  160  405  189  118  45];
g('node_y')=[266  179   83  176  368  252  64  309];
show_graph(g);
d=girth(g)
xdel(winsid())

//====================================================
// ../man/fr/metanet/glist.xml
//====================================================

//====================================================
// ../man/fr/metanet/graph_2_mat.xml
//====================================================
clear;lines(0);
g=load_graph(SCI+'/demos/metanet/colored');
a=graph_2_mat(g)
a=graph_2_mat(g,'node-node')
xdel(winsid())

//====================================================
// ../man/fr/metanet/graph_center.xml
//====================================================
clear;lines(0);
ta=[1 1 2 2 2 3 4 5 5 7 8 8 9 10 10 10 10 11 12 13 13 14 15 16 16 17 17];
he=[2 10 3 5 7 4 2 4 6 8 6 9 7 7 11 13 15 12 13  9 14 11 16 1 17 14 15];
g=make_graph('foo',0,17,ta,he);
g('node_x')=[283 163 63 57 164 164 273 271 339 384 504 513 439 623 631 757 642];
g('node_y')=[59 133 223 318 227 319 221 324 432 141 209 319 428 443 187 151 301];
g('node_diam')=[1:(g('node_number'))]+20;
show_graph(g);
[no,rad] = graph_center(g)
show_nodes(no);
xdel(winsid())

//====================================================
// ../man/fr/metanet/graph_complement.xml
//====================================================
clear;lines(0);
ta=[1 1 2 2 2 3 4 5 5 7 8 8 9 10 10 10 10 11 12 13 13 13 14 15 17 17 16 16];
he=[2 10 3 5 7 4 2 4 6 8 6 9 7 7 11 13 15 12 13 9 10 14 11 16 14 15 1 17];
g=make_graph('foo',1,17,ta,he);
g('node_x')=[283 163 63 57 164 164 273 271 339 384 504 513 439 623 631 757 642];
g('node_y')=[59 133 223 318 227 319 221 324 432 141 209 319 428 443 187 151 301];
g('edge_color')=modulo([1:(edge_number(g))],15)+1;
g('node_diam')=[1:(g('node_number'))]+20;
show_graph(g);
g1=graph_complement(g);
show_graph(g1,'new');
g=graph_complement(g1);
show_graph(g);
xdel(winsid())

//====================================================
// ../man/fr/metanet/graph_diameter.xml
//====================================================
clear;lines(0);
ta=[1 1 2 2 2 3 4 5 5 7 8 8 9 10 10 10 10 11 12 13 13 14 15 16 16 17 17];
he=[2 10 3 5 7 4 2 4 6 8 6 9 7 7 11 13 15 12 13  9 14 11 16 1 17 14 15];
g=make_graph('foo',0,17,ta,he);
g('node_x')=[283 163 63 57 164 164 273 271 339 384 504 513 439 623 631 757 642];
g('node_y')=[59 133 223 318 227 319 221 324 432 141 209 319 428 443 187 151 301];
g('node_diam')=[1:(g('node_number'))]+20;
show_graph(g);
[d,p] = graph_diameter(g)
show_arcs(p);
xdel(winsid())

//====================================================
// ../man/fr/metanet/graph-list.xml
//====================================================
clear;lines(0);
    g=load_graph(SCI+'/demos/metanet/mesh100');
    g('node_color')=int(rand(1:g('node_number'))*16);
    g('edge_color')=int(rand(1:edge_number(g))*16);
    show_graph(g)
xdel(winsid())

//====================================================
// ../man/fr/metanet/graph_power.xml
//====================================================
clear;lines(0);
ta=[1 1 2 4 4 5 6 7 2 3 5 1];
he=[2 6 3 6 7 8 8 8 4 7 3 5];
g=make_graph('foo',1,8,ta,he);
g('node_x')=[285  284  335  160  405  189  118  45];
g('node_y')=[266  179   83  176  368  252  64  309];
show_graph(g);
g1=graph_power(g,2);
show_graph(g1,'new');
xdel(winsid())

//====================================================
// ../man/fr/metanet/graph_simp.xml
//====================================================
clear;lines(0);
ta=[1 1  1 2 2 2 3 4 4 4 5 5 6 7 7 8 8 9 9 10 10 10 10 10 11 12 12 13 13 13 14 15 16 16 17 17];
he=[1 2 10 3 5 7 4 2 9 9 4 6 6 8 2 6 9 7 4  7 11 13 13 15 12 11 13 9 10 14 11 16 1 17 14 15];
g=make_graph('foo',1,17,ta,he);
g('node_x')=[283 163  63  98 164 162 273 235 267 384 504 493 409 573 601 627 642];
g('node_y')=[ 59 133 223 311 227 299 221 288 384 141 209 299 398 383 187 121 301];
show_graph(g);
g1=graph_simp(g);
show_graph(g1,'new');
xdel(winsid())

//====================================================
// ../man/fr/metanet/graph_sum.xml
//====================================================
clear;lines(0);
ta=[1 1 2 2 2 3 4 5 5 7 8 8 9 10 10 10 10 11 12 13 13 13 14 15 16 16 17 17];
he=[2 10 3 5 7 4 2 4 6 8 6 9 7 7 11 13 15 12 13 9 10 14 11 16 1 17 14 15];
g=make_graph('foo',1,17,ta,he);
g('node_x')=[283 163 63 57 164 164 273 271 339 384 504 513 439 623 631 757 642];
g('node_y')=[59 133 223 318 227 319 221 324 432 141 209 319 428 443 187 151 301];
g('edge_color')=modulo([1:(edge_number(g))],15)+1;
g('edge_width')=ones(1,(edge_number(g)));
g('node_diam')=[1:(g('node_number'))]+20;
g('node_name')=['A' 'B' 'C' 'D' 'E' 'F' 'G' 'H' 'I' 'J' 'K' 'L' 'M' 'N' 'O' 'P' 'Q'];
show_graph(g);
ta=[2 3 4 5 11 12 1];
he=[10 5 6 7 15 17 7];
g1=make_graph('foo',1,17,ta,he);
g1('node_x')=[283 163 63 57 164 164 273 271 339 384 504 513 439 623 631 757 642];
g1('node_y')=[59 133 223 318 227 319 221 324 432 141 209 319 428 443 187 151 301];
g1('edge_color')=modulo([1:(edge_number(g1))],15)+1;
g1('edge_width')=10*ones(1,(edge_number(g1)));
g1('node_diam')=[1:(g1('node_number'))]+20;
g1('node_name')=['A' 'B' 'C' 'D' 'E' 'F' 'G' 'H' 'I' 'J' 'K' 'L' 'M' 'N' 'O' 'P' 'Q'];
show_graph(g1,'new');
g2=graph_sum(g,g1);
show_graph(g2,'new');
xdel(winsid())

//====================================================
// ../man/fr/metanet/graph_union.xml
//====================================================
clear;lines(0);
ta=[1 1 2 2 2 3 4 5 5 7 8 8 9 10 10 10 10 10 11 12 13 13 13 14 15 16 16 17 17];
he=[2 10 3 5 7 4 2 4 6 8 6 9 7 7 11 13 13 15 12 13 9 10 14 11 16 1 17 14 15];
g=make_graph('foo',1,17,ta,he);
g('node_x')=[283 163 63 57 164 164 273 271 339 384 504 513 439 623 631 757 642];
g('node_y')=[59 133 223 318 227 319 221 324 432 141 209 319 428 443 187 151 301];
g('edge_color')=modulo([1:(edge_number(g))],15)+1;
g('node_diam')=[1:(g('node_number'))]+20;
g('node_name')=['A' 'B' 'C' 'D' 'E' 'F' 'G' 'H' 'I' 'J' 'K' 'L' 'M' 'N' 'O' 'P' 'Q'];
w=show_graph(g);
v=[7 8 9 10 11 12 13];
show_nodes(v);
g1=subgraph(v,'nodes',g);
show_graph(g1,'new');
v=[1 2 5 6 7 8 9 10];
netwindow(w);
show_nodes(v);
g2=subgraph(v,'nodes',g);
show_graph(g2,'new');
g=graph_union(g1,g2);
show_graph(g,'new');
xdel(winsid())

//====================================================
// ../man/fr/metanet/hamilton.xml
//====================================================
clear;lines(0);
ta=[2  1 3 2 2 4 4 5 6 7 8 8 9 10 10 10 10 11 12 13 13 14 15 16 16 17 17];
he=[1 10 2 5 7 3 2 4 5 8 6 9 7 7 11 13 15 12 13  9 14 11 16 1 17 14 15];
g=make_graph('foo',1,17,ta,he);
g('node_x')=[283 163 63 57 164 164 273 271 339 384 504 513 439 623 631 757 642];
g('node_y')=[59 133 223 318 227 319 221 324 432 141 209 319 428 443 187 151 301];
g('node_diam')=[1:(g('node_number'))]+20;
show_graph(g);
cir=hamilton(g)
show_arcs(cir);
xdel(winsid())

//====================================================
// ../man/fr/metanet/is_connex.xml
//====================================================
clear;lines(0);
g=make_graph('foo',1,3,[1,2,3,1],[2,3,1,3]);
is_connex(g)
g=make_graph('foo',1,4,[1,2,3,1],[2,3,1,3]);
is_connex(g)
xdel(winsid())

//====================================================
// ../man/fr/metanet/knapsack.xml
//====================================================
clear;lines(0);
weight=ones(1,15).*.[1:4];
profit=ones(1,60);
capa=[15 45 30 60];
[earn,ind]=knapsack(profit,weight,capa)
xdel(winsid())

//====================================================
// ../man/fr/metanet/line_graph.xml
//====================================================
clear;lines(0);
ta=[1 1 2 4 4 5 6 7 2 3 5 1];
he=[2 6 3 6 7 8 8 8 4 7 3 5];
g=make_graph('foo',0,8,ta,he);
g('node_x')=[281  284  360  185  405  182  118  45];
g('node_y')=[262  179  130  154  368  248  64  309];
show_graph(g);
g1=line_graph(g);
show_graph(g1,'new');
xdel(winsid())

//====================================================
// ../man/fr/metanet/load_graph.xml
//====================================================
clear;lines(0);
g=load_graph(SCI+'/demos/metanet/mesh100.graph');
show_graph(g);
g=load_graph(SCI+'/demos/metanet/colored');
show_graph(g,'new');
xdel(winsid())

//====================================================
// ../man/fr/metanet/make_graph.xml
//====================================================
clear;lines(0);
// création d'un graphe orienté avec 3 sommets et 4 arcs.
g=make_graph('foo',1,3,[1,2,3,1],[2,3,1,3]);
// création d'un graphe orienté avec 13 sommets et 14 arcs.
ta=[1  1 2 7 8 9 10 10 10 10 11 12 13 13];
he=[2 10 7 8 9 7  7 11 13 13 12 13  9 10];
g=make_graph('foo',1,13,ta,he);
g('node_x')=[120  98  87 188 439 698 226 127 342 467 711 779 477];
g('node_y')=[ 21 184 308 426 435 428 129 360 435  55 109 320 321];
show_graph(g)
// création d'un graphe sans sommet isolé et avec 14 arcs.
g=make_graph('foo',1,0,ta,he);
g('node_x')=[120  98 226 127 342 467 711 779 477];
g('node_y')=[ 21 184 129 360 435  55 109 320 321];
show_graph(g,'new')
xdel(winsid())

//====================================================
// ../man/fr/metanet/mat_2_graph.xml
//====================================================
clear;lines(0);
g=load_graph(SCI+'/demos/metanet/colored');
show_graph(g);
a=graph_2_mat(g);
g1=mat_2_graph(a,1);
g1('node_x')=g('node_x'); g1('node_y')=g('node_y');
show_graph(g1,'new');
a=graph_2_mat(g,'node-node');
g1=mat_2_graph(a,1,'node-node');
g1('node_x')=g('node_x'); g1('node_y')=g('node_y');
show_graph(g1,'new');
xdel(winsid())

//====================================================
// ../man/fr/metanet/max_cap_path.xml
//====================================================
clear;lines(0);
ta=[1 1 2 2 2 3 4 5 5 7 8 8 9 10 10 10 11 12 13 13 13 14 15 16 16 17 17];
he=[2 10 3 5 7 4 2 4 6 8 6 9 7 7 11 15 12 13 9 10 14 11 16 1 17 14 15];
g=make_graph('foo',1,17,ta,he);
g('node_x')=[283 163 63 57 164 164 273 271 339 384 504 513 439 623 631 757 642];
g('node_y')=[59 133 223 318 227 319 221 324 432 141 209 319 428 443 187 151 301];
show_graph(g);
ma=edge_number(g);
g('edge_max_cap')=int(rand(1,ma)*16)+5;
[p,cap]=max_cap_path(1,14,g);
edgecolor=1*ones(1,ma); edgecolor(p)=11*ones(p); g('edge_color')=edgecolor;
x_message(['La capacité maximum est : '+string(cap);
           'Voici le chemin correspondant']);
show_graph(g); show_arcs(p);
xdel(winsid())

//====================================================
// ../man/fr/metanet/max_clique.xml
//====================================================
clear;lines(0);
ta=[1 2 3 4 5 6 6 7 8  9 10 16 16 10 11 11 12 12 11 14 15 15 13 7 13 13];
he=[2 3 4 5 6 7 8 8 9 10 16  2  3 11 12 13  1 14 14 15  5  9 12 4 14 15];
g=make_graph('foo',0,16,ta,he);
g('node_x')=[106 199 369 467 470 403 399 347 308 269 184 108 199 268 345 272];
g('node_y')=[341 420 422 321 180 212 286 246 193 244 243 209  59 134  51 348];
g('node_diam')=[1:(g('node_number'))]+20;
show_graph(g);
[ns,no] = max_clique(g);
show_nodes(no);
g1=graph_complement(g);
[ns,no] = max_clique(g1);
show_nodes(no);
xdel(winsid())

//====================================================
// ../man/fr/metanet/max_flow.xml
//====================================================
clear;lines(0);
ta=[1 1 2 2 3 3 4 4 5 5 5 5 6 6 6 7 7 15 15 15 15 15 15];
ta=[ta, 15 8 9 10 11 12 13 14];
he=[10 13 9 14 8 11 9 11 8 10 12 13 8 9 12 8 11 1 2 3 4];
he=[he, 5 6 7 16 16 16 16 16 16 16];
n=16;
g=make_graph('foo',1,n,ta,he);
g('node_x')=[42 615 231 505 145 312 403 233 506 34 400 312 142 614 260 257];
g('node_y')=[143 145 154 154 147 152 157 270 273 279 269 273 273 274 50 376];
ma=edge_number(g);
g('edge_max_cap')=ones(1,ma);
g('edge_min_cap')=zeros(1,ma);
source=15; sink=16;
nodetype=0*ones(1,n); nodetype(source)=2; nodetype(sink)=1;
g('node_type')=nodetype;
nodecolor=0*ones(1,n); nodecolor(source)=11; nodecolor(sink)=11;
g('node_color')=nodecolor;
show_graph(g);
[v,phi,ierr]=max_flow(source,sink,g);
ii=find(phi<>0); edgecolor=phi; edgecolor(ii)=11*ones(ii);
g('edge_color')=edgecolor;
edgefontsize=8*ones(1,ma); edgefontsize(ii)=18*ones(ii);
g('edge_font_size')=edgefontsize;
g('edge_label')=string(phi);
show_graph(g);
xdel(winsid())

//====================================================
// ../man/fr/metanet/mesh2d.xml
//====================================================
clear;lines(0);

// FIRST CASE
theta=0.025*[1:40]*2.*%pi;
x=1+cos(theta);
y=1.+sin(theta);
theta=0.05*[1:20]*2.*%pi;
x1=1.3+0.4*cos(theta);
y1=1.+0.4*sin(theta);
theta=0.1*[1:10]*2.*%pi;
x2=0.5+0.2*cos(theta);
y2=1.+0.2*sin(theta);
x=[x x1 x2];
y=[y y1 y2];
//
nu=mesh2d(x,y);
nbt=size(nu,2);
jj=[nu(1,:)' nu(2,:)';nu(2,:)' nu(3,:)';nu(3,:)' nu(1,:)'];
as=sparse(jj,ones(size(jj,1),1));
ast=tril(as+abs(as'-as));
[jj,v,mn]=spget(ast);
n=size(x,2);
g=make_graph('foo',0,n,jj(:,1)',jj(:,2)');
g('node_x')=300*x;
g('node_y')=300*y;
g('default_node_diam')=10;
show_graph(g)
// SECOND CASE !!! NEEDS x,y FROM FIRST CASE
x3=2.*rand(1:200);
y3=2.*rand(1:200);
wai=((x3-1).*(x3-1)+(y3-1).*(y3-1));
ii=find(wai >= .94);
x3(ii)=[];y3(ii)=[];
wai=((x3-0.5).*(x3-0.5)+(y3-1).*(y3-1));
ii=find(wai <= 0.055);
x3(ii)=[];y3(ii)=[];
wai=((x3-1.3).*(x3-1.3)+(y3-1).*(y3-1));
ii=find(wai <= 0.21);
x3(ii)=[];y3(ii)=[];
xnew=[x x3];ynew=[y y3];
fr1=[[1:40] 1];fr2=[[41:60] 41];fr2=fr2($:-1:1);
fr3=[[61:70] 61];fr3=fr3($:-1:1);
front=[fr1 fr2 fr3];
//
nu=mesh2d(xnew,ynew,front);
nbt=size(nu,2);
jj=[nu(1,:)' nu(2,:)';nu(2,:)' nu(3,:)';nu(3,:)' nu(1,:)'];
as=sparse(jj,ones(size(jj,1),1));
ast=tril(as+abs(as'-as));
[jj,v,mn]=spget(ast);
n=size(xnew,2);
g=make_graph('foo',0,n,jj(:,1)',jj(:,2)');
g('node_x')=300*xnew;
g('node_y')=300*ynew;
g('default_node_diam')=10;
show_graph(g)
// REGULAR CASE !!! NEEDS PREVIOUS CASES FOR x,y,front
xx=0.1*[1:20];
yy=xx.*.ones(1,20);
zz= ones(1,20).*.xx;
x3=yy;y3=zz;
wai=((x3-1).*(x3-1)+(y3-1).*(y3-1));
ii=find(wai >= .94);
x3(ii)=[];y3(ii)=[];
wai=((x3-0.5).*(x3-0.5)+(y3-1).*(y3-1));
ii=find(wai <= 0.055);
x3(ii)=[];y3(ii)=[];
wai=((x3-1.3).*(x3-1.3)+(y3-1).*(y3-1));
ii=find(wai <= 0.21);
x3(ii)=[];y3(ii)=[];
xnew=[x x3];ynew=[y y3];
nu=mesh2d(xnew,ynew,front);
nbt=size(nu,2);
jj=[nu(1,:)' nu(2,:)';nu(2,:)' nu(3,:)';nu(3,:)' nu(1,:)'];
as=sparse(jj,ones(size(jj,1),1));
ast=tril(as+abs(as'-as));
[jj,v,mn]=spget(ast);
n=size(xnew,2);
g=make_graph('foo',0,n,jj(:,1)',jj(:,2)');
g('node_x')=300*xnew;
g('node_y')=300*ynew;
g('default_node_diam')=3;
show_graph(g)

//An example with a random set of points
function []=test(X,Y)
  Tr=mesh2d(X,Y);
  plot2d(X,Y,[-1,-2,3]);
  [m,n]=size(Tr);
  xpols= matrix(X(Tr),m,n); 
  ypols= matrix(Y(Tr),m,n);
  xset("colormap",rand(2*n,3)); 
  xfpolys(xpols,ypols,[n/4:n/4+n-1]);
endfunction 
N=1000;xbasc();X=rand(1,N); Y=rand(1,N);
xset("wdim",700,700);
test(X,Y);


xdel(winsid())

//====================================================
// ../man/fr/metanet/min_lcost_cflow.xml
//====================================================
clear;lines(0);
ta=[1 1 2 2 2 3 4 4 5 6 6 6 7 7 7 8 9 10 12 12 13 13 13 14 15 14 9 11 10];
he=[2 6 3 4 5 1 3 5 1 7 10 11 5 8 9 5 8 11 10 11 9 11 15 13 14 4 6 9 1];
g=make_graph('foo',1,15,ta,he);
g('node_x')=[194 191 106 194 296 305 305 418 422 432 552 550 549 416 548];
g('node_y')=[56 181 276 278 276 103 174 281 177 86 175 90 290 397 399];
show_graph(g);
g1=g; ma=arc_number(g1); n=g1('node_number');
g1('edge_min_cap')=0*ones(1,ma);
rand('uniform');
g1('edge_max_cap')=round(20*rand(1,ma))+ones(1,ma);
g1('edge_cost')=10*rand(1,ma)+ones(1,ma);
source=15; sink=1; cv=5;
[c,phi,v]=min_lcost_cflow(source,sink,cv,g1);
x_message(['Le cout est: '+string(c);
           'Voici les flots sur les arcs']);
nodetype=0*ones(1,n); nodetype(source)=2; nodetype(sink)=1;
g1('node_type')=nodetype;
ii=find(phi<>0); edgecolor=phi; edgecolor(ii)=11*ones(ii);
g1('edge_color')=edgecolor;
edgefontsize=8*ones(1,ma); edgefontsize(ii)=18*ones(ii);
nodecolor=0*ones(1,n); nodecolor(source)=11; nodecolor(sink)=11;
g1('node_color')=nodecolor;
g1('edge_font_size')=edgefontsize;
g1('edge_label')=string(phi);
show_graph(g1);
xdel(winsid())

//====================================================
// ../man/fr/metanet/min_lcost_flow1.xml
//====================================================
clear;lines(0);
ta=[1 1 2 2 2 3 4 4 5 6 6 6 7 7 7 8 9 10 12 12 13 13 13 14 15 14 9 11 10 1 8];
he=[2 6 3 4 5 1 3 5 1 7 10 11 5 8 9 5 8 11 10 11 9 11 15 13 14 4 6 9 1 12 14];
g=make_graph('foo',1,15,ta,he);
g('node_x')=[194 191 106 194 296 305 305 418 422 432 552 550 549 416 548];
g('node_y')=[56 221 316 318 316 143 214 321 217 126 215 80 330 437 439];
show_graph(g);
g1=g;ma=arc_number(g1);
rand('uniform');
while %T then
  g1('edge_min_cap')=round(20*rand(1,ma));
  g1('edge_max_cap')=round(20*rand(1,ma))+g1('edge_min_cap')+33*ones(1,ma);
  g1('edge_cost')=round(10*rand(1,ma))+ones(1,ma);
  [c,phi,flag]=min_lcost_flow1(g1);
  if flag==1 then break; end;
end;  
x_message(['Le cout est: '+string(c);
           'Flots sur les arcs ']);
ii=find(phi<>0); edgecolor=phi; edgecolor(ii)=11*ones(ii);
g1('edge_color')=edgecolor;
edgefontsize=8*ones(1,ma); edgefontsize(ii)=18*ones(ii);
g1('edge_font_size')=edgefontsize;
g1('edge_label')=string(phi);
show_graph(g1);
xdel(winsid())

//====================================================
// ../man/fr/metanet/min_lcost_flow2.xml
//====================================================
clear;lines(0);
ta=[1 1 2 2 2 3 4 4 5 6 6 6 7 7 7 8 9 10 12 12 13 13 13 14 15 14 9 11 10 1 8];
he=[2 6 3 4 5 1 3 5 1 7 10 11 5 8 9 5 8 11 10 11 9 11 15 13 14 4 6 9 1 12 14];
g=make_graph('foo',1,15,ta,he);
g('node_x')=[194 191 106 194 296 305 305 418 422 432 552 550 549 416 548];
g('node_y')=[56 221 316 318 316 143 214 321 217 126 215 80 330 437 439];
show_graph(g);
g1=g; ma=arc_number(g1); n=g1('node_number');
g1('edge_min_cap')=0.*ones(1,ma);
x_message(['Generation aléatoire des données';
           'Les premiers problèmes générés peuvent être insolubles']);
while %T then
 rand('uniform');
 g1('edge_max_cap')=round(20*rand(1,ma))+20*ones(1,ma);
 g1('edge_cost')=round(10*rand(1,ma)+ones(1,ma));
 rand('normal');
 dd=20.*rand(1,n)-10*ones(1,n);
 dd=round(dd-sum(dd)/n*ones(1,n));
 dd(n)=dd(n)-sum(dd);
 g1('node_demand')=dd;
 [c,phi,flag]=min_lcost_flow2(g1);
 if flag==1 then break; end;
end;
x_message(['Le cout est: '+string(c);
           'Voici le flot sur les arcs et les demandes des sommets']);
ii=find(phi<>0); edgecolor=phi; edgecolor(ii)=11*ones(ii);
g1('edge_color')=edgecolor;
edgefontsize=8*ones(1,ma); edgefontsize(ii)=18*ones(ii);
g1('edge_font_size')=edgefontsize;
g1('edge_label')=string(phi);
g1('node_label')=string(g1('node_demand'));
show_graph(g1);
xdel(winsid())

//====================================================
// ../man/fr/metanet/min_qcost_flow.xml
//====================================================
clear;lines(0);
ta=[1 1 2 2 2 3 4 4 5 6 6 6 7 7 7 8 9 10 12 12 13 13 13 14 15 14 9 11 10 1 8];
he=[2 6 3 4 5 1 3 5 1 7 10 11 5 8 9 5 8 11 10 11 9 11 15 13 14 4 6 9 1 12 14];
g=make_graph('foo',1,15,ta,he);
g('node_x')=[194 191 106 194 296 305 305 418 422 432 552 550 549 416 548];
g('node_y')=[56 221 316 318 316 143 214 321 217 126 215 80 330 437 439];
show_graph(g);
g1=g; ma=arc_number(g1);
rand('uniform');
while %T then
  g1('edge_min_cap')=round(5*rand(1,ma));
  g1('edge_max_cap')=round(20*rand(1,ma))+30*ones(1,ma);
  g1('edge_q_orig')=0*ones(1,ma);
  g1('edge_q_weight')=ones(1,ma);
  [c,phi,flag]=min_qcost_flow(0.001,g1);
 if flag==1 then break; end;
end;
x_message(['Le cout est: '+string(c);
          'Voici le flot sur les arcs']);
ii=find(phi<>0); edgecolor=phi; edgecolor(ii)=11*ones(ii);
g1('edge_color')=edgecolor;
edgefontsize=8*ones(1,ma); edgefontsize(ii)=18*ones(ii);
g1('edge_font_size')=edgefontsize;
g1('edge_label')=string(phi);
show_graph(g1);
xdel(winsid())

//====================================================
// ../man/fr/metanet/min_weight_tree.xml
//====================================================
clear;lines(0);
ta=[1 1 2 2 2 3 4 5 5 7 8 8 9 10 10 10 11 12 13 13 13 14 15 16 16 17 17];
he=[2 10 3 5 7 4 2 4 6 8 6 9 7 7 11 15 12 13 9 10 14 11 16 1 17 14 15];
g=make_graph('foo',1,17,ta,he);
g('node_x')=[283 163 63 57 164 164 273 271 339 384 504 513 439 623 631 757 642];
g('node_y')=[59 133 223 318 227 319 221 324 432 141 209 319 428 443 187 151 301];
show_graph(g);
t=min_weight_tree(1,g); 
g1=g; ma=arc_number(g1); n=g1('node_number');
nodetype=0*ones(1,n); nodetype(1)=2; g1('node_type')=nodetype;
edgecolor=1*ones(1,ma); edgecolor(t)=11*ones(t); g1('edge_color')=edgecolor;
edgewidth=1*ones(1,ma); edgewidth(t)=4*ones(t); g1('edge_width')=edgewidth;
x_message('Arbre de poids minimum à partir du sommet 1');
show_graph(g1);
xdel(winsid())

//====================================================
// ../man/fr/metanet/neighbors.xml
//====================================================
clear;lines(0);
ta=[1  6  2  4  7  5  6  8  4  3  5  1];
he=[2  1  3  6  4  8  8  7  2  7  3  5];
g=make_graph('foo',1,8,ta,he);
g('node_x')=[285  284  335  160  405  189  118  45];
g('node_y')=[266  179   83  176  368  252  64  309];
show_graph(g);
a=neighbors(6,g)
show_nodes(a);
xdel(winsid())

//====================================================
// ../man/fr/metanet/netclose.xml
//====================================================

//====================================================
// ../man/fr/metanet/netwindows.xml
//====================================================

//====================================================
// ../man/fr/metanet/netwindow.xml
//====================================================

//====================================================
// ../man/fr/metanet/node_number.xml
//====================================================

//====================================================
// ../man/fr/metanet/nodes_2_path.xml
//====================================================
clear;lines(0);
ta=[1 1 2 2 2 3 4 5 5 7 8 8 9 10 10 10 11 12 13 13 13 14 15 16 16 17 17];
he=[2 10 3 5 7 4 2 4 6 8 6 9 7 7 11 15 12 13 9 10 14 11 16 1 17 14 15];
g=make_graph('foo',1,17,ta,he);
g('node_x')=[283 163 63 57 164 164 273 271 339 384 504 513 439 623 631 757 642];
g('node_y')=[59 133 223 318 227 319 221 324 432 141 209 319 428 443 187 151 301];
show_graph(g);
ns=[1 10 15 16 17 14 11 12 13 9 7 8 6];
g1=g; nodecolor=1*ones(g('node_x')); nodecolor(ns)=11*ones(ns);
g1('node_color')=nodecolor;
show_graph(g1); show_nodes(ns);
p=nodes_2_path(ns,g);
g1=g; edgecolor=1*ones(ta); edgecolor(p)=11*ones(p);
g1('edge_color')=edgecolor;
show_graph(g1); show_arcs(p);
show_nodes(ns,'sup');
xdel(winsid())

//====================================================
// ../man/fr/metanet/nodes_degrees.xml
//====================================================
clear;lines(0);
ta=[1 1 2 2 2 3 4 5 5 7 8 8 9 10 10 10 11 12 13 13 13 14 15 16 16 17 17];
he=[2 10 3 5 7 4 2 4 6 8 6 9 7 7 11 15 12 13 9 10 14 11 16 1 17 14 15];
g=make_graph('foo',1,17,ta,he);
g('node_x')=[283 163 63 57 164 164 273 271 339 384 504 513 439 623 631 757 642];
g('node_y')=[59 133 223 318 227 319 221 324 432 141 209 319 428 443 187 151 301];
show_graph(g);
[outdegree,indegree]=nodes_degrees(g)
xdel(winsid())

//====================================================
// ../man/fr/metanet/path_2_nodes.xml
//====================================================
clear;lines(0);
ta=[1 1 2 2 2 3 4 5 5 7 8 8 9 10 10 10 11 12 13 13 13 14 15 16 16 17 17];
he=[2 10 3 5 7 4 2 4 6 8 6 9 7 7 11 15 12 13 9 10 14 11 16 1 17 14 15];
g=make_graph('foo',1,17,ta,he);
g('node_x')=[283 163 63 57 164 164 273 271 339 384 504 513 439 623 631 757 642];
g('node_y')=[59 133 223 318 227 319 221 324 432 141 209 319 428 443 187 151 301];
show_graph(g);
p=[2 16 23 25 26 22 17 18 19 13 10 11];
g1=g; edgecolor=1*ones(ta); edgecolor(p)=11*ones(p);
g1('edge_color')=edgecolor;
show_graph(g1); show_arcs(p);
ns=path_2_nodes(p,g);
g1=g; nodecolor=1*ones(g1('node_number')); nodecolor(ns)=11*ones(ns);
g1('node_color')=nodecolor;
show_graph(g1);show_nodes(ns);
show_arcs(p,'sup');
xdel(winsid())

//====================================================
// ../man/fr/metanet/perfect_match.xml
//====================================================
clear;lines(0);
ta=[27 27 3 12 11 12 27 26 26 25 25 24 23 23 21 22 21 20 19 18 18];
ta=[ta  16 15 15 14 12 9 10 6 9 17 8 17 10 20 11 23 23 12 18 28]; 
he=[ 1  2 2  4  5 11 13  1 25 22 24 22 22 19 13 13 14 16 16  9 16];
he=[he  10 10 11 12  2 6  5 5 7  8 7  9  6 11  4 18 13  3 28 17];
n=28;
g=make_graph('foo',0,n,ta,he);
xx=[46 120 207 286 366 453 543 544 473 387 300 206 136 250 346 408];
g('node_x')=[xx 527 443 306 326 196 139 264  55  58  46 118 513];
yy=[36  34  37  40  38  40  35 102 102  98  93  96 167 172 101 179];
g('node_y')=[yy 198 252 183 148 172 256 259 258 167 109 104 253];
show_graph(g);m2=2*size(ta,2);
arcost=round(100.*rand(1,m2));
[cst,nmatch] = perfect_match(g,arcost);
sp=sparse([ta' he'],[1:size(ta,2)]',[n,n]);
sp1=sparse([[1:n]' nmatch'],ones(1,size(nmatch,2))',[n,n]);
[ij,v,mn]=spget(sp.*sp1);
show_arcs(v');
xdel(winsid())

//====================================================
// ../man/fr/metanet/pipe_network.xml
//====================================================
clear;lines(0);
ta=[1 1 2 2 3 3 4 4 5 5 5 5 6 6 6 7 7 15 15 15 15 15 15];
ta=[ta, 15 8 9 10 11 12 13 14];
he=[10 13 9 14 8 11 9 11 8 10 12 13 8 9 12 8 11 1 2 3 4];
he=[he, 5 6 7 16 16 16 16 16 16 16];
n=16;
g=make_graph('foo',1,n,ta,he);
g('node_x')=[42 615 231 505 145 312 403 233 506 34 400 312 142 614 260 257];
g('node_y')=[143 145 154 154 147 152 157 270 273 279 269 273 273 274 50 376];
show_graph(g);
g('node_demand')=[0 0 0 0 0 0 0 0 0 0 0 0 0 0 -100 100];
w = [1 3 2 6 4 7 8 1 2 2 2 4 7 8 9 2 3 5 7 3 2 5 8 2 5 8];
g('edge_weight')=[w, 6 4 3 5 6];
[x,pi] = pipe_network(g)
xdel(winsid())

//====================================================
// ../man/fr/metanet/plot_graph.xml
//====================================================
clear;lines(0);
// graphe simple avec différents choix pour l'affichage
ta=[2 2 1 1 2 4 3 3 4];
he=[2 2 3 2 3 2 1 2 1];
g=make_graph('foo',1,4,ta,he);
g('node_type')=[1 1 1 2];g('node_name')=string([1:4]);
g('node_x')=[73 737 381 391]; g('node_y')=[283 337 458 142];
g('node_color')=[3 3 3 11];
g('node_diam')=[30 30 30 60];
g('edge_color')=[10 0 2 6 11 11 0 0 11];
rep=[2 2 1 1 2 2 2 2 2 2 2 2 2];
rep1=[100 -400 650 300];
xbasc(); plot_graph(g,rep,rep1);
rep=[2 1 1 1 2 2 2 2 2 2 2 2 2];
x_message('Represente le graphe avec differents parametres');
xbasc(); plot_graph(g,rep,rep1);
// Affichage avec une boîte de dialogue
xbasc(); plot_graph(g);
xset("thickness",4);
xbasc();
plot_graph(g);
xdel(winsid())

//====================================================
// ../man/fr/metanet/predecessors.xml
//====================================================
clear;lines(0);
ta=[1  6  2  4  7  5  6  8  4  3  5 1];
he=[2  1  3  6  4  8  8  7  2  7  3 5];
g=make_graph('foo',1,8,ta,he);
g('node_x')=[285  284  335  160  405  189  118  45];
g('node_y')=[266  179   83  176  368  252  64  309];
show_graph(g);
a=predecessors(8,g)
show_nodes(a);
xdel(winsid())

//====================================================
// ../man/fr/metanet/qassign.xml
//====================================================
clear;lines(0);
n=15;
d=100*rand(15,15);
d=d-diag(diag(d));
c=zeros(n,n);f=c;
f(2:n,1)=ones(1:n-1)';
[crit,order]=qassign(c,f,d)
xdel(winsid())

//====================================================
// ../man/fr/metanet/salesman.xml
//====================================================
clear;lines(0);
ta=[2  1 3 2 2 4 4 5 6 7 8 8 9 10 10 10 10 11 12 13 13 14 15 16 16 17 17];
he=[1 10 2 5 7 3 2 4 5 8 6 9 7 7 11 13 15 12 13  9 14 11 16 1 17 14 15];
g=make_graph('foo',0,17,ta,he);
g('node_x')=[283 163 63 57 164 164 273 271 339 384 504 513 439 623 631 757 642];
g('node_y')=[59 133 223 318 227 319 221 324 432 141 209 319 428 443 187 151 301];
g('node_diam')=[1:(g('node_number'))]+20;
show_graph(g);
g1=make_graph('foo1',1,17,[ta he],[he ta]);
m=arc_number(g1);
g1('edge_length')=5+round(30*rand(1,m));
cir = salesman(g1);
ii=find(cir > edge_number(g)); 
if(ii <> []) then cir(ii)=cir(ii)-edge_number(g);end;
show_arcs(cir);
xdel(winsid())

//====================================================
// ../man/fr/metanet/save_graph.xml
//====================================================
clear;lines(0);
g=load_graph(SCI+'/demos/metanet/mesh100');
show_graph(g);
if MSDOS then 
  unix('del mymesh100.graph');
else 
  unix('rm -f mymesh100.graph'); 
end
save_graph(g,'mymesh100.graph');
g=load_graph('mymesh100');
show_graph(g,'new');
xdel(winsid())

//====================================================
// ../man/fr/metanet/shortest_path.xml
//====================================================
clear;lines(0);
ta=[1 1 2 2 2 3 4 4 5 6 6 6 7 7 7 8 9 10 12 12 13 13 13 14 15 14 9 11 10];
he=[2 6 3 4 5 1 3 5 1 7 10 11 5 8 9 5 8 11 10 11 9 11 15 13 14 4 6 9 1];
g=make_graph('foo',1,15,ta,he);
g('node_x')=[194 191 106 194 296 305 305 418 422 432 552 550 549 416 548]; 
g('node_y')=[56 181 276 278 276 103 174 281 177 86 175 90 290 397 399];
show_graph(g);
g1=g;ma=prod(size(g1('head')));
rand('uniform');
g1('edge_length')=int(20*rand(1,ma));
[p,lp]=shortest_path(13,1,g1,'length');
p
x_message(['Voici le chemin le plus court';
           'Choisissez ""Display arc names"" dans le menu Graph pour voir les noms des arcs']);
g1('edge_name')=string(g1('edge_length'));
edgecolor=ones(1:ma);edgecolor(p)=11*ones(p);
g1('edge_color')=edgecolor;
edgefontsize=12*ones(1,ma);edgefontsize(p)=18*ones(p);
g1('edge_font_size')=edgefontsize;
show_graph(g1);
xdel(winsid())

//====================================================
// ../man/fr/metanet/show_arcs.xml
//====================================================
clear;lines(0);

ta=[1 1 2 2 2 3 4 5 5 7 8 8 9 10 10 10 11 12 13 13 13 14 15 16 16 17 17];
he=[2 10 3 5 7 4 2 4 6 8 6 9 7 7 11 15 12 13 9 10 14 11 16 1 17 14 15];
g=make_graph('foo',1,17,ta,he);
g('node_x')=[283 163 63 57 164 164 273 271 339 384 504 513 439 623 631 757 642];
g('node_y')=[59 133 223 318 227 319 221 324 432 141 209 319 428 443 187 151 301];
show_graph(g);
t=min_weight_tree(1,g); g1=g; ma=edge_number(g1);
edgecolor=1*ones(1,ma); g1('edge_color')=edgecolor;
edgewidth=1*ones(1,ma); edgewidth(t)=4*ones(t); g1('edge_width')=edgewidth;
for i=8:12,
 edgecolor(t)=i*ones(t); g1('edge_color')=edgecolor;
 xpause(3d5); show_graph(g1);
 show_arcs(t);
end;

xdel(winsid())

//====================================================
// ../man/fr/metanet/show_graph.xml
//====================================================
clear;lines(0);

ta=[1 1 2 2 2 3 4 5 5 7 8 8 9 10 10 10 11 12 13 13 13 14 15 16 16 17 17];
he=[2 10 3 5 7 4 2 4 6 8 6 9 7 7 11 15 12 13 9 10 14 11 16 1 17 14 15];
g=make_graph('foo',1,17,ta,he);
g('node_x')=[283 163 63 57 164 164 273 271 339 384 504 513 439 623 631 757 642];
g('node_y')=[59 133 223 318 227 319 221 324 432 141 209 319 428 443 187 151 301];
show_graph(g,2);
show_graph(g,0.5);
show_graph(g,1);

xdel(winsid())

//====================================================
// ../man/fr/metanet/show_nodes.xml
//====================================================
clear;lines(0);

ta=[1 1 2 2 2 3 4 5 5 7 8 8 9 10 10 10 11 12 13 13 13 14 15 16 16 17 17];
he=[2 10 3 5 7 4 2 4 6 8 6 9 7 7 11 15 12 13 9 10 14 11 16 1 17 14 15];
g=make_graph('foo',1,17,ta,he);
g('node_x')=[283 163 63 57 164 164 273 271 339 384 504 513 439 623 631 757 642];
g('node_y')=[59 133 223 318 227 319 221 324 432 141 209 319 428 443 187 151 301];
show_graph(g);
for i=2:3:g('node_number'), show_nodes([i]); end;
for i=1:3:g('node_number'), show_nodes([i],'sup'); end;

xdel(winsid())

//====================================================
// ../man/fr/metanet/split_edge.xml
//====================================================
clear;lines(0);
ta=[1 1 2 2 2 3 4 5 5 7 8 8 9 10 10 10 10 10 11 12 13 13 13 14 15 16 16 17 17];
he=[2 10 3 5 7 4 2 4 6 8 6 9 7 7 11 13 13 15 12 13 9 10 14 11 16 1 17 14 15];
g=make_graph('foo',1,17,ta,he);
g('node_x')=[283 163 63 57 164 164 273 271 339 384 504 513 439 623 631 757 642];
g('node_y')=[59 133 223 318 227 319 221 324 432 141 209 319 428 443 187 151 301];
show_graph(g);
gt=split_edge(1,2,g);
show_graph(gt,'new');
xdel(winsid())

//====================================================
// ../man/fr/metanet/strong_connex.xml
//====================================================
clear;lines(0);
ta=[1 1 2 2 2 3 4 4 5 6 6 6 7 7 7 8 9 10 12 12 13 13 13 14 15];
he=[2 6 3 4 5 1 3 5 1 7 10 11 5 8 9 5 8 11 10 11 9 11 15 13 14];
g=make_graph('foo',1,15,ta,he);
g('node_x')=[197 191 106 194 296 305 305 418 422 432 552 550 549 416 548]; 
g('node_y')=[76 181 276 278 276 83 174 281 177 86 175 90 290 397 399];
show_graph(g);
[nc,ncomp]=strong_connex(g);
g1=g; g1('node_color')=8+ncomp; g1('node_diam')=10+5*ncomp;
x_message('Composante fortement connexe du graphe');
show_graph(g1);
xdel(winsid())

//====================================================
// ../man/fr/metanet/strong_con_nodes.xml
//====================================================
clear;lines(0);
ta=[1 1 2 2 2 3 4 4 5 6 6 6 7 7 7 8 9 10 12 12 13 13 13 14 15];
he=[2 6 3 4 5 1 3 5 1 7 10 11 5 8 9 5 8 11 10 11 9 11 15 13 14];
g=make_graph('foo',1,15,ta,he);
g('node_x')=[197 191 106 194 296 305 305 418 422 432 552 550 549 416 548]; 
g('node_y')=[76 181 276 278 276 83 174 281 177 86 175 90 290 397 399];
show_graph(g);
ncomp=strong_con_nodes(3,g);
n=g('node_number');
nodecolor=0*ones(1,n); nodecolor(ncomp)=11*ones(ncomp);
g('node_color')=nodecolor;
nodediam=20*ones(1,n); nodediam(ncomp)=40*ones(ncomp);
g('node_diam')=nodediam;
x_message('Ensemble des sommets de la composante fortement connexe numéro 3');
show_graph(g);
xdel(winsid())

//====================================================
// ../man/fr/metanet/subgraph.xml
//====================================================
clear;lines(0);
ta=[1 1 2 2 2 3 4 5 5 7 8 8 9 10 10 10 10 10 11 12 13 13 13 14 15 16 16 17 17];
he=[2 10 3 5 7 4 2 4 6 8 6 9 7 7 11 13 13 15 12 13 9 10 14 11 16 1 17 14 15];
g=make_graph('foo',1,17,ta,he);
g('node_x')=[283 163 63 57 164 164 273 271 339 384 504 513 439 623 631 757 642];
g('node_y')=[59 133 223 318 227 319 221 324 432 141 209 319 428 443 187 151 301];
g('edge_color')=modulo([1:(edge_number(g))],15)+1;
g('node_diam')=[1:(g('node_number'))]+20;
show_graph(g);
v=[2 3 4 5 17 13 10];
show_nodes(v);
g1=subgraph(v,'nodes',g);
show_graph(g1);
v=[10 13 12 16 20 19];
show_graph(g);
show_arcs(v);
g1=subgraph(v,'edges',g);
show_graph(g1);
xdel(winsid())

//====================================================
// ../man/fr/metanet/successors.xml
//====================================================
clear;lines(0);
ta=[1  6  2  4  7  5  6  8  4  3  5  1];
he=[2  1  3  6  4  8  8  7  2  7  3  5];
g=make_graph('foo',1,8,ta,he);
g('node_x')=[285  284  335  160  405  189  118  45];
g('node_y')=[266  179   83  176  368  252  64  309];
show_graph(g);
a=successors(6,g)
show_nodes(a);
xdel(winsid())

//====================================================
// ../man/fr/metanet/supernode.xml
//====================================================
clear;lines(0);
ta=[1 1 2 2 2 3 4 5 5 7 8 8 9 10 10 10 10 10 11 12 13 13 13 14 15 16 16 17 17];
he=[2 10 3 5 7 4 2 4 6 8 6 9 7 7 11 13 13 15 12 13 9 10 14 11 16 1 17 14 15];
g=make_graph('foo',1,17,ta,he);
g('node_x')=[283 163 63 57 164 164 273 271 339 384 504 513 439 623 631 757 642];
g('node_y')=[59 133 223 318 227 319 221 324 432 141 209 319 428 443 187 151 301];
g('edge_color')=modulo([1:(edge_number(g))],15)+1;
g('node_diam')=[1:(g('node_number'))]+20;
show_graph(g);
v=[7 10 13 9];
show_nodes(v);
g1=supernode(v,g);
show_graph(g1,'new');
xdel(winsid())

//====================================================
// ../man/fr/metanet/trans_closure.xml
//====================================================
clear;lines(0);
ta=[2 3 3 5 3 4 4 5 8];
he=[1 2 4 2 6 6 7 7 4];
g=make_graph('foo',1,8,ta,he);
g('node_x')=[129 200 283 281 128 366 122 333];
g('node_y')=[61 125 129 189 173 135 236 249];
show_graph(g);
g1=trans_closure(g);
vv=1*ones(ta); aa=sparse([ta' he'],vv');
ta1=g1('tail'); he1=g1('head');
ww=1*ones(ta1); bb=sparse([ta1' he1'],ww');
dif=bb-aa; lim=size(ta1); edgecolor=0*ones(ta1);
for i=1:lim(2)
 if dif(ta1(i),he1(i))==1 then edgecolor(i)=11; end;
end;
g1('edge_color')=edgecolor;
x_message('Fermeture transitive du graphe');
show_graph(g1);
xdel(winsid())

//====================================================
// ../man/fr/nonlinear/bvode.xml
//====================================================
clear;lines(0);

deff('df=dfsub(x,z)','df=[0,0,-6/x**2,-6/x]')
deff('f=fsub(x,z)','f=(1 -6*x**2*z(4)-6*x*z(3))/x**3')
deff('g=gsub(i,z)','g=[z(1),z(3),z(1),z(3)];g=g(i)')
deff('dg=dgsub(i,z)',['dg=[1,0,0,0;0,0,1,0;1,0,0,0;0,0,1,0]';
                      'dg=dg(i,:)'])
deff('[z,mpar]=guess(x)','z=0;mpar=0')// unused here

deff('u=trusol(x)',[   //for testing purposes
   'u=0*ones(4,1)';
   'u(1) =  0.25*(10*log(2)-3)*(1-x) + 0.5 *( 1/x   + (3+x)*log(x) - x)'
   'u(2) = -0.25*(10*log(2)-3)       + 0.5 *(-1/x^2 + (3+x)/x      + log(x) - 1)'
   'u(3) = 0.5*( 2/x^3 + 1/x   - 3/x^2)'
   'u(4) = 0.5*(-6/x^4 - 1/x/x + 6/x^3)'])

fixpnt=0;m=4;
ncomp=1;aleft=1;aright=2;
zeta=[1,1,2,2];
ipar=zeros(1,11);
ipar(3)=1;ipar(4)=2;ipar(5)=2000;ipar(6)=200;ipar(7)=1;
ltol=[1,3];tol=[1.e-11,1.e-11];
res=aleft:0.1:aright;
z=bvode(res,ncomp,m,aleft,aright,zeta,ipar,ltol,tol,fixpnt,...
 fsub,dfsub,gsub,dgsub,guess)
z1=[];for x=res,z1=[z1,trusol(x)]; end;  
z-z1

xdel(winsid())

//====================================================
// ../man/fr/nonlinear/dasrt.xml
//====================================================
clear;lines(0);

//dy/dt = ((2*log(y)+8)/t -5)*y,  y(1) = 1,  1<=t<=6
//g1 = ((2*log(y)+8)/t - 5)*y 
//g2 = log(y) - 2.2491 
y0=1;t=2:6;t0=1;y0d=3;
atol=1.d-6;rtol=0;ng=2;

deff('[delta,ires]=res1(t,y,ydot)','ires=0;delta=ydot-((2*log(y)+8)/t-5)*y')
deff('[rts]=gr1(t,y)','rts=[((2*log(y)+8)/t-5)*y;log(y)-2.2491]')

[yy,nn]=dasrt([y0,y0d],t0,t,atol,rtol,res1,ng,gr1);
//(Should return nn=[2.4698972 2])

xdel(winsid())

//====================================================
// ../man/fr/nonlinear/dassl.xml
//====================================================
clear;lines(0);

 deff('[r,ires]=chemres(t,y,yd)',[
         'r(1)=-0.04*y(1)+1d4*y(2)*y(3)-yd(1);';
         'r(2)=0.04*y(1)-1d4*y(2)*y(3)-3d7*y(2)*y(2)-yd(2);'
         'r(3)=y(1)+y(2)+y(3)-1;'
         'ires=0']);
 deff('[pd]=chemjac(x,y,yd,cj)',[
         'pd=[-0.04-cj , 1d4*y(3)               , 1d4*y(2);';
         '0.04    ,-1d4*y(3)-2*3d7*y(2)-cj ,-1d4*y(2);';
         '1       , 1                      , 1       ]'])

y0=[1;0;0];
yd0=[-0.04;0.04;0];
t=[1.d-5:0.02:.4,0.41:.1:4,40,400,4000,40000,4d5,4d6,4d7,4d8,4d9,4d10];


y=dassl([y0,yd0],0,t,chemres);

info=list([],0,[],[],[],0,0);
info(2)=1;
y=dassl([y0,yd0],0,4d10,chemres,info);
y=dassl([y0,yd0],0,4d10,chemres,chemjac,info);

xdel(winsid())

//====================================================
// ../man/fr/nonlinear/datafit.xml
//====================================================
clear;lines(0);

deff('y=FF(x)','y=a*(x-b)+c*x.*x')
X=[];Y=[];
a=34;b=12;c=14;for x=0:.1:3, Y=[Y,FF(x)+100*(rand()-.5)];X=[X,x];end
Z=[Y;X];
deff('e=G(p,z)','a=p(1),b=p(2),c=p(3),y=z(1),x=z(2),e=y-FF(x)')
[p,err]=datafit(G,Z,[3;5;10])

xset('window',0)
xbasc();
plot2d(X',Y',-1) 
plot2d(X',FF(X)',5,'002')
a=p(1),b=p(2),c=p(3);plot2d(X',FF(X)',12,'002')
//same probleme with a known 
deff('e=G(p,z,a)','b=p(1),c=p(2),y=z(1),x=z(2),e=y-FF(x)')
[p,err]=datafit(list(G,a),Z,[5;10])

a=34;b=12;c=14;
deff('s=DG(p,z)','y=z(1),x=z(2),s=-[x-p(2),-p(1),x*x]')
[p,err]=datafit(G,DG,Z,[3;5;10])
xset('window',1)
xbasc();
plot2d(X',Y',-1) 
plot2d(X',FF(X)',5,'002')
a=p(1),b=p(2),c=p(3);plot2d(X',FF(X)',12,'002')

xdel(winsid())

//====================================================
// ../man/fr/nonlinear/derivative.xml
//====================================================
clear;lines(0);

  function y=F(x)
   y=[sin(x(1)*x(2))+exp(x(2)*x(3)+x(1)) ; sum(x.^3)];
 endfunction
 function y=G(x,p) 
   y=[sin(x(1)*x(2)*p)+exp(x(2)*x(3)+x(1)) ; sum(x.^3)];
 endfunction

 x=[1;2;3];[J,H]=derivative(F,x,H_form='blockmat');
 disp(J)
 disp(H)

 n=3;
 // form an orthogonal matrix :   
 nu=0; while nu<n, [Q,nu]=colcomp(rand(n,n)); end  
 for i=[1,2,4]
    [J,H]=derivative(F,x,order=i,H_form='blockmat',Q=Q);
    mprintf("order= %d \n",i);
    disp(H);
 end

 p=1;h=1e-3;
 [J,H]=derivative(list(G,p),x,h,2,H_form='hypermat');
 disp(H);
 [J,H]=derivative(list(G,p),x,h,4,Q=Q);
 disp(H)

 // Taylor series example:
 dx=1e-3*[1;1;-1];
 [J,H]=derivative(F,x);
 F(x+dx)
 F(x+dx)-F(x)
 F(x+dx)-F(x)-J*dx
 F(x+dx)-F(x)-J*dx-1/2*H*(dx .*. dx)

 // A trivial example
 function y=f(x,A,p,w), y=x'*A*x+p'*x+w; endfunction
 // with Jacobian and Hessean given by J(x)=x'*(A+A')+p', and H(x)=A+A'.
 A = rand(3,3); p = rand(3,1); w = 1;
 x = rand(3,1);
 [J,H]=derivative(list(f,A,p,w),x,h=1,H_form='blockmat')
 // Since f(x) is quadratic in x, approximate derivatives of order=2 or 4 by finite
 // differences should be exact for all h~=0. The apparent errors are caused by
 // cancellation in the floating point operations, so a "big" h is choosen.
 // Comparison with the exact matrices:
 Je = x'*(A+A')+p'
 He = A+A'
 clean(Je - J)
 clean(He - H)
  
xdel(winsid())

//====================================================
// ../man/fr/nonlinear/fit_dat.xml
//====================================================
clear;lines(0);

deff('y=FF(x)','y=a*(x-b)+c*x.*x')
X=[];Y=[];
a=34;b=12;c=14;for x=0:.1:3, Y=[Y,FF(x)+100*(rand()-.5)];X=[X,x];end
Z=[Y;X];
deff('e=G(p,z)','a=p(1),b=p(2),c=p(3),y=z(1),x=z(2),e=y-FF(x)')
[p,err]=fit_dat(G,[3;5;10],Z)
xset('window',0)
xbasc();
plot2d(X',Y',-1) 
plot2d(X',FF(X)',5,'002')
a=p(1),b=p(2),c=p(3);plot2d(X',FF(X)',12,'002')

a=34;b=12;c=14;
deff('s=DG(p,z)','y=z(1),x=z(2),s=-[x-p(2),-p(1),x*x]')
[p,err]=fit_dat(G,[3;5;10],Z,DG)
xset('window',1)
xbasc();
plot2d(X',Y',-1) 
plot2d(X',FF(X)',5,'002')
a=p(1),b=p(2),c=p(3);plot2d(X',FF(X)',12,'002')

xdel(winsid())

//====================================================
// ../man/fr/nonlinear/fsolve.xml
//====================================================
clear;lines(0);
    // un exemple simple
    a=[1,7;2,8];b=[10;11];
    deff('[y]=fsol1(x)','y=a*x+b');
    deff('[y]=fsolj1(x)','y=a');
    [xres]=fsolve([100;100],fsol1);
    a*xres+b
    [xres]=fsolve([100;100],fsol1,fsolj1);
    a*xres+b
    // voir routines/default/Ex-fsolve.f
    [xres]=fsolve([100;100],'fsol1','fsolj1',1.e-7);
    a*xres+b
xdel(winsid())

//====================================================
// ../man/fr/nonlinear/impl.xml
//====================================================
clear;lines(0);

y=impl([1;0;0],[-0.04;0.04;0],0,0.4,'resid','aplusp');
// Using hot restart 
//[x1,w,iw]=impl([1;0;0],[-0.04;0.04;0],0,0.2,'resid','aplusp');
// hot start from previous call 
//[x1]=impl([1;0;0],[-0.04;0.04;0],0.2,0.4,'resid','aplusp',w,iw);
//maxi(abs(x1-x))

xdel(winsid())

//====================================================
// ../man/fr/nonlinear/int2d.xml
//====================================================
clear;lines(0);

X=[0,0;1,1;1,0];
Y=[0,0;0,1;1,1];
deff('z=f(x,y)','z=cos(x+y)')
[I,e]=int2d(X,Y,f)
// computes the integrand over the square [0 1]x[0 1]

xdel(winsid())

//====================================================
// ../man/fr/nonlinear/int3d.xml
//====================================================
clear;lines(0);

X=[0;1;0;0];
Y=[0;0;1;0];
Z=[0;0;0;1];
[RESULT,ERROR]=int3d(X,Y,Z,'int3dex')
// computes the integrand exp(x*x+y*y+z*z) over the 
//tetrahedron (0.,0.,0.),(1.,0.,0.),(0.,1.,0.),(0.,0.,1.)


//integration over a cube  -1<=x<=1;-1<=y<=1;-1<=z<=1

//  bottom  -top-     right    -left-   front   -rear- 
X=[ 0, 0,    0, 0,    0, 0,    0, 0,    0, 0,    0, 0;          
   -1,-1,   -1,-1,    1, 1,   -1,-1,   -1,-1,   -1,-1; 
    1,-1,    1,-1,    1, 1,   -1,-1,    1,-1,    1,-1;     
    1, 1,    1, 1,    1, 1,   -1,-1,    1, 1,    1, 1];         
Y=[ 0, 0,    0, 0,    0, 0,    0, 0,    0, 0,    0, 0; 
   -1,-1,   -1,-1,   -1, 1,   -1, 1,   -1,-1,    1, 1;
   -1, 1,   -1, 1,    1, 1,    1, 1,   -1,-1,    1, 1;   
    1, 1,    1, 1,   -1,-1,   -1,-1,   -1,-1,    1, 1]; 
Z=[ 0, 0,    0, 0,    0, 0,    0, 0,    0, 0,    0, 0;
   -1,-1,    1, 1,   -1, 1,   -1, 1,   -1,-1,   -1,-1; 
   -1,-1,    1, 1,   -1,-1,   -1,-1,   -1, 1,   -1, 1;  
   -1,-1,    1, 1,    1,-1,    1,-1,    1, 1,    1, 1];      

function v=f(xyz,numfun),v=exp(xyz'*xyz),endfunction
[result,err]=int3d(X,Y,Z,f,1,[0,100000,1.d-5,1.d-7])

function v=f(xyz,numfun),v=1,endfunction
[result,err]=int3d(X,Y,Z,f,1,[0,100000,1.d-5,1.d-7])

  
xdel(winsid())

//====================================================
// ../man/fr/nonlinear/intc.xml
//====================================================

//====================================================
// ../man/fr/nonlinear/intg.xml
//====================================================
clear;lines(0);
deff('[y]=f(x)','y=x*sin(30*x)/sqrt(1-((x/(2*%pi))^2))')
exact=-2.5432596188;
abs(exact-intg(0,2*%pi,f))
// voir routines/default/Ex-intg.f 
abs(exact-intg(0,2*%pi,'intgex'))
xdel(winsid())

//====================================================
// ../man/fr/nonlinear/intl.xml
//====================================================

//====================================================
// ../man/fr/nonlinear/karmarkar.xml
//====================================================
clear;lines(0);

// n=10;p=20;
// a=rand(n,p);c=rand(p,1);x0=abs(rand(p,1));b=a*x0;x1=karmarkar(a,b,c,x0);

xdel(winsid())

//====================================================
// ../man/fr/nonlinear/leastsq.xml
//====================================================
clear;lines(0);

a=rand(3,2);b=[1;1;1];x0=[1;-1];
deff('f=fun(x,a,b)','f=a*x-b');
deff('g=dfun(x,a,b)','g=a');

[f,xopt]=leastsq(fun,x0)      //Simplest call
xopt-a\b  //compare with linear algebra solution

[f,xopt]=leastsq(fun,dfun,x0)      //specify gradient

[f,xopt]=leastsq(list(fun,[1 2;3 4],[1;2]),x0)    

deff('f=fun(x,a,b)','f=exp(a*x)-b');
deff('g=dfun(x,a,b)','g=a.*(exp(a*x)*ones(1,size(a,2)))');

[f,xopt]=leastsq(list(fun,[1 2;3 4],[1;2]),x0)  
  
xdel(winsid())

//====================================================
// ../man/fr/nonlinear/linpro.xml
//====================================================
clear;lines(0);

//Find x in R^6 such that:
//C1*x = b1  (3 equality constraints i.e me=3)
C1= [1,-1,1,0,3,1;
    -1,0,-3,-4,5,6;
     2,5,3,0,1,0];
b1=[1;2;3];
//C2*x <= b2  (2 inequality constraints)
C2=[0,1,0,1,2,-1;
    -1,0,2,1,1,0];
b2=[-1;2.5];
//with  x between ci and cs:
ci=[-1000;-10000;0;-1000;-1000;-1000];cs=[10000;100;1.5;100;100;1000];
//and minimize p'*x with
p=[1;2;3;4;5;6]
//No initial point is given: x0='v';
C=[C1;C2]; b=[b1;b2] ; me=3; x0='v';
[x,lagr,f]=linpro(p,C,b,ci,cs,me,x0)
// Lower bound constraints 3 and 4 are active and upper bound
// constraint 5 is active --> lagr(3:4) < 0 and lagr(5) > 0.
// Linear (equality) constraints 1 to 3 are active --> lagr(7:9) <> 0

xdel(winsid())

//====================================================
// ../man/fr/nonlinear/lmisolver.xml
//====================================================
clear;lines(0);

//Find diagonal matrix X (i.e. X=diag(diag(X), p=1) such that
//A1'*X+X*A1+Q1 < 0, A2'*X+X*A2+Q2 < 0 (q=2) and trace(X) is maximized 
n=2;A1=rand(n,n);A2=rand(n,n);
Xs=diag(1:n);Q1=-(A1'*Xs+Xs*A1+0.1*eye());
Q2=-(A2'*Xs+Xs*A2+0.2*eye());
deff('[LME,LMI,OBJ]=evalf(Xlist)','X=Xlist(1),LME=X-diag(diag(X));...
LMI=list(-(A1''*X+X*A1+Q1),-(A2''*X+X*A2+Q2)),OBJ= -sum(diag(X))  ');
X=lmisolver(list(zeros(A1)),evalf);X=X(1)
[Y,Z,c]=evalf(X)

xdel(winsid())

//====================================================
// ../man/fr/nonlinear/lmitool.xml
//====================================================

//====================================================
// ../man/fr/nonlinear/lsqrsolve.xml
//====================================================
clear;lines(0);

// A simple example with lsqrsolve 
a=[1,7;
   2,8
   4 3];
b=[10;11;-1];
function y=f1(x,m),y=a*x+b;endfunction
[xsol,v]=lsqrsolve([100;100],f1,3)
xsol+a\b


function y=fj1(x,m),y=a;endfunction
[xsol,v]=lsqrsolve([100;100],f1,3,fj1)
xsol+a\b

// Data fitting problem
// 1 build the data
a=34;b=12;c=14;
deff('y=FF(x)','y=a*(x-b)+c*x.*x');
X=(0:.1:3)';Y=FF(X)+100*(rand()-.5);

//solve
function e=f1(abc,m)
  a=abc(1);b=abc(2),c=abc(3),
  e=Y-(a*(X-b)+c*X.*X);
endfunction
[abc,v]=lsqrsolve([10;10;10],f1,size(X,1));
abc
norm(v)


xdel(winsid())

//====================================================
// ../man/fr/nonlinear/NDcost.xml
//====================================================
clear;lines(0);

   //function to minimize
   function f=rosenbrock(x,varagin)
     p=varargin(1)
     f=1+sum( p*(x(2:$)-x(1:$-1)^2)^2 + (1-x(2:$))^2)
   endfunction

   x0=[1;2;3;4];
   [f,xopt,gopt]=optim(list(NDcost,rosenbrock,200),x0)

xdel(winsid())

//====================================================
// ../man/fr/nonlinear/numdiff.xml
//====================================================
clear;lines(0);

function  f=myfun(x,y,z,t)
f=(x+y)^t+z
endfunction

y=3;z=4;t=2;
g=numdiff(list(myfun,3,4,2),1)

x=1;
exact=t*(x+y)^(t-1)

xdel(winsid())

//====================================================
// ../man/fr/nonlinear/odedc.xml
//====================================================
clear;lines(0);

//Linear system with switching input
deff('xdu=phis(t,x,u,flag)','if flag==0 then xdu=A*x+B*u; else xdu=1-u;end');
x0=[1;1];A=[-1,2;-2,-1];B=[1;2];u=0;nu=1;stdel=[1,0];u0=0;t=0:0.05:10;
xu=odedc([x0;u0],nu,stdel,0,t,phis);x=xu(1:2,:);u=xu(3,:);
nx=2;
plot2d1('onn',t',x',[1:nx],'161');
plot2d2('onn',t',u',[nx+1:nx+nu],'000');
//Fortran external( see fydot2.f): 
norm(xu-odedc([x0;u0],nu,stdel,0,t,'phis'),1)

//Sampled feedback 
//
//        |     xcdot=fc(t,xc,u)
//  (system)   |
//        |     y=hc(t,xc)
//
//
//        |     xd+=fd(xd,y)
//  (feedback) |
//        |     u=hd(t,xd)
//
deff('xcd=f(t,xc,xd,iflag)',...
  ['if iflag==0 then '
   '  xcd=fc(t,xc,e(t)-hd(t,xd));'
   'else '
   '  xcd=fd(xd,hc(t,xc));'
   'end']);
A=[-10,2,3;4,-10,6;7,8,-10];B=[1;1;1];C=[1,1,1];
Ad=[1/2,1;0,1/20];Bd=[1;1];Cd=[1,1];
deff('st=e(t)','st=sin(3*t)')
deff('xdot=fc(t,x,u)','xdot=A*x+B*u')
deff('y=hc(t,x)','y=C*x')
deff('xp=fd(x,y)','xp=Ad*x + Bd*y')
deff('u=hd(t,x)','u=Cd*x')
h=0.1;t0=0;t=0:0.1:2;
x0c=[0;0;0];x0d=[0;0];nd=2;
xcd=odedc([x0c;x0d],nd,h,t0,t,f);
norm(xcd-odedc([x0c;x0d],nd,h,t0,t,'fcd1')) // Fast calculation (see fydot2.f)
plot2d([t',t',t'],xcd(1:3,:)');
xset("window",2);plot2d2("gnn",[t',t'],xcd(4:5,:)');
xset("window",0);

xdel(winsid())

//====================================================
// ../man/fr/nonlinear/ode_discrete.xml
//====================================================
clear;lines(0);

y1=[1;2;3]; deff("yp=a_function(k,y)","yp=A*y+B*u(k)")
A=diag([0.2,0.5,0.9]); B=[1;1;1];u=1:10;n=5;
y=ode("discrete",y1,1,1:n,a_function);
y(:,2)-(A*y1+B*u(1))
// Now y evaluates  at [y3,y5,y7,y9]
y=ode("discrete",y1,1,3:2:9,a_function)

xdel(winsid())

//====================================================
// ../man/fr/nonlinear/odeoptions.xml
//====================================================

//====================================================
// ../man/fr/nonlinear/ode_root.xml
//====================================================
clear;lines(0);

// Integration of the differential equation
// dy/dt=y , y(0)=1, and finds the minimum time t such that y(t)=2
deff("[ydot]=f(t,y)","ydot=y")
deff("[z]=g(t,y)","z=y-2")
y0=1;ng=1;
[y,rd]=ode("roots",y0,0,2,f,ng,g)


deff("[z]=g(t,y)","z=y-[2;2;33]")
[y,rd]=ode("roots",1,0,2,f,3,g)



xdel(winsid())

//====================================================
// ../man/fr/nonlinear/ode.xml
//====================================================
clear;lines(0);
    // EDO à une dimension
    // dy/dt=y^2-y sin(t)+cos(t), y(0)=0
    deff("[ydot]=f(t,y)","ydot=y^2-y*sin(t)+cos(t)")
    y0=0;t0=0;t=0:0.1:%pi;
    y=ode(y0,t0,t,f)
    plot(t,y)
    // Simulation de dx/dt = A x(t) + B u(t) avec u(t)=sin(omega*t),
    // x0=[1;0]
    // la solution x(t) est désirée en t=0.1, 0.2, 0.5 ,1.
    // A et u sont passées dans une liste 
    // et B et omega sont des variables globales
    deff("[xdot]=linear(t,x,A,u)","xdot=A*x+B*u(t)")
    deff("[ut]=u(t)","ut=sin(omega*t)")
    A=[1 1;0 2];B=[1;1];omega=5;
    ode([1;0],0,[0.1,0.2,0.5,1],list(linear,A,u))
    //
    // EDO matricielle
    // Equation différentielle de Ricatti
    // Xdot=A'*X + X*A - X'*B*X + C , X(0)=Identité
    // Solution en t=[1,2] 
    deff("[Xdot]=ric(t,X)","Xdot=A''*X+X*A-X''*B*X+C")   
    A=[1,1;0,2]; B=[1,0;0,1]; C=[1,0;0,1];
    t0=0;t=0:0.1:%pi;
    X=ode(eye(A),0,t,ric)
    //
    // Calcul de exp(A)
    A=[1,1;0,2];
    deff("[xdot]=f(t,x)","xdot=A*x");
    ode(eye(A),0,1,f)
    ode("adams",eye(A),0,1,f)
    // EDO raide, avec la jacobienne fournie
    A=[10,0;0,-1];
    deff("[xdot]=f(t,x)","xdot=A*x");
    deff("[J]=Jacobian(t,y)","J=A")
    ode("stiff",[0;1],0,1,f,Jacobian)
xdel(winsid())

//====================================================
// ../man/fr/nonlinear/optim.xml
//====================================================
clear;lines(0);

xref=[1;2;3];x0=[1;-1;1]
deff('[f,g,ind]=cost(x,ind)','f=0.5*norm(x-xref)^2,g=x-xref');
[f,xopt]=optim(cost,x0)      //Simplest call
[f,xopt,gopt]=optim(cost,x0,'gc')  // By conjugate gradient
[f,xopt,gopt]=optim(cost,x0,'nd')  //Seen as non differentiable
[f,xopt,gopt]=optim(cost,'b',[-1;0;2],[0.5;1;4],x0) //  Bounds on x
[f,xopt,gopt]=optim(cost,'b',[-1;0;2],[0.5;1;4],x0,'gc') //  Bounds on x
[f,xopt,gopt]=optim(cost,'b',[-1;0;2],[0.5;1;4],x0,'gc','ar',3)
// Here, 3 calls to cost are allowed.
// Now calling the Fortran subroutine "genros" in SCIDIR/default/Ex-optim.f
// See also the link function for dynamically linking an objective function
[f,xopt,gopt]=optim('genros',[1;2;3])    //Rosenbrock's function
  
xdel(winsid())

//====================================================
// ../man/fr/nonlinear/quapro.xml
//====================================================
clear;lines(0);

//Find x in R^6 such that:
//C1*x = b1 (3 equality constraints i.e me=3)
C1= [1,-1,1,0,3,1;
    -1,0,-3,-4,5,6;
     2,5,3,0,1,0];
b1=[1;2;3];
//C2*x <= b2 (2 inequality constraints)
C2=[0,1,0,1,2,-1;
    -1,0,2,1,1,0];
b2=[-1;2.5];
//with  x between ci and cs:
ci=[-1000;-10000;0;-1000;-1000;-1000];cs=[10000;100;1.5;100;100;1000];
//and minimize 0.5*x'*Q*x + p'*x with
p=[1;2;3;4;5;6]; Q=eye(6,6);
//No initial point is given;
C=[C1;C2] ; //
b=[b1;b2] ;  //
me=3;
[x,lagr,f]=quapro(Q,p,C,b,ci,cs,me)
//Only linear constraints (1 to 4) are active (lagr(1:6)=0):
[x,lagr,f]=quapro(Q,p,C,b,[],[],me)   //Same result as above

xdel(winsid())

//====================================================
// ../man/fr/nonlinear/semidef.xml
//====================================================
clear;lines(0);

F0=[2,1,0,0;
    1,2,0,0;
    0,0,3,1
    0,0,1,3];
F1=[1,2,0,0;
    2,1,0,0;
    0,0,1,3;
    0,0,3,1]
F2=[2,2,0,0;
    2,2,0,0;
    0,0,3,4;
    0,0,4,4];
blck_szs=[2,2];
F01=F0(1:2,1:2);F02=F0(3:4,3:4);
F11=F1(1:2,1:2);F12=F1(3:4,3:4);
F21=F2(1:2,1:2);F22=F2(3:4,3:4);
x0=[0;0]
Z0=2*F0;
Z01=Z0(1:2,1:2);Z02=Z0(3:4,3:4);
FF=[[F01(:);F02(:)],[F11(:);F12(:)],[F21(:);F22(:)]]
ZZ0=[[Z01(:);Z02(:)]];
c=[trace(F1*Z0);trace(F2*Z0)];
options=[10,1.d-10,1.d-10,0,50];
[x,Z,ul,info]=semidef(x0,pack(ZZ0),pack(FF),blck_szs,c,options)
w=vec2list(unpack(Z,blck_szs),[blck_szs;blck_szs]);Z=sysdiag(w(1),w(2))
c'*x+trace(F0*Z)
spec(F0+F1*x(1)+F2*x(2))
trace(F1*Z)-c(1)
trace(F2*Z)-c(2)

xdel(winsid())

//====================================================
// ../man/fr/polynomials/bezout.xml
//====================================================
clear;lines(0);
// cas des polynômes
x=poly(0,'x');
p1=(x+1)*(x-3)^5;p2=(x-2)*(x-3)^3;
[pgcd,U]=bezout(p1,p2) 
det(U)
clean([p1,p2]*U)
ppcm=p1*U(1,2)
lcm([p1,p2])
// cas des entiers
i1=int32(2*3^5); i2=int32(2^3*3^2);
[thegcd,U]=bezout(i1,i2) 
V=int32([2^2*3^5, 2^3*3^2,2^2*3^4*5]);
[thegcd,U]=gcd(V)
V*U
lcm(V)
xdel(winsid())

//====================================================
// ../man/fr/polynomials/clean.xml
//====================================================
clear;lines(0);
x=poly(0,'x');
w=[x,1,2+x;3+x,2-x,x^2;1,2,3+x]/3;
w*inv(w)
clean(w*inv(w))
xdel(winsid())

//====================================================
// ../man/fr/polynomials/cmndred.xml
//====================================================

//====================================================
// ../man/fr/polynomials/coeff.xml
//====================================================

//====================================================
// ../man/fr/polynomials/coffg.xml
//====================================================
clear;lines(0);
s=poly(0,'s')
a=[ s, s^2+1; s  s^2-1];
[a1,d]=coffg(a);
(a1/d)-inv(a)
xdel(winsid())

//====================================================
// ../man/fr/polynomials/colcompr.xml
//====================================================
clear;lines(0);
s=poly(0,'s');
p=[s;s*(s+1)^2;2*s^2+s^3];
[Y,rk,ac]=colcompr(p*p');
p*p'*Y
xdel(winsid())

//====================================================
// ../man/fr/polynomials/degree.xml
//====================================================

//====================================================
// ../man/fr/polynomials/denom.xml
//====================================================

//====================================================
// ../man/fr/polynomials/derivat.xml
//====================================================
clear;lines(0);
s=poly(0,'s');
derivat(1/s)  // -1/s^2;
xdel(winsid())

//====================================================
// ../man/fr/polynomials/determ.xml
//====================================================
clear;lines(0);
s=poly(0,'s');
w=s*rand(10,10);
determ(w)
det(coeff(w,1))*s^10
xdel(winsid())

//====================================================
// ../man/fr/polynomials/detr.xml
//====================================================

//====================================================
// ../man/fr/polynomials/diophant.xml
//====================================================
clear;lines(0);
s=poly(0,'s');p1=(s+3)^2;p2=(1+s);
x1=s;x2=(2+s);
[x,err]=diophant([p1,p2],p1*x1+p2*x2);
p1*x1+p2*x2-p1*x(1)-p2*x(2)
xdel(winsid())

//====================================================
// ../man/fr/polynomials/factors.xml
//====================================================
clear;lines(0);
n=poly([0.2,2,5],'z');
d=poly([0.1,0.3,7],'z');
R=syslin('d',n,d);
R1=factors(R,'d')
roots(R1('num'))
roots(R1('den'))
w=exp(2*%i*%pi*[0:0.1:1]);
norm(abs(horner(R1,w))-abs(horner(R,w)))
xdel(winsid())

//====================================================
// ../man/fr/polynomials/gcd.xml
//====================================================
clear;lines(0);
//cas des polynômes
s=poly(0,'s');
p=[s,s*(s+1)^2,2*s^2+s^3];
[pgcd,u]=gcd(p);
p*u
//cas des entiers
V=int32([2^2*3^5, 2^3*3^2,2^2*3^4*5]);
[thegcd,U]=gcd(V)
V*U

xdel(winsid())

//====================================================
// ../man/fr/polynomials/hermit.xml
//====================================================
clear;lines(0);
s=poly(0,'s');
p=[s, s*(s+1)^2, 2*s^2+s^3];
[Ar,U]=hermit(p'*p);
clean(p'*p*U), det(U)
xdel(winsid())

//====================================================
// ../man/fr/polynomials/horner.xml
//====================================================
clear;lines(0);
s=poly(0,'s');M=[s,1/s];
horner(M,1)
horner(M,%i)
horner(M,1/s)
xdel(winsid())

//====================================================
// ../man/fr/polynomials/hrmt.xml
//====================================================
clear;lines(0);
x=poly(0,'x');
v=[x*(x+1),x^2*(x+1),(x-2)*(x+1),(3*x^2+2)*(x+1)];
[pg,U]=hrmt(v);U=clean(U)
det(U)
xdel(winsid())

//====================================================
// ../man/fr/polynomials/htrianr.xml
//====================================================
clear;lines(0);
x=poly(0,'x');
M=[x;x^2;2+x^3]*[1,x-2,x^4];
[Mu,U,rk]=htrianr(M)
det(U)
M*U(:,1:2)
xdel(winsid())

//====================================================
// ../man/fr/polynomials/invr.xml
//====================================================
clear;lines(0);
s=poly(0,'s')
H=[s,s*s+2;1-s,1+s]; invr(H)
[Num,den]=coffg(H);Num/den
H=[1/s,(s+1);1/(s+2),(s+3)/s];invr(H)
xdel(winsid())

//====================================================
// ../man/fr/polynomials/lcmdiag.xml
//====================================================
clear;lines(0);
s=poly(0,'s');
H=[1/s,(s+2)/s/(s+1)^2;1/(s^2*(s+2)),2/(s+2)];
[N,D]=lcmdiag(H);
N/D-H
xdel(winsid())

//====================================================
// ../man/fr/polynomials/lcm.xml
//====================================================
clear;lines(0);
//cas des polynomes
s=poly(0,'s');
p=[s,s*(s+1)^2,s^2*(s+2)];
[pp,fact]=lcm(p);
p.*fact, pp
//cas des entiers
V=int32([2^2*3^5, 2^3*3^2,2^2*3^4*5]);
lcm(V)

xdel(winsid())

//====================================================
// ../man/fr/polynomials/ldiv.xml
//====================================================
clear;lines(0);
wss=ssrand(1,1,3);[a,b,c,d]=abcd(wss);
wtf=ss2tf(wss);
x1=ldiv(numer(wtf),denom(wtf),5)
x2=[c*b;c*a*b;c*a^2*b;c*a^3*b;c*a^4*b]
wssbis=markp2ss(x1',5,1,1);
wtfbis=clean(ss2tf(wssbis))
x3=ldiv(numer(wtfbis),denom(wtfbis),5)
xdel(winsid())

//====================================================
// ../man/fr/polynomials/numer.xml
//====================================================

//====================================================
// ../man/fr/polynomials/pdiv.xml
//====================================================
clear;lines(0);
x=poly(0,'x');
p1=(1+x^2)*(1-x);p2=1-x;
[r,q]=pdiv(p1,p2)
p2*q-p1
p2=1+x;
[r,q]=pdiv(p1,p2)
p2*q+r-p1
xdel(winsid())

//====================================================
// ../man/fr/polynomials/pol2des.xml
//====================================================
clear;lines(0);
s=poly(0,'s');
G=[1,s;1+s^2,3*s^3];[N,B,C]=pol2des(G);
G1=clean(C*inv(s*N-eye())*B),G2=numer(G1)
xdel(winsid())

//====================================================
// ../man/fr/polynomials/pol2str.xml
//====================================================

//====================================================
// ../man/fr/polynomials/polfact.xml
//====================================================

//====================================================
// ../man/fr/polynomials/residu.xml
//====================================================
clear;lines(0);
s=poly(0,'s');
H=[s/(s+1)^2,1/(s+2)];N=numer(H);D=denom(H);
w=residu(N.*horner(N,-s),D,horner(D,-s));  //N(s) N(-s) / D(s) D(-s)
sqrt(sum(w))  // La norme H2
h2norm(tf2ss(H))
//
p=(s-1)*(s+1)*(s+2)*(s+10);a=(s-5)*(s-1)*(s*s)*((s+1/2)**2);
b=(s-3)*(s+2/5)*(s+3);
residu(p,a,b)+531863/4410    // Exact
z=poly(0,'z');a=z^3+0.7*z^2+0.5*z-0.3;b=z^3+0.3*z^2+0.2*z+0.1;
atild=gtild(a,'d');btild=gtild(b,'d');
residu(b*btild,z*a,atild)-2.9488038   // Exact
a=a+0*%i;b=b+0*%i;
real(residu(b*btild,z*a,atild)-2.9488038) // Cas complexe
xdel(winsid())

//====================================================
// ../man/fr/polynomials/roots.xml
//====================================================
clear;lines(0);
p=poly([0,10,1+%i,1-%i],'x');
roots(p)
A=rand(3,3);roots(poly(A,'x'))    // comparaison via le polynôme caractéristique
spec(A) 
xdel(winsid())

//====================================================
// ../man/fr/polynomials/routh_t.xml
//====================================================

//====================================================
// ../man/fr/polynomials/rowcompr.xml
//====================================================

//====================================================
// ../man/fr/polynomials/sfact.xml
//====================================================
clear;lines(0);
// polynôme simple
z=poly(0,'z');
p=(z-1/2)*(2-z)
w=sfact(p);
w*numer(horner(w,1/z))
// exemple matriciel
F1=[z-1/2,z+1/2,z^2+2;1,z,-z;z^3+2*z,z,1/2-z];
P=F1*gtild(F1,'d');  // P est symétrique
F=sfact(P)    
roots(det(P))  
roots(det(gtild(F,'d')))  // Les racines stables
roots(det(F))             // Les racines antistables
clean(P-F*gtild(F,'d'))
// Utilisation en temps continu
s=poly(0,'s');
p=-3*(s+(1+%i))*(s+(1-%i))*(s+0.5)*(s-0.5)*(s-(1+%i))*(s-(1-%i));p=real(p);
// p(s) = polynôme dans s^2, cherche un f stable tel que p=f(s)*f(-s) 
w=horner(p,(1-s)/(1+s));  // transformation bilinéaire w=p((1-s)/(1+s))
wn=numer(w);              // prend le numérateur
fn=sfact(wn);f=numer(horner(fn,(1-s)/(s+1))); // Factorisation et transformation inverse
f=f/sqrt(horner(f*gtild(f,'c'),0));f=f*sqrt(horner(p,0));      // normalisation
roots(f)    // f est stable
clean(f*gtild(f,'c')-p)    //f(s)*f(-s) est égal à p(s) 
xdel(winsid())

//====================================================
// ../man/fr/polynomials/simp_mode.xml
//====================================================
clear;lines(0);
s=poly(0,'s');
mod=simp_mode()
simp_mode(%f);hns=s/s
simp_mode(%t);hns=s/s
simp_mode(mod);

xdel(winsid())

//====================================================
// ../man/fr/polynomials/simp.xml
//====================================================
clear;lines(0);
s=poly(0,'s');
[n,d]=simp((s+1)*(s+2),(s+1)*(s-2))

simp_mode(%F);hns=s/s
simp_mode(%T);hns=s/s

xdel(winsid())

//====================================================
// ../man/fr/polynomials/sylm.xml
//====================================================

//====================================================
// ../man/fr/polynomials/systmat.xml
//====================================================

//====================================================
// ../man/fr/programming/abort.xml
//====================================================

//====================================================
// ../man/fr/programming/ans.xml
//====================================================

//====================================================
// ../man/fr/programming/backslash.xml
//====================================================
clear;lines(0);
A=rand(3,2);b=[1;1;1]; x=A\b; y=pinv(A)*b;  x-y
A=rand(2,3);b=[1;1]; x=A\b; y=pinv(A)*b; x-y, A*x-b, A*y-b
A=rand(3,1)*rand(1,2); b=[1;1;1]; x=A\b; y=pinv(A)*b; A*x-b, A*y-b
A=rand(2,1)*rand(1,3); b=[1;1]; x=A\b; y=pinv(A)*b; A*x-b, A*y-b 
xdel(winsid())

//====================================================
// ../man/fr/programming/bool2s.xml
//====================================================
clear;lines(0);
bool2s([%t %t %f %t])
bool2s([2.3 0 10 -1])
xdel(winsid())

//====================================================
// ../man/fr/programming/boolean.xml
//====================================================
clear;lines(0);
[1,2]==[1,3]
[1,2]==1
a=1:5; a(a>2)
xdel(winsid())

//====================================================
// ../man/fr/programming/brackets.xml
//====================================================
clear;lines(0);
[6.9,9.64; sqrt(-1) 0]
[1 +%i  2 -%i  3]
[]
['ceci est';'un vecteur';'de chaines de caracteres']
s=poly(0,'s');[1/s,2/s]
[tf2ss(1/s),tf2ss(2/s)]

[u,s]=schur(rand(3,3))
xdel(winsid())

//====================================================
// ../man/fr/programming/break.xml
//====================================================
clear;lines(0);
k=0; while 1==1, k=k+1; if k > 100 then  break,end; end
xdel(winsid())

//====================================================
// ../man/fr/programming/call.xml
//====================================================

//====================================================
// ../man/fr/programming/case.xml
//====================================================

//====================================================
// ../man/fr/programming/clearglobal.xml
//====================================================
clear;lines(0);
global a b c
a=1;b=2;c=3;
who('global')
clearglobal b
who('global')
xdel(winsid())

//====================================================
// ../man/fr/programming/clear.xml
//====================================================

//====================================================
// ../man/fr/programming/colon.xml
//====================================================

//====================================================
// ../man/fr/programming/comma.xml
//====================================================
clear;lines(0);
a=[1,2,3;4,5,6];
a=1,b=1;c=2
xdel(winsid())

//====================================================
// ../man/fr/programming/comments.xml
//====================================================

//====================================================
// ../man/fr/programming/continue.xml
//====================================================
clear;lines(0);

  for k=1:10,K=k;if k>2&k<=8 then continue,disp('hello'),end,k,end

  for j=1:2
    x=[];
    for k=1:10,if k>j+1&k<=8 then continue,end,x=[x,k];end
    x
  end

xdel(winsid())

//====================================================
// ../man/fr/programming/date.xml
//====================================================
clear;lines(0);
date()
xdel(winsid())

//====================================================
// ../man/fr/programming/debug.xml
//====================================================

//====================================================
// ../man/fr/programming/definedfields.xml
//====================================================
clear;lines(0);

l=list(1);l(3)=5 
k=definedfields(l)

t=tlist('x');t(5)=4
definedfields(t)

m=mlist(['m','a','b']);m.b='sdfgfgd' 
definedfields(m)  

xdel(winsid())

//====================================================
// ../man/fr/programming/dot.xml
//====================================================
clear;lines(0);
1.345
x=[1 2 3];x.^2 .*x // il faut absolument mettre un espace entre le 2 et le point
[123,..
 456]
xdel(winsid())

//====================================================
// ../man/fr/programming/elseif.xml
//====================================================

//====================================================
// ../man/fr/programming/else.xml
//====================================================

//====================================================
// ../man/fr/programming/empty.xml
//====================================================
clear;lines(0);
s=poly(0,'s'); A = [s, s+1]; 
A+[], A*[]
A=rand(2,2); AA=A([],1), size(AA)
svd([])
w=ssrand(2,2,2); wr=[]*w; size(wr), w1=ss2tf(wr), size(w1)
xdel(winsid())

//====================================================
// ../man/fr/programming/end.xml
//====================================================

//====================================================
// ../man/fr/programming/equal.xml
//====================================================
clear;lines(0);

a=sin(3.2)
[u,s]=schur(rand(3,3))
[1:10]==4
1~=2
  
xdel(winsid())

//====================================================
// ../man/fr/programming/errcatch.xml
//====================================================

//====================================================
// ../man/fr/programming/errclear.xml
//====================================================

//====================================================
// ../man/fr/programming/error.xml
//====================================================

//====================================================
// ../man/fr/programming/etime.xml
//====================================================
clear;lines(0);

t1=[2004 06 10 17 00 12.345]
t2=[2004 06 10 17 01 13.965]
E1=etime(t2,t1)
t1=[2004 06 24 162 5 10 17 00 12.345]
t2=[2004 06 24 162 5 10 17 01 13.965]
E2=etime(t2,t1)

xdel(winsid())

//====================================================
// ../man/fr/programming/evstr.xml
//====================================================
clear;lines(0);
    a=1; b=2; Z=['a','b'] ; evstr(Z) 

    a=1; b=2; Z=list(['%(1)','%(1)-%(2)'],['a+1','b+1']);
    evstr(Z)
xdel(winsid())

//====================================================
// ../man/fr/programming/execstr.xml
//====================================================
clear;lines(0);
execstr('a=1') // affecte a=1.
execstr('1+1') // ne fait rien de particulier (alors que evstr('1+1') renvoie 2)

execstr(['if %t then';
         '  a=1';
         '  b=a+1';
         'else'
         ' b=0'
         'end'])

execstr('a=zzzzzzz','errcatch')
execstr('a=zzzzzzz','errcatch','m')
xdel(winsid())

//====================================================
// ../man/fr/programming/exec.xml
//====================================================
clear;lines(0);
    // création d'un script
    write(TMPDIR+'/myscript','a=1;b=2')
    // on l'exécute
    exec(TMPDIR+'/myscript')
    who

    // création d'une function
    function y=foo(x),a=x+1;y=a^2,endfunction
    clear a b
    // appel à la fonction
    foo(1)
    // a est une variable créée dans l'environnement de la fonction foo
    // elle est détruite à la sortie de la fonction.
    who 

    x=1 // on crée la variable x pour qu'elle soit connue à l'exécution de foo
    exec(foo)
    // a et y ont été créées dans l'environnement courant
    who
xdel(winsid())

//====================================================
// ../man/fr/programming/exists.xml
//====================================================
clear;lines(0);
deff('foo(x)',..
['disp([exists(''a12''),exists(''a12'',''local'')])'
 'disp([exists(''x''),exists(''x'',''local'')])'])
foo(1)
a12=[];foo(1)
xdel(winsid())

//====================================================
// ../man/fr/programming/exit.xml
//====================================================

//====================================================
// ../man/fr/programming/external.xml
//====================================================

//====================================================
// ../man/fr/programming/extraction.xml
//====================================================
clear;lines(0);
    // CAS DES MATRICES
    a=[1 2 3;4 5 6]
    a(1,2)
    a([1 1],2)
    a(:,1)
    a(:,3:-1:1)
    a(1)
    a(6)
    a(:)
    a([%t %f %f %t])
    a([%t %f],[2 3])
    a(1:2,$-1)
    a($:-1:1,2)
    a($)
    //
    x='test'
    x([1 1;1 1;1 1])
    //
    b=[1/%s,(%s+1)/(%s-1)]
    b(1,1)
    b(1,$)
    b(2) // le numérateur
    // CAS des LISTES (types LIST et TLIST)
    l=list(1,'qwerw',%s)
    l(1)
    [a,b]=l([3 2])
    l($)
    x=tlist(l(2:3)) // construction d'une TLIST avec les deux derniers termes de l
    //
    dts=list(1,tlist(['x';'a';'b'],10,[2 3]));
    dts(2)('a')
    dts(2)('b')(1,2)
    [a,b]=dts(2)(['a','b'])

xdel(winsid())

//====================================================
// ../man/fr/programming/feval.xml
//====================================================
clear;lines(0);
deff('[z]=f(x,y)','z=x^2+y^2');
feval(1:10,1:5,f)
deff('[z]=f(x,y)','z=x+%i*y');
feval(1:10,1:5,f)
feval(1:10,1:5,'parab')   // voir le fichier ffeval.f
feval(1:10,'parab')
// Pour le lien dynamique (voir l'exemple ftest dans ffeval.f)
// on peut utiliser la commande "link" 
xdel(winsid())

//====================================================
// ../man/fr/programming/find.xml
//====================================================
clear;lines(0);

A=rand(1,20);
w=find(A<0.4)
A(w)
w=find(A>100) 

B=rand(1,20);
w=find(B<0.4,2) //at most 2 returned values

H=rand(4,3,5); //an hypermatrix
[i,j,k]=find(H>0.9)  

H(i(1),j(1),k(1))

xdel(winsid())

//====================================================
// ../man/fr/programming/format.xml
//====================================================
clear;lines(0);

x=rand(1,5);
format('v',10);x
format(20);x
format('e',10);x
format(20);x

x=[100 %eps];
format('e',10);x
format('v',10);x

format("v")

xdel(winsid())

//====================================================
// ../man/fr/programming/fort.xml
//====================================================

//====================================================
// ../man/fr/programming/for.xml
//====================================================
clear;lines(0);
n=5;
for i = 1:n, for j = 1:n, a(i,j) = 1/(i+j-1);end;end
for j = 2:n-1, a(j,j) = j; end; a
for  e=eye(3,3),e,end  
for v=a, write(6,v),end        
for j=1:n,v=a(:,j), write(6,v),end 
for l=list(1,2,'example'); l,end 
xdel(winsid())

//====================================================
// ../man/fr/programming/funptr.xml
//====================================================
clear;lines(0);

// Suppose you want to load some codes via the dynamic 
// loading facilities offers by addinter. By default 
// arguments are passed by values but if you want to 
// pass them by reference you can do the following 
// (name being the scilab name of one of the interfaced 
// routines) :
//
// addinter(files,spnames,fcts)  // args passed by values
// num_interface = floor(funptr(name)/100)
// intppty(num_interface)  // args now passed by reference
//
// Note that if you enter the following
//
// intppty()                
//
// you will see all the interfaces working by reference

xdel(winsid())

//====================================================
// ../man/fr/programming/getdate.xml
//====================================================
clear;lines(0);

w=getdate()
mprintf("Annee:%d,mois:%d,jour:%d",w(1),w(2),w(6));

x=getdate("s")
getdate(x)


xdel(winsid())

//====================================================
// ../man/fr/programming/getenv.xml
//====================================================
clear;lines(0);
 getenv('SCI')
 getenv('FOO','foo') 
xdel(winsid())

//====================================================
// ../man/fr/programming/getfield.xml
//====================================================
clear;lines(0);
l=list(1,'qwerw',%s)
[a,b]=getfield([3 2],l)

a=hypermat([2,2,2],rand(1:2^3));// les hypermatrices sont stockées dans des mlist
a(1) // le terme a(1,1,1)
getfield(1,a) // le premier champ de la mlist
xdel(winsid())

//====================================================
// ../man/fr/programming/getpid.xml
//====================================================
clear;lines(0);
d='SD_'+string(getpid())+'_'
xdel(winsid())

//====================================================
// ../man/fr/programming/getversion.xml
//====================================================
clear;lines(0);

getversion()
[version,opts]=getversion()

xdel(winsid())

//====================================================
// ../man/fr/programming/global.xml
//====================================================
clear;lines(0);
// 1 : l'environnement appelant et une fonction partagent une variable
global a
a=1
deff('y=f1(x)','global a,a=x^2,y=a^2')
f1(2)
a
// 2 : trois fonctions partagent des variables
deff('initdata()','global A C ;A=10,C=30')
deff('letsgo()','global A C ;disp(A) ;C=70')
deff('letsgo1()','global C ;disp(C)')
initdata()
letsgo()
letsgo1()

xdel(winsid())

//====================================================
// ../man/fr/programming/gstacksize.xml
//====================================================

//====================================================
// ../man/fr/programming/hat.xml
//====================================================
clear;lines(0);
2^4
[1 2;2 4]^(1+%i)
s=poly(0,"s");
[1 2 s]^4
[s 1;1  s]^(-1)
xdel(winsid())

//====================================================
// ../man/fr/programming/host.xml
//====================================================
clear;lines(0);
host("ls $SCI/demos");
host("emacs $SCI/demos/wheel2/Makefile");
deff('wd=getdir()','if MSDOS then host(''cd>''+TMPDIR+''\path'');..
                 else host(''pwd>''+TMPDIR+''/path'');end..
      wd=read(TMPDIR+''/path'',1,1,''(a)'')')
wd=getdir()
xdel(winsid())

//====================================================
// ../man/fr/programming/hypermatrices.xml
//====================================================
clear;lines(0);
a(1,1,1,1:2)=[1 2]
a=[1 2;3 4];a(:,:,2)=rand(2,2)
a(1,1,:)
[a a]
xdel(winsid())

//====================================================
// ../man/fr/programming/hypermat.xml
//====================================================
clear;lines(0);
M=hypermat([2 3 2 2],1:24)
xdel(winsid())

//====================================================
// ../man/fr/programming/iconvert.xml
//====================================================
clear;lines(0);
b=int32([1 -120 127 312])
y=iconvert(b,8)

xdel(winsid())

//====================================================
// ../man/fr/programming/ieee.xml
//====================================================
clear;lines(0);
    ieee(1);1/0
    ieee(2);1/0,log(0)
xdel(winsid())

//====================================================
// ../man/fr/programming/if.xml
//====================================================
clear;lines(0);
   i=2
   for j = 1:3, 
      if i == j then
        a(i,j) = 2; 
      elseif abs(i-j) == 1 then 
        a(i,j) = -1; 
      else a(i,j) = 0;
      end,
   end
xdel(winsid())

//====================================================
// ../man/fr/programming/insertion.xml
//====================================================
clear;lines(0);
    // CAS DES MATRICES
    a=[1 2 3;4 5 6]
    a(1,2)=10
    a([1 1],2)=[-1;-2]
    a(:,1)=[8;5]
    a(1,3:-1:1)=[77 44 99]
    a(1)=%s
    a(6)=%s+1
    a(:)=1:6
    a([%t %f],1)=33
    a(1:2,$-1)=[2;4]
    a($:-1:1,1)=[8;7]
    a($)=123
    //
    x='test'
    x([4 5])=['4','5']
    //
    b=[1/%s,(%s+1)/(%s-1)]
    b(1,1)=0
    b(1,$)=b(1,$)+1
    b(2)=[1 2] // le numérateur
    // CAS des LISTES (types LIST et TLIST)
    l=list(1,'qwerw',%s)
    l(1)='Changed'
    l(0)='Added'
    l(6)=['one more';'added']
    //
    //
    dts=list(1,tlist(['x';'a';'b'],10,[2 3]));
    dts(2)('a')=33
    dts(2)('b')(1,2)=-100
xdel(winsid())

//====================================================
// ../man/fr/programming/intppty.xml
//====================================================

//====================================================
// ../man/fr/programming/inttype.xml
//====================================================
clear;lines(0);
x=uint16(1:10);
inttype(x)
xdel(winsid())

//====================================================
// ../man/fr/programming/inv_coeff.xml
//====================================================
clear;lines(0);
A=int(10*rand(2,6))
// construction d'une matrice polynomiale de degré 1
P=inv_coeff(A,1)
norm(coeff(P)-A)
// utilisation de la valeur par défaut du degré
P1=inv_coeff(A)
norm(coeff(P1)-A)
xdel(winsid())

//====================================================
// ../man/fr/programming/iserror.xml
//====================================================

//====================================================
// ../man/fr/programming/isglobal.xml
//====================================================
clear;lines(0);
isglobal(1)
global a
isglobal(a)
xdel(winsid())

//====================================================
// ../man/fr/programming/lasterror.xml
//====================================================
clear;lines(0);

ierr=execstr('a=zzzzzzz','errcatch')
if ierr>0 then disp(lasterror()),end

xdel(winsid())

//====================================================
// ../man/fr/programming/left.xml
//====================================================
clear;lines(0);
[6.9,9.64; sqrt(-1) 0]
[1 +%i  2 -%i  3]
[]
['ceci est';'un vecteur';'de chaines de caracteres']
s=poly(0,'s');[1/s,2/s]
[tf2ss(1/s),tf2ss(2/s)]

[u,s]=schur(rand(3,3))
xdel(winsid())

//====================================================
// ../man/fr/programming/less.xml
//====================================================

//====================================================
// ../man/fr/programming/list.xml
//====================================================
clear;lines(0);
x=list(1,2,3);
x(4)=10;
x(10)='a'
xdel(winsid())

//====================================================
// ../man/fr/programming/lsslist.xml
//====================================================

//====================================================
// ../man/fr/programming/lstcat.xml
//====================================================
clear;lines(0);
lstcat(list(1,2,3),33,list('foo',%s))
lstcat(1,2,3)
xdel(winsid())

//====================================================
// ../man/fr/programming/matrices.xml
//====================================================
clear;lines(0);
E=[1,2;3,4]
E=[%T,%F;1==1,1~=1]
s=poly(0,'s');E=[s,s^2;1,1+s]
E=[1/s,0;s,1/(s+1)]
E=['A11','A12';'A21','A22']
xdel(winsid())

//====================================================
// ../man/fr/programming/matrix.xml
//====================================================
clear;lines(0);
a=[1 2 3;4 5 6]
matrix(a,1,6)
matrix(a,1,-1)
matrix(a,3,2)
xdel(winsid())

//====================================================
// ../man/fr/programming/mlist.xml
//====================================================
clear;lines(0);
M=mlist(['V','name','value'],['a','b','c'],[1 2 3]);
// définition de la fonction d'affichage
deff('%V_p(M)','disp(M.name+'':''+string(M.value))')
// définition de la fonction d'extraction
deff('r=%V_e(i,M)',..
'r=mlist([''V'',''name'',''value''],M.name(i),M.value(i))')
M(2) // Le deuxième terme du vecteur M
M(2).value 

// définition de M comme une tlist
M=tlist(['V','name','value'],['a','b','c'],[1 2 3]);
M(2)

M('name')

// avec deux indices
M=mlist(['V','name','value'],['a','b';'c' 'd'],[1 2;3 4]);
deff('r=%V_e(varargin)',[
    'M=varargin($)';
    'H=[''V'',''name'',''value'']'
    'r=mlist(H,M.name(varargin(1:$-1)),M.value(varargin(1:$-1)))'])

M(:,2)
// tableau multidimensionnel
str=['a','b','c','d','e','f','g','h'];
n=hypermat([2,2,2],str);
v=hypermat([2,2,2],1:8);
M=mlist(['V','name','value'],n,v);
M(1,1:2,2)
xdel(winsid())

//====================================================
// ../man/fr/programming/mode.xml
//====================================================

//====================================================
// ../man/fr/programming/mtlb_mode.xml
//====================================================

//====================================================
// ../man/fr/programming/names.xml
//====================================================
clear;lines(0);
// Noms valides
%eps
A1=123
#Color=8
My_Special_Color_Table=rand(10,3)
// Noms invalides
//1A , b%, .C
xdel(winsid())

//====================================================
// ../man/fr/programming/null.xml
//====================================================
clear;lines(0);
l=list(1,2,3);
l(2)=null() // get list(1,3)
xdel(winsid())

//====================================================
// ../man/fr/programming/overloading.xml
//====================================================
clear;lines(0);

//DISPLAY
deff('[]=%tab_p(l)','disp([['' '';l(3)] [l(2);string(l(4))]])')
tlist('tab',['a','b'],['x';'y'],rand(2,2))

//OPERATOR
deff('x=%c_a_s(a,b)','x=a+string(b)')
's'+1

//FUNCTION
deff('x=%c_sin(a)','x=''sin(''+a+'')''')
sin('2*x')
  
xdel(winsid())

//====================================================
// ../man/fr/programming/parents.xml
//====================================================
clear;lines(0);
3^(-1)
x=poly(0,"x");
//
(x+10)/2
i3=eye(3,3)
//
a=[1 2 3;4 5 6;7 8 9],a(1,3),a([1 3],:),a(:,3)
a(:,3)=[]
a(1,$)=33
a(2,[$ $-1])
a(:,$+1)=[10;11;12]
//
w=ssrand(2,2,2);ssprint(w)
ssprint(w(:,1))
ss2tf(w(:,1)) 
//
l=list(1,2,3,4)
[a,b,c,d]=l(:)
l($+1)='new'
//
v=%t([1 1 1 1 1])
//
[x,y,z]=(1,2,3)
xdel(winsid())

//====================================================
// ../man/fr/programming/pause.xml
//====================================================

//====================================================
// ../man/fr/programming/percent.xml
//====================================================
clear;lines(0);
x1=tlist('x',1,2);
x2=tlist('x',2,3);
deff('x=%xmx(x1,x2)','x=list(''x'',x1(2)*x2(2),x2(3)*x2(3))');
x1*x2
xdel(winsid())

//====================================================
// ../man/fr/programming/plus.xml
//====================================================
clear;lines(0);
[1,2]+1
[]+2
s=poly(0,"s");
s+2
1/s+2
"conca"+"tener"
xdel(winsid())

//====================================================
// ../man/fr/programming/poly.xml
//====================================================
clear;lines(0);
s=poly(0,"s");p=1+s+2*s^2;
A=rand(2,2);poly(A,"x")
xdel(winsid())

//====================================================
// ../man/fr/programming/power.xml
//====================================================
clear;lines(0);
A=[1 2;3 4];
A^2.5,
A.^2.5
(1:10)^2
(1:10).^2

s=poly(0,'s')
s^(1:10)
xdel(winsid())

//====================================================
// ../man/fr/programming/predef.xml
//====================================================

//====================================================
// ../man/fr/programming/pwd.xml
//====================================================
clear;lines(0);
    pwd
    x=pwd()
xdel(winsid())

//====================================================
// ../man/fr/programming/quit.xml
//====================================================

//====================================================
// ../man/fr/programming/quote.xml
//====================================================
clear;lines(0);
[1+%i, 2]'
[1+%i, 2].'
x='Ceci est une chaîne de caractères'
'Et Dieu dit : ''que la lumière soit'''
xdel(winsid())

//====================================================
// ../man/fr/programming/rational.xml
//====================================================
clear;lines(0);
s=poly(0,'s');
W=[1/s,1/(s+1)]
W'*W
Num=[s,s+2;1,s];Den=[s*s,s;s,s*s];
tlist(['r','num','den','dt'],Num,Den,[])
H=Num./Den
syslin('c',Num,Den)
syslin('c',H)
[Num1,Den1]=simp(Num,Den)
xdel(winsid())

//====================================================
// ../man/fr/programming/resume.xml
//====================================================

//====================================================
// ../man/fr/programming/return.xml
//====================================================

//====================================================
// ../man/fr/programming/rlist.xml
//====================================================

//====================================================
// ../man/fr/programming/sciargs.xml
//====================================================

//====================================================
// ../man/fr/programming/select.xml
//====================================================
clear;lines(0);
while %t do
  n=round(10*rand(1,1))
  select n
  case 0 then 
    disp(0)
  case 1 then
    disp(1)
  else
    break
  end
end
  
xdel(winsid())

//====================================================
// ../man/fr/programming/semicolon.xml
//====================================================
clear;lines(0);
a=[1,2,3;4,5,6];
a=1;b=1,c=2
xdel(winsid())

//====================================================
// ../man/fr/programming/semi.xml
//====================================================
clear;lines(0);
a=[1,2,3;4,5,6];
a=1;b=1,c=2
xdel(winsid())

//====================================================
// ../man/fr/programming/setfield.xml
//====================================================
clear;lines(0);
l=list(1,'qwerw',%s)
l(1)='change'
l(0)='ajoute'
l(6)=['un de plus';'ajoute']
//

a=hypermat([2,2,2],rand(1:2^3));// les hypermatrices sont des mlist
setfield(3,1:8,a);a // affectation du champ
xdel(winsid())

//====================================================
// ../man/fr/programming/slash.xml
//====================================================

//====================================================
// ../man/fr/programming/stacksize.xml
//====================================================

//====================================================
// ../man/fr/programming/star.xml
//====================================================

//====================================================
// ../man/fr/programming/symbols.xml
//====================================================

//====================================================
// ../man/fr/programming/testmatrix.xml
//====================================================

//====================================================
// ../man/fr/programming/then.xml
//====================================================

//====================================================
// ../man/fr/programming/tic.xml
//====================================================
clear;lines(0);

tic();
realtimeinit(1);
realtime(0);
realtime(10);
toc();

xdel(winsid())

//====================================================
// ../man/fr/programming/tilda.xml
//====================================================

//====================================================
// ../man/fr/programming/tlist.xml
//====================================================

//====================================================
// ../man/fr/programming/toc.xml
//====================================================
clear;lines(0);

tic();
realtimeinit(1);
realtime(0);
realtime(10);
toc();

xdel(winsid())

//====================================================
// ../man/fr/programming/typename.xml
//====================================================

//====================================================
// ../man/fr/programming/type.xml
//====================================================

//====================================================
// ../man/fr/programming/user.xml
//====================================================

//====================================================
// ../man/fr/programming/varn.xml
//====================================================
clear;lines(0);
s=poly(0,'s');p=[s^2+1,s];
varn(p)
varn(p,'x')
xdel(winsid())

//====================================================
// ../man/fr/programming/what.xml
//====================================================

//====================================================
// ../man/fr/programming/whereami.xml
//====================================================
clear;lines(0);
deff('y=test(a)',['y=sin(a)+1';
                  'y=t1(y)';
                  'y=y+1'])
deff('y=t1(y)',['y=y^2';'whereami()'])
test(1)
xdel(winsid())

//====================================================
// ../man/fr/programming/whereis.xml
//====================================================

//====================================================
// ../man/fr/programming/where.xml
//====================================================

//====================================================
// ../man/fr/programming/while.xml
//====================================================
clear;lines(0);
    e=1; a=1; k=1;
    while norm(a-(a+e),1) > %eps, e=e/2; k=k+1; end
    e,k
xdel(winsid())

//====================================================
// ../man/fr/programming/whos.xml
//====================================================
clear;lines(0);
whos()

whos -type boolean

whos -name %
xdel(winsid())

//====================================================
// ../man/fr/programming/who_user.xml
//====================================================

//====================================================
// ../man/fr/programming/who.xml
//====================================================

//====================================================
// ../man/fr/robust/augment.xml
//====================================================
clear;lines(0);

G=ssrand(2,3,2); //Plant
K=ssrand(3,2,2); //Compensator
[P,r]=augment(G,'T');
T=lft(P,r,K);   //Complementary sensitivity function
Ktf=ss2tf(K);Gtf=ss2tf(G);
Ttf=ss2tf(T);T11=Ttf(1,1);
Oloop=Gtf*Ktf;
Tn=Oloop*inv(eye(Oloop)+Oloop);
clean(T11-Tn(1,1));
//
[Pi,r]=augment(G,'T','i');
T1=lft(Pi,r,K);T1tf=ss2tf(T1); //Input Complementary sensitivity function
Oloop=Ktf*Gtf;
T1n=Oloop*inv(eye(Oloop)+Oloop);
clean(T1tf(1,1)-T1n(1,1))

xdel(winsid())

//====================================================
// ../man/fr/robust/bstap.xml
//====================================================

//====================================================
// ../man/fr/robust/ccontrg.xml
//====================================================

//====================================================
// ../man/fr/robust/colinout.xml
//====================================================

//====================================================
// ../man/fr/robust/copfac.xml
//====================================================

//====================================================
// ../man/fr/robust/dcf.xml
//====================================================

//====================================================
// ../man/fr/robust/des2ss.xml
//====================================================
clear;lines(0);

s=poly(0,'s');G=[1/(s-1),s;1,2/s^3];
S1=tf2des(G);S2=tf2des(G,"withD");
W1=des2ss(S1);W2=des2ss(S2);
clean(ss2tf(W1))
clean(ss2tf(W2))

xdel(winsid())

//====================================================
// ../man/fr/robust/dhinf.xml
//====================================================
clear;lines(0);

//example from Niconet report SLWN1999-12
//Hinf
A=[-0.7  0    0.3  0   -0.5 -0.1
   -0.6  0.2 -0.4 -0.3  0    0
   -0.5  0.7 -0.1  0    0   -0.8
   -0.7  0    0   -0.5 -1    0
    0    0.3  0.6 -0.9  0.1 -0.4
    0.5 -0.8  0    0    0.2 -0.9];
B=[-1 -2 -2  1  0
    1  0  1 -2  1
   -3 -4  0  2 -2
    1 -2  1  0 -1
    0  1 -2  0  3
    1  0  3 -1 -2];
C=[ 1 -1  2 -2  0 -3
   -3  0  1 -1  1  0
    0  2  0 -4  0 -2
    1 -3  0  0  3  1
    0  1 -2  1  0 -2];
D=[1 -1 -2  0  0
   0  1  0  1  0
   2 -1 -3  0  1
   0  1  0  1 -1
   0  0  1  2  1];

ncon=2
nmeas=2
gam=111.30;
[AK,BK,CK,DK] = dhinf(A,B,C,D,ncon,nmeas,gam)

xdel(winsid())

//====================================================
// ../man/fr/robust/dhnorm.xml
//====================================================

//====================================================
// ../man/fr/robust/dtsi.xml
//====================================================

//====================================================
// ../man/fr/robust/fourplan.xml
//====================================================

//====================================================
// ../man/fr/robust/fspecg.xml
//====================================================

//====================================================
// ../man/fr/robust/fstabst.xml
//====================================================
clear;lines(0);

ny=2;nu=3;nx=4;
P22=ssrand(ny,nu,nx);
bigQ=rand(nx+nu,nx+nu);bigQ=bigQ*bigQ';
bigR=rand(nx+ny,nx+ny);bigR=bigR*bigR';
[P,r]=lqg2stan(P22,bigQ,bigR);
J=fstabst(P,r);
Q=ssrand(nu,ny,1);Q('A')=-1;  //Stable Q
K=lft(J,r,Q);
A=h_cl(P,r,K); spec(A)

xdel(winsid())

//====================================================
// ../man/fr/robust/gamitg.xml
//====================================================

//====================================================
// ../man/fr/robust/gcare.xml
//====================================================

//====================================================
// ../man/fr/robust/gfare.xml
//====================================================

//====================================================
// ../man/fr/robust/gtild.xml
//====================================================
clear;lines(0);

//Continuous time
s=poly(0,'s');G=[s,s^3;2+s^3,s^2-5]
Gt=gtild(G,'c')
Gt-horner(G,-s)'   //continuous-time interpretation
Gt=gtild(G,'d');
Gt-horner(G,1/s)'*s^3  //discrete-time interpretation
G=ssrand(2,2,3);Gt=gtild(G);   //State-space (G is cont. time by default)
clean((horner(ss2tf(G),-s))'-ss2tf(Gt))   //Check
// Discrete-time 
z=poly(0,'z');
Gss=ssrand(2,2,3);Gss('dt')='d'; //discrete-time
Gss(5)=[1,2;0,1];   //With a constant D matrix
G=ss2tf(Gss);Gt1=horner(G,1/z)';
Gt=gtild(Gss);
Gt2=clean(ss2tf(Gt)); clean(Gt1-Gt2)  //Check
//Improper systems
z=poly(0,'z');
Gss=ssrand(2,2,3);Gss(7)='d'; //discrete-time
Gss(5)=[z,z^2;1+z,3];    //D(z) is polynomial 
G=ss2tf(Gss);Gt1=horner(G,1/z)';  //Calculation in transfer form
Gt=gtild(Gss);    //..in state-space 
Gt2=clean(ss2tf(Gt));clean(Gt1-Gt2)  //Check

xdel(winsid())

//====================================================
// ../man/fr/robust/h2norm.xml
//====================================================

//====================================================
// ../man/fr/robust/hankelsv.xml
//====================================================
clear;lines(0);

A=diag([-1,-2,-3]);
sl=syslin('c',A,rand(3,2),rand(2,3));[nk2,W]=hankelsv(sl)
[Q,M]=pbig(W,nk2(2)-%eps,'c');
slr=projsl(sl,Q,M);hankelsv(slr)

xdel(winsid())

//====================================================
// ../man/fr/robust/h_cl.xml
//====================================================

//====================================================
// ../man/fr/robust/h_inf_st.xml
//====================================================

//====================================================
// ../man/fr/robust/h_inf.xml
//====================================================

//====================================================
// ../man/fr/robust/hinf.xml
//====================================================
clear;lines(0);

//example from Niconet report SLWN1999-12
//Hinf
A=[-1  0  4  5 -3 -2
   -2  4 -7 -2  0  3
   -6  9 -5  0  2 -1
   -8  4  7 -1 -3  0
    2  5  8 -9  1 -4
    3 -5  8  0  2 -6];

B=[-3 -4 -2  1  0
    2  0  1 -5  2
   -5 -7  0  7 -2
    4 -6  1  1 -2
   -3  9 -8  0  5
    1 -2  3 -6 -2];

C=[ 1 -1  2 -4  0 -3
   -3  0  5 -1  1  1
   -7  5  0 -8  2 -2
    9 -3  4  0  3  7
    0  1 -2  1 -6 -2];

D=[ 1 -2 -3  0  0
    0  4  0  1  0
    5 -3 -4  0  1
    0  1  0  1 -3
    0  0  1  7  1];
Gamma=10.18425636157899;
[AK,BK,CK,DK] = hinf(A,B,C,D,2,2,Gamma)

xdel(winsid())

//====================================================
// ../man/fr/robust/h_norm.xml
//====================================================

//====================================================
// ../man/fr/robust/lcf.xml
//====================================================

//====================================================
// ../man/fr/robust/leqr.xml
//====================================================

//====================================================
// ../man/fr/robust/lft.xml
//====================================================
clear;lines(0);

s=poly(0,'s');
P=[1/s, 1/(s+1); 1/(s+2),2/s]; K= 1/(s-1);
lft(P,K)
lft(P,[1,1],K)
P(1,1)+P(1,2)*K*inv(1-P(2,2)*K)*P(2,1)   //Numerically dangerous!
ss2tf(lft(tf2ss(P),tf2ss(K)))
lft(P,-1)
f=[0,0;0,1];w=P/.f; w(1,1)
//Improper plant (PID control)
W=[1,1;1,1/(s^2+0.1*s)];K=1+1/s+s
lft(W,[1,1],K); ss2tf(lft(tf2ss(W),[1,1],tf2ss(K)))

xdel(winsid())

//====================================================
// ../man/fr/robust/linfn.xml
//====================================================

//====================================================
// ../man/fr/robust/linf.xml
//====================================================

//====================================================
// ../man/fr/robust/lqg_ltr.xml
//====================================================

//====================================================
// ../man/fr/robust/macglov.xml
//====================================================

//====================================================
// ../man/fr/robust/mucomp.xml
//====================================================

//====================================================
// ../man/fr/robust/nehari.xml
//====================================================

//====================================================
// ../man/fr/robust/parrot.xml
//====================================================

//====================================================
// ../man/fr/robust/riccati.xml
//====================================================

//====================================================
// ../man/fr/robust/ric_desc.xml
//====================================================

//====================================================
// ../man/fr/robust/rowinout.xml
//====================================================

//====================================================
// ../man/fr/robust/sensi.xml
//====================================================
clear;lines(0);

G=ssrand(1,1,3);K=ssrand(1,1,3);
[Se,Re,Te]=sensi(G,K);
Se1=inv(eye()+G*K);  //Other way to compute
ss2tf(Se)    //Se seen in transfer form
ss2tf(Se1)
ss2tf(Te)
ss2tf(G*K*Se1)
[Si,Ri,Ti]=sensi(G,K,'i');
w1=[ss2tf(Si);ss2tf(Ri);ss2tf(Ti)]
w2=[ss2tf(inv(eye()+K*G));ss2tf(G*inv(eye()+K*G));ss2tf(K*G*inv(eye()+K*G))];
clean(w1-w2)

xdel(winsid())

//====================================================
// ../man/fr/robust/tf2des.xml
//====================================================
clear;lines(0);

s=poly(0,'s');
G=[1/(s-1),s;1,2/s^3];
S1=tf2des(G);des2tf(S1)
S2=tf2des(G,"withD");des2tf(S2)

xdel(winsid())

//====================================================
// ../man/fr/scicos/*.xml
//====================================================

//====================================================
// ../man/fr/signal/analpf.xml
//====================================================
clear;lines(0);

//Evaluate magnitude response of continuous-time system 
hs=analpf(4,'cheb1',[.1 0],5)
fr=0:.1:15;
hf=freq(hs(2),hs(3),%i*fr);
hm=abs(hf);
plot(fr,hm)

xdel(winsid())

//====================================================
// ../man/fr/signal/buttmag.xml
//====================================================
clear;lines(0);

//squared magnitude response of Butterworth filter
h=buttmag(13,300,1:1000);
mag=20*log(h)'/log(10);
plot2d((1:1000)',mag,[2],"011"," ",[0,-180,1000,20])

xdel(winsid())

//====================================================
// ../man/fr/signal/casc.xml
//====================================================
clear;lines(0);

x=[1,2,3;4,5,6;7,8,9;10,11,12]
cels=casc(x,'z')

xdel(winsid())

//====================================================
// ../man/fr/signal/cepstrum.xml
//====================================================
clear;lines(0);

w=0.1:0.1:5;mag=1+abs(sin(w));
fresp=cepstrum(w,mag);
plot2d([w',w'],[mag(:),abs(fresp)])

xdel(winsid())

//====================================================
// ../man/fr/signal/cheb1mag.xml
//====================================================
clear;lines(0);

//Chebyshev; ripple in the passband
n=13;epsilon=0.2;omegac=3;sample=0:0.05:10;
h=cheb1mag(n,omegac,epsilon,sample);
plot(sample,h,'frequencies','magnitude')

xdel(winsid())

//====================================================
// ../man/fr/signal/cheb2mag.xml
//====================================================
clear;lines(0);

//Chebyshev; ripple in the stopband
n=10;omegar=6;A=1/0.2;sample=0.0001:0.05:10;
h2=cheb2mag(n,omegar,A,sample);
plot(sample,log(h2)/log(10),'frequencies','magnitude in dB')
//Plotting of frequency edges
minval=(-maxi(-log(h2)))/log(10);
plot2d([omegar;omegar],[minval;0],[2],"000");
//Computation of the attenuation in dB at the stopband edge
attenuation=-log(A*A)/log(10);
plot2d(sample',attenuation*ones(sample)',[5],"000")

xdel(winsid())

//====================================================
// ../man/fr/signal/chepol.xml
//====================================================
clear;lines(0);

chepol(4,'x')

xdel(winsid())

//====================================================
// ../man/fr/signal/convol.xml
//====================================================
clear;lines(0);

x=1:3;
h1=[1,0,0,0,0];h2=[0,1,0,0,0];h3=[0,0,1,0,0];
x1=convol(h1,x),x2=convol(h2,x),x3=convol(h3,x),
convol(h1+h2+h3,x)
p1=poly(x,'x','coeff')
p2=poly(h1+h2+h3,'x','coeff')
p1*p2

xdel(winsid())

//====================================================
// ../man/fr/signal/corr.xml
//====================================================
clear;lines(0);

x=%pi/10:%pi/10:102.4*%pi;
rand('seed');rand('normal');
y=[.8*sin(x)+.8*sin(2*x)+rand(x);.8*sin(x)+.8*sin(1.99*x)+rand(x)];
c=[];
for j=1:2,for k=1:2,c=[c;corr(y(k,:),y(j,:),64)];end;end;
c=matrix(c,2,128);cov=[];
for j=1:64,cov=[cov;c(:,(j-1)*2+1:2*j)];end;
rand('unif')
//
rand('normal');x=rand(1,256);y=-x;
deff('[z]=xx(inc,is)','z=x(is:is+inc-1)');
deff('[z]=yy(inc,is)','z=y(is:is+inc-1)');
[c,mxy]=corr(x,y,32);
x=x-mxy(1)*ones(x);y=y-mxy(2)*ones(y);  //centring
c1=corr(x,y,32);c2=corr(x,32);
norm(c1+c2,1)
[c3,m3]=corr('fft',xx,yy,256,32);
norm(c1-c3,1)
[c4,m4]=corr('fft',xx,256,32);
norm(m3,1),norm(m4,1)
norm(c3-c1,1),norm(c4-c2,1)
x1=x(1:128);x2=x(129:256);
y1=y(1:128);y2=y(129:256);
w0=0*ones(1:64);   //32 coeffs
[w1,xu]=corr('u',x1,y1,w0);w2=corr('u',x2,y2,w1,xu);
zz=real(fft(w2,1))/256;c5=zz(1:32);
norm(c5-c1,1)
[w1,xu]=corr('u',x1,w0);w2=corr('u',x2,w1,xu);
zz=real(fft(w2,1))/256;c6=zz(1:32);
norm(c6-c2,1)
rand('unif')
// test for Fortran or C external 
//
deff('[y]=xmacro(sec,ist)','y=sin(ist:(ist+sec-1))');
x=xmacro(100,1);
[cc1,mm1]=corr(x,2^3);
[cc,mm]=corr('fft',xmacro,100,2^3);
[cc2,mm2]=corr('fft','corexx',100,2^3);
[maxi(abs(cc-cc1)),maxi(abs(mm-mm1)),maxi(abs(cc-cc2)),maxi(abs(mm-mm2))]

deff('[y]=ymacro(sec,ist)','y=cos(ist:(ist+sec-1))');
y=ymacro(100,1);
[cc1,mm1]=corr(x,y,2^3);
[cc,mm]=corr('fft',xmacro,ymacro,100,2^3);
[cc2,mm2]=corr('fft','corexx','corexy',100,2^3);
[maxi(abs(cc-cc1)),maxi(abs(mm-mm1)),maxi(abs(cc-cc2)),maxi(abs(mm-mm2))]


xdel(winsid())

//====================================================
// ../man/fr/signal/cspect.xml
//====================================================
clear;lines(0);

rand('normal');rand('seed',0);
x=rand(1:1024-33+1);
//make low-pass filter with eqfir
nf=33;bedge=[0 .1;.125 .5];des=[1 0];wate=[1 1];
h=eqfir(nf,bedge,des,wate);
//filter white data to obtain colored data 
h1=[h 0*ones(1:maxi(size(x))-1)];
x1=[x 0*ones(1:maxi(size(h))-1)];
hf=fft(h1,-1);   xf=fft(x1,-1);yf=hf.*xf;y=real(fft(yf,1));
sm=cspect(100,200,'tr',y);
smsize=maxi(size(sm));fr=(1:smsize)/smsize;
plot(fr,log(sm))

xdel(winsid())

//====================================================
// ../man/fr/signal/czt.xml
//====================================================
clear;lines(0);

a=.7*exp(%i*%pi/6);
[ffr,bds]=xgetech(); //preserve current context
rect=[-1.2,-1.2*sqrt(2),1.2,1.2*sqrt(2)];
t=2*%pi*(0:179)/179;xsetech([0,0,0.5,1]);
plot2d(sin(t)',cos(t)',[2],"012",' ',rect)
plot2d([0 real(a)]',[0 imag(a)]',[3],"000")
xsegs([-1.0,0;1.0,0],[0,-1.0;0,1.0])
w0=.93*exp(-%i*%pi/15);w=exp(-(0:9)*log(w0));z=a*w;
zr=real(z);zi=imag(z);
plot2d(zr',zi',[5],"000")
xsetech([0.5,0,0.5,1]);
plot2d(sin(t)',cos(t)',[2],"012",' ',rect)
plot2d([0 real(a)]',[0 imag(a)]',[-1],"000")
xsegs([-1.0,0;1.0,0],[0,-1.0;0,1.0])
w0=w0/(.93*.93);w=exp(-(0:9)*log(w0));z=a*w;
zr=real(z);zi=imag(z);
plot2d(zr',zi',[5],"000")
xsetech(ffr,bds); //restore context

xdel(winsid())

//====================================================
// ../man/fr/signal/dft.xml
//====================================================
clear;lines(0);

n=8;omega = exp(-2*%pi*%i/n);
j=0:n-1;F=omega.^(j'*j);  //Fourier matrix
x=1:8;x=x(:);
F*x
fft(x,-1)
dft(x,-1)
inv(F)*x
fft(x,1)
dft(x,1)

xdel(winsid())

//====================================================
// ../man/fr/signal/ell1mag.xml
//====================================================
clear;lines(0);

deff('[alpha,beta]=alpha_beta(n,m,m1)',...
'if 2*int(n/2)=n then, beta=K1; else, beta=0;end;...
alpha=%k(1-m1)/%k(1-m);')
epsilon=0.1;A=10;  //ripple parameters
m1=(epsilon*epsilon)/(A*A-1);n=5;omegac=6;
m=find_freq(epsilon,A,n);omegar = omegac/sqrt(m)
%k(1-m1)*%k(m)/(%k(m1)*%k(1-m))-n   //Check...
[alpha,beta]=alpha_beta(n,m,m1)
alpha*%asn(1,m)-n*%k(m1)      //Check
sample=0:0.01:20;
//Now we map the positive real axis into the contour...
z=alpha*%asn(sample/omegac,m)+beta*ones(sample);
plot(sample,ell1mag(epsilon,m1,z))

xdel(winsid())

//====================================================
// ../man/fr/signal/eqfir.xml
//====================================================
clear;lines(0);

hn=eqfir(33,[0 .2;.25 .35;.4 .5],[0 1 0],[1 1 1]);
[hm,fr]=frmag(hn,256);
plot(fr,hm),

xdel(winsid())

//====================================================
// ../man/fr/signal/eqiir.xml
//====================================================
clear;lines(0);

[cells,fact,zzeros,zpoles]=...
eqiir('lp','ellip',[2*%pi/10,4*%pi/10],0.02,0.001)
transfer=fact*poly(zzeros,'z')/poly(zpoles,'z')

xdel(winsid())

//====================================================
// ../man/fr/signal/faurre.xml
//====================================================

//====================================================
// ../man/fr/signal/ffilt.xml
//====================================================

//====================================================
// ../man/fr/signal/fftshift.xml
//====================================================
clear;lines(0);

//make a signal
t=0:0.1:1000;
x=3*sin(t)+8*sin(3*t)+0.5*sin(5*t)+3*rand(t);
//compute the fft
y=fft(x,-1);
//display 
xbasc();
subplot(2,1,1);plot2d(abs(y))
subplot(2,1,2);plot2d(fftshift(abs(y)))

//make a 2D image
t=0:0.1:30;
x=3*sin(t')*cos(2*t)+8*sin(3*t')*sin(5*t)+..
  0.5*sin(5*t')*sin(5*t)+3*rand(t')*rand(t);
//compute the fft
y=fft(x,-1);
//display 
xbasc();
xset('colormap',hotcolormap(256))
subplot(2,1,1);Matplot(abs(y))
subplot(2,1,2);Matplot(fftshift(abs(y)))


xdel(winsid())

//====================================================
// ../man/fr/signal/fft.xml
//====================================================
clear;lines(0);

a=[1;2;3];n=size(a,'*');
norm(1/n*exp(2*%i*%pi*(0:n-1)'.*.(0:n-1)/n)*a -fft(a,1))
norm(exp(-2*%i*%pi*(0:n-1)'.*.(0:n-1)/n)*a -fft(a,-1))  

xdel(winsid())

//====================================================
// ../man/fr/signal/filter.xml
//====================================================

//====================================================
// ../man/fr/signal/find_freq.xml
//====================================================

//====================================================
// ../man/fr/signal/findm.xml
//====================================================

//====================================================
// ../man/fr/signal/frfit.xml
//====================================================
clear;lines(0);

w=0.01:0.01:2;s=poly(0,'s');
G=syslin('c',2*(s^2+0.1*s+2), (s^2+s+1)*(s^2+0.3*s+1));
fresp=repfreq(G,w);
Gid=frfit(w,fresp,4);
frespfit=repfreq(Gid,w);
bode(w,[fresp;frespfit])

xdel(winsid())

//====================================================
// ../man/fr/signal/frmag.xml
//====================================================

//====================================================
// ../man/fr/signal/fsfirlin.xml
//====================================================
clear;lines(0);

//
//Example of how to use the fsfirlin macro for the design 
//of an FIR filter by a frequency sampling technique.
//
//Two filters are designed : the first (response hst1) with 
//abrupt transitions from 0 to 1 between passbands and stop 
//bands; the second (response hst2) with one sample in each 
//transition band (amplitude 0.5) for smoothing.
//
hd=[zeros(1,15) ones(1,10) zeros(1,39)];//desired samples
hst1=fsfirlin(hd,1);//filter with no sample in the transition
hd(15)=.5;hd(26)=.5;//samples in the transition bands
hst2=fsfirlin(hd,1);//corresponding filter
pas=1/prod(size(hst1))*.5;
fg=0:pas:.5;//normalized frequencies grid
plot2d([1 1].*.fg(1:257)',[hst1' hst2']);
// 2nd example
hd=[0*ones(1,15) ones(1,10) 0*ones(1,39)];//desired samples
hst1=fsfirlin(hd,1);//filter with no sample in the transition
hd(15)=.5;hd(26)=.5;//samples in the transition bands
hst2=fsfirlin(hd,1);//corresponding filter
pas=1/prod(size(hst1))*.5;
fg=0:pas:.5;//normalized frequencies grid
n=prod(size(hst1))
plot(fg(1:n),hst1);
plot2d(fg(1:n)',hst2',[3],"000");

xdel(winsid())

//====================================================
// ../man/fr/signal/group.xml
//====================================================
clear;lines(0);

z=poly(0,'z');
h=z/(z-.5);
[tg,fr]=group(100,h);
plot(fr,tg)

xdel(winsid())

//====================================================
// ../man/fr/signal/hank.xml
//====================================================
clear;lines(0);

//Example of how to use the hank macro for 
//building a Hankel matrix from multidimensional 
//data (covariance or Markov parameters e.g.)
//
//This is used e.g. in the solution of normal equations
//by classical identification methods (Instrumental Variables e.g.)
//
//1)let's generate the multidimensional data under the form :
//  C=[c_0 c_1 c_2 .... c_n]
//where each bloc c_k is a d-dimensional matrix (e.g. the k-th correlation 
//of a d-dimensional stochastic process X(t) [c_k = E(X(t) X'(t+k)], ' 
//being the transposition in scilab)
//
//we take here d=2 and n=64
//
c=rand(2,2*64)
//
//generate the hankel matrix H (with 4 bloc-rows and 5 bloc-columns)
//from the data in c
//
H=hank(4,5,c);
//

xdel(winsid())

//====================================================
// ../man/fr/signal/hilb.xml
//====================================================
clear;lines(0);

plot(hilb(51))

xdel(winsid())

//====================================================
// ../man/fr/signal/iirgroup.xml
//====================================================

//====================================================
// ../man/fr/signal/iirlp.xml
//====================================================

//====================================================
// ../man/fr/signal/iir.xml
//====================================================
clear;lines(0);

hz=iir(3,'bp','ellip',[.15 .25],[.08 .03]);
[hzm,fr]=frmag(hz,256);
plot2d(fr',hzm')
xtitle('Discrete IIR filter band pass  0.15<fr<0.25 ',' ',' ');
q=poly(0,'q');     //to express the result in terms of the ...
hzd=horner(hz,1/q) //delay operator q=z^-1

xdel(winsid())

//====================================================
// ../man/fr/signal/intdec.xml
//====================================================

//====================================================
// ../man/fr/signal/jmat.xml
//====================================================

//====================================================
// ../man/fr/signal/kalm.xml
//====================================================

//====================================================
// ../man/fr/signal/lattn.xml
//====================================================

//====================================================
// ../man/fr/signal/lattp.xml
//====================================================

//====================================================
// ../man/fr/signal/levin.xml
//====================================================
clear;lines(0);

//We use the 'levin' macro for solving the normal equations 
//on two examples: a one-dimensional and a two-dimensional process.
//We need the covariance sequence of the stochastic process.
//This example may usefully be compared with the results from 
//the 'phc' macro (see the corresponding help and example in it)
//
//
//1) A one-dimensional process
//   -------------------------
//
//We generate the process defined by two sinusoids (1Hz and 2 Hz) 
//in additive Gaussian noise (this is the observed process); 
//the simulated process is sampled at 10 Hz (step 0.1 in t, underafter).
//
t1=0:.1:100;rand('normal');
y1=sin(2*%pi*t1)+sin(2*%pi*2*t1);y1=y1+rand(y1);plot(t1,y1);
//
//covariance of y1
//
nlag=128;
c1=corr(y1,nlag);
c1=c1';//c1 needs to be given columnwise (see the section PARAMETERS of this help)
//
//compute the filter for a maximum order of n=10
//la is a list-type variable each element of which 
//containing the filters of order ranging from 1 to n; (try varying n)
//in the d-dimensional case this is a matrix polynomial (square, d X d)
//sig gives, the same way, the mean-square error
//
n=15;
[la1,sig1]=levin(n,c1);
//
//verify that the roots of 'la' contain the 
//frequency spectrum of the observed process y
//(remember that y is sampled -in our example 
//at 10Hz (T=0.1s) so that we need to retrieve 
//the original frequencies (1Hz and 2 Hz) through 
//the log and correct scaling by the frequency sampling)
//we verify this for each filter order
//
for i=1:n, s1=roots(la1(i));s1=log(s1)/2/%pi/.1;
//
//now we get the estimated poles (sorted, positive ones only !)
//
s1=sort(imag(s1));s1=s1(1:i/2);end;
//
//the last two frequencies are the ones really present in the observed 
//process ---> the others are "artifacts" coming from the used model size.
//This is related to the rather difficult problem of order estimation.
//
//2) A 2-dimensional process 
//   -----------------------
//(4 frequencies 1, 2, 3, and 4 Hz, sampled at 0.1 Hz :
//   |y_1|        y_1=sin(2*Pi*t)+sin(2*Pi*2*t)+Gaussian noise
// y=|   | with : 
//   |y_2|        y_2=sin(2*Pi*3*t)+sin(2*Pi*4*t)+Gaussian noise
//
//
d=2;dt=0.1;
nlag=64;
t2=0:2*%pi*dt:100;
y2=[sin(t2)+sin(2*t2)+rand(t2);sin(3*t2)+sin(4*t2)+rand(t2)];
c2=[];
for j=1:2, for k=1:2, c2=[c2;corr(y2(k,:),y2(j,:),nlag)];end;end;
c2=matrix(c2,2,128);cov=[];
for j=1:64,cov=[cov;c2(:,(j-1)*d+1:j*d)];end;//covar. columnwise
c2=cov;
//
//in the multidimensional case, we have to compute the 
//roots of the determinant of the matrix polynomial 
//(easy in the 2-dimensional case but tricky if d>=3 !). 
//We just do that here for the maximum desired 
//filter order (n); mp is the matrix polynomial of degree n
//
[la2,sig2]=levin(n,c2);
mp=la2(n);determinant=mp(1,1)*mp(2,2)-mp(1,2)*mp(2,1);
s2=roots(determinant);s2=log(s2)/2/%pi/0.1;//same trick as above for 1D process
s2=sort(imag(s2));s2=s2(1:d*n/2);//just the positive ones !
//
//There the order estimation problem is seen to be much more difficult !
//many artifacts ! The 4 frequencies are in the estimated spectrum 
//but beneath many non relevant others.
//

xdel(winsid())

//====================================================
// ../man/fr/signal/lev.xml
//====================================================

//====================================================
// ../man/fr/signal/lgfft.xml
//====================================================

//====================================================
// ../man/fr/signal/lindquist.xml
//====================================================

//====================================================
// ../man/fr/signal/mese.xml
//====================================================

//====================================================
// ../man/fr/signal/mfft.xml
//====================================================

//====================================================
// ../man/fr/signal/mrfit.xml
//====================================================
clear;lines(0);

w=0.01:0.01:2;s=poly(0,'s');
G=syslin('c',2*(s^2+0.1*s+2),(s^2+s+1)*(s^2+0.3*s+1)); // syslin('c',Num,Den);
fresp=repfreq(G,w);
mag=abs(fresp);
Gid=mrfit(w,mag,4);
frespfit=repfreq(Gid,w);
plot2d([w',w'],[mag(:),abs(frespfit(:))])

xdel(winsid())

//====================================================
// ../man/fr/signal/percentasn.xml
//====================================================
clear;lines(0);

m=0.8;z=%asn(1/sqrt(m),m);K=real(z);Ktilde=imag(z);
x2max=1/sqrt(m);
x1=0:0.05:1;x2=1:((x2max-1)/20):x2max;x3=x2max:0.05:10;
x=[x1,x2,x3];
y=%asn(x,m);
rect=[0,-Ktilde,1.1*K,2*Ktilde];
plot2d(real(y)',imag(y)',1,'011',' ',rect)
//
deff('y=f(t)','y=1/sqrt((1-t^2)*(1-m*t^2))');
intg(0,0.9,f)-%asn(0.9,m)  //Works for real case only!

xdel(winsid())

//====================================================
// ../man/fr/signal/percentk.xml
//====================================================
clear;lines(0);

m=0.4;
%asn(1,m)
%k(m)

xdel(winsid())

//====================================================
// ../man/fr/signal/percentsn.xml
//====================================================
clear;lines(0);

m=0.36;
K=%k(m);
P=4*K; //Real period
real_val=0:(P/50):P;
plot(real_val,real(%sn(real_val,m)))
xbasc();
KK=%k(1-m);
Ip=2*KK;
ima_val1=0:(Ip/50):KK-0.001;
ima_val2=(KK+0.05):(Ip/25):(Ip+KK);
z1=%sn(%i*ima_val1,m);z2=%sn(%i*ima_val2,m);
plot2d([ima_val1',ima_val2'],[imag(z1)',imag(z2)']);
xgrid(3)

xdel(winsid())

//====================================================
// ../man/fr/signal/phc.xml
//====================================================
clear;lines(0);

//
//This example may usefully be compared with the results from 
//the 'levin' macro (see the corresponding help and example)
//
//We consider the process defined by two sinusoids (1Hz and 2 Hz) 
//in additive Gaussian noise (this is the observation); 
//the simulated process is sampled at 10 Hz.
//
t=0:.1:100;rand('normal');
y=sin(2*%pi*t)+sin(2*%pi*2*t);y=y+rand(y);plot(t,y)
//
//covariance of y
//
nlag=128;
c=corr(y,nlag);
//
//hankel matrix from the covariance sequence
//(we can choose to take more information from covariance
//by taking greater n and m; try it to compare the results !
//
n=20;m=20;
h=hank(n,m,c);
//
//compute the Markov representation (mh,mf,mg)
//We just take here a state dimension equal to 4 :
//this is the rather difficult problem of estimating the order !
//Try varying ns ! 
//(the observation dimension is here equal to one)
ns=4;
[mh,mf,mg]=phc(h,1,ns);
//
//verify that the spectrum of mf contains the 
//frequency spectrum of the observed process y
//(remember that y is sampled -in our example 
//at 10Hz (T=0.1s) so that we need 
//to retrieve the original frequencies through the log 
//and correct scaling by the frequency sampling)
//
s=spec(mf);s=log(s);
s=s/2/%pi/.1;
//
//now we get the estimated spectrum
imag(s),
//

xdel(winsid())

//====================================================
// ../man/fr/signal/pspect.xml
//====================================================
clear;lines(0);

rand('normal');rand('seed',0);
x=rand(1:1024-33+1);
//make low-pass filter with eqfir
nf=33;bedge=[0 .1;.125 .5];des=[1 0];wate=[1 1];
h=eqfir(nf,bedge,des,wate);
//filter white data to obtain colored data 
h1=[h 0*ones(1:maxi(size(x))-1)];
x1=[x 0*ones(1:maxi(size(h))-1)];
hf=fft(h1,-1);   xf=fft(x1,-1);yf=hf.*xf;y=real(fft(yf,1));
//plot magnitude of filter
//h2=[h 0*ones(1:968)];hf2=fft(h2,-1);hf2=real(hf2.*conj(hf2));
//hsize=maxi(size(hf2));fr=(1:hsize)/hsize;plot(fr,log(hf2));
//pspect example
sm=pspect(100,200,'tr',y);smsize=maxi(size(sm));fr=(1:smsize)/smsize;
plot(fr,log(sm));
rand('unif');

xdel(winsid())

//====================================================
// ../man/fr/signal/remezb.xml
//====================================================
clear;lines(0);

// Choose the number of cosine functions and create a dense grid 
// in [0,.24) and [.26,.5)
nc=21;ngrid=nc*16;
fg=.24*(0:ngrid/2-1)/(ngrid/2-1);
fg(ngrid/2+1:ngrid)=fg(1:ngrid/2)+.26*ones(1:ngrid/2);
// Specify a low pass filter magnitude for the desired response
ds(1:ngrid/2)=ones(1:ngrid/2);
ds(ngrid/2+1:ngrid)=zeros(1:ngrid/2);
// Specify a uniform weighting function
wt=ones(fg);
// Run remezb
an=remezb(nc,fg,ds,wt)
// Make a linear phase FIR filter 
hn(1:nc-1)=an(nc:-1:2)/2;
hn(nc)=an(1);
hn(nc+1:2*nc-1)=an(2:nc)/2;
// Plot the filter's magnitude response
plot(.5*(0:255)/256,frmag(hn,256));
//////////////
// Choose the number of cosine functions and create a dense grid in [0,.5)
nc=21; ngrid=nc*16;
fg=.5*(0:(ngrid-1))/ngrid;
// Specify a triangular shaped magnitude for the desired response
ds(1:ngrid/2)=(0:ngrid/2-1)/(ngrid/2-1);
ds(ngrid/2+1:ngrid)=ds(ngrid/2:-1:1);
// Specify a uniform weighting function
wt=ones(fg);
// Run remezb
an=remezb(nc,fg,ds,wt)
// Make a linear phase FIR filter 
hn(1:nc-1)=an(nc:-1:2)/2;
hn(nc)=an(1);
hn(nc+1:2*nc-1)=an(2:nc)/2;
// Plot the filter's magnitude response
plot(.5*(0:255)/256,frmag(hn,256));

xdel(winsid())

//====================================================
// ../man/fr/signal/remez.xml
//====================================================

//====================================================
// ../man/fr/signal/rpem.xml
//====================================================

//====================================================
// ../man/fr/signal/Signal.xml
//====================================================

//====================================================
// ../man/fr/signal/sincd.xml
//====================================================
clear;lines(0);

plot(sincd(10,1)) 

xdel(winsid())

//====================================================
// ../man/fr/signal/sinc.xml
//====================================================
clear;lines(0);

plot(sinc(100,0.1))

xdel(winsid())

//====================================================
// ../man/fr/signal/srfaur.xml
//====================================================
clear;lines(0);

//GENERATE SIGNAL
x=%pi/10:%pi/10:102.4*%pi;
rand('seed',0);rand('normal');
y=[1;1]*sin(x)+[sin(2*x);sin(1.9*x)]+rand(2,1024);
//COMPUTE CORRELATIONS
c=[];for j=1:2,for k=1:2,c=[c;corr(y(k,:),y(j,:),64)];end;end
c=matrix(c,2,128);
//FINDING H,F,G with 6 states
hk=hank(20,20,c);
[H,F,G]=phc(hk,2,6);
//SOLVING RICCATI EQN
r0=c(1:2,1:2);
[P,s,t,l,Rt,Tt]=srfaur(H,F,G,r0,200);
//Make covariance matrix exactly symetric
Rt=(Rt+Rt')/2

xdel(winsid())

//====================================================
// ../man/fr/signal/srkf.xml
//====================================================

//====================================================
// ../man/fr/signal/sskf.xml
//====================================================

//====================================================
// ../man/fr/signal/system.xml
//====================================================

//====================================================
// ../man/fr/signal/trans.xml
//====================================================

//====================================================
// ../man/fr/signal/wfir.xml
//====================================================

//====================================================
// ../man/fr/signal/wiener.xml
//====================================================

//====================================================
// ../man/fr/signal/wigner.xml
//====================================================

//====================================================
// ../man/fr/signal/window.xml
//====================================================

//====================================================
// ../man/fr/signal/yulewalk.xml
//====================================================
clear;lines(0);

f=[0,0.4,0.4,0.6,0.6,1];H=[0,0,1,1,0,0];Hz=yulewalk(8,f,H);
fs=1000;fhz = f*fs/2;  
xbasc(0);xset('window',0);plot2d(fhz',H');
xtitle('Desired Frequency Response (Magnitude)')
[frq,repf]=repfreq(Hz,0:0.001:0.5);
xbasc(1);xset('window',1);plot2d(fs*frq',abs(repf'));
xtitle('Obtained Frequency Response (Magnitude)')

xdel(winsid())

//====================================================
// ../man/fr/signal/zpbutt.xml
//====================================================

//====================================================
// ../man/fr/signal/zpch1.xml
//====================================================

//====================================================
// ../man/fr/signal/zpch2.xml
//====================================================

//====================================================
// ../man/fr/signal/zpell.xml
//====================================================

//====================================================
// ../man/fr/sound/analyze.xml
//====================================================
clear;lines(0);

// At first we create 0.5 seconds of sound parameters.
t=soundsec(0.5);
// Then we generate the sound.
s=sin(440*t)+sin(220*t)/2+sin(880*t)/2;
[nr,nc]=size(t);
s(nc/2:nc)=sin(330*t(nc/2:nc));
analyze(s);

xdel(winsid())

//====================================================
// ../man/fr/sound/auread.xml
//====================================================

//====================================================
// ../man/fr/sound/auwrite.xml
//====================================================

//====================================================
// ../man/fr/sound/lin2mu.xml
//====================================================

//====================================================
// ../man/fr/sound/loadwave.xml
//====================================================

//====================================================
// ../man/fr/sound/mapsound.xml
//====================================================
clear;lines(0);

// At first we create 0.5 seconds of sound parameters.
t=soundsec(0.5);
// Then we generate the sound.
s=sin(440*t)+sin(220*t)/2+sin(880*t)/2;
[nr,nc]=size(t);
s(nc/2:nc)=sin(330*t(nc/2:nc));
mapsound(s);

xdel(winsid())

//====================================================
// ../man/fr/sound/mu2lin.xml
//====================================================

//====================================================
// ../man/fr/sound/playsnd.xml
//====================================================
clear;lines(0);

  y=wavread('SCI/demos/signal/sound/chimes.wav');
  playsnd(y)



xdel(winsid())

//====================================================
// ../man/fr/sound/savewave.xml
//====================================================
clear;lines(0);

// At first we create 0.5 seconds of sound parameters.
t=soundsec(0.5);
// Then we generate the sound.
s=sin(440*t)+sin(220*t)/2+sin(880*t)/2;
[nr,nc]=size(t);
s(nc/2:nc)=sin(330*t(nc/2:nc));
savewave(TMPDIR+'/foo.wav',s);

xdel(winsid())

//====================================================
// ../man/fr/sound/sound.xml
//====================================================

//====================================================
// ../man/fr/sound/wavread.xml
//====================================================
clear;lines(0);

  wavread('SCI/demos/sound/chimes.wav','size')

  [y,Fs,bits]=wavread('SCI/demos/sound/chimes.wav');Fs,bits
  subplot(2,1,1)
  plot2d(y(:,1))
  subplot(2,1,2)
  plot2d(y(:,2))
  
  y=wavread('SCI/demos/sound/chimes.wav',[1 5]) //the first five samples


xdel(winsid())

//====================================================
// ../man/fr/sound/wavwrite.xml
//====================================================

//====================================================
// ../man/fr/strings/code2str.xml
//====================================================
clear;lines(0);

code2str([-28 12 18 21 10 11])

xdel(winsid())

//====================================================
// ../man/fr/strings/convstr.xml
//====================================================
clear;lines(0);

A=['this','is';'my','matrix'];
convstr(A,'u')

xdel(winsid())

//====================================================
// ../man/fr/strings/emptystr.xml
//====================================================
clear;lines(0);

x=emptystr();for k=1:10, x=x+','+string(k);end

xdel(winsid())

//====================================================
// ../man/fr/strings/grep.xml
//====================================================
clear;lines(0);

txt=['find matches of a string in a vector of strings'
     'search position of a character string in an other string'
     'Compare Strings'];

grep(txt,'strings')
grep(txt,['strings' 'Strings'])

[r,w]=grep(txt,['strings' 'Strings'])


xdel(winsid())

//====================================================
// ../man/fr/strings/justify.xml
//====================================================
clear;lines(0);

t=['1234','x','adfdfgdfghfgj'
   '1','354556','dgf'
   'sdfgd','','sdfsf'];

justify(t,'l')
justify(t,'c')


xdel(winsid())

//====================================================
// ../man/fr/strings/length.xml
//====================================================

//====================================================
// ../man/fr/strings/part.xml
//====================================================
clear;lines(0);

c=part(['a','abc','abcd'],[1,1,2])

xdel(winsid())

//====================================================
// ../man/fr/strings/str2code.xml
//====================================================
clear;lines(0);

str2code('Scilab')

xdel(winsid())

//====================================================
// ../man/fr/strings/strcat.xml
//====================================================
clear;lines(0);

strcat(string(1:10),',')

xdel(winsid())

//====================================================
// ../man/fr/strings/strindex.xml
//====================================================
clear;lines(0);

k=strindex('SCI/demos/scicos','/')
k=strindex('SCI/demos/scicos','SCI/')
k=strindex('SCI/demos/scicos','!')
k=strindex('aaaaa','aa') 
k=strindex('SCI/demos/scicos',['SCI','sci'])

xdel(winsid())

//====================================================
// ../man/fr/strings/strings.xml
//====================================================
clear;lines(0);

['this','is'; 'a 2x2','matrix']
"matrix"=="mat"+"rix"

xdel(winsid())

//====================================================
// ../man/fr/strings/string.xml
//====================================================
clear;lines(0);

string(rand(2,2))
deff('y=mymacro(x)','y=x+1')
[out,in,text]=string(mymacro)
x=123.356; 'Result is '+string(x)

xdel(winsid())

//====================================================
// ../man/fr/strings/stripblanks.xml
//====================================================
clear;lines(0);

a='  123   ';
'!'+a+'!'
'!'+stripblanks(a)+'!'
a=['  123   ',' xyz']
strcat(stripblanks(a))

xdel(winsid())

//====================================================
// ../man/fr/strings/strsubst.xml
//====================================================
clear;lines(0);

strsubst('SCI/demos/scicos','SCI','.')
strsubst('SCI/demos/scicos','/',' ')

xdel(winsid())

//====================================================
// ../man/fr/strings/tokenpos.xml
//====================================================
clear;lines(0);

str='This is a character string';
kdf=tokenpos(str)
first=part(str,kdf(1,1):kdf(1,2))

xdel(winsid())

//====================================================
// ../man/fr/strings/tokens.xml
//====================================================
clear;lines(0);

tokens('This is a character string')

tokens('SCI/demos/scicos','/')

tokens('y=a+b*2',['=','+','*'])

xdel(winsid())

//====================================================
// ../man/fr/tdcs/artest.xml
//====================================================

//====================================================
// ../man/fr/tdcs/bifish.xml
//====================================================

//====================================================
// ../man/fr/tdcs/boucle.xml
//====================================================

//====================================================
// ../man/fr/tdcs/chaintest.xml
//====================================================

//====================================================
// ../man/fr/tdcs/fishing.xml
//====================================================

//====================================================
// ../man/fr/tdcs/fusee.xml
//====================================================

//====================================================
// ../man/fr/tdcs/lotest.xml
//====================================================

//====================================================
// ../man/fr/tdcs/mine.xml
//====================================================

//====================================================
// ../man/fr/tdcs/obscont1.xml
//====================================================

//====================================================
// ../man/fr/tdcs/portr3d.xml
//====================================================

//====================================================
// ../man/fr/tdcs/portrait.xml
//====================================================
clear;lines(0);

a=rand(2,2)
deff('[ydot]=l_s(t,y)','ydot=a*y')
portrait(l_s)

xdel(winsid())

//====================================================
// ../man/fr/tdcs/recur.xml
//====================================================

//====================================================
// ../man/fr/tdcs/systems.xml
//====================================================

//====================================================
// ../man/fr/tdcs/tangent.xml
//====================================================

//====================================================
// ../man/fr/tdcs/tdinit.xml
//====================================================

//====================================================
// ../man/fr/translation/ascii.xml
//====================================================

//====================================================
// ../man/fr/translation/excel2sci.xml
//====================================================

//====================================================
// ../man/fr/translation/formatman.xml
//====================================================

//====================================================
// ../man/fr/translation/fun2string.xml
//====================================================
clear;lines(0);

txt=fun2string(asinh,'foo');
write(%io(2),txt,'(a)')

xdel(winsid())

//====================================================
// ../man/fr/translation/mfile2sci.xml
//====================================================
clear;lines(0);

// Création d'une fichier .m simple
write(TMPDIR+'/rot90.m',['function B = rot90(A,k)'
 '[m,n] = size(A);'
 'if nargin == 1'
 '    k = 1;'
 'else'
 '    k = rem(k,4);'
 '    if k < 0'
 '        k = k + 4;'
 '    end'
 'end'
 'if k == 1'
 '    A = A.'';'
 '    B = A(n:-1:1,:);'
 'elseif k == 2'
 '    B = A(m:-1:1,n:-1:1);'
 'elseif k == 3'
 '    B = A(m:-1:1,:);'
 '    B = B.'';'
 'else'
 '    B = A;'
 'end']);
// Conversion en Scilab
mfile2sci(TMPDIR+'/rot90.m',TMPDIR)
// Affichage du code
write(%io(2),read(TMPDIR+'/rot90.sci',-1,1,'(a)'))
// get it into scilab
getf(TMPDIR+'/rot90.sci')
// Exécution du code
m=rand(4,2);rot90(m,1)

xdel(winsid())

//====================================================
// ../man/fr/translation/mtlb_load.xml
//====================================================

//====================================================
// ../man/fr/translation/mtlb_save.xml
//====================================================

//====================================================
// ../man/fr/translation/pol2tex.xml
//====================================================
clear;lines(0);

s=poly(0,'s');
p=s^3+2*s-5;
pol2tex(p)

xdel(winsid())

//====================================================
// ../man/fr/translation/sci2for.xml
//====================================================

//====================================================
// ../man/fr/translation/texprint.xml
//====================================================
clear;lines(0);

s=poly(0,'s');
texprint([1/s,s^2])

xdel(winsid())

//====================================================
// ../man/fr/translation/translatepaths.xml
//====================================================

//====================================================
// ../man/fr/translation/tree2code.xml
//====================================================
clear;lines(0);

tree=macr2tree(help);
txt=tree2code(tree,%T);
write(%io(2),txt,'(a)')

xdel(winsid())

//====================================================
// ../man/fr/tksci/close.xml
//====================================================
clear;lines(0);

h=figure();
// creates  figure number 1.  
uicontrol( h, 'style','text', ...
 'string','scilab is great', ...
 'position',[50 70 100 100], ...
 'fontsize',15);
// put a clever text in figure 1
figure();
// create figure 2
uicontrol( 'style','text', ...
 'string','Really great', 'position',[50 70 100 100], 'fontsize',15);
// put a text in figure 2
close();
// close the current graphic window (ie fig. 2)
close(h);
// close figure 1

xdel(winsid())

//====================================================
// ../man/fr/tksci/figure.xml
//====================================================
clear;lines(0);

h=figure(3);
// creates  figure number 1.
uicontrol( h, 'style','text', ...
 'string','This is a figure', ...
 'position',[50 70 100 100], ...
 'fontsize',15);
// put a  text in figure 3
figure();
// create figure 1
uicontrol( 'style','text', ...
 'string','Another figure', ...
 'position',[50 70 100 100], ...
 'fontsize',15);
// put a text in figure 1
close();
// close the current graphic window (ie fig. 1)
close(h);
// close figure 3

xdel(winsid())

//====================================================
// ../man/fr/tksci/findobj.xml
//====================================================
clear;lines(0);

h=figure();
// creates  figure number 1.
uicontrol( h, 'style','text', ...
 'string','This is a figure', ...
 'position',[50 70 100 100], ...
 'fontsize',15, ...
 'tag','Alabel');
// put a  text in figure 1
lab=findobj('tag','Alabel');
// find the object which 'tag' value is 'Alabel'
disp('the handle of the label is '+string(lab));
close();

xdel(winsid())

//====================================================
// ../man/fr/tksci/ScilabEval.xml
//====================================================
clear;lines(0);

//Create a Tcl script using ScilabEval
tcl_script=['toplevel .w1'
'button .w1.b -text ""Click here to see a new Scilab Graphic Window""\'
'  -command {ScilabEval ""xselect()""}'
'pack .w1.b ']
mputl(tcl_script,TMPDIR+'/test.tcl')
// Execute the tcl script
TK_EvalFile(TMPDIR+'/test.tcl')

xdel(winsid())

//====================================================
// ../man/fr/tksci/TK_EvalFile.xml
//====================================================
clear;lines(0);

TK_EvalFile(SCI+'/demos/tk/puzzle')

xdel(winsid())

//====================================================
// ../man/fr/tksci/TK_EvalStr.xml
//====================================================
clear;lines(0);

//with one call
TK_EvalStr(['toplevel .foo1'
   'label .foo1.l -text ""TK married Scilab !!!""'
   'pack .foo1.l'
   'button .foo1.b -text close -command {destroy .foo1}'
   'pack .foo1.b'])

//step by step (debugging)
TK_EvalStr('toplevel .foo2');
// creates a toplevel TK window. 
TK_EvalStr('label .foo2.l -text ""TK married Scilab !!!""');
// create a static label
TK_EvalStr('pack .foo2.l');
// pack the label widget. It appears on the screen.
text='button .foo2.b -text close -command {destroy .foo2}';
TK_EvalStr(text);
TK_EvalStr('pack .foo2.b');

//kill the windows by program
TK_EvalStr('destroy .foo1');
TK_EvalStr('destroy .foo2');

xdel(winsid())

//====================================================
// ../man/fr/tksci/TK_GetVar.xml
//====================================================
clear;lines(0);

TK_EvalStr('toplevel .tst1');
// creates a toplevel TK window. 
TK_EvalStr('entry .tst1.e -textvariable tvar');
// create an editable entry
TK_EvalStr('set tvar foobar');
// set the entry value
TK_EvalStr('pack .tst1.e');
// pack the entry widget. It appears on the screen.
text=TK_GetVar('tvar')
// retrieve the variable value
// change the entry text and repeat the last command ...

//delete the toplevel TK window.
TK_EvalStr('destroy .tst1')


xdel(winsid())

//====================================================
// ../man/fr/tksci/TK_SetVar.xml
//====================================================
clear;lines(0);

TK_EvalStr('toplevel .tst2');
// creates a toplevel TK window. 
TK_EvalStr('label .tst2.l -textvariable tvar');
// create a static label
TK_EvalStr('pack .tst2.l');
// pack the label widget. It appears on the screen.
TK_SetVar('tvar','This text has been set directly within scilab');

//destroy toplevel  TK window. 
TK_EvalStr('destroy .tst2')



xdel(winsid())

//====================================================
// ../man/fr/tksci/uicontrol.xml
//====================================================
clear;lines(0);

f=figure();
// create a figure
h=uicontrol(f,'style','listbox', ...
 'position', [10 10 150 160]);
// create a listbox
set(h, 'string', "item 1|item 2|item3");
// fill the list
set(h, 'value', [1 3]);
// select item 1 and 3 in the list
close();
// close the figure
f=figure();
// create a figure
h=uicontrol(f,'style','listbox', ...
 'position', [10 10 150 160]);
// create a listbox
set(h, 'string', "item 1|item 2|item3");
// fill the list
set(h, 'value', [1 3]);
// select (highlight) the item 1 and 3 in the list
close();
//close the figure

xdel(winsid())

//====================================================
// ../man/fr/tksci/uimenu.xml
//====================================================
clear;lines(0);

f=figure('position', [10 10 300 200]);
// create a figure
m=uimenu(f,'label', 'windows');
// create an item on the menu bar
m1=uimenu(m,'label', 'operations');
m2=uimenu(m,'label', 'quit scilab', 'callback', "exit");
//create two items in the menu "windows"
m11=uimenu(m1,'label', 'new window', 'callback',"xselect()");
m12=uimenu(m1,'label', 'clear  window', 'callback',"xbasc()");
// create a submenu to the item "operations"
close(f);
// close the figure

xdel(winsid())

//====================================================
// ../man/fr/utilities/add_demo.xml
//====================================================
clear;lines(0);

  //create a simple demo script
  path=TMPDIR+'/foo.sce';
  mputl('disp Hello',path)
  add_demo('My first demo',path)
  //the demo can now be run using the "Demos" menu.
 
xdel(winsid())

//====================================================
// ../man/fr/utilities/add_help_chapter.xml
//====================================================

//====================================================
// ../man/fr/utilities/add_palette.xml
//====================================================

//====================================================
// ../man/fr/utilities/apropos.xml
//====================================================
clear;lines(0);
apropos "+"
apropos ode 
apropos "list of"
xdel(winsid())

//====================================================
// ../man/fr/utilities/basename.xml
//====================================================
clear;lines(0);

files=basename('SCI/macros/util/poo.sci')
files=basename('SCI/macros\util/poo.sci')
files=basename('SCI/macros\util/poo.sci.k')

xdel(winsid())

//====================================================
// ../man/fr/utilities/chdir.xml
//====================================================
clear;lines(0);
    chdir(TMPDIR);
    pwd
    cd 
    cd SCI
xdel(winsid())

//====================================================
// ../man/fr/utilities/clc.xml
//====================================================

//====================================================
// ../man/fr/utilities/c_link.xml
//====================================================

//====================================================
// ../man/fr/utilities/dec2hex.xml
//====================================================
clear;lines(0);
dec2hex([2748 10;11 3])
xdel(winsid())

//====================================================
// ../man/fr/utilities/dirname.xml
//====================================================
clear;lines(0);

files=dirname('SCI/macros/util/poo.sci')
files=dirname('SCI/macros\util/poo.sci')
files=dirname('SCI/macros\util/poo.sci.k')

xdel(winsid())

//====================================================
// ../man/fr/utilities/foo.xml
//====================================================
clear;lines(0);

deff("y=foo(x)","y=x"); // define the foo function as the identity function
foo("toto")

xdel(winsid())

//====================================================
// ../man/fr/utilities/gethistory.xml
//====================================================

//====================================================
// ../man/fr/utilities/G_make.xml
//====================================================

//====================================================
// ../man/fr/utilities/head_comments.xml
//====================================================
clear;lines(0);
    head_comments sinc
xdel(winsid())

//====================================================
// ../man/fr/utilities/help_skeleton.xml
//====================================================
clear;lines(0);

   function [y,z]=foo(a,b),y=a+b,z=1,endfunction
    p=help_skeleton('foo',TMPDIR)
    scipad(p)
 
xdel(winsid())

//====================================================
// ../man/fr/utilities/help.xml
//====================================================

//====================================================
// ../man/fr/utilities/hex2dec.xml
//====================================================
clear;lines(0);

hex2dec(['ABC','0','A'])

xdel(winsid())

//====================================================
// ../man/fr/utilities/ilib_build.xml
//====================================================
clear;lines(0);

//Here with give a complete example on adding new primitive to Scilab
//create the procedure files
f1=['extern double fun2();'
    'void fun1(x,y)'
    'double *x, *y;'
    '{*y=fun2(*x)/(*x);}'];

mputl(f1,'fun1.c')

f2=['#include <math.h>'
    'double fun2(x)'
    'double x;'
    '{ return( sin(x+1.));}'];
mputl(f2,'fun2.c');

//creating the interface file
i=['#include ""stack-c.h""'
   'extern int fun1 _PARAMS(( double *x, double *y));'
   'int intfun1(fname)' 
   'char * fname;'
   '{'
   '  int m1,n1,l1;'
   '  CheckRhs(1,1);'
   '  CheckLhs(1,1);'
   '  GetRhsVar(1, ""d"", &m1, &n1, &l1);'
   '  fun1(stk(l1),stk(l1));'
   '  LhsVar(1) = 1;'
   '  return 0;'
   '}'];
mputl(i,'intfun1.c')

//creating the shared library (a gateway, a Makefile and a loader are 
//generated. 

files=['fun1.o','fun2.o','intfun1.o'];
ilib_build('foo',['scifun1','intfun1'],files,[]);

// load the shared library 

exec loader.sce 

//using the new primitive
scifun1(33)

xdel(winsid())

//====================================================
// ../man/fr/utilities/ilib_compile.xml
//====================================================

//====================================================
// ../man/fr/utilities/ilib_for_link.xml
//====================================================
clear;lines(0);


f1=['int ext1c(n, a, b, c)'
    'int *n;     double *a, *b, *c;'
    '{int k;'
    '  for (k = 0; k < *n; ++k) '
    '      c[k] = a[k] + b[k];'
    '  return(0);}'];

mputl(f1,'fun1.c')

//creating the shared library (a gateway, a Makefile and a loader are 
//generated. 

ilib_for_link('ext1c','fun1.o',[],"c") 

// load the shared library 

exec loader.sce 

//using the new primitive
a=[1,2,3];b=[4,5,6];n=3;
c=call('ext1c',n,1,'i',a,2,'d',b,3,'d','out',[1,3],4,'d');
if norm(c-(a+b)) > %eps then pause,end


xdel(winsid())

//====================================================
// ../man/fr/utilities/ilib_gen_gateway.xml
//====================================================

//====================================================
// ../man/fr/utilities/ilib_gen_loader.xml
//====================================================

//====================================================
// ../man/fr/utilities/ilib_gen_Make.xml
//====================================================

//====================================================
// ../man/fr/utilities/intersci.xml
//====================================================

//====================================================
// ../man/fr/utilities/language.xml
//====================================================

//====================================================
// ../man/fr/utilities/link.xml
//====================================================

//====================================================
// ../man/fr/utilities/listfiles.xml
//====================================================
clear;lines(0);

files=listfiles(['SCI/macros/util/*.sci';'SCI/macros/util/*.bin']);

xdel(winsid())

//====================================================
// ../man/fr/utilities/loadhistory.xml
//====================================================
clear;lines(0);

  <SEE_ALSO>
    <SEE_ALSO_ITEM>
      <LINK>savehistory</LINK>
    </SEE_ALSO_ITEM>

    <SEE_ALSO_ITEM>
      <LINK>resethistory</LINK>
    </SEE_ALSO_ITEM>

    <SEE_ALSO_ITEM>
      <LINK>gethistory</LINK>
    </SEE_ALSO_ITEM>
  </SEE_ALSO>

  <AUTHORS>
    <AUTHORS_ITEM>A.C</AUTHORS_ITEM>
  </AUTHORS>
</MAN>xdel(winsid())

//====================================================
// ../man/fr/utilities/make_index.xml
//====================================================

//====================================================
// ../man/fr/utilities/man.xml
//====================================================
clear;lines(0);
             deff("y=foo(x)","y=x"); // define the foo function as the identity function
             foo("toto")
xdel(winsid())

//====================================================
// ../man/fr/utilities/pathconvert.xml
//====================================================
clear;lines(0);

pathconvert('SCI/macros\util/foo.sci',%f,%f,'u')
pathconvert('SCI/macros\util/foo.sci',%f,%f,'w')
pathconvert('SCI/macros/util/foo.sci',%f,%t,'w')
pathconvert('HOME/macros/util/foo.sci',%t,%t,'w')
pathconvert('c:/tmp',%f,%t,'u')
pathconvert('/cygdrive/c/tmp',%f,%f,'w')

xdel(winsid())

//====================================================
// ../man/fr/utilities/percenthelps.xml
//====================================================

//====================================================
// ../man/fr/utilities/realtime.xml
//====================================================
clear;lines(0);



   realtimeinit(1/2);//sets time unit to half a second
   realtime(0);//sets current date to 0
   for k=1:10,realtime(k);mprintf('current time is '+string(k/2)+'sec .\r\n');end
 
  //next instruction outputs a dot each 2 seconds
   realtimeinit(2); realtime(0);for k=1:10,realtime(k);mprintf('.\r\n');end

   realtimeinit(1);realtime(0);
   dt=getdate('s'); realtime(10);   getdate('s')-dt




xdel(winsid())

//====================================================
// ../man/fr/utilities/resethistory.xml
//====================================================

//====================================================
// ../man/fr/utilities/savehistory.xml
//====================================================
clear;lines(0);

  <SEE_ALSO>
    <SEE_ALSO_ITEM>
      <LINK>loadhistory</LINK>
    </SEE_ALSO_ITEM>

    <SEE_ALSO_ITEM>
      <LINK>resethistory</LINK>
    </SEE_ALSO_ITEM>

    <SEE_ALSO_ITEM>
      <LINK>gethistory</LINK>
    </SEE_ALSO_ITEM>
  </SEE_ALSO>

  <AUTHORS>
    <AUTHORS_ITEM>A.C</AUTHORS_ITEM>
  </AUTHORS>
</MAN>xdel(winsid())

//====================================================
// ../man/fr/utilities/sci2exp.xml
//====================================================
clear;lines(0);
  a=[1 2;3 4]
  sci2exp(a,'aa')
  sci2exp(a,'aa',0)
  sci2exp(ssrand(2,2,2))
  sci2exp(poly([1 0 3 4],'s'),'fi')
xdel(winsid())

//====================================================
// ../man/fr/utilities/sci2map.xml
//====================================================
clear;lines(0);
txt=[sci2map([1 2;3 4],'a');
     sci2map(%s^2+3*%s+4,'p')]
xdel(winsid())

//====================================================
// ../man/fr/utilities/scilab.xml
//====================================================

//====================================================
// ../man/fr/utilities/scilink.xml
//====================================================

//====================================================
// ../man/fr/utilities/scipad.xml
//====================================================
clear;lines(0);
 
   scipad(SCI+'/scilab.star')


xdel(winsid())

//====================================================
// ../man/fr/utilities/setlanguage.xml
//====================================================

//====================================================
// ../man/fr/utilities/timer.xml
//====================================================
clear;lines(0);
timer();A=rand(100,100);timer()
xdel(winsid())

//====================================================
// ../man/fr/utilities/tohome.xml
//====================================================

//====================================================
// ../man/fr/utilities/ulink.xml
//====================================================

//====================================================
// ../man/fr/utilities/unix_g.xml
//====================================================
clear;lines(0);
function d=DIR(path)
  path=pathconvert(path,%t,%t)
  if MSDOS then
    d=unix_g('dir '+path)
  else
    d=unix_g('ls '+path)
  end
endfunction

DIR('SCI/demos')
xdel(winsid())

//====================================================
// ../man/fr/utilities/unix_s.xml
//====================================================
clear;lines(0);
if MSDOS then 
  unix_s("del foo");
else 
  unix_s("rm -f foo"); 
end
xdel(winsid())

//====================================================
// ../man/fr/utilities/unix_w.xml
//====================================================
clear;lines(0);
if MSDOS then unix_w("dir "+WSCI+"\demos");
else unix_w("ls $SCI/demos"); end
xdel(winsid())

//====================================================
// ../man/fr/utilities/unix.xml
//====================================================
clear;lines(0);
unix("ls $SCI/demos");
deff('wd=getdir()','if MSDOS then unix(''cd>''+TMPDIR+''\path'');..
                 else unix(''pwd>''+TMPDIR+''/path'');end..
      wd=read(TMPDIR+''/path'',1,1,''(a)'')')
wd=getdir()
xdel(winsid())

//====================================================
// ../man/fr/utilities/unix_x.xml
//====================================================
clear;lines(0);
if MSDOS then unix_x("dir "+WSCI+"\demos");
else unix_x("ls $SCI/demos"); end
xdel(winsid())

//====================================================
// ../man/fr/utilities/with_pvm.xml
//====================================================

//====================================================
// ../man/fr/utilities/with_texmacs.xml
//====================================================

//====================================================
// ../man/fr/utilities/with_tk.xml
//====================================================

//====================================================
// ../man/fr/utilities/xmltohtml.xml
//====================================================

//====================================================
// ../man/fr/statistics/center.xml
//====================================================
clear;lines(0);

x=[0.2113249  0.0002211 0.6653811;
   0.7560439  0.3303271 0.6283918]
s=center(x)
s=center(x,'r')
s=center(x,'c')


xdel(winsid())

//====================================================
// ../man/fr/statistics/centrered.xml
//====================================================
clear;lines(0);

x=[0.2113249 0.0002211 0.6653811;
   0.7560439 0.3303271 0.6283918]
s=wcenter(x)
s=wcenter(x,'r')
s=wcenter(x,'c')

xdel(winsid())

//====================================================
// ../man/fr/statistics/cmoment.xml
//====================================================
clear;lines(0);

x=[0.2113249 0.0002211 0.6653811;
   0.7560439 0.3303271 0.6283918]
mom=cmoment(x,3)
mom=cmoment(x,2,'r')
mom=cmoment(x,3,'c')

xdel(winsid())

//====================================================
// ../man/fr/statistics/correl.xml
//====================================================
clear;lines(0);

x=[2.5 7.5 12.5 17.5]
h=[0 1 2]
fre=[.03 .12 .07;.02 .13 .11;.01 .13 .14;.01 .09 .14]
rho=correl(x,h,fre)

xdel(winsid())

//====================================================
// ../man/fr/statistics/covar.xml
//====================================================
clear;lines(0);

x=[10 20 30 40]
y=[10 20 30 40]
fre=[.20 .04 .01  0;
     .10 .36 .09  0;
       0 .05 .10  0;
       0   0   0 .05]
s=covar(x,y,fre)

xdel(winsid())

//====================================================
// ../man/fr/statistics/ftest.xml
//====================================================
clear;lines(0);

samples=[46 55 54;
         53 54 50; 
         49 58 51;
         50 61 51;
         46 52 49]
[f,p]=ftest(samples)

xdel(winsid())

//====================================================
// ../man/fr/statistics/ftuneq.xml
//====================================================
clear;lines(0);

samples=[46 55 54;53 54 50; 49 58 51;50 61 51;46 52 49]
[f,p]=ftest(samples)

xdel(winsid())

//====================================================
// ../man/fr/statistics/geomean.xml
//====================================================

//====================================================
// ../man/fr/statistics/harmean.xml
//====================================================

//====================================================
// ../man/fr/statistics/iqr.xml
//====================================================

//====================================================
// ../man/fr/statistics/labostat.xml
//====================================================

//====================================================
// ../man/fr/statistics/mad.xml
//====================================================

//====================================================
// ../man/fr/statistics/meanf.xml
//====================================================
clear;lines(0);

x=[0.2113249 0.0002211 0.6653811;0.7560439 0.3303271 0.6283918]
m=meanf(x,rand(x))
m=meanf(x,[10 10 10;1 1 1],'r')
m=meanf(x,[10 10 10;1 1 1],'c')

xdel(winsid())

//====================================================
// ../man/fr/statistics/mean.xml
//====================================================
clear;lines(0);

A=[1,2,10;7,7.1,7.01];
mean(A)
mean(A,'r')
mean(A,'c')

xdel(winsid())

//====================================================
// ../man/fr/statistics/median.xml
//====================================================
clear;lines(0);
A=[1,2,10;7,7.1,7.01];
median(A)
median(A,'r')
median(A,'c')
xdel(winsid())

//====================================================
// ../man/fr/statistics/moment.xml
//====================================================
clear;lines(0);

x=[0.2113249 0.0002211 0.6653811;0.7560439 0.3303271 0.6283918]
mom=moment(x,3)
mom=moment(x,2,'r')
mom=moment(x,3,'c')

xdel(winsid())

//====================================================
// ../man/fr/statistics/msd.xml
//====================================================
clear;lines(0);


x=[0.2113249 0.0002211 0.6653811;0.7560439 0.3303271 0.6283918]
m=msd(x)
m=msd(x,'r')
m=msd(x,'c')

xdel(winsid())

//====================================================
// ../man/fr/statistics/mvvacov.xml
//====================================================
clear;lines(0);


x=[0.2113249 0.0002211 0.6653811;0.7560439 0.4453586 0.6283918]
v=mvvacov(x)

xdel(winsid())

//====================================================
// ../man/fr/statistics/nancumsum.xml
//====================================================
clear;lines(0);

a=[1 2 3;4 5 6]
s=nancumsum(a)
s=nancumsum(a,'r')
s=nancumsum(a,'c')


xdel(winsid())

//====================================================
// ../man/fr/statistics/nand2mean.xml
//====================================================

//====================================================
// ../man/fr/statistics/nanmax.xml
//====================================================
clear;lines(0);

x=[0.2113249 %nan 0.6653811;0.7560439 0.3303271 0.6283918]
m=nanmax(x)
m=nanmax(x,'r')
m=nanmax(x,'c')

xdel(winsid())

//====================================================
// ../man/fr/statistics/nanmeanf.xml
//====================================================
clear;lines(0);

x=[0.2113249 %nan 0.6653811;0.7560439 0.3303271 0.6283918]
fre=[34 12 25;12 23 5]
m=nanmeanf(x,fre)
m=nanmeanf(x,fre,1)
m=nanmeanf(x,fre,2)

xdel(winsid())

//====================================================
// ../man/fr/statistics/nanmean.xml
//====================================================
clear;lines(0);

x=[0.2113249 %nan 0.6653811;0.7560439 0.3303271 0.6283918]
m=nanmean(x)
m=nanmean(x,1)
m=nanmean(x,2)

xdel(winsid())

//====================================================
// ../man/fr/statistics/nanmedian.xml
//====================================================
clear;lines(0);

x=[0.2113249 %nan 0.6653811;0.7560439 0.3303271 0.6283918]
m=nanmedian(x)
m=nanmedian(x,1)
m=nanmedian(x,2)

xdel(winsid())

//====================================================
// ../man/fr/statistics/nanmin.xml
//====================================================
clear;lines(0);

x=[0.2113249 %nan 0.6653811;0.7560439 0.3303271 0.6283918]
m=nanmin(x)
m=nanmin(x,'r')
m=nanmin(x,'c')

xdel(winsid())

//====================================================
// ../man/fr/statistics/nanstdev.xml
//====================================================
clear;lines(0);


x=[0.2113249 0.0002211 0.6653811;
   0.7560439 %nan      0.6283918;
   0.3       0.2       0.5      ];
s=nanstdev(x)
s=nanstdev(x,'r')
s=nanstdev(x,'c')

xdel(winsid())

//====================================================
// ../man/fr/statistics/nansum.xml
//====================================================
clear;lines(0);
    x=[0.2113249 %nan 0.6653811;0.7560439 0.3303271 0.6283918]
    m=nansum(x)
    m=nansum(x,1)
    m=nansum(x,2)
xdel(winsid())

//====================================================
// ../man/fr/statistics/nfreq.xml
//====================================================
clear;lines(0);

x=[2 8 0 3 7 6 8 7 9 1]
m=nfreq(x)

xdel(winsid())

//====================================================
// ../man/fr/statistics/pca.xml
//====================================================

//====================================================
// ../man/fr/statistics/perctl.xml
//====================================================
clear;lines(0);


x=[6 7 0 7 10 4 2 2 7 1;
   6 0 5 5 5 2 0 6 8 10;
   8 6 4 3 5 9 8 3 4 7;
   1 3 2 7 6 1 1 4 8 2;
   6 3 5 1 6 5 9 9 5 5;
   1 6 4 4 5 4 0 8 1 8;
   7 1 3 7 8 0 2 8 10 8;
   3 6 1 9 8 5 5 3 2 1;
   5 7 6 2 10 8 7 4 0 8;
   10 3 3 4 8 6 9 4 8 3]
y=[10 20 30]
p=perctl(x,y)


xdel(winsid())

//====================================================
// ../man/fr/statistics/quart.xml
//====================================================
clear;lines(0);


x=[6 7 0 7 10 4 2 2 7 1;
   6 0 5 5 5 2 0 6 8 10;
   8 6 4 3 5 9 8 3 4 7;
   1 3 2 7 6 1 1 4 8 2;
   6 3 5 1 6 5 9 9 5 5;
   1 6 4 4 5 4 0 8 1 8;
   7 1 3 7 8 0 2 8 10 8;
   3 6 1 9 8 5 5 3 2 1;
   5 7 6 2 10 8 7 4 0 8;
   10 3 3 4 8 6 9 4 8 3]
q=quart(x)
q=quart(x,'r')
q=quart(x,'c')


xdel(winsid())

//====================================================
// ../man/fr/statistics/regress.xml
//====================================================
clear;lines(0);

x=[0.5608486 0.6623569 0.7263507 0.1985144 0.5442573 0.2320748 0.2312237]
y=[0.3616361 0.2922267 0.5664249 0.4826472 0.3321719 0.5935095 0.5015342]
coefs=regress(x,y)

xdel(winsid())

//====================================================
// ../man/fr/statistics/samplef.xml
//====================================================
clear;lines(0);
   a=[3 7 9;22 4 2]
   f1=[10 1 1 1 1 1]
   f2=[10 1 1]
   f3=[1 ; 15]
   s=samplef(15,a,f1)
   s=samplef(15,a,f2,'r')
   s=samplef(15,a,f3,'c')

xdel(winsid())

//====================================================
// ../man/fr/statistics/sample.xml
//====================================================
clear;lines(0);
X=['a' 'dd' 'arreu'; 'ber' 'car' 'zon']
s=sample(25,X)
s=sample(25,X,'r')
s=sample(25,X,'c')
xdel(winsid())

//====================================================
// ../man/fr/statistics/samwr.xml
//====================================================
clear;lines(0);
    a=[0.33 1.24 2.1 1.03]
    s=samwr(4,12,a)
xdel(winsid())

//====================================================
// ../man/fr/statistics/stdevf.xml
//====================================================
clear;lines(0);


x=[0.2113249 0.0002211 0.6653811;0.7560439 0.9546254 0.6283918]
fre=[1 2 3;3 4 3]
m=stdevf(x,fre)
m=stdevf(x,fre,'r')
m=stdevf(x,fre,'c')


xdel(winsid())

//====================================================
// ../man/fr/statistics/st_deviation.xml
//====================================================
clear;lines(0);
A=[1,2,10;7,7.1,7.01];
st_deviation(A)
st_deviation(A,'r')
st_deviation(A,'c')
xdel(winsid())

//====================================================
// ../man/fr/statistics/strange.xml
//====================================================

//====================================================
// ../man/fr/statistics/tabul.xml
//====================================================
clear;lines(0);

// first example
X = [2 8 0 3 7 6 8 7 9 1 6 7 7 2 5 2 2 2 9 7]
m1 = tabul(X)
m2 = tabul(X, "i")

// second example 
X = ["ba" "baba" "a" "A" "AA" "a" "aa" "aa" "aa" "A" "ba"]
m = tabul(X,"i")

// third example
n = 50000;
X = grand(n,1,"bin",70,0.5);
m = tabul(X,"i");
xbasc()
plot2d3(m(:,1), m(:,2)/n)
xtitle("empiral probabilities of B(70,0.5)")

// last example : computes the occurrences of words of the scilab license
text = read(SCI+"/license.txt",-1,1,"(A)"); // read the scilab license
bigstr = strcat(text," ");                  // put all the lines in a big string
sep =  [" " "," "." ";" "*" ":" "-" """"];  // words separators 
words = tokens(bigstr, sep);                // cut the big string into words
m = tabul(words);                           // computes occurrences of each word
[occ , p] = sort(m(2));                     // sort by decreasing frequencies
results = [m(1)(p) string(occ)]             // display result

xdel(winsid())

//====================================================
// ../man/fr/statistics/thrownan.xml
//====================================================
clear;lines(0);


x=[0.2113249 %nan 0.6653811;0.7560439 0.3303271 0.6283918]
[nonan numb]=thrownan(x)


xdel(winsid())

//====================================================
// ../man/fr/statistics/trimmean.xml
//====================================================

//====================================================
// ../man/fr/statistics/variancef.xml
//====================================================
clear;lines(0);


x=[0.2113249 0.0002211 0.6653811;0.7560439 0.9546254 0.6283918]
fre=[1 2 3;3 4 3]
m=variancef(x,fre)
m=variancef(x,fre,'r')
m=variancef(x,fre,'c')


xdel(winsid())

//====================================================
// ../man/fr/statistics/variance.xml
//====================================================
clear;lines(0);


x=[0.2113249 0.0002211 0.6653811;0.7560439 0.4453586 0.6283918]
s=variance(x)
s=variance(x,'r')
s=variance(x,'c')


xdel(winsid())

