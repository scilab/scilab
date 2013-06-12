
//com_ord2avec2pfc1
//comment implanter un PFC 2nd ordre avec 2 blocs PFC 1er ordre..!
mode(-1)
clear ;
close ;
tf=4600;tech=2;w=1:1:tf;
//------------------------
u=ones(1,tf);e=u;sp=u;sm1=u;sm2=u;MV=u;spp=u;sr1=u;sr2=u;G=u;
e1=u;e2=u;smm1=sm1+10;smm2=sm2;e1=u;e2=u;E=u;EE=E;dist=u;pert=u;
K=0.8;//-------------------------------------------------------CALCUL DES K1 K2
tau1=450;//process
tau2=500;//process = model rapport 10
rp=20;//retard process
//-------------------------------2pfc1
//décomposition
K1=K*tau1/(tau1-tau2);
K2=K-K1;//------------------------------------------------------------------------
Emax=800;

k1=K1*1.5; k2=K2*1.5;// pour perturbation de gain X 1.5 / voir ligne 73
//---------------------------------------------------------
//break
am1=exp(-tech/tau1);bm1=1-am1;
am2=exp(-tech/tau2);bm2=1-am2;
//com
trbfdesir=400;
//----------------------------calcul externe du hoptimal
T=min(tau1, tau2);
gl=1;
for ii=2:1:tf;// recherche du h optimal max
    u(ii)=100;
    smm1(ii)=smm1(ii-1)*am1+bm1*u(ii-1)*K1;
    smm2(ii)=smm2(ii-1)*am2+bm2*u(ii-1)*K2;
    sm(ii)=smm1(ii)+smm2(ii);
    dev(ii)=sm(ii)-sm(ii-1);
    if  ii>T & dev(ii)<dev(ii-1)& gl==1 ;hh=ii;gl=0;;end
end

h=floor(hh*.8) ;// correction du h
//disp(h)
disp("h pt de coincidence = "+string(h));
scf(1)
a=gca();
a.auto_clear="on";
plot(w,u,"k",w,sm,"r",h,sm(h),"*k",hh,sm(hh),"*g")
xgrid;
xtitle ("RECHERCHE DU POINT DE COINCIDENCE H* OPTIMAL/ INFLEXION ");
//pause
sm=u*0;smm1=sm;smm2=sm;
//calcul externe
lh=1-exp(-tech*3*h/trbfdesir);
trbfapli=-tech*3*h/log(1-0.5*lh);//----------------------nouveau trbf
lhh=1-exp(-tech*3*h/trbfapli);
bm1h=1-am1^(h);
bm2h=1-am2^(h);
//------------------------perturbation
ap=exp(-tech/20);bp=1-ap;Kp=.5;bph=1-ap^h;//perturbation
rd=37;//retard perturbation----------------attention le retard de la perturbation doit...
//être plus grand que celui du processus
//------------------convexe extérieur aux PFC
L=(K1*bm1h)/((K1*bm1h)+K2*bm2h);
//-----------------------------------------------------
fl=input("fl=1 avec prise en tendance, 0 sans/   ")
for ii=2+rd+rp:1:tf;
    if ii<2500; G(ii)=0.8;else; G(ii)=0.8*1.5;end
    if ii< 1500;pert(ii)=0; else; pert(ii)=40;end
    dist(ii)=dist(ii-1)*ap+bp*Kp*pert(ii-1-rd);//perturbation
    if ii<3500;cons(ii)=500;else;cons(ii)=550;end

    //modèle- commande ordinaire d'ordre 2--------------------------------------------------------
    //sm1(ii)=sm1(ii-1)*am1+bm1*e(ii-1)*K1;
    //sm2(ii)=sm2(ii-1)*am2+bm2*e(ii-1)*K2;
    //sp(ii)=sm1(ii)+sm2(ii)+dist(ii);
    //DP=bph*(Kp*pert(ii-rd)-dist(ii));
    //d=(cons-sp(ii))*lh+sm1(ii)*bm1h+sm2(ii)*bm2h-DP;
    //e(ii)=d/(K1*bm1h+K2*bm2h);
    //----------------------------------------------------------processus 2x1
    //commande avec 2 pfc ordre1
    if ii>2500; K1=k1; K2=k2;end
    smm1(ii)=smm1(ii-1)*am1+bm1*E(ii-1-rp)*1*K1;
    smm2(ii)=smm2(ii-1)*am2+bm2*E(ii-1-rp)*1*K2;
    sm(ii)=smm1(ii)+smm2(ii);
    spp(ii)=sm(ii-rp)+dist(ii)*1;//processus =modèle
    //---------------
    DPP=bph*(Kp*pert(ii-(rd-rp))-dist(ii));//tendance
    //premier pfc
    spred=spp(ii)+smm1(ii)-smm1(ii-rp);
    d1=(cons(ii)-spred)*lhh+smm1(ii)*bm1h-DPP*fl/1;//2 pfc élémentaires
    e1(ii)=d1/(1*K1*bm1h);
    //deuxième pfc
    spred=spp(ii)+smm2(ii)-smm2(ii-rp);//prédiction process
    d2=(cons(ii)-spred)*lhh+smm2(ii)*bm2h-DPP*fl/1;
    e2(ii)=d2/(1*K2*bm2h);
    //------
    E(ii)=e1(ii)*L +e2(ii)*(1-L);//convexe
    //contrainte à redistribuer sur les 2 pfc..!
    if E(ii)> Emax; E(ii)=Emax;
    e1(ii)=Emax*0.5*L;e2(ii)=Emax*0.5*(1-L);end
    if E(ii)<0 ;e1(ii)=0; e2(ii)=0;end
end

scf(2)
a=gca();
//a.auto_clear='on';
plot(w,E,"k",w,spp,"r",w,pert,"b",w,G*100,"g",w,cons,"r");
xgrid
xstring(1200,115, "GAIN*100")
xstring(1200,480, "CV")
xstring(1200,640, "MV")
xstring(3200,60, "PERTURBATION")
xtitle( "COMMANDE 2ORDRE avec 2 PFC 1er ORDRE " )
