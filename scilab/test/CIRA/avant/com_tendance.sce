//com_tendance

clear
close
tech=1;tf=1000;
w=1:1:tf;
MV=zeros(1,tf);CV=MV;Pert=MV;pertf=MV;sp=MV;
spert=MV;sm=MV;spertm=sm;
//process
K1= 1; tau1=30 ;a1=exp(-tech/tau1);b1=1-a1;
K2= 2; tau2=30 ;a2=exp(-tech/tau2);b2=1-a2;//disturbance
kf=1; tauf =5; af=exp(-tech/tauf);bf=1-af;
TRBF=70;
h=1;b1h=1-a1^h;
lh=1-exp(-tech*3*h/TRBF);
fl=input("fl =1 with feed forward fl=0 without");
K2m=K2*1;
rt=20;
rp=0;
for ii =2+rt:1:tf
    //disturbance
    if ii>150;Pert(ii)=30;end
    if ii> 500 ; Pert(ii)=30+20*sin(2*(%pi)*ii/200);end
    pertf(ii)=pertf(ii-1)*af+bf*Pert(ii-1);//disturbance
    spert(ii)= spert(ii-1)*a2+b2*pertf(ii-1-rt+rp)*K2;
    //process
    sp(ii)=sp(ii-1)*a1+b1*MV(ii-1-rp)*K1;
    CV(ii)=sp(ii)+spert(ii);
    //model of perturbation
    sm(ii)=sm(ii-1)*a1+b1*MV(ii-1)*K1;
    spertm(ii)= spertm(ii-1)*a2+b2*pertf(ii-1)*K2m;
    //process  increment
    spred =CV(ii)+sm(ii)-sm(ii-rp);
    dpert=spertm(ii-rt+rp+h)-spertm(ii-rt+rp); //delay comp
    //control
    d=( 100-spred)*lh +sm(ii)*b1h-dpert*fl;
    MV(ii)= d/(K1*b1h);
end
scf(1)
a=gca();
a.auto_clear="on";
xgrid;
plot(w,CV, "r",w,MV, "k",w,pertf,"b");
xgrid;
xstring(410 ,30,"PERT");
xstring (410, 70, "MV");
xstring(410 ,110, "CV");
xstring (900, 10, "sec");

if fl==1 ;title( " COMMANDE AVEC PRISE EN TENDANCE  ","fontsize",3);end
if fl==0 ;title( " COMMANDE SANS PRISE EN TENDANCE  ");;end
