function [K,r]=obscont(P,Kc,Kf)
//Returns the observer-based controller associated with a 
//plant P=[A,B,C,D]. The full-state control gain is Kc and filter
//gain is Kf. A+B*Kc and A+C*Kf are assumed stable.
// Copyright INRIA
[LHS,RHS]=argn(0)
[A,B,C,D]=abcd(P);
K=syslin(P(7),A+B*Kc+Kf*C+Kf*D*Kc,-Kf,Kc)
if LHS==1 then r=[];return;end
zro=0*Kc*Kf;I1=eye(Kc*B);I2=eye(C*Kf);
K=syslin(P(7),A+B*Kc+Kf*C+Kf*D*Kc,[-Kf,B+Kf*D],[Kc;-C-D*Kc],[zro,I1;I2,-D]);
r=size(D);
endfunction
