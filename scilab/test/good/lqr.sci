function [K,X]=lqr(P12)
//lqr gain for full-state LQ problem
//(discrete or continuous)
//          discrete                        continuous
//      |I   0   0|   | A    0   B  |      |I   0   0|   | A    0    B  |
//     z|0   A'  0| - |-C'C  I   -S'|    s |0   I   0| - |-C'C -A'  -S' |
//      |0   B'  0|   | S    0   D'D|      |0   0   0|   | S   -B'   D'D|
// Copyright INRIA
flag=P12(1)
if flag(1)<>'lss' then error('lqr: state-space only!');end
[A,B2,C1,D12]=P12(2:5);
[n,nu]=size(B2);
[ny,n]=size(C1);
select P12(7)
  case [] then
error('lqr: time domain is not defined ( P(7)=''c'' or ''d'')')
  case 'c' then
Z=0*A;I=eye(A);
E=[I,Z,0*B2;
    Z,I,0*B2;
    0*ones(nu,2*n+nu)];

Aa=[A,Z,B2;
    -C1'*C1,-A',-C1'*D12;
     D12'*C1,B2',D12'*D12];
[w,ks]=schur(Aa,E,'c');
if ks<>n then error('lqr: stable subspace too small!');end
ws=w(:,1:n);
X12=ws(1:n,:);
phi12=ws(n+1:2*n,:);
u12=ws(2*n+1:2*n+nu,:);
if rcond(X12)< 1.d-5 then warning('lqr: bad conditionning!');end
K=u12/X12;
X=phi12/X12;
return
  case 'd' then
I=eye(A);Z=0*I;
E=[I,Z,0*B2;
   Z,A',0*B2;
   0*B2',-B2',0*B2'*B2];

Aa=[A,Z, B2;
    -C1'*C1,I, -C1'*D12;
     D12'*C1, 0*B2', D12'*D12];

[w,ks]=schur(Aa,E,'d');
if ks<>n then error('lqr: stable subspace too small!');end
ws=w(:,1:n);
X12=ws(1:n,:);
phi12=ws(n+1:2*n,:);
u12=ws(2*n+1:2*n+nu,:);
if rcond(X12)< 1.d-5 then warning('lqr: bad conditionning!');end
K=u12/X12;
X=phi12/X12;
return
end
endfunction
