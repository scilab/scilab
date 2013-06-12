//----------------------------------------------------------------------------
// ident-chauf.sce
// Auteur : Jacques Richalet
// 2007
// commande en cascade avec transfert de contrainte
//----------------------------------------------------------------------------
clear("all") ;
close() ;
xdel(winsid());
mode(-1);
//----------------------------------------------------------------------------
load("test1.dat");
tf=length(w);
e=v1;
sp=v2;
tech=1;
//----------------------------------------------------------------------------
// Representation des donnees
scf(1);
a=gca();
a.auto_clear="on";
plot(w,e,"k",w,sp,"r");
xgrid
xtitle("IDENTIFICATION E k / SP r")
//----------------------------------------------------------------------------
v1=v1-10;
v2=v2-26.5;
e=v1;
sp=v2;
scf(2);
a=gca();
a.auto_clear="on";
plot(w,e,"k",w,sp,"r");
xgrid();
xtitle("IDENTIFICATION E k / SP r")
//----------------------------------------------------------------------------
//-----identification rapide
// fitrage parallèle
tauh=300;
ah=exp(-tech/tauh);
bh=1-ah;
v1b=v1;
v2b=v2;
v1h=v1*0;
v2h=v2*0;
for ii=2:1:tf;
    v1b(ii)=v1b(ii-1)*ah+bh*v1(ii);
    v1h(ii)=v1(ii)-v1b(ii);
    v2b(ii)=v2b(ii-1)*ah+bh*v2(ii);
    v2h(ii)=v2(ii)-v2b(ii);
end // fin du for
e=v1h;
sp=v2h;
scf(3);
a=gca();
a.auto_clear="on";
plot(w,e,"k",w,sp,"r");
xgrid();
xtitle("IDENTIFICATION E k / SP r");
//----------------------------------------------------------------------------
smm=sp;
PP=sp'*sp;
CC=10e10;
taumin=input("taumin = ");
taumax=input("taumax = ");
//taumin=120;
//taumax=180;
N=40;
NN=N-1;
rm=input("rm(retard process)=  ");
disp("rm ="+string(rm));
for j=1:1:N // for (1)
    taum=taumin+(j-1)*(taumax-taumin)/NN;
    //disp(taum);
    am=exp(-tech/taum);
    bm=1-am;

    MP=0;
    MM=0;
    smm=zeros(1,tf);

    for ii=2+rm:1:tf//-----------------simulation modèle
        smm(ii)=smm(ii-1)*am+bm*e(ii-1-rm);
        MP=MP+smm(ii)*sp(ii);
        MM=MM+smm(ii)*smm(ii);
    end // fin du for

    K=MP/MM;
    //C=K*K*MM-2*K*MP+PP;au choix
    C=PP-(MP*MP/MM);
    if ( C < CC ) then
        CC=C;
        Kopt=K;
        tauopt=taum;
    end

end // fin for (1)
//----------------------------------------------------------------------------
disp("Kopt =" +string(Kopt));
disp("tauopt  ="+string(tauopt));
//----------------------------------------------------------------------------
//pause
//tracé model:process
am=exp(-tech/tauopt);
bm=1-am;
smm=sp;

for ii=2+rm:1:tf //-----------------simulation modèle
    smm(ii)=smm(ii-1)*am+bm*e(ii-1-rm)*Kopt;
end

V=smm-sp;
disp("CRIT = " +string(mean(abs(V))));
//----------------------------------------------------------------------------
// Representation
//----------------------------------------------------------------------------
scf(4);
a=gca();
a.auto_clear="on";
plot(w, e, "b",w,smm,"k",w,sp,"r");
xgrid();
xtitle ("IDENTIFICATION E b / SMODEL k / SPROCESS r /");
pause ();
d=input("d (3.5)");
L=input("L (25)");
Vol=3.1415*d*d*L*0.25/10000;
REIF=(3000)*(1.4)*Vol;
disp("REIF = "+string(REIF));
disp("RIDENT= "+string(tauopt/Kopt));
//----------------------------------------------------------------------------


