//iso_cira_sollac

mode(-1)

//load 'qdescira2.dat'
load qdescira.dat
tf=length(w);
tech=1;
w=1:1:tf;
u=ones(1,tf);
qdes=v1;tdes=v2;tsur=v3;
scf(1)
a=gca(1);a.auto_clear="on";
plot(w, qdes, "b");//,w, tdes,'r',w, tsur)
xgrid
xtitle ( "Avant filtrage qdes b/ tdes  r")
//disp(tf)
qdespb=qdes,tdespb=tdes;//filtres
qdesph1=qdes,tdesph1=tdes;
qdesph2=qdes*0,tdesph2=tdes*0;

//FILTRAGE parallèle
//passe bas
//taub=input('taub (20) ');//%constante de temps du filtre  ( omegcoupure=1/tau  , ne pas oublier..!)
taub=20;
afb=exp(-tech/taub) ; bfb=1-afb;//% passage en discret
//passe haut
//tauh=input('tauh (1200)');//%constante de temps du filtre  ( omegcoupure=1/tau  , ne pas oublier..!)
tauh=1000;
afh=exp(-tech/tauh) ; bfh=1-afh;//% passage en discre
for ii=2:1:tf
    //  filtre passe bas
    qdespb(ii)=qdespb(ii-1)*afb+bfb*qdes(ii);
    tdespb(ii)=tdespb(ii-1)*afb+bfb*tdes(ii);

    //filtre passe-haut
    qdesph1(ii)=qdesph1(ii-1)*afh+bfh*qdespb(ii);
    tdesph1(ii)=tdesph1(ii-1)*afh+bfh*tdespb(ii);

    //%----------
    qdesph2(ii)=qdespb(ii)-qdesph1(ii);
    tdesph2(ii)=tdespb(ii)-tdesph1(ii);

end

qdes=qdesph2,tdes=tdesph2,T=tsur*0-tdes,Tm=T;

scf(2)//:----------------------------------------------------------------fig2
a=gca();
a.auto_clear="on";
plot(w, qdes, "b",w,T,"r")
xgrid
xtitle ( "Après filtrage parallèle :Qdes b/ Tsur-Tdes  r")

//%-----identification rapide
CC=10e10;
taumin=20;taumax=39;
tinit=410; //% début identification;
tfin=tf;
N=60;
e= qdes ; sp=T;smm=sp*0;
VPP=0;niv=0;
ecart=1.1;
for ii=2:1:tf
    if ii>tinit & ii<tfin;
    VPP=VPP+sp(ii)*sp(ii);end
    niv=niv+ecart*ecart;
end

niv=niv/(tf-tinit);
VMP=0;VMM=0;smm=zeros(1,tf);
Cmin= 10e10;
niv
timer()
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
    deter = Vmp(j)*Vmp(j)-Vmm(j)*(VPP-Copt*niv);
    if deter>0;k1(j)=(Vmp(j)-sqrt(deter))/Vmm(j);k2(j)=(Vmp(j)+sqrt(deter))/Vmm(j);
    else; k1(j)=Vmp(j)/Vmm(j);k2(j)=k1(j);end
end

//break
//disp( ' ')
//disp( ' ')
disp(Kopt)
//disp(['Kopt     = ' num2str(Kopt)])
//disp(['tauopt   = ' num2str(tauopt)])
//%disp(['retard   = ' num2str(rm)])
//%tracé model:process


am=exp(-tech/tauopt); bm=1-am;smm=sp*0;
for ii=2:1:tf;//%-----------------simulation modèle
    if ii<tinit;smm(ii)=sp(ii)*0;else;
    smm(ii)=smm(ii-1)*am+bm*e(ii-1)*Kopt;end
end
timer()
disp(timer)
CRIT=mean(abs(smm (tinit:tfin)-sp(tinit:tfin)));
disp(CRIT)
scf(3)// -------------------------------------------------------fig3
a=gca();
a.auto_clear="on";

plot(w, e, "b",w,smm,"k",w,sp,"r")
xgrid
xtitle(" IDENTIFICATION  Tdes Sollac :qdes b/ tsur-tdes r/ smm k")

scf(4)
a=gca();%a.auto_clear="on";
//set(gca),("data bounds" (20 45;0 5)) ???--------------------------------------------axis?
plot(t,k1,"r",t,k2,"r",t,K,"b",tauopt,Kopt,"r*");
xgrid

T=floor(tauopt*100)/100
kmax=max(k1);
kmin=min(k2);
K=floor(Kopt*100)/100
tabsc=taumax-0.05*(taumax-taumin)

xstring(tabsc, kmin+0.01*(kmax-kmin),"tau")
xstring(1.01*taumin, kmax,"K")
//xstring(tauopt,Kopt*1.005,['tau=', num2str(T)],'FontSize',12)------------------num2string
//xstring(tauopt,Kopt/1.005,['K=', num2str(K)],'FontSize',12)
xtitle(" ISO-DISTANCE   tau / K ")
