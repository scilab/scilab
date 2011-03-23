// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2010 - DIGITEO - Allan CORNET
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================
//
// <-- TEST WITH GRAPHIC --> 
// <-- NO CHECK ERROR OUTPUT --> 
// <-- LONG TIME EXECUTION -->
//
// <-- Non-regression test for bug 6962 -->
//
// <-- Short Description -->
// bvode generated a error: forrtl: severe (38): error during write, unit 6, file CONOUT$
// on windows
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=6962
//
//
//reactor agitat G-L ODE, v no-pel·lícula
//continu-discontinu
//reacció: aA+bB <-->cC+dD o subcasos
//balanços a la zona de no-pel·lícula
//pel·lícula gas i líquid
//càlcul de E
//
//ref.: E.P. van Elk, ... 
//"Modelling of gas-liquid reactors..."
//Chem. Eng. J. 76(2000) 223-237
clc;
clear();
lines(0);
scf(1); clf();
scf(2); clf();
scf(3); clf();
//variables:
//pel·lícula: cAx, cBx, cCx, cDx
//no-pel·lícula: cAg, cAl, cBl, cCl, cDl
//
global rA cbv
function bds=fsub(x,z)
  rA=ki*(z(5)^cestq(3))*(z(7)^cestq(4))-kd*(z(1)^cestq(1))*(z(3)^cestq(2));
  bds(1)=-cestq(1)*rA/dif;
  bds(2)=-cestq(2)*rA/dif;
  bds(3)=cestq(3)*rA/dif;
  bds(4)=cestq(4)*rA/dif;
endfunction
//
function g=gsub(i,z)
  g=[kg*(cbv(1)-H*z(1))+dif*z(2), z(4), z(6), z(8), z(1)-cbv(2), z(3)-cbv(3), z(5)-cbv(4), z(7)-cbv(5)];
  g=g(i);
endfunction
//
function [z,bes]=inici(x)
  z(1)=1.5*cbv(2);
  z(3)=0.5*cbv(3);
  z(5)=cbv(4);
  z(7)=cbv(5);
  z(2)=0; z(4)=0; z(6)=0; z(8)=0;
  bes=zeros(4,1);
endfunction
//
//========================================================================
//funcions ODE
function ydot=bdode(t,y)
//
//disp (t)
//crida bvode
le=0;
ld=delta;
malla=100;
x=linspace(le,ld,malla);
n=4;
m=[2 2 2 2];
zeta=[le le le le ld ld ld ld];
for iy=1:5
  cbv(iy)=y(iy);
end
//disp(x)
//
cpel=bvodeS(x,m,n,le,ld,fsub,gsub,zeta,ystart=inici);
//z=[y1;y1';y2;y2';y3;y3',...]
//
cAli=cpel(1,1);
dcAxd=cpel(2,$);
dcBxd=cpel(4,$);
dcCxd=cpel(6,$);
dcDxd=cpel(8,$);
intAx=inttrap(x,cpel(1,:));
intBx=inttrap(x,cpel(3,:));
intCx=inttrap(x,cpel(5,:));
intDx=inttrap(x,cpel(7,:));
E=(-dif*cpel(2,1))/(kl*(cpel(1,1)-cpel(1,$)));
//
scf(2); clf();
plot2d(x',[cpel(1,:)' cpel(3,:)' cpel(5,:)']);
title ("perfils pel·lícula, temps= "+string(t)+", E= "+string(E));
legend ("A","B","C",-5);
xgrid(2);
//
scf(3);
plot2d(t,E,style=-4);
title ("factor acceleració, temps");
xgrid(2);
glp=gce();
glp.children.mark_foreground=2;
//gas
  ydot(1)=(1/epsg/VR)*(Qvg*(cAge-y(1))-a*VR*kg*(y(1)-H*cAli));
//
//líquid
  rA=ki*(y(4)^cestq(3))*(y(5)^cestq(4))-kd*(y(2)^cestq(1))*(y(3)^cestq(2));
  ydot(2)=(1/(epsl-a*delta)/VR)*(Qvl*(cjle(1)-y(2))+a*VR*(-dif*dcAxd)-cestq(1)*(-rA)*(epsl-a*delta)*VR);
  ydot(3)=(1/(epsl-a*delta)/VR)*(Qvl*(cjle(2)-y(3))+a*VR*(-dif*dcBxd)-cestq(2)*(-rA)*(epsl-a*delta)*VR);
  ydot(4)=(1/(epsl-a*delta)/VR)*(Qvl*(cjle(3)-y(4))+a*VR*(-dif*dcCxd)+cestq(3)*(-rA)*(epsl-a*delta)*VR);
  ydot(5)=(1/(epsl-a*delta)/VR)*(Qvl*(cjle(4)-y(5))+a*VR*(-dif*dcDxd)+cestq(4)*(-rA)*(epsl-a*delta)*VR);
endfunction
//========================================================================
//programa principal
//
//cas/dades
cas=1;
select cas;
  case 1 then
//reacció: aA+bB <-->cC+dD, reactor discontinu    
    kd=10;                //ctants cinet. m6mol-2s-1
    ki=1.0;
    cestq=ones(4,1);      //coef. esteq.
    kl=5e-5;              //coef tranf mat ms-1
    kg=100;
    dif=1e-9;             //difusivitat m2s-1
    difg=1e-5;
    delta=dif/kl;         //delta pel·lícula, m
    deltag=difg/kg;
    H=2;                  //ctant Henry cAg=H*cAl
    VR=10;                //vol reactor m3
    epsg=0.5;             //holdup gas
    epsl=1-epsg;
    a=100;                //àrea espec. m-1
    cAg0=1;               //cA gas inicial
    cAl0=0;
    cBl0=1;               //cB líquid inicial
    cCl0=0;
    cDl0=0;
    Qvg=0;
    Qvl=0;
    cAge=0;
    cjle=zeros(4,1);
    t0=0;
    tf=500;
    t=t0:1:tf;
    inicial=[cAg0,cAl0,cBl0,cCl0,cDl0]';
  case 2 then
//reacció: aA --> cC, reactor continu
    kd=10;                //ctants cinet. m6mol-2s-1
    ki=0;
    cestq=ones(4,1);      //coef. esteq.
    cestq(2)=0;
    cestq(4)=0;
    kl=5e-5;              //coef tranf mat ms-1
    kg=100;
    dif=1e-9;             //difusivitat m2s-1
    difg=1e-5;
    delta=dif/kl;         //delta pel·lícula, m
    deltag=difg/kg;
    H=2;                  //ctant Henry cAg=H*cAl
    VR=10;                //vol reactor m3
    epsg=0.5;             //holdup gas
    epsl=1-epsg;
    a=250;                //àrea espec. m-1
    cAg0=1;               //cA gas inicial
    cAl0=0;
    cBl0=0;               //cB líquid inicial
    cCl0=0;
    cDl0=0;
    Qvg=0.01;
    Qvl=0.01;
    cAge=1;
    cjle=zeros(4,1);
    t0=0;
    tf=250;
    t=t0:1:tf;
    inicial=[cAg0,cAl0,cBl0,cCl0,cDl0]';    
end
//
//crida ODE
rtol=1e-3;
conc=ode(inicial,t0,t,rtol,bdode);
//resultats
//
//gràfics
scf(1);
clf();
plot2d(t',[conc(1,:)' conc(2,:)' conc(3,:)' conc(4,:)' conc(5,:)']);
title('perfils <--> t');
legend("Ag","A","B","C","D",-5);
xgrid(2);
//
