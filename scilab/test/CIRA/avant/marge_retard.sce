
// marge_retard
mode(0);
ieee(1);
clear;
close;
xdel(winsid())
tech = 1;
tau = 80;a = exp(-tech/tau);b = 1-a;
//tau process= tau modèlepwd

rm = 30;// retard modèl=process nominal
//retard modèle
for z = 1:1:100;
    trbf = 30+(120*(z-1))/99;
    TRBF(1,z) = trbf;
    lh = 1-exp(-(tech*3)/trbf);
    q = lh/b;
    omeg = 0.001*ones(1,30);
    //  u = omeg;
    for j = 2:1:30;
        xm = omeg(j-1)*rm;
        U = q*q+2*cos(xm)-((2*tau)*omeg(j-1))*sin(xm)-2;
        omeg(j) = sqrt(U)/tau;
        T(1,j) = (2*%pi)/omeg(j);
    end;
    TT(1,z) = T(30);
    rp(1,z) = acos((cos(((rm*2)*%pi)/TT(z))-1)/q)/omeg(30);
end;

scf(1);
a=gca();a.auto_clear= "on";//---------------------?
plot(TRBF,TT,"b",TRBF,rp,"k");
xgrid;
title(" MARGE DE RETARD  k / Période b / ");
xstring(100,120,"MARGE DE RETARD");
xstring(100,341,"Période sec");
xstring(31,341,"Tau= 80 sec");
xstring(31,380,"Retard modèle = 30 sec");
xstring(25,425," sec");
xstring(145,15," sec");
xstring(125,15," TRBF");

