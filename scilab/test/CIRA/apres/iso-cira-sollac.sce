//----------------------------------------------------------------------------
// iso-cira-sollac.sce
// Auteur : Jacques Richalet
// 2007
//----------------------------------------------------------------------------
clear("all") ;
close() ;
xdel(winsid());
mode(-1);
//----------------------------------------------------------------------------
load qdescira.dat
tf=length(w);// w est dans le fichier
tech=1;
u=ones(1,tf);
qdes=v1;
tdes=v2;
tsur=v3;// affichage des variables

scf(1);
a=gca(1);
a.auto_clear="on";
plot(w, qdes, "b",w, tdes,"r",w, tsur,"k");
xgrid();
xtitle ( "Avant filtrage qdes b/ tdes  r");

qdespb=qdes;
tdespb=tdes;//filtres
qdesph1=qdes;
tdesph1=tdes;
qdesph2=qdes*0;
tdesph2=tdes*0;

//FILTRAGE parallèle
//passe bas
//taub=input('taub (20) ');//%constante de temps du filtre  ( omegcoupure=1/tau  , ne pas oublier..!)
taub=20;
afb=exp(-tech/taub) ;
bfb=1-afb;//% passage en discret
//passe haut
//tauh=input('tauh (1200)');//%constante de temps du filtre  ( omegcoupure=1/tau  , ne pas oublier..!)
tauh=1000;
afh=exp(-tech/tauh);
bfh=1-afh;//% passage en discret

for ii=2:1:tf
    //  filtre passe bas
    qdespb(ii)=qdespb(ii-1)*afb+bfb*qdes(ii);
    tdespb(ii)=tdespb(ii-1)*afb+bfb*tdes(ii);

    //filtre passe-haut
    qdesph1(ii)=qdesph1(ii-1)*afh+bfh*qdespb(ii);
    tdesph1(ii)=tdesph1(ii-1)*afh+bfh*tdespb(ii);

    qdesph2(ii)=qdespb(ii)-qdesph1(ii);
    tdesph2(ii)=tdespb(ii)-tdesph1(ii);
end

qdes=qdesph2;
tdes=tdesph2;
T=tsur*0-tdes;
Tm=T;

// Representation
scf(2);
a=gca();
a.auto_clear="on";
plot(w, qdes, "b",w,T,"r");
xgrid();
xtitle ( "Après filtrage parallèle :Qdes b/ Tsur-Tdes  r");
//%-----identification globale
CC=10e10;
taumin=20;
taumax=39;
tinit=410; //% début identification;
tfin=tf;// fin identification
N=60;// nombre de points test de tau
e= qdes ;
sp=T;
smm=sp*0;
VPP=0;
niv=0;
ecart=input("ecart /1.0 ") ;//niveau d'erreur de l'iso recherch
for ii=2:1:tf
    if ( ii > tinit) & (ii < tfin ) then
        VPP=VPP+sp(ii)*sp(ii)
    end
    niv=niv+ecart*ecart;
end

niv=niv/(tf-tinit);
VMP=0;
VMM=0;
smm=zeros(1,tf);
Cmin= 10e10;
disp("Niveau moyen = "+string(niv));// niveau normalis
timer();
//-----------------------------------------------------------------------------
for j=1:1:N  // for (1)
    taum=taumin+(j-1)*(taumax-taumin)/(N-1);
    am=exp(-tech/taum);
    bm=1-am;
    smm=sp;
    t(j)=taum;
    VMM=0;
    VMP=0;
    for ii=2:1:tf
        if ( ii < tinit ) then
            smm(ii)=sp(ii)*0;
        else
            smm(ii)=smm(ii-1)*am+bm*e(ii-1);
        end
        if ( ii > tinit ) & (ii < tfin ) then
            VMM=VMM+smm(ii)*smm(ii);
            VMP=VMP+smm(ii)*sp(ii);
        end
    end
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
end // fin du for (1)

for j=1:1:N
    deter = Vmp(j)*Vmp(j)-Vmm(j)*(VPP-Copt*niv);
    if ( deter > 0 ) then
        k1(j)=(Vmp(j)-sqrt(deter))/Vmm(j);
        k2(j)=(Vmp(j)+sqrt(deter))/Vmm(j);
    else
        k1(j)=Vmp(j)/Vmm(j);k2(j)=k1(j);
    end
end

disp("Kopt         = " + string(Kopt));
disp("tauopt       = " + string(tauopt));

// modèle optimal
am=exp(-tech/tauopt);
bm=1-am;smm=sp*0;
for ii=2:1:tf;//%-----------------simulation modèle
    if ( ii < tinit ) then
        smm(ii)=sp(ii)*0;
    else
        smm(ii)=smm(ii-1)*am+bm*e(ii-1)*Kopt;
    end
end
timer()
//disp(timer)
CRIT=mean(abs(smm (tinit:tfin)-sp(tinit:tfin)));

disp("CRITmoy.abs  = " + string(CRIT));
disp(" ");

// Representation
scf(3);
a=gca();
a.auto_clear="on";
plot(w, e, "b",w,smm,"k",w,sp,"r");
xgrid();
xtitle(" IDENTIFICATION  Tdes Sollac :qdes b/ tsur-tdes r/ smm k");

scf(4);
a=gca();
fl= input(" fl =0 sans superposition");
if ( fl == 0 ) then
    a.auto_clear="on";
end
plot(t,k1,"r",t,k2,"r",t,K,"b",tauopt,Kopt,"r*");
xgrid();
xtitle(" ISO-DISTANCE   tau / K ");
T=floor(tauopt*100)/100;
K=floor(Kopt*100)/100;

a=gca();
a.x_label.text="tau";
a.y_label.text="K";
a.x_label.font_size=4;
a.y_label.font_size=4;

mk1=min(k1);
NIVaff=floor(niv*100)/100;
xstring(tauopt,mk1,"niv= "+string(NIVaff));
xstring(tauopt,Kopt*1.01,"tau=" + string(T));
xstring(tauopt,Kopt/1.005,"K=" + string(K));
a.children(1).font_size=4;
a.children(2).font_size=4;
a.children(3).font_size=4;
xtitle(" ISO-DISTANCE  SOLLAC   tau / K ");
//----------------------------------------------------------------------------

