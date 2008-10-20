function [X1,X2,zero]=ric_desc(H,E)
//[X1,X2,zero]=ric_desc(H [,E])  Descriptor Riccati solver with
// hamiltonian matrices as inputs.
//        (see also riccati)
//In the continuous time case calling sequence is ric_desc(H) (one input).
// Riccati equation is: 
//  (Ec)   A'*X + X*A + X*R*X -Q = 0.
// Defining the hamiltonian matrix H by:
// H = [A  R;
//      Q -A']
// with [X1,X2,err]=ric_desc(H),solution X is given by X=X1/X2.
// zero=norm 1 of lhs of (Ec)
//
// (solution X is also given by   X=riccati(A,Q,R,'c'))
//
//
//In the discrete-time case calling sequence is ric_desc(H,E) (two inputs).
// Riccati solution is:
//   (Ed)  A'*X*A-(A'*X*B*(R+B'*X*B)^-1)*(B'*X*A)+C-X = 0.
// 
//  Defining G=B/R*B' and the hamiltonian pencil (E,H) by: 
//      E=[eye(n,n),G;
//         0*ones(n,n),A']
//
//      H=[A, 0*ones(n,n);
//        -C, eye(n,n)];
// with [X1,X2,err]=ric_desc(H,E),solution X is given by X=X1/X2.
// zero=norm 1 of lhs of (Ed)
//   
//  (solution X is also given by X=riccati(A,G,C,'d')  with G=B/R*B')
//!
// Copyright INRIA
[LHS,RHS]=argn(0);
if RHS==1 then
   [n2,n2]=size(H);
   n1=n2/2;
   A=H(1:n1,1:n1);
   //R=H(1:n1,n1+1:n2); Q=H(n1+1:n2,1:n1);
   [Hb,W1]=bdiag(H);
   if cond(W1) > 1.d10*norm(H,1) then
//     write(%io(2),'Warning : Bad conditioning => balancing');
     [Hb,W1]=balanc(H);
   end
   if cond(W1) > 1.d+10*norm(H,1) then Hb=H,W1=eye(W1);end
 
   [W2,n]=schur(Hb,'c');Hb=[]
   if n<>n1 then write(%io(2),'Stationary Riccati solver fails!!!!');end
   W1=W1*W2;W2=[]
   UV=W1(:,1:n1);W1=[]
   X2=UV(1:n1,:);X1=UV(n1+1:n2,:);UV=[];
   if n<>n1 then
     X2=eye(n1,n1);X1=0;
   end
   zr=X2'*A'*X1+X1'*A*X2+X1'*H(1:n1,n1+1:n2)*X1-X2'*H(n1+1:n2,1:n1)*X2;
   zero=norm(zr,1);
end
if LHS==1 then X1=X1/X2;end
if RHS==2 then
   [n2,n2]=size(H);n1=n2/2;
   n1=n2/2;
   [UV,n]=schur(H,E,'d');
   X2=UV(1:n,1:n);X1=UV(n+1:2*n,1:n);
if LHS==3 then
   A=H(1:n1,1:n1);G=E(1:n,n+1:2*n);C=-H(n+1:2*n,1:n);B=real(sqrtm(G));R=eye(A);
   X=X1/X2;zero=A'*X*A-(A'*X*B/(R+B'*X*B))*(B'*X*A)+C-X; 
end
end
if LHS==1 then X=X1/X2;end
endfunction
