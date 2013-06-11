// com_cascade2
//commande en cascade avec transfert de contrainte
clear ;
close ;
//definition du processus
tech =1;
tau1 = 20;a1=exp(-tech/tau1); b1=1-a1; k1=0.5;        //processusinterne
tau2 = 60;a2=exp(-tech/tau2); b2=1-a2; kp=2;rp=10;    //processus externe
tf =700;w=1:1:tf;
//variables
u=zeros(1,tf) ;
pert=u;tec=u; tmass= u;tmasmod=u;tecm=u;mv=u;constec=u;
//tec
trbfcalo=25; lh1=1-exp(-tech*3/trbfcalo);//pfc interne ---------------------------trbf tec
//tmass
trbfmass=110; lh2=1-exp(-tech*3/trbfmass);//pfc externe--------------------------------trbf tmasse
mvmax=120;//max de mv1
fl=input("fl =1 avec transfert fl =0 sans ");

for ii=2+rp : 1:tf;
    if ii>500 ;pert(ii)=10; else; pert(ii)=0;end
    tec(ii)=tec(ii-1)*a1+b1*k1*mv(ii-1);    //processus interne type niveau 0
    tmass(ii)=tmass(ii-1)*a2+b2*kp*(tec(ii-1-rp)+pert(ii));//processus externe type temperature de masse
    //--  -régulateur aval tmasse
    tmasmod(ii)=tmasmod(ii-1)*a2+b2*kp*constec(ii-1);
    spred= tmass(ii)+tmasmod(ii)-tmasmod(ii-rp);//compensation retard
    d=( 100-tmass(ii))*lh2+b2*tmasmod(ii);
    constec(ii)=d/(kp*b2);
    //----régulateur amont  tecalo
    tecm(ii)= tecm(ii-1)*a1+b1*k1*mv(ii-1);//modèle
    d=(constec(ii)-tec(ii))*lh1+b1*tecm(ii);
    mv(ii)=d/(k1*b1);
    //transfert de contrainte ou non
    if fl==1;if mv(ii)>=mvmax;mv(ii)=mvmax;//transfert de contrainte------------------------------
    constec(ii)=tec(ii)+(mvmax*k1*b1-b1*tecm(ii)/lh1);end;end
    if fl==1;if mv(ii)<0; mv(ii)=0;constec(ii)=tec(ii)+(0*k1*b1-b1*tecm(ii)/lh1);end;end
    if fl==0;if mv(ii)<0;mv(ii)=0;end;end
    if fl==0;if mv(ii)>=mvmax;mv(ii)=mvmax;end;end//sans transfert
end
if fl==1;
    scf(1);
    a=gca();
    a.auto_clear="on";

    plot(w,mv,"k ",w, tmass,"r",w,ones(1,tf)*100,"r",w,constec,"b--",w,tec,"b",w,zeros(1,tf),"k",w,pert,"g");
    xgrid;
    set(gca(),"data_bounds",[1, -5 ;700 ,160]);

    //text(110 ,80, 'Tmass')
    //text(15, 125, 'mv');
    //text(90, 40, 'ConsTec');
    //text(25, 40, 'Tec')
    //text(650,5,'sec')
    //text( 555, 12, 'Pert')
    xtitle( "AVEC TRANSFERT DE CONTRAINTE :MV1MAX=120 ");
end

if fl==0;
    scf(1);
    a=gca();
    a.auto_clear="on";
    plot(w,mv,"k ",w, tmass,"r",w,ones(1,tf)*100,"r",w,constec,"b--",w,tec,"b",w,zeros(1,tf),"k",w,pert,"g");
    xgrid;
    set(gca(),"data_bounds",[1, -5 ;700 ,160]);
    //text(205 ,105, 'Tmass');
    //text(30 ,125, 'mv');
    //text(260, 70, 'ConsTec');
    //text(30, 55, 'Tec');
    //text(650,5,'sec')
    //text( 555, 12, 'Pert')
    xtitle( "SANS TRANSFERT DE CONSTRAINTE :MV1MAX=120 ");
end

