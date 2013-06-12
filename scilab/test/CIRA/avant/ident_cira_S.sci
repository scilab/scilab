//ident_cira_S
//pause
clear ;
mode(-1)
close;
tf=1000;w=1:1:tf;tech=1;
taup=45;ap=exp(-tech/taup); bp=1-ap;//------------------process
tau= 20;am=exp(-tech/tau);bm=1-am;
u=ones(1,tf);//model
sm=u*0;e=u*0;sp=u*0;
sme=u*0;ee=u*0;spm=u*0;pee=u*0;em=u;cons=u;smm=u;
pert=u*0;
trbf=100;
lh=1-exp(-tech*3/trbf);
kp=1.5;km=1;

PP=0;MM=0;MP=0;

//B=input('B');//niveau de bruit
B=100
//obtention de la sortie du processus
for ii=2:1:tf
    cons(ii)=100*sign(sin(ii*2*%pi/(700-0.3*ii)));
    pert(ii)= B*(rand(1)-0.5);//perturbation
    sp(ii)=sp(ii-1)*ap+bp*kp*(e(ii-1)+pert(ii-1));
    sm(ii)=sm(ii-1)*am+bm*km*(e(ii-1));
    d= (cons(ii)-sp(ii))*lh+sm(ii)*bm;
    e(ii)=d/(km*bm);
end

scf(1);//-------------------------------------------
a=gca();a.auto_clear="on";
plot(w,e,"k",w,sp,"r")
xgrid
xtitle("IDENTIFICATION E k / SP r")
//pause
//-----identification rapide
CC=10e10;
for j=1:1:40
    taum=20+(j-1)*40/40;
    am=exp(-tech/taum); bm=1-am;
    //timer()
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
Kopt
tauopt

//tracé model:process
am=exp(-tech/tauopt); bm=1-am;
for ii=2:1:tf//-----------------simulation modèle
    smm(ii)=smm(ii-1)*am+bm*e(ii-1)*Kopt;
end
//timer()
//disp(timer)
V=smm-sp;
CRIT=mean(abs(V));
disp("CRIT      = " +string(CRIT))
[m,n]=max(V);
disp("Maxerreur =" +string(m))

scf(2);//----------------------------------------------------
a=gca();
fl= input(" fl =0 sans superposition /  ")
if fl==0;a.auto_clear="on";end

plot(w, e, "b",w,smm,"k",w,sp,"r");
xgrid;
xtitle(" IDENTIFICATION RAPIDE e b/ sm k/ sp r")


