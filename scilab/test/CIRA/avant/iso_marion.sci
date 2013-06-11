//iso_marion
clear ;
close ;
mode(-1)
load test1.dat;
tf=length(w);
e=v1;sp=v2;tech=1;
scf(1);
a=gca();a.auto_clear="on";
plot(w,e,"k",w,sp,"r")
xgrid
xtitle("IDENTIFICATION E k / SP r")

v1=v1-10; v2=v2-26.5;
e=v1;sp=v2;
scf(2);
a=gca();a.auto_clear="on";
plot(w,e,"k",w,sp,"r")
xgrid
xtitle("IDENTIFICATION E k / SP r")
//identification globale
// fitrage parallèle
tauh=300;ah=exp(-tech/tauh);bh=1-ah;
v1b=v1;v2b=v2;v1h=v1*0;v2h=v2*0;
for ii=2:1:tf;
    v1b(ii)=v1b(ii-1)*ah+bh*v1(ii);
    v1h(ii)=v1(ii)-v1b(ii);
    v2b(ii)=v2b(ii-1)*ah+bh*v2(ii);
    v2h(ii)=v2(ii)-v2b(ii);
end
e=v1h;sp=v2h;
scf(3);
a=gca();a.auto_clear="on";
plot(w,e,"k",w,sp,"r")
xgrid
xtitle("IDENTIFICATION E k / SP r")

CC=10e10;
taumin=120;taumax=140;;// recherche pour taum compris entre taumin et taumax
tinit=260; //% début identification;
tfin=tf;
N=40;// nombre de tau testés
smm=sp*0;
VPP=0;niv=0;
ecart=1.0;
rm=input("rm (81) =");

for ii=2:1:tf
    if ii>tinit & ii<tfin;
    VPP=VPP+sp(ii)*sp(ii);end
    niv=niv+ecart*ecart;
end
niv=niv/(tf-tinit);// niveau isodistance
VMP=0;VMM=0;smm=zeros(1,tf);
Cmin= 10e10;
disp("Niv = "+string(niv));
pvm_set_timer();
//-----------------------------------------------------------------------------
for j=1:1:N
    taum=taumin+(j-1)*(taumax-taumin)/(N-1);

    am=exp(-tech/taum); bm=1-am;
    smm=sp;t(j)=taum;
    VMM=0;VMP=0;
    for ii=2+rm:1:tf
        if ii<tinit;smm(ii)=sp(ii)*0;else;
        smm(ii)=smm(ii-1)*am+bm*e(ii-1-rm);end
        if ii>tinit & ii<tfin;
        VMM=VMM+smm(ii)*smm(ii); VMP=VMP+smm(ii)*sp(ii);end
    end
    Vmm(j)=VMM;Vmp(j)=VMP;
    K(j)=VMP/VMM;
    C=K(j)*K(j)*VMM-2*K(j)*VMP+VPP;
    if C<Cmin; Cmin=C;Kopt=K(j); tauopt=t(j);Copt=C;jopt=j;end
end
for j=1:1:N
    deter = Vmp(j)*Vmp(j)-Vmm(j)*(VPP-Copt*niv);
    if deter>0;k1(j)=(Vmp(j)-sqrt(deter))/Vmm(j);k2(j)=(Vmp(j)+sqrt(deter))/Vmm(j);
    else; k1(j)=Vmp(j)/Vmm(j);k2(j)=k1(j);end
end
exetime=pvm_get_timer()/1000;
disp("exetime = "+ string(exetime)+" ms");
disp( " ");
disp("Kopt         = " + string(Kopt));
disp("tauopt       = " + string(tauopt));
disp("rm           = " +string(rm ));
//disp('retard     = ' + string(rm));

am=exp(-tech/tauopt); bm=1-am;smm=sp*0;
for ii=2+rm:1:tf;//%-----------------simulation modèle optimal
    if ii<tinit;smm(ii)=sp(ii)*0;else;
    smm(ii)=smm(ii-1)*am+bm*e(ii-1-rm)*Kopt;end;
end
CRIT=mean(abs(smm (tinit:tfin)-sp(tinit:tfin)));
disp("CRITmoy.abs  = " + string(CRIT));
disp("maxabserror  = " + string(max(abs(smm-sp))));
scf(3);//-------------------------------------------------------------------fig3
a=gca();
a.auto_clear="on";
plot(w, e, "b",w,smm,"k",w,sp,"r");
xgrid;
xtitle(" IDENTIFICATION  Tdes Sollac :qdes b/ tsur-tdes r/ smm k");
scf(4);//-------------------------------------------------------------------fig4
a=gca();
disp(" ");
fl= input(" fl =0 sans superposition");
if fl==0;a.auto_clear="on";end;
plot(t,k1,"r",t,k2,"r",t,K,"b",tauopt,Kopt,"r*");
//set(gca(),"data_bounds",[20,3.3;40,4.4]);
xgrid ;

T=floor(tauopt*100)/100;
K=floor(Kopt*100)/100;

a.x_label.text="tau";
a.y_label.text="K";
a.x_label.font_size=4;
a.y_label.font_size=4;
//xstring(taup*1.01, kp*.97,'Process(4,30)' );
xstring(tauopt,Kopt*1.025,"tauopt=" + string(T));
xstring(tauopt,Kopt/1.020,"K=" + string(K));
a.children(1).font_size=4;
a.children(2).font_size=4;
a.children(3).font_size=2;
//a.children(4).font_size=4;
xtitle(" ISO-DISTANCE   tau / K ");


