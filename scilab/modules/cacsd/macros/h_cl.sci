function Aclosed=h_cl(P,r,K)
//[Aclosed]=h_cl(P,r,K)
//Given the standard plant P (with r=size(P22)) and the controller
//K (computed e.g. by h_inf) this macro returns the closed loop
//matrix Aclosed whose poles allows to checks the internal stability
//of the closed loop system.
//Aclosed is the A matrix of [I -P22;-K I]^-1;
//!
//FD.
// Copyright INRIA
P1=P(1);
if P1(1)=='r' then P=tf2ss(P);end
[LHS,RHS]=argn(0);
if RHS==2 then K=r;[A,B2,C2,D22]=P(2:5);end
K1=K(1);      
if K1(1)=='r' then K=tf2ss(K);end
if RHS==3 then
    [A,B1,B2,C1,C2,D11,D12,D21,D22]=smga(P,r);
end
//if type(K)==1 then [ny,nu]=size(K),Ac=0;Bc=0*ones(1,nu);Cc=0*ones(ny,1);Dc=K;
[Ac,Bc,Cc,Dc]=K(2:5);
[n,pp]=size(B2);[ndc1,ndc2]=size(Dc);[nd1,nd2]=size(D22);
[m,q]=size(Bc);
Bw=[B2,0*ones(n,ndc2);
    0*ones(m,pp),Bc];
[n1,m2]=size(Cc);
[n2,m1]=size(C2);
Cw=[0*ones(ndc1,m1),Cc;
    C2,0*ones(n2,m2)];
Aclosed=[A, 0*ones(n,m);
    0*ones(m,n),Ac]+...
    Bw*inv([eye(ndc1,ndc1),-Dc;-D22,eye(nd1,nd1)])*Cw;
endfunction
