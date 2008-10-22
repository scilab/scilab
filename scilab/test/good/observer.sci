function [Obs,U,m]=observer(Sys,flag,alfa)
//Obs=observer for (observable part of) linear system Sys
//Obs is a linear system with matrices [Ao,Bo,Identity].
//where Ao is no x no, Bo is no x (nu+ny) and Co is no x no
//and no=nx-m;
//input to Obs is [u,y] (assuming Sys: dotx=A x + Bu, y=Cx + Du)
//output of Obs is: 
// xhat=estimate of x modulo unobservable subsp. (case 'pp')
// or
// xhat=estimate of x modulo unstable unobservable subsp. (case 'st')
//
//case flag='st':
// z=H*x can be estimated with stable observer iff H*U(:,1:m) = 0
// assignable poles of the observer are set to alfa(1),alfa(2),...
// 
//case flag='pp':
// z=H*x can be estimated with given error spectrum iff H*U(:,1:m)=0 
//all poles of the observer are assigned and set to alfa(1),alfa(2),...
//
//If H satifies the constraint: H*U(:,1:m)=0  (ker(H) contains unobs-subsp
//of Sys) one has H*U=[0,H2] and the observer for 
// z=H*x is is H2*Obs with H2=H*U(:,m+1:nx) i.e. Co, the C-matrix of the
// observer for H*x, is Co=H2.
//
//EXAMPLE:
// nx=5;nu=1;ny=1;un=3;us=2;Sys=ssrand(ny,nu,nx,list('dt',us,us,un));
// nx=5 states, nu=1 input, ny=1 output, 
// un=3 unobservable states, us=2 of them unstable.
// [Obs,U,m]=observer(Sys);  Stable observer (default)
// W=U';H=W(m+1:nx,:);[A,B,C,D]=abcd(Sys);  //H*U=[0,eye(no,no)];
// Sys2=ss2tf(syslin('c',A,B,H))  //Transfer u-->z
// Idu=eye(nu,nu);ss2tf(Obs*sysdiag(Idu,Sys)*[Idu;Idu])  
// Transfer u-->[u;u]-->w=[u;y=Sys*u]-->Obs*w  i.e. u-->output of Obs
// this transfer must equal Sys2, the u-->z transfer  (H2=eye).
//FD.
// Copyright INRIA
[nx,nx]=size(Sys(2));
td=Sys(7);x0=Sys(6);
[LHS,RHS]=argn(0);
if RHS<>2 then [m1,m2,U,sl2]=dt_ility(Sys);end
if RHS==1 then
  flag='st';alfa=-ones(1,nx);
end
if RHS==2 then 
  //poles are not given-->set to -ones
  alfa=-ones(1,nx);
  [A,B,C,D]=abcd(Sys);  //
  J=flag;
//  F=A+J*C;G=[B+J*D,-J]; //
   Obs=syslin(td,A+J*C,[B+J*D,-J],eye(A));U=[];m=[];return;  //Ao
end
if RHS==3 then
  if size(alfa,'*')==1 then alfa=alfa*ones(1,nx);end
end
select flag
case 'pp'
m=m2;
no=nx-m;
alfa=alfa(1:no);
[A,B,C,D]=abcd(sl2);
Ao=A(m+1:nx,m+1:nx);
Bo=B(m+1:nx,:);
Co=C(:,m+1:nx);
Do=D;
  J=-ppol(Ao',Co',alfa);J=J';
  F=Ao+J*Co;G=[Bo+J*Do,-J];
  Obs=syslin(td,F,G,eye(Ao));
  return;
case 'st'
m=m1;
no=nx-m;
alfa=alfa(1:no);
[A,B,C,D]=abcd(sl2);
Ao=A(m+1:nx,m+1:nx);
Bo=B(m+1:nx,:);
Co=C(:,m+1:nx);
Do=D;
  J=stabil(Ao',Co',alfa);J=J';
  F=Ao+J*Co;G=[Bo+J*Do,-J];
  Obs=syslin(td,F,G,eye(Ao));
  return;
end
endfunction
