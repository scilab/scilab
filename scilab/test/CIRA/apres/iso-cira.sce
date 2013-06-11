//----------------------------------------------------------------------------
// iso-cira.sce
// Auteur : Jacques Richalet
// 2007
//----------------------------------------------------------------------------
clear("all") ;
close() ;
xdel(winsid());
mode(-1);
//----------------------------------------------------------------------------
tf=1200;
tech=1;
w=1:1:tf;
u=ones(1,tf);
qdes=u*0;
tdes= u*40;
taup=30;
ap=exp(-tech/taup);
bp=1-ap;// process simulé
kp=4;
//KB=input('KB ');
KB=1.7;// niveau de bruit
for ii=2:1:tf
    br(ii)=KB*(rand(1)-0.5);
    if ( ii < 100 ) then
        qdes(ii)=0;
    else
        qdes(ii)= sign( sin (2*%pi*ii/( 650-ii*400/tf)));
    end
    // entrée
    tdes(ii)=tdes(ii-1)*ap+bp*kp*(qdes(ii-1)+10+br(ii));//sortie process
end
//----------------------------------------------------------------------------
// Representation
scf(1);
a=gca();
a.auto_clear="on";
plot(w, qdes, "b",w, tdes,"r");
xgrid();
xtitle ( "Avant filtrage qdes b/ tdes  r");
qdespb=qdes;
tdespb=tdes;//filtres
qdesph1=qdes;
tdesph1=tdes;
qdesph2=qdes*0;
tdesph2=tdes*0;
//----------------------------------------------------------------------------
//FILTRAGE parallèle
//passe bas
//taub=input('taub (20) ');//%constante de temps du filtre  ( omegcoupure=1/tau  , ne pas oublier..!)
taub=20;
afb=exp(-tech/taub) ;
bfb=1-afb;//% passage en discret
//----------------------------------------------------------------------------
//passe haut
//tauh=input('tauh (1200)');//%constante de temps du filtre  ( omegcoupure=1/tau  , ne pas oublier..!)
tauh=1000;
afh=exp(-tech/tauh);
bfh=1-afh;//% passage en discre
for ii=2:1:tf
    //  filtre passe bas
    qdespb(ii)=qdespb(ii-1)*afb+bfb*qdes(ii);
    tdespb(ii)=tdespb(ii-1)*afb+bfb*tdes(ii);

    //filtre passe-haut
    qdesph1(ii)=qdesph1(ii-1)*afh+bfh*qdespb(ii);
    tdesph1(ii)=tdesph1(ii-1)*afh+bfh*tdespb(ii);

    qdesph2(ii)=qdespb(ii)-qdesph1(ii);
    tdesph2(ii)=tdespb(ii)-tdesph1(ii);

end // fin du for

qdes=qdesph2;
tdes=tdesph2;

scf(2);
a=gca();
a.auto_clear="on";
plot(w, qdes, "b",w,tdes,"r");
xgrid();
xtitle ( "Après filtrage parallèle :Qdes b/ Tsur-Tdes  r");
//%-----identification rapide
CC=10e10;
taumin=20;
taumax=39;// recherche pour taum compris entre taumin et taumax
tinit=80; //% début identification;
tfin=tf;
N=80;// nombre de tau testés
e= qdes;
sp=tdes;
smm=sp*0;
VPP=0;
niv=0;
ecart=1.15;
for ii=2:1:tf
    if ( ii > tinit ) & ( ii < tfin ) then
        VPP=VPP+sp(ii)*sp(ii);
    end
    niv=niv+ecart*ecart;
end

niv=niv/(tf-tinit);// niveau isodistance
VMP=0;
VMM=0;
smm=zeros(1,tf);
Cmin= 10e10;
disp("Niv = "+string(niv));
timer();
//-----------------------------------------------------------------------------
for j=1:1:N // for (1)
    taum=taumin+(j-1)*(taumax-taumin)/(N-1);
    am=exp(-tech/taum); bm=1-am;
    smm=sp;
    t(j)=taum;
    VMM=0;
    VMP=0;
    for ii=2:1:tf // for (2)
        if ( ii < tinit ) then
            smm(ii)=sp(ii)*0;
        else
            smm(ii)=smm(ii-1)*am+bm*e(ii-1);
        end
        if ( ii > tinit ) & ( ii < tfin ) then
            VMM=VMM+smm(ii)*smm(ii);
            VMP=VMP+smm(ii)*sp(ii);
        end
    end // fin for (2)
    Vmm(j)=VMM;
    Vmp(j)=VMP;
    K(j)=VMP/VMM;
    C=K(j)*K(j)*VMM-2*K(j)*VMP+VPP;
    if ( C < Cmin ) then
        Cmin=C;
        Kopt=K(j);
        tauopt=t(j);
        Copt=C;
        jopt=j;
    end
end // fin for (1)

for j=1:1:N
    deter = Vmp(j)*Vmp(j)-Vmm(j)*(VPP-Copt*niv);
    if ( deter > 0 ) then
        k1(j)=(Vmp(j)-sqrt(deter))/Vmm(j);
        k2(j)=(Vmp(j)+sqrt(deter))/Vmm(j);
    else
        k1(j)=Vmp(j)/Vmm(j);k2(j)=k1(j);
    end
end
exetime=timer();
disp("exetime = "+ string(exetime)+" ms");
disp( " ");
disp("Kopt        = " + string(Kopt));
disp("tauopt      = " + string(tauopt));
//disp('retard   = ' + string(rm))

am=exp(-tech/tauopt);
bm=1-am;
smm=sp*0;
for ii=2:1:tf;//%-----------------simulation modèle optimal
    if ( ii < tinit ) then
        smm(ii)=sp(ii)*0;
    else
        smm(ii)=smm(ii-1)*am+bm*e(ii-1)*Kopt;
    end
end
CRIT=mean(abs(smm (tinit:tfin)-sp(tinit:tfin)));
disp("CRITmoy.abs  =" + string(CRIT));
disp("maxabserror  =" + string(max(abs(smm-sp))));
//-----------------------------------------------------------------------------
//Representation
//-----------------------------------------------------------------------------
scf(3);
a=gca();
a.auto_clear="on";
plot(w, e, "b",w,smm,"k",w,sp,"r");
xgrid();
xtitle(" IDENTIFICATION  Tdes Sollac :qdes b/ tsur-tdes r/ smm k");
//-----------------------------------------------------------------------------
//Representation
//-----------------------------------------------------------------------------
scf(4);
a=gca();
disp(" ");
fl= input(" fl =0 sans superposition");
if ( fl == 0 ) then a.auto_clear="on";end
plot(t,k1,"r",t,k2,"r",t,K,"b",tauopt,Kopt,"r*",taup,kp,"*b");
xgrid();

T=floor(tauopt*100)/100;
K=floor(Kopt*100)/100;

a.x_label.text="tau";
a.y_label.text="K";
a.x_label.font_size=4;
a.y_label.font_size=4;

xstring(taup*1.01, kp*.97,"Process(4,30)" );
a=gce();
a.font_size=4;

xstring(tauopt,Kopt*1.025,"tau=" + string(T))
a=gce();
a.font_size=4;

xstring(tauopt,Kopt/1.020,"K=" + string(K));
a=gce();
a.font_size=2;

xtitle(" ISO-DISTANCE   tau / K ");
//-----------------------------------------------------------------------------

