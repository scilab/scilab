function [SS]=%lss_m_lss(S1,S2)
//S=%lss_m_lss(S1,S2)  computes S1*S2 in state-space form.
//  --> S2 --> S1 -->
//!
// Copyright INRIA
[S1,S2]=sysconv(S1,S2)
[A1,B1,C1,D1,x1,dom1]=S1(2:7),
[A2,B2,C2,D2,x2]=S2(2:6),
//
if maxi(degree(D1))==0 & maxi(degree(D2))==0 then
  D1=coeff(D1);D2=coeff(D2);
  B1C2=B1*C2
  SS=tlist(['lss','A','B','C','D','X0','dt'],[A1,B1C2;0*B1C2' ,A2],[B1*D2;B2],...
                            [C1,D1*C2],D1*D2,[x1;x2],dom1),
  return
end
//improper systems

J=[A1,B1*C2;
   0*ones(B1*C2)',A2];
Ls=[C1 D1*C2]'
Ms=[B1*D2;B2]

if Ms==[]|Ls==[] then
   SS=tlist(['lss','A','B','C','D','X0','dt'],[],[],[],D1*D2,[x1;x2],dom1)
   return;
end
//
if type(D1*D2)~=1 
s=poly(0,varn(D1*D2));
end
deg=maxi(degree(Ms));
B=coeff(Ms,deg);
Ps=0*B
for i=1:deg
  Ps=s*Ps+B
  B=J*B+coeff(Ms,deg-i)
end
//
deg=maxi(degree(Ls));  J=J'
C=coeff(Ls,deg);
pps=0*C
for i=1:deg
  pps=s*pps+C
  C=J*C+coeff(Ls,deg-i)
end
//
C=C';
D=pps'*B+Ls'*Ps+D1*D2;
Dg=maxi(degree(D));
if Dg==0 then D=coeff(D);end

SS=tlist(['lss','A','B','C','D','X0','dt'],J',B,C,D,[x1;x2],dom1);
endfunction
