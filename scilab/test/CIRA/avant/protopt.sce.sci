//protopt
clear ("all")
close
xdel(winsid())
mode(-1)
tf=1200;
tech=1;
w=1:1:tf;
u=ones(1,tf); qdes=u*0;tdes= u*0;
taup=30 ; ap=exp(-tech/taup); bp=1-ap;// process simulé
kp=3.4;
PI=%pi;
gl=input("gl=1:basse freq/ gl=2 haute freq / gl=3 optimum / gl=4 wobulé");
for ii=2:1:tf
    if gl==1;  qdes(ii)= 1.1*sign( sin (2*PI*ii/ 3500));end;// entrée
    if gl==2;  qdes(ii)= 1.1*sign( sin (2*PI*ii/ 60));end;// entrée
    if gl==3   qdes(ii)= 1.1*sign( sin (2*PI*ii/ 360));end;// entrée
    if gl==4;  qdes(ii)= 1.1*sign( sin (2*PI*ii/( 650-ii*400/tf)));end;// entrée

    if ii<100; qdes (ii)=0;end
    tdes(ii)=tdes(ii-1)*ap+bp*kp*(qdes(ii-1));//sortie process
end

scf(1);
a=gca();a.auto_clear="on";//----------------------------------------?fig1
plot(w, qdes, "b",w, tdes,"r");
set(gca(),"data_bounds",[1,-5;tf,5]);
xgrid;
xtitle ( "PROTO qdes b/ tdes  r");

//%-----identification rapide
CC=10e10;
taumin=20;taumax=39;;// recherche pour taum compris entre taumin et taumax
tinit=80; //% début identification;
tfin=tf;
N=80;;// nombre de tau testés
e= qdes ; sp=tdes;smm=sp*0;
VPP=0;niv=0;
ecart=2.4;
for ii=2:1:tf
    if ii>tinit & ii<tfin;
    VPP=VPP+sp(ii)*sp(ii);end
    niv=niv+ecart*ecart;
end
niv=niv/(tf-tinit);// niveau isodistance
VMP=0;VMM=0;smm=zeros(1,tf);
Cmin= 10e10;
disp("Niv = "+string(niv))
pvm_set_timer()
//-----------------------------------------------------------------------------
for j=1:1:N
    taum=taumin+(j-1)*(taumax-taumin)/(N-1);
    am=exp(-tech/taum); bm=1-am;
    smm=sp;t(j)=taum;
    VMM=0;VMP=0;
    for ii=2:1:tf
        if ii<tinit;smm(ii)=sp(ii)*0;else;
        smm(ii)=smm(ii-1)*am+bm*e(ii-1);end
        if ii>tinit & ii<tfin;
        VMM=VMM+smm(ii)*smm(ii); VMP=VMP+smm(ii)*sp(ii);end
    end
    Vmm(j)=VMM;Vmp(j)=VMP;
    K(j)=VMP/VMM;
    C=K(j)*K(j)*VMM-2*K(j)*VMP+VPP;
    if C<Cmin; Cmin=C;Kopt=K(j); tauopt=t(j);Copt=C;jopt=j;end
end
for j=1:1:N
    deter = Vmp(j)*Vmp(j)-Vmm(j)*(VPP-(Copt+niv));
    if deter>0;k1(j)=(Vmp(j)-sqrt(deter))/Vmm(j);k2(j)=(Vmp(j)+sqrt(deter))/Vmm(j);
    else; k1(j)=Vmp(j)/Vmm(j);k2(j)=k1(j);end
end
exetime=pvm_get_timer()/1000;
disp("exetime = "+ string(exetime)+" ms")
disp( " ")
disp("Kopt        = " + string(Kopt))
disp("tauopt      = " + string(tauopt))
//disp('retard   = ' + string(rm))

am=exp(-tech/tauopt); bm=1-am;smm=sp*0;
for ii=2:1:tf;//%-----------------simulation modèle optimal
    if ii<tinit;smm(ii)=sp(ii)*0;else;
    smm(ii)=smm(ii-1)*am+bm*e(ii-1)*Kopt;end
end
CRIT=mean(abs(smm (tinit:tfin)-sp(tinit:tfin)));
disp("CRITmoy.abs  =" + string(CRIT))
disp("maxabserror  =" + string(max(abs(smm-sp))))
scf(2);//-------------------------------------------------------------------fig2
a=gca();
a.auto_clear="on";
plot(w, e, "b",w,smm,"k",w,sp,"r")
xgrid

disp(" ")
fl= input(" fl =0 sans superposition")
a=gca();

if fl==0;a.auto_clear="on";end

scf(3);//------------------------------------------------------------------fig3
a=gca();
plot(t,k1,"r",t,k2,"r",t,K,"b",tauopt,Kopt,"r*",taup,kp,"*b");
xgrid;
set(gca(),"data_bounds",[20,3;40,4]);

T=floor(tauopt*100)/100;
K=floor(Kopt*100)/100;

a.x_label.text="tau"
a.y_label.text="K"
a.x_label.font_size=4
a.y_label.font_size=4

xstring(taup*1.01, kp*.97,"Process(4,30)" );
text1 = gce() ; // dernier handle créé, ici l'objet texte
text1.font_size = 2 ;
xstring(tauopt,Kopt*1.025,"tau=" + string(T));
text2 = gce() ; // dernier handle créé, ici l'objet texte
text2.font_size = 4 ;
xstring(tauopt,Kopt/1.020,"K=" + string(K));
text3 = gce() ; // dernier handle créé, ici l'objet texte
text3.font_size = 4 ;

xtitle(" ISO-DISTANCE   tau / K ");

