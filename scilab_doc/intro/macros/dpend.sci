function [zdot]=dpend(time,z)
// Copyright INRIA
th1=z(1);th2=z(2);th1d=z(3);th2d=z(4);
   s12=sin(th1-th2);c12=cos(th1-th2);
   m12=m1+m2;s1=sin(th1);s2=sin(th2);
   mat=[m12*r1 m2*r2*c12;...
        r1*c12 r2];
   vec=-[m12*g*s1+m2*r2*th2d*th2d*s12;...
         g*s2-r1*th1d*th1d*s12];
   res=mat\vec;
   th1dd=res(1);th2dd=res(2);
   zdot=[th1d;th2d;th1dd;th2dd];

function []=pp(z)
// Copyright INRIA
th1=z(1,:);th2=z(2,:);
rc1=r1*cos(th1);rc2=r2*cos(th2);
rs1=r1*sin(th1);rs2=r2*sin(th2);
rs12=rs1+rs2;rc12=rc1+rc2;
rect=[-2.1,-3.1,2.1,1.1];
for k=1:maxi(size(th1));
    plot2d([0 rs1(k) rs12(k)]',-[0 rc1(k) rc12(k)]',...
    [-1],"011",' ',rect,[10,3,10,3]);
end;


