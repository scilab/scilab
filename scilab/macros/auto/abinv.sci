function [X,dims,F,U,k,Z]=abinv(Sl,Alfa,Beta,flag)
//Output nulling subspace (maximal unobservable subspace) for
// Sl = linear system defined by [A,B,C,D];
// The dimV first columns of X i.e V=X(:,1:dimV), spans this subspace
// which is also the unobservable subspace of (A+B*F,C+D*F);
// The dimR first columns of X spans R, the controllable part of V (dimR<=dimV).
// (dimR=0 for a left invertible system).
// The dimVg first columns of X spans Vg=maximal AB-stabilizable subspace.
// (dimR<=dimVg<=dimV). The modes of X2'*(A*BF)*X(:,1:dimVg) are either
// assignable or stable.
// For X=[V,X2] (X2=X(:,dimV+1:nx)) one has X2'*(A+B*F)*V=0 and (C+D*F)*V=0
// The zeros (transmission zeros for minimal Sl) are given by :
// X0=X(:,dimR+1:dimV); spec(X0'*(A+B*F)*X0) i.e. dimV-dimR closed-loop fixed modes
// If optional real parameter Alfa is given as input, the dimR controllable 
// modes of (A+BF) are set to Alfa.
// Generically, for strictly proper systems one has:
// Fat plants (ny<nu): dimV=dimR=nx-nu --> no zeros
// Tall plants (ny>nu): dimV=dimR=0 --> no zeros
// Square plants (ny=nu): dimV=nx-nu, dimR=0, --> dimV zeros
// For proper (D full rank) plants, generically:
// Square plants: dimV=nx, dimR=0, --> nx zeros
// Tall plants: dimV=dimR=0 --> no zeros
// Fat plants: dimV=dimR=nx --> no zeros
// Z is a column compression of Sl and k the normal rank of Sl.
//
//DDPS:
//   Find u=Fx+Rd which reject Q*d and stabilizes the plant:
//
//    xdot=Ax+Bu+Qd
//    y=Cx+Du
//
//     DDPS has a solution iff Im(Q) is included in Vg + Im(B). 
//     Let G=(X(:,dimVg+1:nx))'= left anihilator of Vg i.e. G*Vg=0;
//     B2=G*B; Q2=G*Q; DDP solvable if B2(:,1:k)*R1 + Q2 =0 has a solution.
//     R=[R1;*] is the solution  (with F=output of abinv).
//     Im(Q2) is in Im(B2) means row-compression of B2=>row-compression of Q2
//     Then C*[(sI-A-B*F)^(-1)+D]*(Q+B*R) =0   (<=>G*(Q+B*R)=0)
//F.D.
// Copyright INRIA
//function [X,dims,F,U,k,Z]=abinv(Sl,Alfa,Beta,flag)
[LHS,RHS]=argn(0);
if RHS==1 then Alfa=-1;Beta=-1;flag='ge';end
if RHS==2 then Beta=Alfa;flag='ge';end
if RHS==3 then flag='ge';end
if RHS==4 then 
if type(flag)~=10 then error('abinv: flag must be a string');end
end
timedomain=Sl.dt;
if timedomain==[] then warning('abinv: time domain not given =>Sl assumed continuous (default)');timedomain='c';end
[A,B,C,D]=abcd(Sl);
[nx,nu]=size(B);
Vi=eye(A);
[X,dimV]=im_inv([A;C],[Vi,B;zeros(C*Vi),D]);
Vi1=X(:,1:dimV);
while %t,
[X,n1]=im_inv([A;C],[Vi1,B;zeros(C*Vi1),D]);
if dimV==n1 then break;end
dimV=n1;Vi1=X(:,1:n1);
end

//V=X(:,1:dimV);    // V subspace
// Fast return if V=[];
if dimV==0 then 
	dimR=0;dimVg=0;
	[U,k]=colcomp([B;D]);
	[ns,nc,X]=st_ility(Sl);
	F=stabil(Sl('A'),Sl('B'),Beta);
	if flag=='ge' then dims=[0,0,0,nc,ns];end
	if flag=='st' then dims=[0,0,nc,ns];end
	if flag=='pp' then dims=[0,nc,ns];end
	Z=syslin(timedomain,A+B*F,B*U,F,U);
        return;
end
Anew=X'*A*X;Bnew=X'*B;Cnew=C*X;
//   Determining dimR and dimVg
B1V=Bnew(1:dimV,:);B2V=Bnew(dimV+1:nx,:);
[U,k]=colcomp([B2V;D]);   //U is nu x nu
Uker=U(:,1:nu-k);Urange=U(:,nu-k+1:nu);
slV=syslin(timedomain,Anew(1:dimV,1:dimV),B1V*Uker,[]);
[dimVg,dimR,Ur]=st_ility(slV);
X(:,1:dimV)=X(:,1:dimV)*Ur;
Anew=X'*A*X;Bnew=X'*B;Cnew=C*X;
//Bnew=Bnew*U;
//   Cut appropriate subspace  
dim=dimVg;   //dim=dimVg   //dim=dimR
select flag
case 'ge'
	dim=dimV
case 'st'
	dim=dimVg
case 'pp'
	dim=dimR
end
A11=Anew(1:dim,1:dim);
B1=Bnew(1:dim,:);B2=Bnew(dim+1:nx,:);
[U,k]=colcomp([B2;D]);   //U is nu x nu
Uker=U(:,1:nu-k);Urange=U(:,nu-k+1:nu);
B1t=B1*Uker;B1bar=B1*Urange;
sl1=syslin(timedomain,A11,B1t,[]);    //
[dimVg1,dimR1,Ur]=st_ility(sl1);
A21=Anew(dim+1:nx,1:dim);
A22=Anew(dim+1:$,dim+1:$);
C1=Cnew(:,1:dim);
B2bar=B2*Urange;Dbar=D*Urange; 
// B2bar,Dbar have k columns , B1t has nu-k columns and dim rows
Z=[A21,B2bar;C1,Dbar]; //Z is (nx-dim)+ny x dim+k 
[W,nn]=colcomp(Z);    // ? (dim+k-nn)=dim  <=> k=nn ? if not-->problem
W1=W(:,1:dim)*inv(W(1:dim,1:dim));
F1bar=W1(dim+1:dim+k,:);  
//[A21,B2bar;C1,Dbar]*[eye(dim,dim);F1bar]=zeros(nx-dim+ny,dim)
//
A11=A11+B1bar*F1bar;  //add B1bar*F1bar is not necessary.
if B1t ~= [] then
	voidB1t=%f;
       if RHS==1 then
         warning('abinv: needs Alfa =>use default value Alfa=-1')
         Alfa=-1;
       end
	F1t_tmp=0*sl1('B')'; //nu-k rows, dimV columns
else
	voidB1t=%t;F1t_tmp=[];dimR=0;
end
     
if ~voidB1t then
if norm(B1t,1)<1.d-10 then
   F1t_tmp=zeros(nu-k,dim);dimR=0;
end       
end     

sl2=syslin(timedomain,A22,B2*Urange,0*(B2*Urange)');
[ns2,nc2,U2,sl3]=st_ility(sl2);
if (nc2~=0)&(RHS==1|RHS==2) then
  warning('abinv: needs Beta => use default value Beta=-1');
end
F2=Urange*stabil(sl2('A'),sl2('B'),Beta);

//final patch
Ftmp=[U*[F1t_tmp;F1bar],F2]*X';
An=X'*(A+B*Ftmp)*X;Bn=X'*B*U;
[m,n]=size(F1t_tmp);
A11=An(1:n,1:n);B11=Bn(1:n,1:m);
F1t=stabil(A11,B11,Alfa);

F=[U*[F1t;F1bar],F2]*X';
X=X*sysdiag(eye(Ur),U2);
select flag
case 'ge'
	dims=[dimR,dimVg,dimV,dimV+nc2,dimV+ns2];
case 'st'
	dims=[dimR,dimVg,dimVg+nc2,dimVg+ns2];
case 'pp'
	dims=[dimR,dimR+nc2,dimR+ns2];
end

Z=syslin(timedomain,A+B*F,B*U,F,U);
endfunction
