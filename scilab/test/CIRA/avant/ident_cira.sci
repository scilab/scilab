//ident_cira
clear
close

tf=1000;w=1:1:tf;tech=1;
taup=45;ap=exp(-tech/taup); bp=1-ap;//------------------process
tau= 20;am=exp(-tech/tau);bm=1-am;u=ones(1,tf);//model
sm=u*0;e=u*0;sp=u*0;
pert=u*0;
trbf=100;
lh=1-exp(-tech*3/trbf);
kp=1.5;km=1;
sme=u*0;ee=u*0;spm=u*0;pee=u*0;em=u;cons=u;smm=u;sm=u*0;e=u*0;sp=u*0;

PP=0;MM=0;MP=0;
B=input(" B (0 ou 200!)");//niveau de bruit
//B=200;
//obtention de la sortie du processus régulé
for ii=2:1:tf
    cons(ii)=100*sign(sin(ii*2*(%pi)/(700-0.3*ii)));
    pert(ii)= B*(rand(1)-0.5);//perturbation
    sp(ii)=sp(ii-1)*ap+bp*kp*(e(ii-1)+pert(ii-1));
    sm(ii)=sm(ii-1)*am+bm*km*(e(ii-1));
    d= (cons(ii)-sp(ii))*lh+sm(ii)*bm;
    e(ii)=d/(km*bm);
end

scf(1);
a=gca();a.auto_clear="on";
plot(w,e,"k",w,sp,"r")
xgrid
xtitle("IDENTIFICATION E k / SP r")

//-----identification rapide

CC=10e10;
for j=1:1:40
    taum=20+(j-1)*40/40;
    am=exp(-tech/taum); bm=1-am;

    MP=0;MM=0;smm=zeros(1,tf);
    for ii=2:1:tf//-----------------simulation modèle
        smm(ii)=smm(ii-1)*am+bm*e(ii-1);
        MP=MP+smm(ii)*sp(ii);
        MM=MM+smm(ii)*smm(ii);
    end
    K=MP/MM;
    //C=K*K*MM-2*K*MP+PP;au choix
    C=PP-(MP*MP/MM);
    if C<CC; CC=C; Kopt=K;tauopt=taum;end
end

disp("Kopt (1.5)=" +string(Kopt));//---------------------------------------------
disp("tauopt(45) ="+string(tauopt))

//pause
//tracé model:process
am=exp(-tech/tauopt); bm=1-am;
for ii=2:1:tf//-----------------simulation modèle
    smm(ii)=smm(ii-1)*am+bm*e(ii-1)*Kopt;
end
V=smm-sp;
disp("CRIT = " +string(mean(abs(V))))
scf(2);
a=gca();a.auto_clear="on";
plot(w, e, "b",w,smm,"k",w,sp,"r")
xgrid

xtitle ("IDENTIFICATION E b / SMODEL k / SPROCESS r /")


