//----------------------------------------------------------------------------
// com-intg1ordre.sce
// Auteur : Jacques Richalet
// 2007
//----------------------------------------------------------------------------
clear("all") ;
close() ;
xdel(winsid());
mode(-1);
//----------------------------------------------------------------------------
//test duration
tf=1500;
w=1:1:tf;
//sampling time
tech=1;
u=zeros(1,tf);
e=u;
sp1=u;
sp=u;
spp=u;
sm1=u;
sm2=u;
sm=u;
sm3=u;
z=u;
pert=u;
v=ones(1,tf);
//process time constant
T=30;
//process gain
Kp=.05;
//delay process
r=40;
am=exp(-tech/T);
bm=1-am;
//MPC
//decomposition process
theta=T*3;
as=exp(-tech/theta);
bs=1-as;
K2=Kp*theta*theta/(theta-T);
K1=(Kp*theta)-K2;
K3=1;
trbf=200;
h=(trbf/2);
//coincidence
lh=1-exp(-tech*3*h/trbf);
bmh=1-am^h;
bsh=1-as^h;
MG=1;
for ii=2+r:1:tf
    //process
    if ( ii > 650 ) then pert(ii) = 2;end
    sp1(ii)=am*sp1(ii-1)+bm*Kp*(e(ii-1)*MG+pert(ii));
    sp(ii)=sp(ii-1)+sp1(ii-r);
    //model
    sm1(ii)=am*sm1(ii-1)+bm*K1*e(ii-1);
    sm2(ii)=as*sm2(ii-1)+bs*K2*e(ii-1);
    sm3(ii)=as*sm3(ii-1)+bs*K3*spp(ii-1);
    //model
    sm(ii)=sm1(ii)+sm2(ii)+sm3(ii);
    //prediction
    spp(ii)=sp(ii)+sm(ii)-sm(ii-r);
    d=(100-spp(ii))*lh+sm1(ii)*bmh+sm2(ii)*bsh+sm3(ii)*bsh-spp(ii)*bsh*K3;
    e(ii)=d/(K1*bmh+K2*bsh);
    if ( e(ii) > 25 ) then e(ii) = 25;end
end
//----------------------------------------------------------------------------
// Representation
scf(1);
a=gca();
a.auto_clear="on";
plot(w,e,"k",w,sp,"r",w,v*100,"k",w,pert*20,"b");
xgrid();
xtitle ( "COMANDE PROCESSUS INTEGRATEUR  / 1ER ORDRE ");
xstring(550, 95, "CV ");
xstring(850, 50 ,"PERTURBATION  x 20");
//----------------------------------------------------------------------------

