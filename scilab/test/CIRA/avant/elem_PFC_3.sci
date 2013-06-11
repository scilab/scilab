
//elem_PFC_3
mode(-1);
clear
close
//Target :To evaluate the performance of PFC with respect to performance and
//time delay compensation
//--------
tf = 400;// duration of test
w = 1:1:tf;//time
tech = 1;// sampling period ( secondes)
//process definition first ordre K T D
kp = 1;// gain
Tp = 30;// time constant (sec)
np = 20;//20;     %pure delay in sampling periods
//model definition
km = input("km (1) ");
Tm = input("Tm (30) ");
nm = input("nm (20) ");
//process
ap = exp(-tech/Tp);bp = 1-ap;// difference equation
//model
am = exp(-tech/Tm);bm = 1-am;
//TRBF  closed loop time response (95%)
trbf = input("trbf (50) ");
h = 1;//coincidence point here 1 for a first order process!
lh = 1-exp(-((tech*h)*3)/trbf);//exponential reference trajectory
bh = 1-am^h;// ( free mode)
//constraints
MVmax = 130;// max value of MV
MVmin = -10;// min value of MV
DMV = 50;// speed limit of MV

//variables
MV = zeros(1,tf);// manipulated variable
CV = MV;// controlled variable
sm = MV;// model output
sp = MV;// process output without disturbance
pert = MV;// disturbance
Sp = 100;// set point
//---------------------------------------

for ii = 2+max(np,nm):1:tf
    if ii>200 then pert(ii)=20;else pert(ii)=0;end;
    //process
    sp(ii)=sp(ii-1)*ap+(bp*kp)*MV(ii-1-np);
    CV(ii)=sp(ii)+pert(ii);
    //model with no dealy
    sm(ii)=sm(ii-1)*am+((bm*km)*MV(ii-1));
    //prediction of process : initialisation of the exponentail reference trajectory
    spred = CV(ii)+sm(ii)-sm(ii-nm);
    SFree = sm(ii)*(am^h)-sm(ii);  //incremental free mode = sm(ii)*bh
    d = (Sp-spred)*lh+sm(ii)*bh;
    Sforced = bh*km;  //base function is only one step
    MV(ii)=d/Sforced;
    // constraints
    if MV(ii)>(MV(ii-1)+DMV) then MV(ii)=MV(ii-1)+DMV;end;  // max speed constraint
    if MV(ii)<(MV(ii-1)-DMV) then MV(ii)=MV(ii-1)-DMV;end;  // min speed constraint
    if MV(ii)>MVmax then MV(ii)=MVmax;end;  //max value of MV
    if MV(ii)<MVmin then MV(ii)=MVmin;end;  //min value of MV
end;
scf(1)
a=gca();a.auto_clear="on"
plot(w,MV,"k",w,CV,"r",w,ones(1,tf)*Sp,"r",w,sm,"b",w,pert,"k");
xgrid

xtitle("PFC ELEMENTAIRE")
xstring(52,130,"MV")
xstring(360,5,"sec")
xstring(62,sm(62),"sm ")
xstring(50,CV(50),"CV")
xstring(225,71,"MV")
xstring(225,89,"sm")
xstring(225,26,"PERT")
xstring(225,123,"CV")


// note
//installé dans l'automate !
//sm(ii)=sm(ii-1)*am+bm*km*MV(ii-1);
//spred = CV(ii)+sm(ii)-sm(ii-nm);
//MV(ii)=(Sp-spred)*k0+sm(ii)*k1; (ki sont calculés à part)
