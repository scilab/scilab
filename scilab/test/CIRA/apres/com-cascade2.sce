//----------------------------------------------------------------------------
// com-cascade2.sce
// Auteur : Jacques Richalet
// 2007
// commande en cascade avec transfert de contrainte
//----------------------------------------------------------------------------
clear("all") ;
close() ;
xdel(winsid());
mode(-1);
//----------------------------------------------------------------------------
//definition du processus
//----------------------------------------------------------------------------
tech =1;

//processus interne

tau1 = 20;
a1=exp(-tech/tau1);
b1=1-a1;
k1=0.5;

//processus externe

tau2 = 60;
a2=exp(-tech/tau2);
b2=1-a2;
kp=2;
rp=10;
tf =700;
w=1:1:tf;

//variables

u=zeros(1,tf);
pert=u;
tec=u;
tmass= u;
tmasmod=u;
tecm=u;
mv=u;
constec=u;
//tec
trbfcalo=25;
//pfc interne ---------------------------trbf tec
lh1=1-exp(-tech*3/trbfcalo);
//tmass
trbfmass=110;0
//pfc externe--------------------------------trbf tmasse
lh2=1-exp(-tech*3/trbfmass);
mvmax=120;//max de mv1
fl=input("fl =1 avec transfert fl =0 sans ");

for ii=2+rp : 1:tf;
    if ( ii > 500 ) then
        pert(ii)=10;
    else
        pert(ii)=0;
    end
    //processus interne type niveau 0
    tec(ii)=tec(ii-1)*a1+b1*k1*mv(ii-1);
    //processus externe type temperature de masse
    tmass(ii)=tmass(ii-1)*a2+b2*kp*(tec(ii-1-rp)+pert(ii));
    //--  -régulateur aval tmasse
    tmasmod(ii)=tmasmod(ii-1)*a2+b2*kp*constec(ii-1);
    //compensation retard
    spred= tmass(ii)+tmasmod(ii)-tmasmod(ii-rp);
    d=(100-tmass(ii))*lh2+b2*tmasmod(ii);
    constec(ii)=d/(kp*b2);
    //----régulateur amont  tecalo
    //modèle
    tecm(ii)= tecm(ii-1)*a1+b1*k1*mv(ii-1);
    d=(constec(ii)-tec(ii))*lh1+b1*tecm(ii);
    mv(ii)=d/(k1*b1);
    //transfert de contrainte ou non
    if ( ( fl == 1 ) & ( mv(ii) >= mvmax) ) then
        mv(ii)=mvmax;
        //transfert de contrainte------------------------------
        constec(ii)=tec(ii)+(mvmax*k1*b1-b1*tecm(ii)/lh1);
    end

    if ( fl == 1 ) & ( mv(ii) < 0 ) then
        mv(ii)=0;
        constec(ii)=tec(ii)+(0*k1*b1-b1*tecm(ii)/lh1);
    end

    if ( fl == 0 ) & ( mv(ii) < 0 ) then
        mv(ii)=0;
    end

    if ( fl == 0 ) & ( mv(ii) >= mvmax ) then
        mv(ii)=mvmax;
    end//sans transfert

end // fin du for

// Dessin
scf(1);
a=gca();
a.auto_clear="on";
plot(w,mv,"k ",w, tmass,"r",w,ones(1,tf)*100,"r",w,constec,"b--",w,tec,"b",w,zeros(1,tf),"k",w,pert,"g");
xgrid();
set(gca(),"data_bounds",[1, -5 ;700 ,160]);

if ( fl == 1 ) | ( fl == 0 ) then
    if ( fl == 1 ) then
        str = "AVEC";
    end

    if ( fl == 0 ) then
        str = "SANS";
    end

    xtitle( str+" TRANSFERT DE CONTRAINTE :MV1MAX=120 ");
end
//----------------------------------------------------------------------------


